#include "qemu/osdep.h"
#include "util.h"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "cpu.h"
#include "cpu-csr.h"
#include "sizes.h"

#if !defined(CONFIG_USER_ONLY)

#define DIFFTEST_EMU_CKP_DEFAULT_SIZE (768ULL << 20)
#define DIFFTEST_EMU_CKP_STATE_OFF    0x2000ULL
#define DIFFTEST_EMU_CKP_PMEM_BASE    0x1c000000ULL
#define DIFFTEST_EMU_CKP_PAGE_SIZE    0x1000ULL

extern char *ram;

typedef struct {
    uint32_t *buf;
    size_t cap;
    size_t count;
} CodeBuf;

static uint8_t zero_page[DIFFTEST_EMU_CKP_PAGE_SIZE] __attribute__((aligned(DIFFTEST_EMU_CKP_PAGE_SIZE)));

static uint64_t parse_size_env(const char *name, uint64_t default_size)
{
    const char *s = getenv(name);
    if (!s || !*s) {
        return default_size;
    }

    char *end = NULL;
    uint64_t value = strtoull(s, &end, 0);
    if (end && (*end == 'G' || *end == 'g')) {
        value <<= 30;
    } else if (end && (*end == 'M' || *end == 'm')) {
        value <<= 20;
    } else if (end && (*end == 'K' || *end == 'k')) {
        value <<= 10;
    }
    return value;
}

static void pwrite_nofail(int fd, const void *buf, size_t size, uint64_t off)
{
    const uint8_t *p = buf;
    while (size != 0) {
        ssize_t n = pwrite(fd, p, size, off);
        if (n < 0) {
            fprintf(stderr, "pwrite failed at off=0x%lx: %s\n", off, strerror(errno));
            laemu_exit(1);
        }
        p += n;
        off += n;
        size -= n;
    }
}

static void emit(CodeBuf *code, uint32_t inst)
{
    lsassert(code->count < code->cap);
    code->buf[code->count++] = inst;
}

static void emit_li(CodeBuf *code, uint32_t rd, uint64_t value)
{
    const uint32_t rj = rd;

    if (value == 0) {
        emit(code, 0x02c00000U | rd); // addi.d rd, zero, 0
        return;
    }

    emit(code, 0x14000000U | (((value >> 12) & 0xfffffU) << 5) | rd); // lu12i.w
    if ((value & 0xfffU) != 0) {
        emit(code, 0x03800000U | ((value & 0xfffU) << 10) | (rj << 5) | rd); // ori
    }
    if ((value >> 32) != 0) {
        emit(code, 0x16000000U | (((value >> 32) & 0xfffffU) << 5) | rd); // lu32i.d
        if ((value >> 52) != 0) {
            emit(code, 0x03000000U | (((value >> 52) & 0xfffU) << 10) | (rj << 5) | rd); // lu52i.d
        }
    }
}

static void emit_ld_d(CodeBuf *code, uint32_t rd, uint32_t rj, uint32_t si12)
{
    lsassert(si12 < 2048);
    emit(code, 0x28c00000U | (si12 << 10) | (rj << 5) | rd);
}

static void emit_nop(CodeBuf *code)
{
    emit(code, 0x03400000U);
}

static void emit_csrwr(CodeBuf *code, uint32_t rd, uint32_t csr)
{
    lsassert(csr < 0x4000);
    emit(code, 0x04000020U | (csr << 10) | rd);
}

static uint64_t crmd_to_merrctl(uint64_t crmd)
{
    uint64_t merrctl = 1;
    merrctl |= (crmd & 0x3) << 2;          // PPLV
    merrctl |= ((crmd >> 2) & 0x1) << 4;   // PIE
    merrctl |= ((crmd >> 3) & 0x1) << 7;   // PDA
    merrctl |= ((crmd >> 4) & 0x1) << 8;   // PPG
    merrctl |= ((crmd >> 5) & 0x3) << 9;   // PDATF
    merrctl |= ((crmd >> 7) & 0x3) << 11;  // PDATM
    return merrctl;
}

static size_t append_value(uint64_t *state, size_t *count, uint64_t value)
{
    size_t off = *count * sizeof(uint64_t);
    state[(*count)++] = value;
    return off;
}

static void emit_load_csr(CodeBuf *code, uint64_t *state, size_t *state_count, uint32_t csr, uint64_t value)
{
    const uint32_t reg_base = 12; // t0
    const uint32_t reg_tmp = 13;  // t1
    size_t off = append_value(state, state_count, value);
    emit_ld_d(code, reg_tmp, reg_base, off);
    emit_nop(code);
    emit_csrwr(code, reg_tmp, csr);
}

static void emit_load_gpr(CodeBuf *code, uint64_t *state, size_t *state_count, uint32_t reg, uint64_t value)
{
    const uint32_t reg_base = 12; // t0
    size_t off = append_value(state, state_count, value);
    emit_ld_d(code, reg, reg_base, off);
    emit_nop(code);
}

