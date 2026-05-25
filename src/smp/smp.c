#include "qemu/osdep.h"
#include "smp.h"

#include <unistd.h>

typedef struct LoongArchIPICore {
    uint32_t status;
    uint32_t en;
    uint32_t buf[8];
} LoongArchIPICore;

CPUState *first_cpu;
CPUState *current_cpu;

static LoongArchIPICore *ipi_cores;
static unsigned int ipi_num_cpus;
static uint64_t iocsr_misc[IOCSR_MEM_SIZE / sizeof(uint64_t)];
static unsigned int smp_quantum = 1024;

int exec_env(CPULoongArchState *env);
static void ipi_update_irq(CPUState *cpu);

static uint64_t extract_iocsr_value(uint64_t val, target_ulong addr,
                                    unsigned size)
{
    unsigned shift = (addr & 7) * 8;

    val >>= shift;
    switch (size) {
    case 1:
        return val & UINT8_MAX;
    case 2:
        return val & UINT16_MAX;
    case 4:
        return val & UINT32_MAX;
    case 8:
        return val;
    default:
        return 0;
    }
}

static uint64_t make_iocsr_mask(target_ulong addr, unsigned size)
{
    unsigned shift = (addr & 7) * 8;

    switch (size) {
    case 1:
        return (uint64_t)UINT8_MAX << shift;
    case 2:
        return (uint64_t)UINT16_MAX << shift;
    case 4:
        return (uint64_t)UINT32_MAX << shift;
    case 8:
        return UINT64_MAX;
    default:
        return 0;
    }
}

static void merge_u64_write(uint64_t *old, target_ulong addr, uint64_t val,
                            unsigned size)
{
    unsigned shift = (addr & 7) * 8;
    uint64_t mask = make_iocsr_mask(addr, size);

    *old = (*old & ~mask) | ((val << shift) & mask);
}

static void merge_u32_write(uint32_t *old, target_ulong addr, uint64_t val,
                            unsigned size)
{
    unsigned shift = (addr & 3) * 8;
    uint32_t mask;

    switch (size) {
    case 1:
        mask = UINT8_MAX << shift;
        break;
    case 2:
        mask = UINT16_MAX << shift;
        break;
    default:
        mask = UINT32_MAX;
        shift = 0;
        break;
    }

    *old = (*old & ~mask) | (((uint32_t)val << shift) & mask);
}

static uint32_t ipi_core_load_u32(LoongArchIPICore *core, unsigned off)
{
    unsigned index;

    switch (off) {
    case CORE_STATUS_OFF:
        return core->status;
    case CORE_EN_OFF:
        return core->en;
    case CORE_SET_OFF:
    case CORE_CLEAR_OFF:
        return 0;
    case CORE_BUF_20 ... CORE_BUF_38 + 4:
        index = (off - CORE_BUF_20) >> 2;
        return core->buf[index];
    default:
        qemu_log_mask(LOG_UNIMP, "invalid IOCSR IPI read: off=0x%x\n", off);
        return 0;
    }
}

static void ipi_core_store_u32(CPUState *cpu, LoongArchIPICore *core,
                               unsigned off, uint32_t val, unsigned size)
{
    unsigned index;
    unsigned reg_off = off & ~3U;
    uint32_t shifted_val = size == 4 ? val : val << ((off & 3) * 8);

    switch (reg_off) {
    case CORE_STATUS_OFF:
        break;
    case CORE_EN_OFF:
        merge_u32_write(&core->en, off, val, size);
        ipi_update_irq(cpu);
        break;
    case CORE_SET_OFF:
        core->status |= shifted_val;
        ipi_update_irq(cpu);
        break;
    case CORE_CLEAR_OFF:
        core->status &= ~shifted_val;
        ipi_update_irq(cpu);
        break;
    case CORE_BUF_20 ... CORE_BUF_38 + 4:
        index = (reg_off - CORE_BUF_20) >> 2;
        merge_u32_write(&core->buf[index], off, val, size);
        break;
    default:
        qemu_log_mask(LOG_UNIMP,
                      "invalid IOCSR IPI write: off=0x%x val=0x%x\n",
                      off, val);
        break;
    }
}

static CPUState *cpu_by_arch_id(uint32_t cpuid)
{
    CPUState *cpu;

    for (cpu = first_cpu; cpu; cpu = CPU_NEXT(cpu)) {
        if ((uint32_t)cpu->cpu_index == cpuid) {
            return cpu;
        }
    }

    return NULL;
}

static LoongArchIPICore *ipi_core_for_cpu(CPUState *cpu)
{
    if (!cpu || cpu->cpu_index < 0 ||
        (unsigned int)cpu->cpu_index >= ipi_num_cpus) {
        return NULL;
    }

    return &ipi_cores[cpu->cpu_index];
}

