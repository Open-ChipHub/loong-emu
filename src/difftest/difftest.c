#define _GNU_SOURCE

#include "qemu/osdep.h"
#include "util.h"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/mman.h>

#include <elf.h>

#include "sizes.h"
#include "cpu.h"
#include "internals.h"
#include "difftest-ref.h"
#include "smp.h"

#define REF_TO_DUT 0
#define DUT_TO_REF 1
#define SWIFTCORE_PMEM_ALIAS_BASE 0x1c000000ULL

extern char* ram;

extern int64_t singlestep;
extern int check_level;

extern int exec_env(CPULoongArchState *env);
extern void cpu_reset(CPUState* cs);
extern uint64_t helper_read_csr(CPULoongArchState *env, int csr_index);
void difftest_exec(uint64_t n);

extern const char* const csrnames[];

static size_t saved_ram_size = 0;
static bool difftest_intr_era_override_valid = false;
static uint64_t difftest_intr_era_override = 0;
static bool difftest_intr_dbg_inited = false;
static bool difftest_intr_dbg = false;

typedef struct DifftestExecutionGuide {
    bool force_raise_exception;
    uint64_t exception_num;
    uint64_t mtval;
    uint64_t stval;
    uint64_t mtval2;
    uint64_t htval;
    uint64_t vstval;
    bool force_set_jump_target;
    uint64_t jump_target;
} DifftestExecutionGuide;

static inline uint8_t* guest_to_host(uint64_t guest_paddr)
{
    return (uint8_t*)(ram + guest_paddr);
}

static inline size_t ram_range_clip(uint64_t guest_paddr, size_t n)
{
    size_t ram_size_bytes = saved_ram_size;
    if (ram_size_bytes == 0) {
        ram_size_bytes = 8UL * 1024 * 1024 * 1024;
    }
    if (guest_paddr >= ram_size_bytes) {
        return 0;
    }
    if (n > ram_size_bytes - guest_paddr) {
        return ram_size_bytes - guest_paddr;
    }
    return n;
}

static inline bool swiftcore_alias_range(uint64_t addr, size_t n, uint64_t *alias)
{
    size_t ram_size_bytes = saved_ram_size;
    if (ram_size_bytes == 0) {
        ram_size_bytes = 8UL * 1024 * 1024 * 1024;
    }
    if (n == 0 || addr + n - 1 < addr) {
        return false;
    }
    if (addr >= SWIFTCORE_PMEM_ALIAS_BASE) {
        uint64_t low = addr - SWIFTCORE_PMEM_ALIAS_BASE;
        if (low + n - 1 >= low && low + n <= ram_size_bytes) {
            *alias = low;
            return true;
        }
    } else if (addr + SWIFTCORE_PMEM_ALIAS_BASE + n - 1 >= addr + SWIFTCORE_PMEM_ALIAS_BASE &&
               addr + SWIFTCORE_PMEM_ALIAS_BASE + n <= ram_size_bytes) {
        *alias = addr + SWIFTCORE_PMEM_ALIAS_BASE;
        return true;
    }
    return false;
}

static inline void sync_timer_from_tval(CPULoongArchState *env, uint64_t tval)
{
    env->CSR_TVAL = tval;
    env->timer_counter = (int64_t)tval;
}

static inline uint64_t ref_timer_tval(CPULoongArchState *env)
{
    return (uint64_t)env->timer_counter;
}

static inline uint32_t pending_enabled_intr(CPULoongArchState *env)
{
    return FIELD_EX64(env->CSR_ESTAT, CSR_ESTAT, IS) &
           FIELD_EX64(env->CSR_ECFG, CSR_ECFG, LIE);
}

static inline void set_difftest_intr_pending(CPULoongArchState *env, uint64_t intr_num)
{
    uint32_t vector = (intr_num < 13) ? (uint32_t)intr_num : IRQ_TIMER;
    env->CSR_ESTAT |= (1ULL << vector);
}

