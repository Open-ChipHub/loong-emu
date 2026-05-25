#ifndef LOONGARCH_SMP_H
#define LOONGARCH_SMP_H

#include "cpu.h"

#define SMP_IPI_MAILBOX         0x1000ULL

#define CORE_STATUS_OFF         0x0
#define CORE_EN_OFF             0x4
#define CORE_SET_OFF            0x8
#define CORE_CLEAR_OFF          0xc
#define CORE_BUF_20             0x20
#define CORE_BUF_28             0x28
#define CORE_BUF_30             0x30
#define CORE_BUF_38             0x38
#define IOCSR_IPI_SEND          0x40
#define IOCSR_MAIL_SEND         0x48
#define IOCSR_ANY_SEND          0x158

#define MAIL_SEND_ADDR          (SMP_IPI_MAILBOX + IOCSR_MAIL_SEND)
#define ANY_SEND_ADDR           (SMP_IPI_MAILBOX + IOCSR_ANY_SEND)

void cpu_register(CPUState *cpu);
CPUState *loongarch_cpu_by_index(int cpu_index);
unsigned int loongarch_smp_cpu_count(void);

void loongarch_smp_init_iocsr(void);
uint64_t loongarch_iocsr_read(CPULoongArchState *env, target_ulong addr,
                              unsigned size);
void loongarch_iocsr_write(CPULoongArchState *env, target_ulong addr,
                           uint64_t val, unsigned size);

bool loongarch_smp_should_yield(CPULoongArchState *env);
int loongarch_smp_run(void);
CPULoongArchState *loongarch_smp_find_env_by_timerid(timer_t timerid);

#endif
