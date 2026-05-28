/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Diffnet — Network-based differential testing against QEMU GDB stub
 *
 * Connects to QEMU's built-in GDB server (start QEMU with -s -S).
 * After each emulated instruction, steps QEMU and compares register state.
 */

#include "qemu/osdep.h"
#include "cpu.h"
#include "diffnet.h"
#include "qrsp.h"

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

bool diffnet_enabled;
int diffnet_max_steps = 70000;
int diffnet_batch_size = 2000;
int diffnet_cmp_mask = CMP_GPR;

static qrsp_conn_t g_qrsp;
static pid_t g_child_pid = 0;
static uint64_t g_batch_count;
static int g_reg_bytes = 8;  /* 8 for LA64, 4 for LA32 */
static bool g_pending_mismatch;
static uint64_t g_pending_mismatch_batch;

/* Pre-assembled CSR dump blob (tools/dump_csr.S).
 * Writes 30 CSRs to memory at address in $t0, uses $t1 as scratch.
 * Ends with 'b .' (infinite loop) at the last 4 bytes. */
static uint64_t g_csr_dump_addr = 0x80000300;
static const uint8_t csr_dump_blob[] = {
    0x0d, 0x00, 0x00, 0x04, 0x8d, 0x01, 0xc0, 0x29, 0x0d, 0x04, 0x00, 0x04,
    0x8d, 0x21, 0xc0, 0x29, 0x0d, 0x08, 0x00, 0x04, 0x8d, 0x41, 0xc0, 0x29,
    0x0d, 0x10, 0x00, 0x04, 0x8d, 0x61, 0xc0, 0x29, 0x0d, 0x14, 0x00, 0x04,
    0x8d, 0x81, 0xc0, 0x29, 0x0d, 0x18, 0x00, 0x04, 0x8d, 0xa1, 0xc0, 0x29,
    0x0d, 0x1c, 0x00, 0x04, 0x8d, 0xc1, 0xc0, 0x29, 0x0d, 0x30, 0x00, 0x04,
    0x8d, 0xe1, 0xc0, 0x29, 0x0d, 0x40, 0x00, 0x04, 0x8d, 0x01, 0xc1, 0x29,
    0x0d, 0x44, 0x00, 0x04, 0x8d, 0x21, 0xc1, 0x29, 0x0d, 0x48, 0x00, 0x04,
    0x8d, 0x41, 0xc1, 0x29, 0x0d, 0x4c, 0x00, 0x04, 0x8d, 0x61, 0xc1, 0x29,
    0x0d, 0x60, 0x00, 0x04, 0x8d, 0x81, 0xc1, 0x29, 0x0d, 0x64, 0x00, 0x04,
    0x8d, 0xa1, 0xc1, 0x29, 0x0d, 0x68, 0x00, 0x04, 0x8d, 0xc1, 0xc1, 0x29,
    0x0d, 0x78, 0x00, 0x04, 0x8d, 0xe1, 0xc1, 0x29, 0x0d, 0x04, 0x01, 0x04,
    0x8d, 0x01, 0xc2, 0x29, 0x0d, 0x08, 0x01, 0x04, 0x8d, 0x21, 0xc2, 0x29,
    0x0d, 0x10, 0x01, 0x04, 0x8d, 0x41, 0xc2, 0x29, 0x0d, 0x80, 0x01, 0x04,
    0x8d, 0x61, 0xc2, 0x29, 0x0d, 0x00, 0x06, 0x04, 0x8d, 0x81, 0xc2, 0x29,
    0x0d, 0x04, 0x06, 0x04, 0x8d, 0xa1, 0xc2, 0x29, 0x0d, 0x20, 0x02, 0x04,
    0x8d, 0xc1, 0xc2, 0x29, 0x0d, 0x24, 0x02, 0x04,
    0x8d, 0xe1, 0xc2, 0x29, 0x0d, 0x28, 0x02, 0x04, 0x8d, 0x01, 0xc3, 0x29,
    0x0d, 0x2c, 0x02, 0x04, 0x8d, 0x21, 0xc3, 0x29, 0x0d, 0x30, 0x02, 0x04,
    0x8d, 0x41, 0xc3, 0x29, 0x0d, 0x34, 0x02, 0x04, 0x8d, 0x61, 0xc3, 0x29,
    0x0d, 0x38, 0x02, 0x04, 0x8d, 0x81, 0xc3, 0x29, 0x0d, 0x3c, 0x02, 0x04,
    0x8d, 0xa1, 0xc3, 0x29, 0x00, 0x00, 0x00, 0x50
};
#define CSR_DUMP_LEN       (sizeof(csr_dump_blob))            /* 244 bytes of code */
#define CSR_DUMP_DATA_OFF   CSR_DUMP_LEN                       /* data right after code */
#define CSR_DUMP_CSR_COUNT  30
#define CSR_DUMP_DATA_SIZE  (CSR_DUMP_CSR_COUNT * 8)
#define CSR_DUMP_TOTAL      (CSR_DUMP_LEN + CSR_DUMP_DATA_SIZE)
#define CSR_DUMP_END_PC     (g_csr_dump_addr + CSR_DUMP_LEN - 4)

