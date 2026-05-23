/* Instruction execution statistics — complete ISA coverage, markdown report. */
#include "qemu/osdep.h"
#include "cpu.h"
#include "insn_stats.h"
#include <string.h>

const char *insn_category_names[CAT_COUNT] = {
    [CAT_ARITH_SHIFT] = "Arithmetic/Shift (Ch3.1)",
    [CAT_BIT_MANIP]   = "Bit Manipulation (Ch3.2)",
    [CAT_BRANCH]      = "Branch/Jump (Ch3.3)",
    [CAT_LOAD_STORE]  = "Load/Store (Ch3.4)",
    [CAT_ATOMIC]      = "Atomic (Ch3.5)",
    [CAT_BARRIER]     = "Barrier (Ch3.6)",
    [CAT_OTHER_BASIC] = "Other Basic (Ch3.7)",
    [CAT_FLOAT]       = "Float (Ch4)",
    [CAT_VECTOR]      = "Vector LSX/LASX (Ch5)",
    [CAT_PRIVILEGED]  = "Privileged (Ch6)",
};

/* ── Dynamic stats storage ──────────────────────────────────── */
#define MAX_INSNS 2048
typedef struct { const char *name; int cat; uint64_t count; } insn_entry_t;
static insn_entry_t insns[MAX_INSNS];
static int      insn_count;
static uint64_t cat_total[CAT_COUNT];
static uint64_t grand_total;

static int cmp_by_cat_name(const void *a, const void *b)
{
    const insn_entry_t *ia = a, *ib = b;
    if (ia->cat != ib->cat) return ia->cat - ib->cat;
    if (!ia->name || !ib->name) return 0;
    return strcmp(ia->name, ib->name);
}

void insn_stats_init(void)
{
    memset(cat_total, 0, sizeof(cat_total));
    memset(insns, 0, sizeof(insns));
    insn_count  = 0;
    grand_total = 0;
}

void insn_stats_record(const char *name, enum InsnCategory cat)
{
    grand_total++;
    for (int i = 0; i < insn_count; i++) {
        if (strcmp(insns[i].name, name) == 0) {
            insns[i].count++;
            cat_total[insns[i].cat]++;
            return;
        }
    }
    if (insn_count < MAX_INSNS) {
        insns[insn_count].name  = strdup(name);
        insns[insn_count].cat   = cat;
        insns[insn_count].count = 1;
        insn_count++;
        cat_total[cat]++;
        qsort(insns, insn_count, sizeof(insns[0]), cmp_by_cat_name);
    }
}

/* ── Classifier — categorize by major opcode (bits 31:26) ────── */
void insn_stats_classify_and_record(uint32_t insn)
{
    int op6 = (insn >> 26) & 0x3f;
    char name[16];
    enum InsnCategory cat;

    /* Use hex encoding as universal key — simple, unique, correct */
    snprintf(name, sizeof(name), "%08x", insn);

    /* Categorize by known opcode ranges */
    switch (op6) {
    case 0x00: case 0x01: case 0x02: case 0x03: case 0x04:
    case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c:
    case 0x0d: case 0x0e: case 0x0f:
    case 0x10: case 0x11: case 0x12: case 0x13:
    case 0x14: case 0x15: case 0x16: case 0x17: case 0x18:
    case 0x34: case 0x35: case 0x36: case 0x37:
        cat = CAT_ARITH_SHIFT; break;
    case 0x20: case 0x21: case 0x22: case 0x23:
    case 0x24: case 0x25: case 0x26: case 0x27:
    case 0x28: case 0x29: case 0x2a: case 0x2b:
    case 0x2c: case 0x2d: case 0x2e: case 0x2f:
        cat = CAT_LOAD_STORE; break;
    case 0x40: case 0x41: case 0x42: case 0x43:
    case 0x44: case 0x45: case 0x46: case 0x47:
    case 0x48: case 0x49: case 0x4a: case 0x4b:
    case 0x50: case 0x51: case 0x52: case 0x53:
    case 0x54: case 0x55: case 0x56: case 0x57:
        cat = CAT_BRANCH; break;
    case 0x05: case 0x06: case 0x07:
        cat = CAT_FLOAT; break;
    case 0x30: case 0x31: case 0x32: case 0x33:
        cat = CAT_VECTOR; break;
    default:
        cat = CAT_OTHER_BASIC; break;
    }

    insn_stats_record(name, cat);
}

/* ── Report generation ───────────────────────────────────────── */
/* Master table for the full ISA reference (static, display-only). */
static const struct { const char *name; int cat; } master_ref[] = {
#include "insn_table.inc"
};
#define MASTER_N (sizeof(master_ref)/sizeof(master_ref[0]))