static void difftest_enter_interrupt(CPULoongArchState *env)
{
    uint32_t vec_size = FIELD_EX64(env->CSR_ECFG, CSR_ECFG, VS);
    uint32_t pending = pending_enabled_intr(env);
    uint32_t vector = pending ? (31 - clz32(pending)) : IRQ_TIMER;

    env->CSR_PRMD = FIELD_DP64(env->CSR_PRMD, CSR_PRMD, PPLV,
                               FIELD_EX64(env->CSR_CRMD, CSR_CRMD, PLV));
    env->CSR_PRMD = FIELD_DP64(env->CSR_PRMD, CSR_PRMD, PIE,
                               FIELD_EX64(env->CSR_CRMD, CSR_CRMD, IE));
    env->CSR_ERA = difftest_intr_era_override_valid ?
                   difftest_intr_era_override : env->pc;
    difftest_intr_era_override_valid = false;
    env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, PLV, 0);
    env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, IE, 0);

    if (vec_size) {
        vec_size = (1 << vec_size) * 4;
    }
    env->irq_count++;
    env->pc = env->CSR_EENTRY + (EXCCODE_EXTERNAL_INT + vector) * vec_size;
    env_cpu(env)->exception_index = -1;
}

static bool difftest_intr_debug_enabled(void)
{
    if (!difftest_intr_dbg_inited) {
        difftest_intr_dbg = getenv("SWIFTCORE_REF_INTR_DBG") != NULL;
        difftest_intr_dbg_inited = true;
    }
    return difftest_intr_dbg;
}

// ── RAM and init ──

static void difftest_init_ram(size_t size)
{
    if (ram == NULL) {
        ram = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        lsassert(ram != NULL);
    }
}

bool addr_in_ram(hwaddr pa)
{
    size_t ram_size_bytes = saved_ram_size;
    if (ram_size_bytes == 0) {
        ram_size_bytes = 8UL * 1024 * 1024 * 1024;
    }

    return pa < ram_size_bytes;
}

bool addr_range_in_ram(hwaddr pa, unsigned size)
{
    if (size == 0 || pa + size - 1 < pa) {
        return false;
    }

    return addr_in_ram(pa + size - 1);
}

void difftest_set_ramsize(size_t n)
{
    saved_ram_size = n;
}

void difftest_init(void)
{
    size_t ram_size_bytes = saved_ram_size;
    if (ram_size_bytes == 0) {
        ram_size_bytes = 8UL * 1024 * 1024 * 1024;
    }

    LoongArchCPU* cpu = aligned_alloc(64, sizeof(LoongArchCPU));
    memset(cpu, 0, sizeof(LoongArchCPU));
    CPUState *cs = CPU(cpu);
    cs->cluster_index = -1;
    cpu_register(cs);
    current_cpu = cs;
    CPULoongArchState* env = &cpu->env;
    cs->env = env;
    cpu_reset(cs);
    loongarch_core_initfn(env);
    cpu_clear_tc(env);
    env->timer_counter = INT64_MAX;
    hw_ptw = true;

    current_env = env;

    difftest_init_ram(ram_size_bytes);

    check_level |= CPU_CHECK_TLB_MHIT;
}

void difftest_init_v2(unsigned state_size)
{
    if (state_size != sizeof(la_ref_state_t)) {
        fprintf(stderr, "ERROR: loong64-emu REF state size mismatch: REF=%zuB DUT=%uB\n",
                sizeof(la_ref_state_t), state_size);
        abort();
    }

    if (ram == NULL) {
        difftest_init();
    }
}

int difftest_status(void)
{
    return 0;
}

void difftest_close(void)
{
}

// ── Execution ──

void difftest_exec(uint64_t n)
{
    singlestep = n;
    exec_env(current_env);
}

static bool difftest_is_normal_page_exception(uint64_t exception)
{
    return exception >= EXCCODE_PIL && exception <= EXCCODE_PPI;
}

