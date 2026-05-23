/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Diffnet — Network-based differential testing against QEMU GDB stub
 */
#ifndef DIFFNET_H
#define DIFFNET_H

#include <stdbool.h>
#include <stdint.h>
#include "cpu.h"

/* Default QEMU binary for diffnet_spawn() */
#ifndef DIFFNET_QEMU_PATH
#define DIFFNET_QEMU_PATH "/home/airxs/user/qemu/qemu-10.1.2/build/qemu-system-loongarch64"
#endif

/* Set by command-line -N flag, checked by exec_env(). */
extern bool diffnet_enabled;

/* Connect to an already-running QEMU at host:port.
 * Returns 0 on success, -1 on failure. */
int diffnet_init(const char *host, int port, CPULoongArchState *env);

/* Spawn QEMU as a child process, wait for its GDB stub, then connect.
 * qemu_path: path to qemu-system-loongarch64 binary
 * kernel:    test binary loaded via -device loader
 * load_addr: load address (entry point) for the test binary
 * port:      GDB stub TCP port
 * env:       CPU state (GPRs synced from QEMU on success)
 * Returns 0 on success, -1 on failure. */
int diffnet_spawn(const char *qemu_path, const char *kernel,
                  uint64_t load_addr, int port, CPULoongArchState *env);

/* Called after each instruction in exec_env().
 * Steps QEMU by 1 instruction, reads its GPRs + PC, compares with env.
 * Prints mismatches to stderr. Returns 0 on match, -1 on mismatch. */
int diffnet_check(CPULoongArchState *env);

/* Called on the exception/interrupt path to keep QEMU in sync.
 * Steps QEMU but does NOT compare registers. */
int diffnet_step_exception(CPULoongArchState *env);

/* Disconnect from QEMU. */
void diffnet_cleanup(void);

#endif /* DIFFNET_H */
