/*
 * LOONGARCH gdb server stub
 *
 * Copyright (c) 2021 Loongson Technology Corporation Limited
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#include "qemu/osdep.h"
#include "cpu.h"
#include "internals.h"
#include "vec.h"
#include "../riscv64/cpu_bits.h"

static inline int gdb_get_reg32(GByteArray *buf, uint32_t val)
{
    uint32_t to_long = val;
    g_byte_array_append(buf, (uint8_t *) &to_long, 4);
    return 4;
}

static inline int gdb_get_reg64(GByteArray *buf, uint64_t val)
{
    uint64_t to_quad = val;
    g_byte_array_append(buf, (uint8_t *) &to_quad, 8);
    return 8;
}

static void riscv_cpu_set_pc(CPUState *cs, vaddr value)
{
    LoongArchCPU *cpu = RISCV_CPU(cs);
    CPULoongArchState *env = &cpu->env;

    if (env->xl == MXL_RV32) {
        env->pc = (int32_t)value;
    } else {
        env->pc = value;
    }
}


static vaddr riscv_cpu_get_pc(CPUState *cs)
{
    LoongArchCPU *cpu = RISCV_CPU(cs);
    CPULoongArchState *env = &cpu->env;

    /* Match cpu_get_tb_cpu_state. */
    if (env->xl == MXL_RV32) {
        return env->pc & UINT32_MAX;
    }
    return env->pc;
}

int riscv_cpu_gdb_read_register(CPUState *cs, GByteArray *mem_buf, int n)
{
    LoongArchCPU *cpu = RISCV_CPU(cs);
    CPULoongArchState *env = &cpu->env;
    target_ulong tmp;

    if (n < 32) {
        tmp = env->gpr[n];
    } else if (n == 32) {
        tmp = env->pc;
    } else {
        return 0;
    }

    switch (env->xl) {
    case MXL_RV32:
        return gdb_get_reg32(mem_buf, tmp);
    case MXL_RV64:
    case MXL_RV128:
        return gdb_get_reg64(mem_buf, tmp);
    default:
        g_assert_not_reached();
    }
    return 0;
}


int riscv_cpu_gdb_write_register(CPUState *cs, uint8_t *mem_buf, int n)
{
    LoongArchCPU *cpu = RISCV_CPU(cs);
    CPULoongArchState *env = &cpu->env;
    int length = 0;
    target_ulong tmp;

    switch (env->xl) {
    case MXL_RV32:
        tmp = (int32_t)ldl_he_p(mem_buf);
        length = 4;
        break;
    case MXL_RV64:
    case MXL_RV128:
        if (env->xl < MXL_RV64) {
            tmp = (int32_t)ldq_he_p(mem_buf);
        } else {
            tmp = ldq_he_p(mem_buf);
        }
        length = 8;
        break;
    default:
        g_assert_not_reached();
    }
    if (n > 0 && n < 32) {
        env->gpr[n] = tmp;
    } else if (n == 32) {
        env->pc = tmp;
    }

    return length;
}


static int riscv_gdb_get_fpu(CPUState *cs, GByteArray *buf, int n)
{
    LoongArchCPU *cpu = RISCV_CPU(cs);
    CPULoongArchState *env = &cpu->env;

    if (n < 32) {
        if (env->misa_ext & RVD) {
            return gdb_get_reg64(buf, env->fpr[n].vreg.D[0]);
        }
        if (env->misa_ext & RVF) {
            return gdb_get_reg32(buf, env->fpr[n].vreg.W[0]);
        }
    }
    return 0;
}

static int riscv_gdb_set_fpu(CPUState *cs, uint8_t *mem_buf, int n)
{
    LoongArchCPU *cpu = RISCV_CPU(cs);
    CPULoongArchState *env = &cpu->env;

    if (n < 32) {
        env->fpr[n].vreg.D[0] = ldq_he_p(mem_buf); /* always 64-bit */
        return sizeof(uint64_t);
    }
    return 0;
}

static int riscv_gdb_get_vector(CPUState *cs, GByteArray *buf, int n)
{
    LoongArchCPU *cpu = RISCV_CPU(cs);
    CPULoongArchState *env = &cpu->env;
    uint16_t vlenb = cpu->cfg.vlenb;
    if (n < 32) {
        int i;
        int cnt = 0;
        for (i = 0; i < vlenb; i += 8) {
            cnt += gdb_get_reg64(buf,
                                 env->vreg[(n * vlenb + i) / 8]);
        }
        return cnt;
    }

    return 0;
}

static int riscv_gdb_set_vector(CPUState *cs, uint8_t *mem_buf, int n)
{
    LoongArchCPU *cpu = RISCV_CPU(cs);
    CPULoongArchState *env = &cpu->env;
    uint16_t vlenb = cpu->cfg.vlenb;
    if (n < 32) {
        int i;
        for (i = 0; i < vlenb; i += 8) {
            env->vreg[(n * vlenb + i) / 8] = ldq_he_p(mem_buf + i);
        }
        return vlenb;
    }

    return 0;
}

/*

static int riscv_gdb_get_csr(CPUState *cs, GByteArray *buf, int n)
{
    LoongArchCPU *cpu = RISCV_CPU(cs);
    CPULoongArchState *env = &cpu->env;

    if (n < CSR_TABLE_SIZE) {
        target_ulong val = 0;
        int result;

        result = riscv_csrrw_debug(env, n, &val, 0, 0);
        if (result == RISCV_EXCP_NONE) {
            return gdb_get_regl(buf, val);
        }
    }
    return 0;
}

static int riscv_gdb_set_csr(CPUState *cs, uint8_t *mem_buf, int n)
{
    LoongArchCPU *cpu = RISCV_CPU(cs);
    CPULoongArchState *env = &cpu->env;

    if (n < CSR_TABLE_SIZE) {
        target_ulong val = ldtul_p(mem_buf);
        int result;

        result = riscv_csrrw_debug(env, n, NULL, val, -1);
        if (result == RISCV_EXCP_NONE) {
            return sizeof(target_ulong);
        }
    }
    return 0;
}

*/