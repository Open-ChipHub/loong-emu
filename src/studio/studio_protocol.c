#include "qemu/osdep.h"
#include "cpu.h"
#include "cpu-csr.h"
#include "studio_protocol.h"

typedef struct StudioMemoryAccess {
    const char *op;
    uint64_t pc;
    uint64_t address;
    uint64_t physical_address;
    int size;
    uint64_t value;
} StudioMemoryAccess;

static bool studio_enabled;
static bool studio_stop_requested;
static uint64_t studio_break_pcs[32];
static int studio_break_pc_count = -1;
static StudioMemoryAccess studio_memory_accesses[512];
static uint64_t studio_memory_access_count;
static int studio_memory_trace_enabled_cache = -1;
static long studio_max_instructions = -1;

#ifndef CONFIG_USER_ONLY
typedef struct StudioMemoryRange {
    uint64_t address;
    uint64_t length;
} StudioMemoryRange;

static StudioMemoryRange studio_memory_ranges[8];
static int studio_memory_range_count = -1;
#endif

void studio_protocol_set_enabled(bool enabled)
{
    studio_enabled = enabled;
}

bool studio_protocol_enabled(void)
{
    return studio_enabled;
}

static bool studio_memory_trace_enabled(void)
{
    const char *value;

    if (studio_memory_trace_enabled_cache >= 0) {
        return studio_memory_trace_enabled_cache != 0;
    }

    value = getenv("STUDIO_MEMORY_TRACE");
    studio_memory_trace_enabled_cache = value && value[0] && strcmp(value, "0") != 0;
    return studio_memory_trace_enabled_cache != 0;
}

void studio_protocol_record_memory_access(CPULoongArchState *env,
        const char *op, uint64_t address, uint64_t physical_address,
        int size, uint64_t value)
{
    uint64_t index;

    if (!studio_enabled || !studio_memory_trace_enabled()) {
        return;
    }

    index = studio_memory_access_count % ARRAY_SIZE(studio_memory_accesses);
    studio_memory_accesses[index] = (StudioMemoryAccess){
        .op = op,
        .pc = env->pc,
        .address = address,
        .physical_address = physical_address,
        .size = size,
        .value = value,
    };
    studio_memory_access_count++;
}

static void studio_init_breakpoints(void)
{
    char *copy;
    char *saveptr = NULL;
    char *token;
    const char *breakpoints;

    if (studio_break_pc_count >= 0) {
        return;
    }

    studio_break_pc_count = 0;
    breakpoints = getenv("STUDIO_BREAK_PCS");
    if (!breakpoints || !breakpoints[0]) {
        return;
    }

    copy = strdup(breakpoints);
    if (!copy) {
        return;
    }
    for (token = strtok_r(copy, ",", &saveptr);
         token && studio_break_pc_count < ARRAY_SIZE(studio_break_pcs);
         token = strtok_r(NULL, ",", &saveptr)) {
        studio_break_pcs[studio_break_pc_count++] = strtoull(token, NULL, 0);
    }
    free(copy);
}

bool studio_protocol_hit_breakpoint(CPULoongArchState *env)
{
    if (!studio_enabled) {
        return false;
    }

    studio_init_breakpoints();
    for (int i = 0; i < studio_break_pc_count; i++) {
        if (env->pc == studio_break_pcs[i]) {
            return true;
        }
    }
    return false;
}

bool studio_protocol_reached_instruction_limit(CPULoongArchState *env)
{
    const char *value;

    if (!studio_enabled) {
        return false;
    }
    if (studio_max_instructions < 0) {
        value = getenv("EMU_MAX_INSTRS");
        studio_max_instructions = value ? strtol(value, NULL, 0) : 0;
    }
    return studio_max_instructions > 0 && env->icount >= (uint64_t)studio_max_instructions;
}

void studio_protocol_request_stop(void)
{
    studio_stop_requested = true;
}

bool studio_protocol_stop_requested(void)
{
    return studio_enabled && studio_stop_requested;
}