enum {
    CSR_DUMP_CRMD,
    CSR_DUMP_PRMD,
    CSR_DUMP_EUEN,
    CSR_DUMP_ECFG,
    CSR_DUMP_ESTAT,
    CSR_DUMP_ERA,
    CSR_DUMP_BADV,
    CSR_DUMP_EENTRY,
    CSR_DUMP_TLBIDX,
    CSR_DUMP_TLBEHI,
    CSR_DUMP_TLBELO0,
    CSR_DUMP_TLBELO1,
    CSR_DUMP_ASID,
    CSR_DUMP_PGDL,
    CSR_DUMP_PGDH,
    CSR_DUMP_STLBPS,
    CSR_DUMP_TCFG,
    CSR_DUMP_TVAL,
    CSR_DUMP_TICLR,
    CSR_DUMP_LLBCTL,
    CSR_DUMP_DMW0,
    CSR_DUMP_DMW1,
    CSR_DUMP_TLBRENTRY,
    CSR_DUMP_TLBRBADV,
    CSR_DUMP_TLBRERA,
    CSR_DUMP_TLBRSAVE,
    CSR_DUMP_TLBRELO0,
    CSR_DUMP_TLBRELO1,
    CSR_DUMP_TLBREHI,
    CSR_DUMP_TLBRPRMD,
};

/* Decode one 8-byte register from binary buffer (little-endian). */
static inline uint64_t reg_from_bin(const uint8_t *bin)
{
    uint64_t val = 0;
    for (int i = 0; i < 8; i++)
        val |= (uint64_t)bin[i] << (i * 8);
    return val;
}

static inline uint64_t reg_from_bin_sized(const uint8_t *bin, int bytes)
{
    if (bytes == 4) {
        uint32_t v32 = (uint32_t)bin[0] | ((uint32_t)bin[1] << 8) |
                       ((uint32_t)bin[2] << 16) | ((uint32_t)bin[3] << 24);
        return (uint64_t)(int64_t)(int32_t)v32;
    }
    return reg_from_bin(bin);
}

static int hex_digit(int ch)
{
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    }
    if (ch >= 'a' && ch <= 'f') {
        return ch - 'a' + 10;
    }
    if (ch >= 'A' && ch <= 'F') {
        return ch - 'A' + 10;
    }
    return -1;
}

static int qemu_read_memory_quiet(uint64_t addr, uint8_t *buf, int len)
{
    char cmd[64];
    char hex[4096];

    snprintf(cmd, sizeof(cmd), "m%lx,%x", addr, len);
    if (qrsp_send_packet(&g_qrsp, cmd, strlen(cmd)) < 0) {
        return -1;
    }

    int n = qrsp_recv_packet(&g_qrsp, hex, sizeof(hex));
    if (n < 0 || n == 0 || hex[0] == 'E') {
        return -1;
    }

    int bytes = n / 2;
    if (bytes > len) {
        bytes = len;
    }
    for (int i = 0; i < bytes; i++) {
        int hi = hex_digit(hex[i * 2]);
        int lo = hex_digit(hex[i * 2 + 1]);
        if (hi < 0 || lo < 0) {
            return -1;
        }
        buf[i] = (uint8_t)((hi << 4) | lo);
    }
    return bytes;
}

static inline bool reg_equal_sized(uint64_t emu_val, uint64_t qemu_val)
{
    if (g_reg_bytes == 4) {
        return (uint32_t)emu_val == (uint32_t)qemu_val;
    }
    return emu_val == qemu_val;
}

static bool qemu_reg_matches(int regnum, uint64_t emu_val)
{
    uint8_t single[8] = {0};
    if (qrsp_read_register(&g_qrsp, regnum, single, g_reg_bytes) != g_reg_bytes) {
        return false;
    }
    return reg_equal_sized(emu_val, reg_from_bin_sized(single, g_reg_bytes));
}

static inline uint64_t csr_dump_val(const uint8_t *csr_buf, int index)
{
    return reg_from_bin(csr_buf + index * 8);
}

static bool diffnet_csr_in_exception_state(CPULoongArchState *env,
                                           const uint8_t *qemu_csr)
{
    uint64_t qemu_crmd = csr_dump_val(qemu_csr, CSR_DUMP_CRMD);
    uint64_t qemu_tlbrera = csr_dump_val(qemu_csr, CSR_DUMP_TLBRERA);
    bool emu_tlbr = FIELD_EX64(env->CSR_TLBRERA, CSR_TLBRERA, ISTLBR);
    bool qemu_tlbr = qemu_tlbrera & R_CSR_TLBRERA_ISTLBR_MASK;
    bool emu_direct = FIELD_EX64(env->CSR_CRMD, CSR_CRMD, DA) &&
                      !FIELD_EX64(env->CSR_CRMD, CSR_CRMD, PG);
    bool qemu_direct = (qemu_crmd & R_CSR_CRMD_DA_MASK) &&
                       !(qemu_crmd & R_CSR_CRMD_PG_MASK);

    return emu_tlbr || qemu_tlbr || emu_direct != qemu_direct;
}

static bool diffnet_emu_in_exception_state(CPULoongArchState *env)
{
    bool emu_tlbr = FIELD_EX64(env->CSR_TLBRERA, CSR_TLBRERA, ISTLBR);
    bool emu_direct = FIELD_EX64(env->CSR_CRMD, CSR_CRMD, DA) &&
                      !FIELD_EX64(env->CSR_CRMD, CSR_CRMD, PG);

    return emu_tlbr || emu_direct;
}