static void difftest_enter_exception(CPULoongArchState *env, uint64_t exception, uint64_t badv)
{
    uint32_t vec_size = FIELD_EX64(env->CSR_ECFG, CSR_ECFG, VS);

    if (difftest_is_normal_page_exception(exception)) {
        env->CSR_BADV = badv;
        env->CSR_TLBEHI = badv & (TARGET_PAGE_MASK << 1);
    }

    env->CSR_ESTAT = FIELD_DP64(env->CSR_ESTAT, CSR_ESTAT, ECODE, EXCODE_MCODE(exception));
    env->CSR_ESTAT = FIELD_DP64(env->CSR_ESTAT, CSR_ESTAT, ESUBCODE, EXCODE_SUBCODE(exception));
    env->CSR_PRMD = FIELD_DP64(env->CSR_PRMD, CSR_PRMD, PPLV,
                               FIELD_EX64(env->CSR_CRMD, CSR_CRMD, PLV));
    env->CSR_PRMD = FIELD_DP64(env->CSR_PRMD, CSR_PRMD, PIE,
                               FIELD_EX64(env->CSR_CRMD, CSR_CRMD, IE));
    env->CSR_ERA = env->pc;
    env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, PLV, 0);
    env->CSR_CRMD = FIELD_DP64(env->CSR_CRMD, CSR_CRMD, IE, 0);

    if (vec_size) {
        vec_size = (1 << vec_size) * 4;
    }
    set_pc(env, env->CSR_EENTRY + EXCODE_MCODE(exception) * vec_size);
    env_cpu(env)->exception_index = -1;
}

void difftest_guided_exec(void *guide_buf)
{
    DifftestExecutionGuide *guide = (DifftestExecutionGuide *)guide_buf;

    if (guide->force_raise_exception) {
        difftest_enter_exception(current_env, guide->exception_num, guide->mtval);
        return;
    }

    if (guide->force_set_jump_target) {
        difftest_intr_era_override_valid = true;
        difftest_intr_era_override = guide->jump_target;
        if (difftest_intr_debug_enabled()) {
            fprintf(stderr, "[ref-guide] pc=%016lx era_override=%016lx\n",
                    current_env->pc, difftest_intr_era_override);
        }
    }
}

static inline void difftest_cpy_helper(void* ref_buf, void* dut_buf, size_t n, bool direction)
{
    if (direction == DUT_TO_REF) {
        memcpy(ref_buf, dut_buf, n);
    } else {
        memcpy(dut_buf, ref_buf, n);
    }
}

// ── Memory copy ──

void difftest_memcpy(uint64_t guest_paddr, void* dut_buf, size_t n, bool direction)
{
    if (direction == DUT_TO_REF) {
        uint64_t alias = 0;
        if (swiftcore_alias_range(guest_paddr, n, &alias)) {
            memcpy(guest_to_host(alias), dut_buf, n);
        }
    }

    size_t direct_n = ram_range_clip(guest_paddr, n);
    if (direct_n != 0) {
        void* ref_buf = (void*)guest_to_host(guest_paddr);
        difftest_cpy_helper(ref_buf, dut_buf, direct_n, direction);
    }
}

void difftest_memcpy_init(uint64_t guest_paddr, void* dut_buf, size_t n, bool direction)
{
    difftest_memcpy(guest_paddr, dut_buf, n, direction);
}

// ── difftest_regcpy: register state copy using la_ref_state_t ──
// is_fp == false: copy GPRs + CSRs + PC
// is_fp == true:  copy FPRs + FCSR0 + CF

