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

/* Decode one 8-byte register from binary buffer (little-endian).
 * bin points to 8 bytes in the g-packet binary output. */
static inline uint64_t reg_from_bin(const uint8_t *bin)
{
    uint64_t val = 0;
    for (int i = 0; i < 8; i++) {
        val |= (uint64_t)bin[i] << (i * 8);
    }
    return val;
}

int diffnet_init(const char *host, int port, CPULoongArchState *env)
{
    if (qrsp_connect(&g_qrsp, host, port) < 0) {
        fprintf(stderr, "DIFFNET: Failed to connect to QEMU at %s:%d\n",
                host, port);
        return -1;
    }
    fprintf(stderr, "DIFFNET: Connected to QEMU at %s:%d\n", host, port);

    /* Sync emu's initial GPRs from QEMU (QEMU virt machine may set some regs).
     * Don't sync PC — both should be at the same entry point. */
    uint8_t reg_buf[280];
    if (qrsp_read_g_packet(&g_qrsp, reg_buf, sizeof(reg_buf)) == 0) {
        const uint8_t *bin = reg_buf;
        for (int i = 0; i < 32; i++) {
            env->gpr[i] = reg_from_bin(bin + i * 8);
        }
        uint64_t qemu_pc = reg_from_bin(bin + 33 * 8);
        fprintf(stderr, "DIFFNET: synced GPRs from QEMU, QEMU PC=0x%016lx emu PC=0x%016lx\n",
                qemu_pc, env->pc);
    }

    /* Allow environment overrides */
    const char *env_val;
    if ((env_val = getenv("DIFFNET_MAX_STEPS")))
        diffnet_max_steps = atoi(env_val);
    if ((env_val = getenv("DIFFNET_BATCH_SIZE")))
        diffnet_batch_size = atoi(env_val);
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
        uint64_t _pc = reg_from_bin(g_bin + 33 * 8); \
        fprintf(stderr, "\n=== MISMATCH batch=%ld icount=%ld prev_pc=0x%016lx qemu_pc=0x%016lx ===\n", \
                g_batch_count, env->icount, env->prev_pc, _pc); \
    } \
} while(0)

    /* ---- 1. GPRs (r0-r31) + PC (always compared) ---- */
    for (int i = 0; i < 32; i++) {
        uint64_t qemu_val = reg_from_bin(g_bin + i * 8);
        if (env->gpr[i] != qemu_val) {
            CMP_HDR(); mismatch++;
            fprintf(stderr, "  gpr %-4s: emu=0x%016lx  qemu=0x%016lx\n",
                    regnames[i], env->gpr[i], qemu_val);
        }
    }
    {
        uint64_t qemu_pc = reg_from_bin(g_bin + 33 * 8);
        if (env->pc != qemu_pc) {
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

    /* ---- 5. CSR dump (CMP_CSR) ---- */
    if (diffnet_cmp_mask & CMP_CSR) {
        uint8_t csr_buf[256];
        if (qrsp_read_memory(&g_qrsp, 0x1c000200, csr_buf, 176) == 176) {
            int nonzero = 0;
            for (int i = 0; i < 176; i++) { if (csr_buf[i]) { nonzero = 1; break; } }
            if (nonzero) {
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

    /* Batch mode: only sync at batch boundaries */
    if (g_batch_count % diffnet_batch_size != 0)
        return 0;

    /* Step QEMU batch_size times to catch up with emu */
    for (int i = 0; i < diffnet_batch_size; i++) {
        if (qrsp_step(&g_qrsp) < 0) {
            fprintf(stderr, "DIFFNET: batch step %d failed at batch=%ld\n", i, g_batch_count);
            return -1;
        }
    }

    uint8_t reg_buf[280];
    if (qrsp_read_g_packet(&g_qrsp, reg_buf, sizeof(reg_buf)) < 0) {
        fprintf(stderr, "DIFFNET: read g packet failed at batch=%ld\n", g_batch_count);
        return -1;
    }

    int mism = diffnet_compare(env, reg_buf);
    if (mism > 0)
        return -1;

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

        /* Use -gdb tcp::PORT for the GDB stub, -S to stop at entry */
        execlp(qemu_path, qemu_path,
               "-M", "virt",
               "-device", loader_arg,
               "-gdb", gdb_arg,
               "-S",
               "-nographic",
               "-monitor", "none",
               "-display", "none",
               (char *)NULL);

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