static bool diffnet_pc_in_exception_handler(CPULoongArchState *env)
{
    uint64_t pc = env->pc;
    uint64_t eentry = env->CSR_EENTRY;
    uint64_t era = FIELD_EX64(env->CSR_TLBRERA, CSR_TLBRERA, ISTLBR)
                   ? (env->CSR_TLBRERA & ~0x3ull)
                   : env->CSR_ERA;
    uint64_t handler_lo = eentry >= 0x1000 ? eentry - 0x1000 : 0;
    uint64_t handler_hi = eentry + 0x1000;

    return era != 0 && pc >= handler_lo && pc < handler_hi && pc < era;
}

static bool diffnet_defer_compare(CPULoongArchState *env)
{
    return diffnet_emu_in_exception_state(env) ||
           diffnet_pc_in_exception_handler(env);
}

static int diffnet_catchup_qemu_from_exception(CPULoongArchState *env,
                                               uint8_t *reg_buf,
                                               uint64_t *qemu_pc)
{
    for (int i = 0; i < 2000; i++) {
        if (reg_equal_sized(env->pc, *qemu_pc)) {
            return 1;
        }
        if (qrsp_step(&g_qrsp) < 0) {
            fprintf(stderr,
                    "DIFFNET: QEMU connection closed during exception catch-up after step=%ld, treating as program exit\n",
                    g_batch_count);
            laemu_exit(0);
        }
        if (qrsp_read_g_packet(&g_qrsp, reg_buf, 280) < 0) {
            fprintf(stderr,
                    "DIFFNET: QEMU register read failed during exception catch-up after step=%ld, treating as program exit\n",
                    g_batch_count);
            laemu_exit(0);
        }
        *qemu_pc = reg_from_bin_sized(reg_buf + 33 * g_reg_bytes, g_reg_bytes);
    }
    return 0;
}

static int diffnet_catchup_qemu_to_pc(CPULoongArchState *env,
                                      uint8_t *reg_buf,
                                      uint64_t *qemu_pc,
                                      int max_steps)
{
    for (int i = 0; i < max_steps; i++) {
        if (reg_equal_sized(env->pc, *qemu_pc)) {
            return 1;
        }
        if (qrsp_step(&g_qrsp) < 0) {
            fprintf(stderr,
                    "DIFFNET: QEMU connection closed during PC catch-up after step=%ld, treating as program exit\n",
                    g_batch_count);
            laemu_exit(0);
        }
        if (qrsp_read_g_packet(&g_qrsp, reg_buf, 280) < 0) {
            fprintf(stderr,
                    "DIFFNET: QEMU register read failed during PC catch-up after step=%ld, treating as program exit\n",
                    g_batch_count);
            laemu_exit(0);
        }
        *qemu_pc = reg_from_bin_sized(reg_buf + 33 * g_reg_bytes, g_reg_bytes);
    }
    return reg_equal_sized(env->pc, *qemu_pc) ? 1 : 0;
}

static int diffnet_sync_lasx_upper_after_lsx(CPULoongArchState *env)
{
    (void)env;
    return 0;
}

/* ---- CSR trampoline: run pre-encoded dump blob on emu side ---- */
/* Saves original memory at dump_addr, runs the blob, restores memory. */
static void emu_csr_trampoline(CPULoongArchState *env)
{
    uint64_t base = g_csr_dump_addr;
    uint8_t *gm = (uint8_t *)(ram + (base & 0xffffffff));
    uint64_t data_addr = base + CSR_DUMP_DATA_OFF;

    /* Save original memory (code + data area) */
    uint8_t orig[CSR_DUMP_TOTAL];
    memcpy(orig, gm, CSR_DUMP_TOTAL);

    /* Save CPU context */
    uint64_t saved_pc = env->pc;
    uint64_t saved_t0 = env->gpr[12];
    uint64_t saved_t1 = env->gpr[13];

    /* Inject blob and set $t0 = data area */
    memcpy(gm, csr_dump_blob, CSR_DUMP_LEN);
    env->gpr[12] = data_addr;

    /* Execute blob until 'b .' loop */
    env->pc = base;
    uint64_t end_pc = CSR_DUMP_END_PC;
    for (int i = 0; i < 100; i++) {
        if (env->pc == end_pc) break;
        uint32_t insn = ram_lduw(env->pc);
        interpreter(env, insn, NULL);
    }

    /* Restore CPU context */
    env->pc = saved_pc;
    env->gpr[12] = saved_t0;
    env->gpr[13] = saved_t1;

    /* Restore original memory */
    memcpy(gm, orig, CSR_DUMP_TOTAL);
}