static uint64_t ipi_mailbox0(LoongArchIPICore *core)
{
    return ((uint64_t)core->buf[1] << 32) | core->buf[0];
}

static void ipi_update_irq(CPUState *cpu)
{
    LoongArchIPICore *core = ipi_core_for_cpu(cpu);
    bool pending;
    uint64_t entry;

    if (!core) {
        return;
    }

    pending = (core->status & core->en) != 0;
    loongarch_cpu_set_irq(cpu->env, IRQ_IPI, pending);
    if (pending) {
        cpu->halted = 0;
        if (!cpu->env->pc && ipi_mailbox0(core)) {
            cpu->env->pc = ipi_mailbox0(core);
        }
    }

    /*
     * Direct kernel boot has no QEMU-style secondary firmware loop to enable
     * IPI and poll mailbox0.  Treat an IPI sent to a parked secondary with a
     * non-zero mailbox0 as that firmware wakeup.
     */
    entry = ipi_mailbox0(core);
    if (!pending && cpu->halted && !cpu->env->pc && entry && core->status) {
        cpu->halted = 0;
        cpu->env->pc = entry;
    }
}

void cpu_register(CPUState *cpu)
{
    CPUState *tail;
    int max_index = -1;

    cpu->next_cpu = NULL;
    if (!first_cpu) {
        cpu->cpu_index = 0;
        first_cpu = cpu;
        return;
    }

    tail = first_cpu;
    while (tail->next_cpu) {
        if (tail->cpu_index > max_index) {
            max_index = tail->cpu_index;
        }
        tail = tail->next_cpu;
    }
    if (tail->cpu_index > max_index) {
        max_index = tail->cpu_index;
    }

    cpu->cpu_index = max_index + 1;
    tail->next_cpu = cpu;
}

CPUState *loongarch_cpu_by_index(int cpu_index)
{
    CPUState *cpu;

    for (cpu = first_cpu; cpu; cpu = CPU_NEXT(cpu)) {
        if (cpu->cpu_index == cpu_index) {
            return cpu;
        }
    }

    return NULL;
}

unsigned int loongarch_smp_cpu_count(void)
{
    unsigned int count = 0;
    CPUState *cpu;

    for (cpu = first_cpu; cpu; cpu = CPU_NEXT(cpu)) {
        count++;
    }

    return count;
}

void loongarch_smp_init_iocsr(void)
{
    unsigned int count = loongarch_smp_cpu_count();

    free(ipi_cores);
    ipi_num_cpus = count;
    ipi_cores = count ? calloc(count, sizeof(*ipi_cores)) : NULL;

    memset(iocsr_misc, 0, sizeof(iocsr_misc));
    iocsr_misc[VERSION_REG / 8] = 0x11;
    iocsr_misc[FEATURE_REG / 8] = (1ULL << IOCSRF_MSI) |
                                  (1ULL << IOCSRF_EXTIOI) |
                                  (1ULL << IOCSRF_CSRIPI);
    iocsr_misc[VENDOR_REG / 8] = 0x6e6f73676e6f6f4cULL;
    iocsr_misc[CPUNAME_REG / 8] = 0x303030354133ULL;
    iocsr_misc[MISC_FUNC_REG / 8] = 1ULL << IOCSRM_EXTIOI_EN;
}

static uint64_t ipi_core_read(CPUState *cpu, target_ulong addr, unsigned size)
{
    LoongArchIPICore *core = ipi_core_for_cpu(cpu);
    uint64_t val = 0;

    if (!core) {
        return 0;
    }

    for (unsigned i = 0; i < size; i++) {
        unsigned byte_off = (addr + i) & 0xff;
        unsigned reg_off = byte_off & ~3U;
        uint32_t reg = ipi_core_load_u32(core, reg_off);

        val |= ((uint64_t)((reg >> ((byte_off & 3) * 8)) & UINT8_MAX)) <<
               (i * 8);
    }

    return val;
}

static void ipi_core_write(CPUState *cpu, target_ulong addr, uint64_t val,
                           unsigned size)
{
    LoongArchIPICore *core = ipi_core_for_cpu(cpu);
    unsigned off = addr & 0xff;

    if (!core) {
        return;
    }

    if ((off & ~3U) == IOCSR_IPI_SEND) {
        uint32_t cpuid = (val >> 16) & 0x3ff;
        uint32_t vector = val & 0x1f;
        CPUState *target = cpu_by_arch_id(cpuid);

        if (target) {
            LoongArchIPICore *target_core = ipi_core_for_cpu(target);
            target_core->status |= 1U << vector;
            ipi_update_irq(target);
        }
        return;
    }

    for (unsigned i = 0; i < size; i++) {
        unsigned byte_off = (addr + i) & 0xff;
        uint32_t data = (val >> (i * 8)) & UINT8_MAX;

        ipi_core_store_u32(cpu, core, byte_off, data, 1);
    }
}

