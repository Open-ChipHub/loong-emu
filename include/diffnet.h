/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Diffnet — Network-based differential testing against QEMU GDB stub
 */
#ifndef DIFFNET_H
#define DIFFNET_H

#include <stdbool.h>
#include <stdint.h>
#include "cpu.h"

/* Set by command-line -N flag, checked by exec_env(). */
extern bool diffnet_enabled;

/* Connect to QEMU at host:port. Sync initial GPRs and PC from QEMU.
 * Must be called after cpu_reset() and memory loading, before exec_env().
 * Returns 0 on success, -1 on failure. */
int diffnet_init(const char *host, int port, CPULoongArchState *env);

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