void difftest_regcpy(void* state, bool is_from_dut, bool is_fp)
{
    la_ref_state_t* s = (la_ref_state_t*)state;
    CPULoongArchState* env = current_env;

    if (is_fp) {
        difftest_cpy_helper(&env->fpr, &s->frf.value[0], sizeof(env->fpr[0]) * 32, is_from_dut);
        return;
    }

    // GPRs
    difftest_cpy_helper(&env->gpr, &s->xrf.value[0], sizeof(env->gpr[0]) * 32, is_from_dut);

    // PC
    if (is_from_dut) {
        env->pc = s->pc;
    } else {
        s->pc = env->pc;
    }

    // CSRs — direct field mapping, no RISC-V translation
    if (is_from_dut) {
        env->CSR_CRMD     = s->csr.crmd;
        env->CSR_PRMD     = s->csr.prmd;
        env->CSR_EUEN     = s->csr.euen;
        env->CSR_ECFG     = s->csr.ecfg;
        env->CSR_ESTAT    = s->csr.estat;
        env->CSR_ERA      = s->csr.era;
        env->CSR_BADV     = s->csr.badv;
        env->CSR_EENTRY   = s->csr.eentry;
        env->CSR_TLBIDX   = s->csr.tlbidx;
        env->CSR_TLBEHI   = s->csr.tlbehi;
        env->CSR_TLBELO0  = s->csr.tlbelo0;
        env->CSR_TLBELO1  = s->csr.tlbelo1;
        env->CSR_ASID     = s->csr.asid;
        env->CSR_PGDL     = s->csr.pgdl;
        env->CSR_PGDH     = s->csr.pgdh;
        env->CSR_SAVE[0]  = s->csr.save0;
        env->CSR_SAVE[1]  = s->csr.save1;
        env->CSR_SAVE[2]  = s->csr.save2;
        env->CSR_SAVE[3]  = s->csr.save3;
        env->CSR_SAVE[4]  = s->csr.save4;
        env->CSR_SAVE[5]  = s->csr.save5;
        env->CSR_SAVE[6]  = s->csr.save6;
        env->CSR_SAVE[7]  = s->csr.save7;
        env->CSR_TID      = s->csr.tid;
        env->CSR_TCFG     = s->csr.tcfg;
        sync_timer_from_tval(env, s->csr.tval);
        env->CSR_TICLR    = s->csr.ticlr;
        env->CSR_LLBCTL   = s->csr.llbctl;
        env->CSR_TLBRENTRY = s->csr.tlbrentry;
        env->CSR_DMW[0]   = s->csr.dmw0;
        env->CSR_DMW[1]   = s->csr.dmw1;
        env->CSR_DMW[2]   = s->csr.dmw2;
        env->CSR_DMW[3]   = s->csr.dmw3;
    } else {
        s->csr.crmd       = env->CSR_CRMD;
        s->csr.prmd       = env->CSR_PRMD;
        s->csr.euen       = env->CSR_EUEN;
        s->csr.ecfg       = env->CSR_ECFG;
        s->csr.estat      = env->CSR_ESTAT;
        s->csr.era        = env->CSR_ERA;
        s->csr.badv       = env->CSR_BADV;
        s->csr.eentry     = env->CSR_EENTRY;
        s->csr.tlbidx     = env->CSR_TLBIDX;
        s->csr.tlbehi     = env->CSR_TLBEHI;
        s->csr.tlbelo0    = env->CSR_TLBELO0;
        s->csr.tlbelo1    = env->CSR_TLBELO1;
        s->csr.asid       = env->CSR_ASID;
        s->csr.pgdl       = env->CSR_PGDL;
        s->csr.pgdh       = env->CSR_PGDH;
        s->csr.save0      = env->CSR_SAVE[0];
        s->csr.save1      = env->CSR_SAVE[1];
        s->csr.save2      = env->CSR_SAVE[2];
        s->csr.save3      = env->CSR_SAVE[3];
        s->csr.save4      = env->CSR_SAVE[4];
        s->csr.save5      = env->CSR_SAVE[5];
        s->csr.save6      = env->CSR_SAVE[6];
        s->csr.save7      = env->CSR_SAVE[7];
        s->csr.tid        = env->CSR_TID;
        s->csr.tcfg       = env->CSR_TCFG;
        s->csr.tval       = ref_timer_tval(env);
        s->csr.ticlr      = env->CSR_TICLR;
        s->csr.llbctl     = env->CSR_LLBCTL;
        s->csr.tlbrentry  = env->CSR_TLBRENTRY;
        s->csr.dmw0       = env->CSR_DMW[0];
        s->csr.dmw1       = env->CSR_DMW[1];
        s->csr.dmw2       = env->CSR_DMW[2];
        s->csr.dmw3       = env->CSR_DMW[3];
    }
}

// ── difftest_csrcpy: CSR-only state copy ──