#ifndef CONFIG_USER_ONLY
static void studio_init_memory_ranges(void)
{
    char *copy;
    char *saveptr = NULL;
    char *token;
    const char *ranges;

    if (studio_memory_range_count >= 0) {
        return;
    }

    studio_memory_range_count = 0;
    ranges = getenv("STUDIO_MEMORY_RANGES");
    if (!ranges || !ranges[0]) {
        return;
    }

    copy = strdup(ranges);
    if (!copy) {
        return;
    }
    for (token = strtok_r(copy, ",", &saveptr);
         token && studio_memory_range_count < ARRAY_SIZE(studio_memory_ranges);
         token = strtok_r(NULL, ",", &saveptr)) {
        char *separator = strchr(token, ':');
        uint64_t length;

        if (!separator) {
            continue;
        }
        *separator = '\0';
        length = strtoull(separator + 1, NULL, 0);
        if (length == 0) {
            continue;
        }
        studio_memory_ranges[studio_memory_range_count++] = (StudioMemoryRange){
            .address = strtoull(token, NULL, 0),
            .length = MIN(length, 0x10000),
        };
    }
    free(copy);
}
#endif

static void studio_dump_register(FILE *out, const char *prefix,
        const char *name, uint64_t value)
{
    fprintf(out, "%s{\"name\":\"%s\",\"value\":\"0x%016lx\"}",
            prefix, name, (unsigned long)value);
}

static void studio_dump_memory_ranges(FILE *out)
{
    fprintf(out, ",\"memory\":[");
#ifndef CONFIG_USER_ONLY
    studio_init_memory_ranges();
    for (int i = 0; i < studio_memory_range_count; i++) {
        StudioMemoryRange *range = &studio_memory_ranges[i];
        bool valid = addr_range_in_ram(range->address, range->length);

        fprintf(out, "%s{\"address\":\"0x%016lx\",\"length\":%lu,\"valid\":%s,\"bytes\":\"",
                i == 0 ? "" : ",", (unsigned long)range->address,
                (unsigned long)range->length, valid ? "true" : "false");
        if (valid) {
            for (uint64_t offset = 0; offset < range->length; offset++) {
                fprintf(out, "%02x", (unsigned int)ram_ldub(range->address + offset));
            }
        }
        fprintf(out, "\"}");
    }
#endif
    fprintf(out, "]");
}

static void studio_dump_memory_accesses(FILE *out)
{
    uint64_t stored = MIN(studio_memory_access_count, ARRAY_SIZE(studio_memory_accesses));
    uint64_t start = studio_memory_access_count > ARRAY_SIZE(studio_memory_accesses)
        ? studio_memory_access_count - ARRAY_SIZE(studio_memory_accesses) : 0;

    fprintf(out, ",\"memoryAccesses\":[");
    for (uint64_t i = 0; i < stored; i++) {
        StudioMemoryAccess *access = &studio_memory_accesses[(start + i) % ARRAY_SIZE(studio_memory_accesses)];
        fprintf(out, "%s{\"op\":\"%s\",\"pc\":\"0x%016lx\",\"address\":\"0x%016lx\",\"physicalAddress\":\"0x%016lx\",\"size\":%d,\"value\":\"0x%016lx\"}",
                i == 0 ? "" : ",", access->op, (unsigned long)access->pc,
                (unsigned long)access->address, (unsigned long)access->physical_address,
                access->size, (unsigned long)access->value);
    }
    fprintf(out, "]");
}

static void studio_dump_csr_state(FILE *out, CPULoongArchState *env)
{
    fprintf(out, ",\"csr\":[");
    studio_dump_register(out, "", "CRMD", env->CSR_CRMD);
    studio_dump_register(out, ",", "PRMD", env->CSR_PRMD);
    studio_dump_register(out, ",", "EUEN", env->CSR_EUEN);
    studio_dump_register(out, ",", "ECFG", env->CSR_ECFG);
    studio_dump_register(out, ",", "ESTAT", env->CSR_ESTAT);
    studio_dump_register(out, ",", "ERA", env->CSR_ERA);
    studio_dump_register(out, ",", "BADV", env->CSR_BADV);
    studio_dump_register(out, ",", "BADI", env->CSR_BADI);
    studio_dump_register(out, ",", "EENTRY", env->CSR_EENTRY);
    studio_dump_register(out, ",", "ASID", env->CSR_ASID);
    studio_dump_register(out, ",", "SAVE0", env->CSR_SAVE[0]);
    studio_dump_register(out, ",", "TID", env->CSR_TID);
    studio_dump_register(out, ",", "TCFG", env->CSR_TCFG);
    studio_dump_register(out, ",", "TVAL", env->CSR_TVAL);
    studio_dump_register(out, ",", "LLBCTL", env->CSR_LLBCTL);
    fprintf(out, "]");
}

