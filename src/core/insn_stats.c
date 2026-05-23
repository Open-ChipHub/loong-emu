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
    [CAT_LSX]         = "Vector LSX (Ch5)",
    [CAT_LASX]        = "Vector LASX (Ch5)",
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

/* ── Classifier — use name from decoder if available, else hex ── */
void insn_stats_classify_and_record(uint32_t insn, const char *name)
{
    char buf[32];
    enum InsnCategory cat;

    if (name)
        snprintf(buf, sizeof(buf), "%s", name);
    else
        snprintf(buf, sizeof(buf), "%08x", insn);

    /* Categorize: prefer name-based when available, fall back to opcode */
    if (name) {
        const char *n = name;
        /* Float: all f-prefixed (fadd, fsub, flogb, fsqrt, fsel, fld, fst, ...) */
        if (n[0] == 'f')
            cat = CAT_FLOAT;
        /* Vector: v-prefix = LSX, xv-prefix = LASX */
        else if (n[0] == 'v')
            cat = CAT_LSX;
        else if (strncmp(n, "xv", 2) == 0)
            cat = CAT_LASX;
        /* Load/store: ld_* / st_* (exclude lddir/ldpte which are privileged) */
        else if ((strncmp(n, "ld", 2) == 0 && strncmp(n, "ldd", 3) != 0 && strncmp(n, "ldp", 3) != 0) ||
                 (strncmp(n, "st", 2) == 0) ||
                 strncmp(n, "ll_", 3) == 0 || strncmp(n, "sc_", 3) == 0 ||
                 strncmp(n, "lld", 3) == 0 || strncmp(n, "preld", 5) == 0)
            cat = CAT_LOAD_STORE;
        /* Branch */
        else if (strncmp(n, "beq", 3) == 0 || strncmp(n, "bne", 3) == 0 ||
                 strncmp(n, "blt", 3) == 0 || strncmp(n, "bge", 3) == 0 ||
                 strncmp(n, "bl", 2) == 0 || n[0] == 'b' ||
                 strncmp(n, "jirl", 4) == 0)
            cat = CAT_BRANCH;
        /* Barrier */
        else if (strcmp(n, "dbar") == 0 || strcmp(n, "ibar") == 0)
            cat = CAT_BARRIER;
        /* Atomic: all am* instructions (amadd, amand, ammax, ammin, amor, amswap, amxor) */
        else if (strncmp(n, "am", 2) == 0)
            cat = CAT_ATOMIC;
        /* Privileged */
        else if (strncmp(n, "csr", 3) == 0 || strncmp(n, "iocsr", 5) == 0 ||
                 strncmp(n, "tlb", 3) == 0 || strncmp(n, "ertn", 4) == 0 ||
                 strcmp(n, "idle") == 0 || strncmp(n, "invtlb", 6) == 0 ||
                 strncmp(n, "lddir", 5) == 0 || strncmp(n, "ldpte", 5) == 0 ||
                 strcmp(n, "cacop") == 0 || strcmp(n, "cpucfg") == 0)
            cat = CAT_PRIVILEGED;
        else
            cat = CAT_ARITH_SHIFT;
    } else {
        int op6 = (insn >> 26) & 0x3f;
        if (op6 >= 0x20 && op6 <= 0x2f) cat = CAT_LOAD_STORE;
        else if (op6 >= 0x40 && op6 <= 0x57) cat = CAT_BRANCH;
        else cat = CAT_OTHER_BASIC;
    }

    insn_stats_record(buf, cat);
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

    /* ── Full ISA reference, grouped by category, with executed counts ── */
    fprintf(f, "## Complete ISA Reference\n\n");
    fprintf(f, "> All %lu static ISA instructions grouped by category.  \n", MASTER_N);
    fprintf(f, "> Count column shows executed count (from dynamic stats).\n\n");

    int total_refs = 0;
    for (int c = 0; c < CAT_COUNT; c++) {
        /* Count entries in this category */
        int cat_entries = 0, cat_executed = 0;
        for (size_t i = 0; i < MASTER_N; i++)
            if (master_ref[i].cat == c) cat_entries++;
        if (cat_entries == 0) continue;

        fprintf(f, "### %s\n\n", insn_category_names[c]);
        fprintf(f, "| # | Instruction | Count |\n");
        fprintf(f, "|---|------------|-------|\n");

        int idx = 0;
        for (size_t i = 0; i < MASTER_N; i++) {
            if (master_ref[i].cat != c) continue;
            idx++;
            /* Look up count from dynamic stats */
            uint64_t cnt = 0;
            for (int j = 0; j < insn_count; j++) {
                if (strcmp(insns[j].name, master_ref[i].name) == 0) {
                    cnt = insns[j].count; break;
                }
            }
            if (cnt > 0) {
                fprintf(f, "| %d | `%s` | **%lu** |\n", idx, master_ref[i].name, cnt);
                cat_executed++;
            } else {
                fprintf(f, "| %d | `%s` | 0 |\n", idx, master_ref[i].name);
            }
        }
        fprintf(f, "\n*%d/%d executed in this category.*\n\n", cat_executed, cat_entries);
        total_refs += cat_entries;
    }

    fclose(f);
    fprintf(stderr, "insn_stats: %s — %lu insns, %d distinct executed / %lu static\n",
            filename, grand_total, rank, MASTER_N);
}