void difftest_csrcpy(void* state, bool is_from_dut)
{
    la_ref_state_t* s = (la_ref_state_t*)state;
    CPULoongArchState* env = current_env;

    if (is_from_dut) {
        env->CSR_CRMD     = s->csr.crmd;
        env->CSR_PRMD     = s->csr.prmd;
        env->CSR_EUEN     = s->csr.euen;
        env->CSR_ECFG     = s->csr.ecfg;
        env->CSR_ESTAT    = s->csr.estat;
        env->CSR_ERA      = s->csr.era;
        env->CSR_BADV     = s->csr.badv;
        env->CSR_EENTRY   = s->csr.eentry;
        env->CSR_TLBIDX   = s->csr.tlbidx;
        env->CSR_TLBEHI   = s->csr.tlbehi;
        env->CSR_TLBELO0  = s->csr.tlbelo0;
        env->CSR_TLBELO1  = s->csr.tlbelo1;
        env->CSR_ASID     = s->csr.asid;
        env->CSR_PGDL     = s->csr.pgdl;
        env->CSR_PGDH     = s->csr.pgdh;
        env->CSR_SAVE[0]  = s->csr.save0;
        env->CSR_SAVE[1]  = s->csr.save1;
        env->CSR_SAVE[2]  = s->csr.save2;
        env->CSR_SAVE[3]  = s->csr.save3;
        env->CSR_SAVE[4]  = s->csr.save4;
        env->CSR_SAVE[5]  = s->csr.save5;
        env->CSR_SAVE[6]  = s->csr.save6;
        env->CSR_SAVE[7]  = s->csr.save7;
        env->CSR_TID      = s->csr.tid;
        env->CSR_TCFG     = s->csr.tcfg;
        sync_timer_from_tval(env, s->csr.tval);
        env->CSR_TICLR    = s->csr.ticlr;
        env->CSR_LLBCTL   = s->csr.llbctl;
        env->CSR_TLBRENTRY = s->csr.tlbrentry;
        env->CSR_DMW[0]   = s->csr.dmw0;
        env->CSR_DMW[1]   = s->csr.dmw1;
        env->CSR_DMW[2]   = s->csr.dmw2;
        env->CSR_DMW[3]   = s->csr.dmw3;
    } else {
        s->csr.crmd       = env->CSR_CRMD;
        s->csr.prmd       = env->CSR_PRMD;
        s->csr.euen       = env->CSR_EUEN;
        s->csr.ecfg       = env->CSR_ECFG;
        s->csr.estat      = env->CSR_ESTAT;
        s->csr.era        = env->CSR_ERA;
        s->csr.badv       = env->CSR_BADV;
        s->csr.eentry     = env->CSR_EENTRY;
        s->csr.tlbidx     = env->CSR_TLBIDX;
        s->csr.tlbehi     = env->CSR_TLBEHI;
        s->csr.tlbelo0    = env->CSR_TLBELO0;
        s->csr.tlbelo1    = env->CSR_TLBELO1;
        s->csr.asid       = env->CSR_ASID;
        s->csr.pgdl       = env->CSR_PGDL;
        s->csr.pgdh       = env->CSR_PGDH;
        s->csr.save0      = env->CSR_SAVE[0];
        s->csr.save1      = env->CSR_SAVE[1];
        s->csr.save2      = env->CSR_SAVE[2];
        s->csr.save3      = env->CSR_SAVE[3];
        s->csr.save4      = env->CSR_SAVE[4];
        s->csr.save5      = env->CSR_SAVE[5];
        s->csr.save6      = env->CSR_SAVE[6];
        s->csr.save7      = env->CSR_SAVE[7];
        s->csr.tid        = env->CSR_TID;
        s->csr.tcfg       = env->CSR_TCFG;
        s->csr.tval       = ref_timer_tval(env);
        s->csr.ticlr      = env->CSR_TICLR;
        s->csr.llbctl     = env->CSR_LLBCTL;
        s->csr.tlbrentry  = env->CSR_TLBRENTRY;
        s->csr.dmw0       = env->CSR_DMW[0];
        s->csr.dmw1       = env->CSR_DMW[1];
        s->csr.dmw2       = env->CSR_DMW[2];
        s->csr.dmw3       = env->CSR_DMW[3];
    }
}

// ── difftest_display ──

void difftest_display(void)
{
    CPULoongArchState* env = current_env;
    fprintf(stderr, "── LoongArch64 REF State ──\n");
    fprintf(stderr, "PC = 0x%016lx  insn = 0x%08x\n", env->prev_pc, env->insn);
    for (int i = 0; i < 32; i++) {
        fprintf(stderr, "  %3s = 0x%016lx%s",
                regnames[i], env->gpr[i], (i % 4 == 3) ? "\n" : "");
    }
    fprintf(stderr, "CRMD=0x%016lx PRMD=0x%016lx EUEN=0x%016lx\n",
            env->CSR_CRMD, env->CSR_PRMD, env->CSR_EUEN);
    fprintf(stderr, "ECFG=0x%016lx ESTAT=0x%016lx ERA=0x%016lx\n",
            env->CSR_ECFG, env->CSR_ESTAT, env->CSR_ERA);
    fprintf(stderr, "BADV=0x%016lx EENTRY=0x%016lx\n",
            env->CSR_BADV, env->CSR_EENTRY);
    fprintf(stderr, "PGDL=0x%016lx PGDH=0x%016lx ASID=0x%016lx\n",
            env->CSR_PGDL, env->CSR_PGDH, env->CSR_ASID);
    fprintf(stderr, "DMW0=0x%016lx DMW1=0x%016lx\n",
            env->CSR_DMW[0], env->CSR_DMW[1]);
}