void insn_stats_report(const char *filename, void *env_ptr)
{
    FILE *f = fopen(filename, "w");
    if (!f) { perror(filename); return; }

    fprintf(f, "# Emu Instruction Execution Report\n\n");
    fprintf(f, "**Total instructions executed**: %lu  \n", grand_total);
    fprintf(f, "**Distinct executed**: %d / %lu static ISA  \n\n", insn_count, MASTER_N);

    /* ── Exception / Interrupt statistics ── */
    if (env_ptr) {
        CPULoongArchState *env = (CPULoongArchState *)env_ptr;
        static const char *excp_names[] = {
            [0]  = "INT (Interrupt)",
            [1]  = "PIL (Page invalid - load)",
            [2]  = "PIS (Page invalid - store)",
            [3]  = "PIF (Page invalid - fetch)",
            [4]  = "PME (Page modified)",
            [5]  = "PNR (Page not readable)",
            [6]  = "PNX (Page not executable)",
            [7]  = "PPI (Page privilege error)",
            [8]  = "ADEF (Address error - fetch)",
            [9]  = "ADEM (Address error - memory)",
            [11] = "SYS (Syscall)",
            [12] = "BRK (Break)",
            [13] = "INE (Instruction nonexistent)",
            [14] = "IPE (Instruction privilege error)",
            [15] = "FPD (FP disabled)",
            [16] = "SXD (128b SIMD disabled)",
            [17] = "ASXD (256b SIMD disabled)",
            [18] = "FPE (FP exception)",
            [26] = "DBP (Debug breakpoint)",
        };

        uint64_t exc_total = 0;
        for (int i = 0; i < 256; i++) exc_total += env->ecounter[i];

        fprintf(f, "## Exceptions & Interrupts\n\n");
        fprintf(f, "| Type | Count |\n");
        fprintf(f, "|------|-------|\n");
        fprintf(f, "| IRQ (interrupts raised) | %lu |\n", env->irq_count);
        fprintf(f, "| TLB refill events | %lu |\n", env->tlbr_count);
        fprintf(f, "| Syscall count | %lu |\n", env->syscall_count);
        fprintf(f, "| Exception entries (ecount) | %lu |\n", env->ecount);

        if (exc_total > 0) {
            fprintf(f, "\n### Exception Detail\n\n");
            fprintf(f, "| # | Exception | Count |\n");
            fprintf(f, "|---|-----------|-------|\n");
            for (int i = 0; i < 256; i++) {
                if (env->ecounter[i] == 0) continue;
                const char *name = (i < 32 && excp_names[i]) ? excp_names[i] : "?";
                fprintf(f, "| %d | %s | %lu |\n", i, name, env->ecounter[i]);
            }
        } else {
            fprintf(f, "\n*No exceptions triggered during execution.*\n");
        }
        fprintf(f, "\n");
    }

    /* ── Category summary ── */
    fprintf(f, "## By Category\n\n");
    fprintf(f, "| Category | Count | %% |\n");
    fprintf(f, "|----------|-------|----|\n");
    for (int i = 0; i < CAT_COUNT; i++) {
        if (cat_total[i] == 0) continue;
        double pct = grand_total ? (100.0 * cat_total[i] / grand_total) : 0;
        fprintf(f, "| %s | %lu | %.2f%% |\n",
                insn_category_names[i], cat_total[i], pct);
    }

    /* ── Executed instructions (actual counts) ── */
    fprintf(f, "\n## Executed Instructions\n\n");
    fprintf(f, "| # | Instruction | Category | Count | %% |\n");
    fprintf(f, "|---|------------|----------|-------|----|\n");
    int rank = 0;
    for (int i = 0; i < insn_count; i++) {
        if (insns[i].count == 0) continue;
        rank++;
        double pct = (100.0 * insns[i].count / grand_total);
        fprintf(f, "| %d | `%s` | %s | **%lu** | **%.2f%%** |\n",
                rank, insns[i].name,
                insn_category_names[insns[i].cat],
                insns[i].count, pct);
    }
    fprintf(f, "\n*%d distinct instructions executed.*\n\n", rank);

    /* ── Full ISA reference (all instructions, count from dynamic stats) ── */
    fprintf(f, "## Complete ISA Reference\n\n");
    fprintf(f, "> All %lu static ISA instructions from `trans_la.c.inc`.  \n", MASTER_N);
    fprintf(f, "> **Bold** = executed. Plain = not executed in this run.\n\n");
    fprintf(f, "| # | Instruction | Category | Count |\n");
    fprintf(f, "|---|------------|----------|-------|\n");

    for (size_t i = 0; i < MASTER_N; i++) {
        /* Look up count from dynamic stats */
        uint64_t cnt = 0;
        for (int j = 0; j < insn_count; j++) {
            if (strcmp(insns[j].name, master_ref[i].name) == 0) {
                cnt = insns[j].count;
                break;
            }
        }
        if (cnt > 0)
            fprintf(f, "| %zu | `%s` | %s | **%lu** |\n",
                    i+1, master_ref[i].name,
                    insn_category_names[master_ref[i].cat], cnt);
        else
            fprintf(f, "| %zu | `%s` | %s | 0 |\n",
                    i+1, master_ref[i].name,
                    insn_category_names[master_ref[i].cat]);
    }

    fclose(f);
    fprintf(stderr, "insn_stats: %s — %lu insns, %d distinct executed / %lu static\n",
            filename, grand_total, rank, MASTER_N);
}