/* ---- CSR trampoline: inject, run, read, restore on QEMU side ---- */
static int qemu_csr_trampoline_at(uint8_t *csr_buf, uint64_t base)
{
    uint64_t data_addr = base + CSR_DUMP_DATA_OFF;
    uint8_t orig[CSR_DUMP_TOTAL];
    uint8_t sav_t0[8], sav_t1[8], sav_pc[8];

    /* 1. Save QEMU's original memory at the dump area */
    if (qemu_read_memory_quiet(base, orig, CSR_DUMP_TOTAL) != CSR_DUMP_TOTAL)
        return -1;

    /* 2. Save QEMU's $t0, $t1, PC */
    if (qrsp_read_register(&g_qrsp, 12, sav_t0, 8) != 8) goto restore_mem;
    if (qrsp_read_register(&g_qrsp, 13, sav_t1, 8) != 8) goto restore_mem;
    if (qrsp_read_register(&g_qrsp, 33, sav_pc, 8) != 8) goto restore_mem;

    /* 3. Write blob to QEMU memory */
    if (qrsp_write_memory(&g_qrsp, base, csr_dump_blob, CSR_DUMP_LEN) < 0)
        goto restore_mem;

    /* 4. Set $t0 = data area and PC = blob start in QEMU */
    {
        uint8_t val8[8];
        /* Write $t0 = data_addr */
        for (int i = 0; i < 8; i++) val8[i] = (data_addr >> (i*8)) & 0xff;
        {
            char cmd[64]; char reply[16];
            int h = snprintf(cmd, sizeof(cmd), "P%x=", 12);
            for (int i = 0; i < 8; i++) snprintf(cmd + h + i*2, 3, "%02x", val8[i]);
            if (qrsp_send_packet(&g_qrsp, cmd, h+16) < 0) goto restore_regs;
            qrsp_recv_packet(&g_qrsp, reply, sizeof(reply));
        }
        /* Write PC = base */
        for (int i = 0; i < 8; i++) val8[i] = (base >> (i*8)) & 0xff;
        {
            char cmd[64]; char reply[16];
            int h = snprintf(cmd, sizeof(cmd), "P%x=", 33);
            for (int i = 0; i < 8; i++) snprintf(cmd + h + i*2, 3, "%02x", val8[i]);
            if (qrsp_send_packet(&g_qrsp, cmd, h+16) < 0) goto restore_regs;
            qrsp_recv_packet(&g_qrsp, reply, sizeof(reply));
        }
    }

    /* 5. Step through dump instructions (csrrd + st.d for each CSR). */
    for (int i = 0; i < CSR_DUMP_CSR_COUNT * 2; i++) {
        if (qrsp_step(&g_qrsp) < 0) goto restore_regs;
    }

    /* 6. Read dumped CSR values */
    if (qemu_read_memory_quiet(data_addr, csr_buf, CSR_DUMP_DATA_SIZE) != CSR_DUMP_DATA_SIZE)
        goto restore_regs;

restore_regs:
    /* 7. Restore QEMU's $t0, $t1, PC */
    {
        char cmd[64]; char reply[16]; int h;
        h = snprintf(cmd, sizeof(cmd), "P%x=", 12);
        for (int i = 0; i < 8; i++) snprintf(cmd + h + i*2, 3, "%02x", sav_t0[i]);
        qrsp_send_packet(&g_qrsp, cmd, h+16);
        qrsp_recv_packet(&g_qrsp, reply, sizeof(reply));

        h = snprintf(cmd, sizeof(cmd), "P%x=", 13);
        for (int i = 0; i < 8; i++) snprintf(cmd + h + i*2, 3, "%02x", sav_t1[i]);
        qrsp_send_packet(&g_qrsp, cmd, h+16);
        qrsp_recv_packet(&g_qrsp, reply, sizeof(reply));

        h = snprintf(cmd, sizeof(cmd), "P%x=", 33);
        for (int i = 0; i < 8; i++) snprintf(cmd + h + i*2, 3, "%02x", sav_pc[i]);
        qrsp_send_packet(&g_qrsp, cmd, h+16);
        qrsp_recv_packet(&g_qrsp, reply, sizeof(reply));
    }

restore_mem:
    /* 8. Restore QEMU's original memory */
    qrsp_write_memory(&g_qrsp, base, orig, CSR_DUMP_TOTAL);
    return 0;
}

static int qemu_csr_trampoline(uint8_t *csr_buf)
{
    return qemu_csr_trampoline_at(csr_buf, g_csr_dump_addr);
}

