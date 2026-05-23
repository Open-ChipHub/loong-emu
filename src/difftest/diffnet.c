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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

bool diffnet_enabled;
int diffnet_max_steps = 500;

static qrsp_conn_t g_qrsp;
static pid_t g_child_pid = 0;

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
    return 0;
}

int diffnet_check(CPULoongArchState *env)
{
    if (diffnet_max_steps > 0 && env->icount >= diffnet_max_steps) {
        fprintf(stderr, "DIFFNET: reached step limit %d, exiting\n", diffnet_max_steps);
        laemu_exit(0);
    }
    if (qrsp_step(&g_qrsp) < 0) {
        fprintf(stderr, "DIFFNET: step failed at icount=%ld\n", env->icount);
        return -1;
    }

    uint8_t reg_buf[280];
    if (qrsp_read_g_packet(&g_qrsp, reg_buf, sizeof(reg_buf)) < 0) {
        fprintf(stderr, "DIFFNET: read g packet failed at icount=%ld\n", env->icount);
        return -1;
    }

    const uint8_t *bin = reg_buf;
    int mismatch = 0;

    for (int i = 0; i < 32; i++) {
        uint64_t qemu_val = reg_from_bin(bin + i * 8);
        if (env->gpr[i] != qemu_val) {
            if (mismatch == 0) {
                uint64_t qemu_pc_debug = reg_from_bin(bin + 33 * 8);
                fprintf(stderr,
                        "\n=== DIFFNET MISMATCH icount=%ld prev_pc=0x%016lx qemu_pc=0x%016lx ===\n",
                        env->icount, env->prev_pc, qemu_pc_debug);
            }
            fprintf(stderr, "  %-4s: emu=0x%016lx  qemu=0x%016lx\n",
                    regnames[i], env->gpr[i], qemu_val);
            mismatch++;
        }
    }

    uint64_t qemu_pc = reg_from_bin(bin + 33 * 8);
    if (env->pc != qemu_pc) {
        if (mismatch == 0) {
            fprintf(stderr,
                    "\n=== DIFFNET MISMATCH icount=%ld prev_pc=0x%016lx ===\n",
                    env->icount, env->prev_pc);
        }
        fprintf(stderr, "  PC  : emu=0x%016lx  qemu=0x%016lx\n",
                env->pc, qemu_pc);
        mismatch++;
    }

    if (mismatch > 0) {
        fprintf(stderr, "  (prev instruction at pc=0x%016lx: 0x%08x)\n",
                env->prev_pc, env->insn);
        return -1;
    }

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