static void studio_dump_fpu_state(FILE *out, CPULoongArchState *env)
{
    fprintf(out, ",\"fpu\":{\"fcsr\":[{\"name\":\"FCSR0\",\"value\":\"0x%08x\"},{\"name\":\"FCSR0_MASK\",\"value\":\"0x%08x\"}],\"registers\":[",
            env->fcsr0, env->fcsr0_mask);
    for (int i = 0; i < 32; i++) {
        fprintf(out, "%s{\"name\":\"f%d/v%d\",\"value\":\"0x%016lx_%016lx\"}",
                i == 0 ? "" : ",", i, i,
                (unsigned long)env->fpr[i].vreg.UD[1],
                (unsigned long)env->fpr[i].vreg.UD[0]);
    }
    fprintf(out, "]}");
}

static void studio_dump_mmu_state(FILE *out, CPULoongArchState *env)
{
    fprintf(out, ",\"mmu\":{\"state\":[");
    studio_dump_register(out, "", "TLBIDX", env->CSR_TLBIDX);
    studio_dump_register(out, ",", "TLBEHI", env->CSR_TLBEHI);
    studio_dump_register(out, ",", "TLBELO0", env->CSR_TLBELO0);
    studio_dump_register(out, ",", "TLBELO1", env->CSR_TLBELO1);
    studio_dump_register(out, ",", "PGDL", env->CSR_PGDL);
    studio_dump_register(out, ",", "PGDH", env->CSR_PGDH);
    studio_dump_register(out, ",", "PGD", env->CSR_PGD);
    fprintf(out, "],\"tlb\":[");
#ifndef CONFIG_USER_ONLY
    int emitted = 0;
    for (int i = 0; i < LOONGARCH_TLB_MAX && emitted < 24; i++) {
        bool valid = FIELD_EX64(env->tlb[i].tlb_misc, TLB_MISC, E);
        if (!valid && emitted >= 8) {
            continue;
        }
        fprintf(out, "%s{\"index\":%d,\"valid\":%s,\"vppn\":\"0x%016lx\",\"ps\":%lu,\"asid\":%lu,\"global\":%s,\"entry0\":\"0x%016lx\",\"entry1\":\"0x%016lx\"}",
                emitted == 0 ? "" : ",", i, valid ? "true" : "false",
                (unsigned long)FIELD_EX64(env->tlb[i].tlb_misc, TLB_MISC, VPPN),
                (unsigned long)FIELD_EX64(env->tlb[i].tlb_misc, TLB_MISC, PS),
                (unsigned long)FIELD_EX64(env->tlb[i].tlb_misc, TLB_MISC, ASID),
                FIELD_EX64(env->tlb[i].tlb_misc, TLB_MISC, G) ? "true" : "false",
                (unsigned long)env->tlb[i].tlb_entry0,
                (unsigned long)env->tlb[i].tlb_entry1);
        emitted++;
    }
#endif
    fprintf(out, "]}");
}

void studio_protocol_dump_final_state(CPULoongArchState *env)
{
    CPUState *cs = env_cpu(env);

    fprintf(stdout, "STUDIO_FINAL_STATE {\"pc\":\"0x%016lx\",\"icount\":%lu,\"gpr\":[",
            (unsigned long)env->pc, (unsigned long)env->icount);
    for (int i = 0; i < 32; i++) {
        fprintf(stdout, "%s{\"name\":\"%s\",\"value\":\"0x%016lx\"}",
                i == 0 ? "" : ",", regnames[i], (unsigned long)env->gpr[i]);
    }
    fprintf(stdout, "]");
    studio_dump_csr_state(stdout, env);
    studio_dump_fpu_state(stdout, env);
    studio_dump_mmu_state(stdout, env);
    fprintf(stdout, ",\"exception\":{\"current\":[{\"name\":\"current\",\"value\":\"%d\"},{\"name\":\"ESTAT.ECODE\",\"value\":\"0x%02lx\"},{\"name\":\"ERA\",\"value\":\"0x%016lx\"},{\"name\":\"BADV\",\"value\":\"0x%016lx\"}],\"counters\":[]}",
            cs->exception_index,
            (unsigned long)FIELD_EX64(env->CSR_ESTAT, CSR_ESTAT, ECODE),
            (unsigned long)env->CSR_ERA, (unsigned long)env->CSR_BADV);
    studio_dump_memory_ranges(stdout);
    studio_dump_memory_accesses(stdout);
    fprintf(stdout, ",\"vga\":null}\n");
    fflush(stdout);
}