int diffnet_init(const char *host, int port, CPULoongArchState *env)
{
    if (qrsp_connect(&g_qrsp, host, port) < 0) {
        fprintf(stderr, "DIFFNET: Failed to connect to QEMU at %s:%d\n",
                host, port);
        return -1;
    }
    fprintf(stderr, "DIFFNET: Connected to QEMU at %s:%d\n", host, port);

    /* Sync emu's initial GPRs from QEMU. Detect register size from g-packet. */
    uint8_t reg_buf[280];
    int reg_len = qrsp_read_g_packet(&g_qrsp, reg_buf, sizeof(reg_buf));
    if (reg_len > 0) {
        g_reg_bytes = reg_len <= 140 ? 4 : 8;
        fprintf(stderr, "DIFFNET: detected %d-byte registers\n", g_reg_bytes);

        const uint8_t *bin = reg_buf;
        for (int i = 0; i < 32; i++) {
            if (g_reg_bytes == 4) {
                uint32_t v32 = (uint32_t)bin[i*4] | ((uint32_t)bin[i*4+1] << 8) |
                               ((uint32_t)bin[i*4+2] << 16) | ((uint32_t)bin[i*4+3] << 24);
                env->gpr[i] = (uint64_t)(int64_t)(int32_t)v32;
            } else {
                env->gpr[i] = reg_from_bin(bin + i * 8);
            }
        }
        uint64_t qemu_pc;
        if (g_reg_bytes == 4) {
            uint32_t v32 = (uint32_t)bin[33*4] | ((uint32_t)bin[33*4+1] << 8) |
                           ((uint32_t)bin[33*4+2] << 16) | ((uint32_t)bin[33*4+3] << 24);
            qemu_pc = (uint64_t)(int32_t)v32;
        } else {
            qemu_pc = reg_from_bin(bin + 33 * 8);
        }
        fprintf(stderr, "DIFFNET: synced GPRs from QEMU, QEMU PC=0x%016lx emu PC=0x%016lx\n",
                qemu_pc, env->pc);
    }

    /* Allow environment overrides */
    const char *env_val;
    if ((env_val = getenv("DIFFNET_MAX_STEPS")))
        diffnet_max_steps = atoi(env_val);
    if ((env_val = getenv("DIFFNET_BATCH_SIZE")))
        diffnet_batch_size = atoi(env_val);
    if ((env_val = getenv("DIFFNET_CSR_DUMP_ADDR")))
        g_csr_dump_addr = strtoull(env_val, NULL, 0);
    if ((env_val = getenv("DIFFNET_CMP_MASK"))) {
        diffnet_cmp_mask = 0;
        if (strstr(env_val, "all"))  diffnet_cmp_mask = CMP_ALL;
        if (strstr(env_val, "gpr"))  diffnet_cmp_mask |= CMP_GPR;
        if (strstr(env_val, "fpr"))  diffnet_cmp_mask |= CMP_FPR;
        if (strstr(env_val, "lsx"))  diffnet_cmp_mask |= CMP_LSX;
        if (strstr(env_val, "lasx")) diffnet_cmp_mask |= CMP_LASX;
        if (strstr(env_val, "csr"))  diffnet_cmp_mask |= CMP_CSR;
        if (strstr(env_val, "mem"))  diffnet_cmp_mask |= CMP_MEM;
    }
    if (g_reg_bytes == 4) {
        diffnet_cmp_mask &= ~(CMP_FPR | CMP_LSX | CMP_LASX);
    }
    fprintf(stderr, "DIFFNET: max_steps=%d batch_size=%d cmp=0x%02x\n",
            diffnet_max_steps, diffnet_batch_size, diffnet_cmp_mask);

    return 0;
}

/* Compare emu's current register state against a g-packet response from QEMU.
 * Comparison categories controlled by diffnet_cmp_mask (CMP_* flags). */
