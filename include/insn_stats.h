/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Instruction execution statistics — per-instruction and per-category counters.
 * Categories follow the LoongArch ISA manual chapter structure.
 */
#ifndef INSN_STATS_H
#define INSN_STATS_H

#include <stdint.h>
#include <stdbool.h>

/* ISA manual chapters */
enum InsnCategory {
    CAT_ARITH_SHIFT = 0,   /* Ch3.1 算术运算/移位 */
    CAT_BIT_MANIP,         /* Ch3.2 位操作 */
    CAT_BRANCH,            /* Ch3.3 控制转移 */
    CAT_LOAD_STORE,        /* Ch3.4 访存 */
    CAT_ATOMIC,            /* Ch3.5 原子 */
    CAT_BARRIER,           /* Ch3.6 屏障 */
    CAT_OTHER_BASIC,       /* Ch3.7 其他基础 */
    CAT_FLOAT,             /* Ch4   浮点 */
    CAT_VECTOR,            /* Ch5   向量 (LSX/LASX) */
    CAT_PRIVILEGED,        /* Ch6   特权 */
    CAT_COUNT
};

extern const char *insn_category_names[CAT_COUNT];

/* Per-instruction counter. max 256 tracked distinct instructions. */
#define INSN_NAME_MAX 16
typedef struct {
    char     name[INSN_NAME_MAX];
    uint64_t count;
} InsnCounter;

void insn_stats_init(void);
void insn_stats_record(const char *name, enum InsnCategory cat);
void insn_stats_classify_and_record(uint32_t insn);
void insn_stats_report(const char *filename, void *env);

#endif