// ── Other difftest API ──

void update_dynamic_config(void* config)
{
    (void)config;
}

void difftest_uarchstatus_sync(void* status)
{
    (void)status;
}

int difftest_store_commit(uint64_t* addr, uint64_t* data, uint8_t* mask)
{
    (void)addr; (void)data; (void)mask;
    return 0;
}

void difftest_raise_intr(uint64_t intr_num)
{
    CPULoongArchState* env = current_env;

    if (difftest_intr_debug_enabled()) {
        fprintf(stderr, "[ref-raise] pc=%016lx intr=%lu override_valid=%d override=%016lx estat=%016lx crmd=%016lx tval=%016lx\n",
                env->pc, intr_num, difftest_intr_era_override_valid,
                difftest_intr_era_override, env->CSR_ESTAT, env->CSR_CRMD,
                (uint64_t)env->timer_counter);
    }
    set_difftest_intr_pending(env, intr_num);
    difftest_enter_interrupt(env);
}

// ── Fine-grained helpers ──

void difftest_gprcpy(void* dut_buf, bool direction)
{
    difftest_cpy_helper(&current_env->gpr, dut_buf, sizeof(current_env->gpr[0]) * 32, direction);
}

void difftest_gprcpy_idx(int gpr_idx, uint64_t* dut_buf, bool direction)
{
    assert(gpr_idx >= 0 && gpr_idx <= 31);
    if (direction == DUT_TO_REF) {
        current_env->gpr[gpr_idx] = *dut_buf;
    } else {
        *dut_buf = current_env->gpr[gpr_idx];
    }
}

void difftest_pccpy(uint64_t* dut_buf, bool direction)
{
    if (direction == DUT_TO_REF) {
        current_env->pc = *dut_buf;
    } else {
        *dut_buf = current_env->prev_pc;
    }
}

void difftest_insncpy(uint32_t* dut_buf)
{
    *dut_buf = current_env->insn;
}

void difftest_fprcpy(void* dut_buf, bool direction)
{
    difftest_cpy_helper(&current_env->fpr, dut_buf, sizeof(current_env->fpr[0]) * 32, direction);
}

void difftest_fprcpy_idx(int fpr_idx, void* dut_buf, bool direction)
{
    assert(fpr_idx >= 0 && fpr_idx <= 31);
    difftest_cpy_helper(&current_env->fpr[fpr_idx], dut_buf, sizeof(current_env->fpr[0]), direction);
}

void difftest_cfcpy(void* dut_buf, bool direction)
{
    difftest_cpy_helper(&current_env->cf, dut_buf, sizeof(current_env->cf[0]) * 8, direction);
}

void difftest_cfcpy_idx(int cf_idx, uint8_t* dut_buf, bool direction)
{
    assert(cf_idx >= 0 && cf_idx <= 7);
    if (direction == DUT_TO_REF) {
        current_env->cf[cf_idx] = *dut_buf;
    } else {
        *dut_buf = current_env->cf[cf_idx];
    }
}

void difftest_fcsr0cpy(uint32_t* dut_buf, bool direction)
{
    if (direction == DUT_TO_REF) {
        current_env->fcsr0 = *dut_buf;
    } else {
        *dut_buf = current_env->fcsr0;
    }
}

#define CSR_CPY_HELPER(CSR) \
    case LOONGARCH_CSR_ ## CSR : csr_base_addr = &(current_env->CSR_ ## CSR); break;