static int diffnet_compare(CPULoongArchState *env, const uint8_t *g_bin)
{
    int mismatch = 0;
    uint8_t buf[32];

#define CMP_HDR() do { \
    if (mismatch == 0) { \
        uint64_t _pc = reg_from_bin_sized(g_bin + 33 * g_reg_bytes, g_reg_bytes); \
        fprintf(stderr, "\n=== MISMATCH batch=%ld icount=%ld prev_pc=0x%016lx qemu_pc=0x%016lx ===\n", \
                g_batch_count, env->icount, env->prev_pc, _pc); \
    } \
} while(0)

    /* ---- 1. GPRs (r0-r31) + PC (always compared) ---- */
    for (int i = 0; i < 32; i++) {
        uint64_t qemu_val = reg_from_bin_sized(g_bin + i * g_reg_bytes,
                                               g_reg_bytes);
        if (!reg_equal_sized(env->gpr[i], qemu_val)) {
            if (qemu_reg_matches(i, env->gpr[i])) {
                continue;
            }
            CMP_HDR(); mismatch++;
            fprintf(stderr, "  gpr %-4s: emu=0x%016lx  qemu=0x%016lx\n",
                    regnames[i], env->gpr[i], qemu_val);
        }
    }
    {
        uint64_t qemu_pc = reg_from_bin_sized(g_bin + 33 * g_reg_bytes,
                                              g_reg_bytes);
        if (!reg_equal_sized(env->pc, qemu_pc)) {
            if (qemu_reg_matches(33, env->pc)) {
                return mismatch;
            }
            CMP_HDR(); mismatch++;
            fprintf(stderr, "  PC  : emu=0x%016lx  qemu=0x%016lx\n", env->pc, qemu_pc);
        }
    }

    /* ---- 2. FPR + FCC + FCSR (CMP_FPR) ---- */
    if ((diffnet_cmp_mask & CMP_FPR) && FIELD_EX32(env->cpucfg[2], CPUCFG2, FP)) {
        for (int i = 0; i < 32; i++) {
            if (qrsp_read_register(&g_qrsp, 35 + i, buf, 8) == 8) {
                uint64_t qemu_val = reg_from_bin(buf);
                uint64_t emu_val = env->fpr[i].vreg.D[0];
                if (emu_val != qemu_val) {
                    CMP_HDR(); mismatch++;
                    fprintf(stderr, "  fpr %-4s: emu=0x%016lx  qemu=0x%016lx\n",
                            fregnames[i], emu_val, qemu_val);
                }
            }
        }
        for (int i = 0; i < 8; i++) {
            if (qrsp_read_register(&g_qrsp, 67 + i, buf, 1) == 1) {
                uint8_t emu_val = env->cf[i] ? 1 : 0;
                if (emu_val != buf[0]) {
                    CMP_HDR(); mismatch++;
                    fprintf(stderr, "  fcc%d: emu=%d qemu=%d\n", i, emu_val, buf[0]);
                }
            }
        }
        if (qrsp_read_register(&g_qrsp, 75, buf, 4) == 4) {
            uint32_t qemu_val = (uint32_t)buf[0] | ((uint32_t)buf[1] << 8) |
                                ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 24);
            if (env->fcsr0 != qemu_val) {
                CMP_HDR(); mismatch++;
                fprintf(stderr, "  fcsr: emu=0x%08x qemu=0x%08x\n", env->fcsr0, qemu_val);
            }
        }
    }

    /* ---- 3. LSX vectors (CMP_LSX) ---- */
    if ((diffnet_cmp_mask & CMP_LSX) && FIELD_EX32(env->cpucfg[2], CPUCFG2, LSX)) {
        for (int i = 0; i < 32; i++) {
            if (qrsp_read_register(&g_qrsp, 76 + i, buf, 16) == 16) {
                for (int j = 0; j < 2; j++) {
                    uint64_t emu_val = env->fpr[i].vreg.D[j];
                    uint64_t qemu_val = reg_from_bin(buf + j * 8);
                    if (emu_val != qemu_val) {
                        CMP_HDR(); mismatch++;
                        fprintf(stderr, "  vr%d.D[%d]: emu=0x%016lx qemu=0x%016lx\n",
                                i, j, emu_val, qemu_val);
                    }
                }
            }
        }
    }

    /* ---- 4. LASX vectors (CMP_LASX) ---- */
    if ((diffnet_cmp_mask & CMP_LASX) && FIELD_EX32(env->cpucfg[2], CPUCFG2, LASX)) {
        for (int i = 0; i < 32; i++) {
            if (qrsp_read_register(&g_qrsp, 108 + i, buf, 32) == 32) {
                for (int j = 0; j < 4; j++) {
                    uint64_t emu_val = env->fpr[i].vreg.D[j];
                    uint64_t qemu_val = reg_from_bin(buf + j * 8);
                    if (emu_val != qemu_val) {
                        CMP_HDR(); mismatch++;
                        fprintf(stderr, "  xr%d.D[%d]: emu=0x%016lx qemu=0x%016lx\n",
                                i, j, emu_val, qemu_val);
                    }
                }
            }
        }
    }

    /* ---- 5. CSR dump (CMP_CSR) via trampoline ---- */
    if (diffnet_cmp_mask & CMP_CSR) {
        uint8_t csr_buf[256];

        /* Run the dump on QEMU side (save mem → inject → step → read → restore) */
        if (qemu_csr_trampoline(csr_buf) == 0) {
            /* Run same dump on emu side and compare CSR-by-CSR */
            emu_csr_trampoline(env);

            struct { const char *n; uint64_t v; } csrs[] = {
                {"CRMD",env->CSR_CRMD},{"PRMD",env->CSR_PRMD},{"EUEN",env->CSR_EUEN},
                {"ECFG",env->CSR_ECFG},{"ESTAT",env->CSR_ESTAT},{"ERA",env->CSR_ERA},
                {"BADV",env->CSR_BADV},{"EENTRY",env->CSR_EENTRY},
                {"TLBIDX",env->CSR_TLBIDX},{"TLBEHI",env->CSR_TLBEHI},
                {"TLBELO0",env->CSR_TLBELO0},{"TLBELO1",env->CSR_TLBELO1},
                {"ASID",env->CSR_ASID},{"PGDL",env->CSR_PGDL},{"PGDH",env->CSR_PGDH},
                {"STLBPS",env->CSR_STLBPS},{"TCFG",env->CSR_TCFG},{"TVAL",env->CSR_TVAL},
                {"TICLR",env->CSR_TICLR},{"LLBCTL",env->CSR_LLBCTL},
                {"DMW0",env->CSR_DMW[0]},{"DMW1",env->CSR_DMW[1]},
                {"TLBRENTRY",env->CSR_TLBRENTRY},{"TLBRBADV",env->CSR_TLBRBADV},
                {"TLBRERA",env->CSR_TLBRERA},{"TLBRSAVE",env->CSR_TLBRSAVE},
                {"TLBRELO0",env->CSR_TLBRELO0},{"TLBRELO1",env->CSR_TLBRELO1},
                {"TLBREHI",env->CSR_TLBREHI},{"TLBRPRMD",env->CSR_TLBRPRMD},
            };
            int ncsr = sizeof(csrs)/sizeof(csrs[0]);
            for (int i = 0; i < ncsr; i++) {
                uint64_t qv = reg_from_bin(csr_buf + i * 8);
                if (csrs[i].v != qv) {
                    CMP_HDR(); mismatch++;
                    fprintf(stderr, "  csr %-8s: emu=0x%016lx qemu=0x%016lx\n",
                            csrs[i].n, csrs[i].v, qv);
                }
            }
        }
    }

    /* ---- 6. Test output memory (CMP_MEM) ---- */
    if (diffnet_cmp_mask & CMP_MEM) {
        uint8_t mem_buf[4];
        if (qrsp_read_memory(&g_qrsp, 0x1c000100, mem_buf, 4) == 4) {
            int nonzero = 0;
            for (int i = 0; i < 4; i++) { if (mem_buf[i]) { nonzero = 1; break; } }
            if (nonzero) {
                uint32_t qemu_val = (uint32_t)mem_buf[0] | ((uint32_t)mem_buf[1] << 8) |
                                    ((uint32_t)mem_buf[2] << 16) | ((uint32_t)mem_buf[3] << 24);
                uint32_t emu_val = ram_lduw(0x1c000100);
                if (emu_val != qemu_val) {
                    CMP_HDR(); mismatch++;
                    fprintf(stderr, "  mem[0x1c000100]: emu=0x%08x qemu=0x%08x\n", emu_val, qemu_val);
                }
            }
        }
    }