static size_t build_restore_stub(CPULoongArchState *env, uint32_t *code, size_t code_words,
                                 uint64_t *state, size_t state_words)
{
    CodeBuf cb = { .buf = code, .cap = code_words, .count = 0 };
    size_t state_count = 0;
    const uint64_t state_addr = DIFFTEST_EMU_CKP_PMEM_BASE + DIFFTEST_EMU_CKP_STATE_OFF;

    emit_li(&cb, 12, state_addr);

    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_PRMD, env->CSR_PRMD);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_EUEN, env->CSR_EUEN);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_MISC, env->CSR_MISC);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_ECFG, env->CSR_ECFG);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_ESTAT, env->CSR_ESTAT);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_ERA, env->CSR_ERA);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_BADV, env->CSR_BADV);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_BADI, env->CSR_BADI);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_EENTRY, env->CSR_EENTRY);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TLBIDX, env->CSR_TLBIDX);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TLBEHI, env->CSR_TLBEHI);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TLBELO0, env->CSR_TLBELO0);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TLBELO1, env->CSR_TLBELO1);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_ASID, env->CSR_ASID);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_PGDL, env->CSR_PGDL);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_PGDH, env->CSR_PGDH);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_PWCL, env->CSR_PWCL);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_PWCH, env->CSR_PWCH);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_STLBPS, env->CSR_STLBPS);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_RVACFG, env->CSR_RVACFG);
    for (int i = 0; i < 16; i++) {
        emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_SAVE(i), env->CSR_SAVE[i]);
    }
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TID, env->CSR_TID);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TCFG, env->CSR_TCFG);
    /*
     * TVAL is a live countdown value, not a normal writable checkpoint CSR.
     * Restoring it with csrwr only returns a time-dependent old value into the
     * source GPR and does not reconstruct the private timer counter on real
     * hardware.  Let TCFG rebuild the timer path and let difftest's timer CSR
     * synchronization handle the live countdown after the restore stub.
     */
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_CNTC, env->CSR_CNTC);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_LLBCTL, env->CSR_LLBCTL);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TLBRENTRY, env->CSR_TLBRENTRY);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TLBRBADV, env->CSR_TLBRBADV);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TLBRERA, env->CSR_TLBRERA);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TLBRSAVE, env->CSR_TLBRSAVE);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TLBRELO0, env->CSR_TLBRELO0);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TLBRELO1, env->CSR_TLBRELO1);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TLBREHI, env->CSR_TLBREHI);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_TLBRPRMD, env->CSR_TLBRPRMD);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_MERRINFO1, env->CSR_MERRINFO1);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_MERRINFO2, env->CSR_MERRINFO2);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_MERRENTRY, env->CSR_MERRENTRY);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_MERRSAVE, env->CSR_MERRSAVE);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_DMW(0), env->CSR_DMW[0]);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_DMW(1), env->CSR_DMW[1]);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_DMW(2), env->CSR_DMW[2]);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_DMW(3), env->CSR_DMW[3]);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_MERRERA, env->pc);
    emit_load_csr(&cb, state, &state_count, LOONGARCH_CSR_MERRCTL, crmd_to_merrctl(env->CSR_CRMD));

    for (int i = 1; i < 32; i++) {
        if (i != 12 && i != 13) {
            emit_load_gpr(&cb, state, &state_count, i, env->gpr[i]);
        }
    }
    emit_load_gpr(&cb, state, &state_count, 13, env->gpr[13]);
    emit_load_gpr(&cb, state, &state_count, 12, env->gpr[12]);

    emit(&cb, 0x06483800U); // ertn

    lsassert(state_count <= state_words);
    return cb.count * sizeof(uint32_t);
}

void save_difftest_emu_checkpoint(CPULoongArchState *env, const char *filename)
{
    uint64_t image_size = parse_size_env("LAEMU_DIFFTEST_EMU_CKP_RAM_SIZE", DIFFTEST_EMU_CKP_DEFAULT_SIZE);
    if (image_size < DIFFTEST_EMU_CKP_PMEM_BASE + DIFFTEST_EMU_CKP_STATE_OFF + DIFFTEST_EMU_CKP_PAGE_SIZE) {
        image_size = DIFFTEST_EMU_CKP_PMEM_BASE + DIFFTEST_EMU_CKP_STATE_OFF + DIFFTEST_EMU_CKP_PAGE_SIZE;
    }

    int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (fd < 0) {
        fprintf(stderr, "cannot create difftest emu checkpoint %s: %s\n", filename, strerror(errno));
        laemu_exit(1);
    }

    uint64_t copied_pages = 0;
    for (uint64_t off = 0; off < image_size; off += DIFFTEST_EMU_CKP_PAGE_SIZE) {
        if (memcmp(ram + off, zero_page, DIFFTEST_EMU_CKP_PAGE_SIZE) != 0) {
            pwrite_nofail(fd, ram + off, DIFFTEST_EMU_CKP_PAGE_SIZE, off);
            copied_pages++;
        }
    }

    uint32_t code[1024] = {0};
    uint64_t state[256] = {0};
    size_t code_size = build_restore_stub(env, code, sizeof(code) / sizeof(code[0]),
                                          state, sizeof(state) / sizeof(state[0]));
    pwrite_nofail(fd, code, code_size, DIFFTEST_EMU_CKP_PMEM_BASE);
    pwrite_nofail(fd, state, sizeof(state), DIFFTEST_EMU_CKP_PMEM_BASE + DIFFTEST_EMU_CKP_STATE_OFF);

    if (ftruncate(fd, image_size) != 0) {
        fprintf(stderr, "ftruncate failed for %s: %s\n", filename, strerror(errno));
        close(fd);
        laemu_exit(1);
    }
    close(fd);

    fprintf(stderr,
            "difftest/emu gcpt saved to %s, size=0x%lx, copied_pages=%lu, restore_pc=0x%016lx\n",
            filename, image_size, copied_pages, env->pc);
}

#else

void save_difftest_emu_checkpoint(CPULoongArchState *env, const char *filename)
{
    (void)env;
    (void)filename;
}

#endif