void difftest_csrcpy_idx(int csr_idx, uint64_t* dut_buf, uint64_t mask, bool direction)
{
    uint64_t csr_value;

    if (direction == REF_TO_DUT) {
        csr_value = helper_read_csr(current_env, csr_idx);
        *dut_buf = (*dut_buf & ~mask) | (csr_value & mask);
        return;
    }

    uint64_t* csr_base_addr = &csr_value;

    switch (csr_idx)
    {
    CSR_CPY_HELPER(CRMD)
    CSR_CPY_HELPER(PRMD)
    CSR_CPY_HELPER(EUEN)
    CSR_CPY_HELPER(MISC)
    CSR_CPY_HELPER(ECFG)
    CSR_CPY_HELPER(ESTAT)
    CSR_CPY_HELPER(ERA)
    CSR_CPY_HELPER(BADV)
    CSR_CPY_HELPER(BADI)
    CSR_CPY_HELPER(EENTRY)
    CSR_CPY_HELPER(TLBIDX)
    CSR_CPY_HELPER(TLBEHI)
    CSR_CPY_HELPER(TLBELO0)
    CSR_CPY_HELPER(TLBELO1)
    CSR_CPY_HELPER(ASID)
    CSR_CPY_HELPER(PGDL)
    CSR_CPY_HELPER(PGDH)
    CSR_CPY_HELPER(PGD)
    CSR_CPY_HELPER(PWCL)
    CSR_CPY_HELPER(PWCH)
    CSR_CPY_HELPER(STLBPS)
    CSR_CPY_HELPER(RVACFG)
    CSR_CPY_HELPER(CPUID)
    CSR_CPY_HELPER(PRCFG1)
    CSR_CPY_HELPER(PRCFG2)
    CSR_CPY_HELPER(PRCFG3)
    case LOONGARCH_CSR_SAVE(0) ... LOONGARCH_CSR_SAVE(15): csr_base_addr = &(current_env->CSR_SAVE[csr_idx - LOONGARCH_CSR_SAVE(0)]); break;
    CSR_CPY_HELPER(TID)
    CSR_CPY_HELPER(TCFG)
    CSR_CPY_HELPER(TVAL)
    CSR_CPY_HELPER(CNTC)
    CSR_CPY_HELPER(TICLR)
    CSR_CPY_HELPER(LLBCTL)
    CSR_CPY_HELPER(IMPCTL1)
    CSR_CPY_HELPER(IMPCTL2)
    CSR_CPY_HELPER(TLBRENTRY)
    CSR_CPY_HELPER(TLBRBADV)
    CSR_CPY_HELPER(TLBRERA)
    CSR_CPY_HELPER(TLBRSAVE)
    CSR_CPY_HELPER(TLBRELO0)
    CSR_CPY_HELPER(TLBRELO1)
    CSR_CPY_HELPER(TLBREHI)
    CSR_CPY_HELPER(TLBRPRMD)
    CSR_CPY_HELPER(MERRCTL)
    CSR_CPY_HELPER(MERRINFO1)
    CSR_CPY_HELPER(MERRINFO2)
    CSR_CPY_HELPER(MERRENTRY)
    CSR_CPY_HELPER(MERRERA)
    CSR_CPY_HELPER(MERRSAVE)
    CSR_CPY_HELPER(CTAG)
    case LOONGARCH_CSR_DMW(0) ... LOONGARCH_CSR_DMW(3): csr_base_addr = &(current_env->CSR_DMW[csr_idx - LOONGARCH_CSR_DMW(0)]); break;
    CSR_CPY_HELPER(DBG)
    CSR_CPY_HELPER(DERA)
    CSR_CPY_HELPER(DSAVE)
    default:
        fprintf(stderr, "NOT IMPLEMENTED %s %x\n", __func__, csr_idx);
        break;
    }

    *csr_base_addr = (*csr_base_addr & ~mask) | (*dut_buf & mask);
    if (csr_idx == LOONGARCH_CSR_TVAL) {
        current_env->timer_counter = (int64_t)current_env->CSR_TVAL;
    }
}

void difftest_tlbcpy(void *state, bool direction)
{
    la_tlb_state_t *tlb_state = (la_tlb_state_t *)state;

    _Static_assert(LA_DIFFTEST_TLB_ENTRIES == LOONGARCH_TLB_MAX,
                   "LoongArch DiffTest TLB entry count mismatch");
    _Static_assert(sizeof(la_tlb_entry_t) == sizeof(LoongArchTLB),
                   "LoongArch DiffTest TLB entry size mismatch");

    lsassert(tlb_state != NULL);
    difftest_cpy_helper(current_env->tlb, tlb_state->entry,
                        sizeof(current_env->tlb), direction);

    if (direction == DUT_TO_REF) {
        cpu_clear_tc(current_env);
    }
}