#undef CMP_HDR

    if (mismatch > 0)
        fprintf(stderr, "  (prev_pc=0x%016lx insn=0x%08x)\n", env->prev_pc, env->insn);
    return mismatch;
}

int diffnet_check(CPULoongArchState *env)
{
    g_batch_count++;

    if (diffnet_max_steps > 0 && g_batch_count > diffnet_max_steps) {
        fprintf(stderr, "DIFFNET: reached step limit %d, exiting\n", diffnet_max_steps);
        laemu_exit(0);
    }

    /* Per-step mode: original behavior */
    if (diffnet_batch_size <= 1) {
        if (qrsp_step(&g_qrsp) < 0) {
            fprintf(stderr, "DIFFNET: step failed at step=%ld\n", g_batch_count);
            return -1;
        }
        uint8_t reg_buf[280];
        if (qrsp_read_g_packet(&g_qrsp, reg_buf, sizeof(reg_buf)) < 0) {
            fprintf(stderr, "DIFFNET: read g packet failed at step=%ld\n", g_batch_count);
            return -1;
        }
        return diffnet_compare(env, reg_buf) > 0 ? -1 : 0;
    }

    /* Batch compare mode: QEMU still follows every retired emu instruction.
     * Only the expensive full-state comparison is delayed to batch boundaries.
     */
    if (qrsp_step(&g_qrsp) < 0) {
        fprintf(stderr, "DIFFNET: QEMU connection closed after step=%ld, treating as program exit\n",
                g_batch_count);
        laemu_exit(0);
    }
    if (diffnet_sync_lasx_upper_after_lsx(env) < 0) {
        return -1;
    }

    if (g_batch_count % diffnet_batch_size != 0) {
        return 0;
    }

    uint8_t reg_buf[280];
    if (qrsp_read_g_packet(&g_qrsp, reg_buf, sizeof(reg_buf)) < 0) {
        fprintf(stderr, "DIFFNET: read g packet failed at batch=%ld\n", g_batch_count);
        return -1;
    }

    uint64_t qemu_pc = reg_from_bin_sized(reg_buf + 33 * g_reg_bytes, g_reg_bytes);
    if (diffnet_defer_compare(env)) {
        if (g_batch_count % (diffnet_batch_size * 10) == 0) {
            fprintf(stderr,
                    "DIFFNET: batch %ld deferred in emu exception state "
                    "(emu_pc=0x%016lx qemu_pc=0x%016lx emu_tlbrera=0x%016lx)\n",
                    g_batch_count, env->pc, qemu_pc, env->CSR_TLBRERA);
        }
        return 0;
    }

    if (!reg_equal_sized(env->pc, qemu_pc)) {
        uint8_t csr_buf[CSR_DUMP_DATA_SIZE];
        if (qemu_csr_trampoline_at(csr_buf, qemu_pc) == 0 &&
            diffnet_csr_in_exception_state(env, csr_buf)) {
            int caught = diffnet_catchup_qemu_from_exception(env, reg_buf, &qemu_pc);
            if (caught < 0) {
                fprintf(stderr, "DIFFNET: exception catch-up failed at batch=%ld\n",
                        g_batch_count);
                return -1;
            }
            if (caught == 0) {
                if (g_batch_count % (diffnet_batch_size * 10) == 0) {
                    fprintf(stderr,
                            "DIFFNET: batch %ld deferred in CSR exception state "
                            "(emu_pc=0x%016lx qemu_pc=0x%016lx emu_tlbrera=0x%016lx qemu_tlbrera=0x%016lx)\n",
                            g_batch_count, env->pc, qemu_pc, env->CSR_TLBRERA,
                            csr_dump_val(csr_buf, CSR_DUMP_TLBRERA));
                }
                return 0;
            }
            if (diffnet_sync_lasx_upper_after_lsx(env) < 0) {
                return -1;
            }
            if (g_batch_count % (diffnet_batch_size * 10) == 0) {
                fprintf(stderr,
                        "DIFFNET: batch %ld caught QEMU up after exception "
                        "(pc=0x%016lx)\n",
                        g_batch_count, env->pc);
            }
        }
    }

    if (!reg_equal_sized(env->pc, qemu_pc)) {
        int caught = diffnet_catchup_qemu_to_pc(env, reg_buf, &qemu_pc, 256);
        if (caught > 0) {
            if (diffnet_sync_lasx_upper_after_lsx(env) < 0) {
                return -1;
            }
            if (g_batch_count % (diffnet_batch_size * 10) == 0) {
                fprintf(stderr,
                        "DIFFNET: batch %ld caught QEMU up to emu PC "
                        "(pc=0x%016lx)\n",
                        g_batch_count, env->pc);
            }
        }
    }

    int mism = diffnet_compare(env, reg_buf);
    if (mism > 0) {
        if (!g_pending_mismatch) {
            g_pending_mismatch = true;
            g_pending_mismatch_batch = g_batch_count;
            fprintf(stderr,
                    "DIFFNET: mismatch pending at batch=%ld, rechecking next compare batch\n",
                    g_batch_count);
            return 0;
        }
        fprintf(stderr,
                "DIFFNET: mismatch confirmed at batch=%ld (first seen at batch=%lu)\n",
                g_batch_count, g_pending_mismatch_batch);
        return -1;
    }

    if (g_pending_mismatch) {
        fprintf(stderr,
                "DIFFNET: pending mismatch at batch=%lu cleared at batch=%ld\n",
                g_pending_mismatch_batch, g_batch_count);
        g_pending_mismatch = false;
        g_pending_mismatch_batch = 0;
    }

    if (g_batch_count % (diffnet_batch_size * 10) == 0)
        fprintf(stderr, "DIFFNET: batch %ld ok (icount=%ld)\n", g_batch_count, env->icount);

    return 0;
}