static void send_ipi_data(CPUState *target, target_ulong addr, uint64_t val)
{
    uint32_t mask = 0;
    uint32_t data = (uint32_t)(val >> 32);

    if ((val >> 27) & 0xf) {
        uint32_t old = (uint32_t)loongarch_iocsr_read(target->env, addr, 4);

        for (int i = 0; i < 4; i++) {
            if (val & (1ULL << (27 + i))) {
                mask |= 0xffU << (i * 8);
            }
        }

        data = (old & mask) | (data & ~mask);
    }

    loongarch_iocsr_write(target->env, addr, data, 4);
}

static void ipi_mail_send(uint64_t val)
{
    uint32_t cpuid = (val >> 16) & 0x3ff;
    CPUState *target = cpu_by_arch_id(cpuid);
    target_ulong addr = SMP_IPI_MAILBOX + CORE_BUF_20 + (val & 0x1c);

    if (target) {
        send_ipi_data(target, addr, val);
    }
}

static void ipi_any_send(uint64_t val)
{
    uint32_t cpuid = (val >> 16) & 0x3ff;
    CPUState *target = cpu_by_arch_id(cpuid);

    if (target) {
        send_ipi_data(target, val & 0xffff, val);
    }
}

uint64_t loongarch_iocsr_read(CPULoongArchState *env, target_ulong addr,
                              unsigned size)
{
    CPUState *cpu = env_cpu(env);

    if (addr < IOCSR_MEM_SIZE) {
        uint64_t val = iocsr_misc[(addr & ~7ULL) / 8];
        return extract_iocsr_value(val, addr, size);
    }

    if (addr >= SMP_IPI_MAILBOX && addr < SMP_IPI_MAILBOX + IOCSR_MAIL_SEND) {
        return ipi_core_read(cpu, addr - SMP_IPI_MAILBOX, size);
    }

    qemu_log_mask(LOG_UNIMP, "unknown IOCSR read: addr=0x%lx size=%u pc=0x%lx\n",
                  (uint64_t)addr, size, env->pc);
    return 0;
}

void loongarch_iocsr_write(CPULoongArchState *env, target_ulong addr,
                           uint64_t val, unsigned size)
{
    CPUState *cpu = env_cpu(env);

    if (addr < IOCSR_MEM_SIZE) {
        target_ulong base = addr & ~7ULL;

        if (base == MISC_FUNC_REG) {
            uint64_t new_val = iocsr_misc[MISC_FUNC_REG / 8];

            merge_u64_write(&new_val, addr, val, size);
            if (new_val & (1ULL << IOCSRM_EXTIOI_EN)) {
                iocsr_misc[MISC_FUNC_REG / 8] |= 1ULL << IOCSRM_EXTIOI_EN;
            }
            if (new_val & (1ULL << IOCSRM_EXTIOI_INT_ENCODE)) {
                iocsr_misc[MISC_FUNC_REG / 8] |=
                    1ULL << IOCSRM_EXTIOI_INT_ENCODE;
            }
        }
        return;
    }

    if (addr >= SMP_IPI_MAILBOX && addr < SMP_IPI_MAILBOX + IOCSR_MAIL_SEND) {
        ipi_core_write(cpu, addr - SMP_IPI_MAILBOX, val, size);
        return;
    }

    if (addr == MAIL_SEND_ADDR && size == 8) {
        ipi_mail_send(val);
        return;
    }

    if (addr == ANY_SEND_ADDR && size == 8) {
        ipi_any_send(val);
        return;
    }

    qemu_log_mask(LOG_UNIMP,
                  "unknown IOCSR write: addr=0x%lx size=%u val=0x%lx pc=0x%lx\n",
                  (uint64_t)addr, size, val, env->pc);
}

bool loongarch_smp_should_yield(CPULoongArchState *env)
{
    return loongarch_smp_cpu_count() > 1 &&
           smp_quantum != 0 &&
           (env->icount % smp_quantum) == 0;
}

CPULoongArchState *loongarch_smp_find_env_by_timerid(timer_t timerid)
{
    CPUState *cpu;

    for (cpu = first_cpu; cpu; cpu = CPU_NEXT(cpu)) {
        if (cpu->env && cpu->env->timerid == timerid) {
            return cpu->env;
        }
    }

    return NULL;
}

int loongarch_smp_run(void)
{
    while (1) {
        bool ran = false;

        for (CPUState *cpu = first_cpu; cpu; cpu = CPU_NEXT(cpu)) {
            if (cpu->halted) {
                continue;
            }

            ran = true;
            current_cpu = cpu;
            current_env = cpu->env;
            int ret = exec_env(cpu->env);
            if (ret != 3) {
                return ret;
            }
        }

        if (!ran) {
            usleep(1000);
        }
    }
}