int diffnet_step_exception(CPULoongArchState *env)
{
    (void)env;
    if (qrsp_step(&g_qrsp) < 0) {
        fprintf(stderr, "DIFFNET: exception step failed\n");
        return -1;
    }
    return 0;
}

void diffnet_cleanup(void)
{
    qrsp_close(&g_qrsp);
    if (g_child_pid > 0) {
        kill(g_child_pid, SIGTERM);
        waitpid(g_child_pid, NULL, 0);
        g_child_pid = 0;
        fprintf(stderr, "DIFFNET: QEMU child process terminated\n");
    }
}

/* Poll until QEMU's GDB stub is accepting connections. */
static int wait_for_gdb(int port, int timeout_sec)
{
    int sock;
    struct sockaddr_in addr;
    int waited = 0;

    while (waited < timeout_sec) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) return -1;

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_aton("127.0.0.1", &addr.sin_addr);

        if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
            close(sock);
            return 0;
        }
        close(sock);
        usleep(500000); /* 0.5s */
        waited++;
    }
    return -1;
}

int diffnet_spawn(const char *qemu_path, const char *kernel,
                  uint64_t load_addr, int port, CPULoongArchState *env)
{
    fprintf(stderr, "DIFFNET: spawning QEMU: %s\n", qemu_path);
    fprintf(stderr, "DIFFNET:   kernel=%s addr=0x%lx port=%d\n",
            kernel, load_addr, port);

    pid_t pid = fork();
    if (pid < 0) {
        perror("DIFFNET: fork");
        return -1;
    }

    if (pid == 0) {
        /* ---- child: exec QEMU ---- */
        int devnull = open("/dev/null", O_RDWR);
        if (devnull >= 0) {
            dup2(devnull, STDIN_FILENO);
            dup2(devnull, STDOUT_FILENO);
            /* keep stderr for diagnostics */
            close(devnull);
        }

        char addr_str[32];
        snprintf(addr_str, sizeof(addr_str), "0x%lx", load_addr);

        char loader_arg[512];
        snprintf(loader_arg, sizeof(loader_arg),
                 "loader,file=%s,addr=%s", kernel, addr_str);

        char gdb_arg[32];
        snprintf(gdb_arg, sizeof(gdb_arg), "tcp::%d", port);

        /* Read QEMU CPU model from env (for LA32 diffnet) */
        const char *qemu_cpu = getenv("DIFFNET_QEMU_CPU");

        /* Use -gdb tcp::PORT for the GDB stub, -S to stop at entry */
        if (qemu_cpu && qemu_cpu[0]) {
            execlp(qemu_path, qemu_path,
                   "-M", "virt",
                   "-cpu", qemu_cpu,
                   "-device", loader_arg,
                   "-gdb", gdb_arg,
                   "-S",
                   "-nographic",
                   "-monitor", "none",
                   "-display", "none",
                   (char *)NULL);
        } else {
            execlp(qemu_path, qemu_path,
                   "-M", "virt",
                   "-device", loader_arg,
                   "-gdb", gdb_arg,
                   "-S",
                   "-nographic",
                   "-monitor", "none",
                   "-display", "none",
                   (char *)NULL);
        }

        /* execlp only returns on error */
        fprintf(stderr, "DIFFNET: failed to exec QEMU: %s\n", strerror(errno));
        _exit(1);
    }

    /* ---- parent: wait for GDB stub, then connect ---- */
    g_child_pid = pid;
    atexit(diffnet_cleanup);

    fprintf(stderr, "DIFFNET: waiting for QEMU GDB stub on port %d...\n", port);
    if (wait_for_gdb(port, 60) < 0) {
        fprintf(stderr, "DIFFNET: timeout waiting for QEMU GDB stub\n");
        diffnet_cleanup();
        return -1;
    }

    /* Now connect via standard path */
    return diffnet_init("127.0.0.1", port, env);
}
