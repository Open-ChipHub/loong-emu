#include "qemu/osdep.h"
#include "cpu.h"
#include "internals.h"
#include "tcg/tcg-gvec-desc.h"
#include "fpu/softfloat.h"

#if defined(CONFIG_USER_ONLY)
#include "user.h"
#endif
#ifdef CONFIG_INSN_STATS
#include "insn_stats.h"
#endif
#include "studio_protocol.h"

#include "util.h"
#include "smp.h"

#define g_assert_not_reached abort
#define g_assert assert

#ifndef CONFIG_DIFF
static inline long long la_get_tval(CPULoongArchState *env){
    if (determined) {
        return current_env->icount / TIME_SCALE;
    } else {
        return nano_second() / TIMER_PERIOD;
    }
}
#endif

#ifndef CONFIG_USER_ONLY

#define CHECK_FPE(bytes)                                                                                              \
    do {                                                                                                              \
        if (bytes == 8) {                                                                                             \
            if (!FIELD_EX32(env->cpucfg[2], CPUCFG2, FP)) {return false;};                                            \
            if (!FIELD_EX64(env->CSR_EUEN, CSR_EUEN, FPE)) {do_raise_exception(env, EXCCODE_FPD, 0); return true;}    \
            PERF_INC(COUNTER_INST_FP);                                                                                \
        } else if (bytes == 16) {                                                                                     \
            if (!FIELD_EX32(env->cpucfg[2], CPUCFG2, LSX)) {return false;};                                           \
            if (!FIELD_EX64(env->CSR_EUEN, CSR_EUEN, SXE)) {do_raise_exception(env, EXCCODE_SXD, 0); return true;}    \
            PERF_INC(COUNTER_INST_LSX);                                                                               \
        } else if (bytes == 32) {                                                                                     \
            if (!FIELD_EX32(env->cpucfg[2], CPUCFG2, LASX)) {return false;};                                          \
            if (!FIELD_EX64(env->CSR_EUEN, CSR_EUEN, ASXE)) {do_raise_exception(env, EXCCODE_ASXD, 0); return true;}  \
            PERF_INC(COUNTER_INST_LASX);                                                                              \
        } else {lsassert(0);};                                                                                        \
    } while (0)

#define CHECK_PLV(plv)                                                                                               \
    do {                                                                                                             \
        if (FIELD_EX64(env->CSR_CRMD, CSR_CRMD, PLV) != plv) {do_raise_exception(env, EXCCODE_IPE, 0); return true;} \
    } while (0)

#define CHECK_LA64()                                                                                                 \
    do {                                                                                                             \
        if (!is_la64(env)) {do_raise_exception(env, EXCCODE_INE, 0); return true;}                                   \
    } while (0)

#define CHECK_LA32S()                                                                                                \
    do {                                                                                                             \
        if (is_la32r(env)) {do_raise_exception(env, EXCCODE_INE, 0); return true;}                                   \
    } while (0)

#else
#define CHECK_FPE(bytes)                                                                                              \
    do {                                                                                                              \
        if (bytes == 8) {                                                                                             \
            PERF_INC(COUNTER_INST_FP);                                                                                \
        } else if (bytes == 16) {                                                                                     \
            PERF_INC(COUNTER_INST_LSX);                                                                               \
        } else if (bytes == 32) {                                                                                     \
            PERF_INC(COUNTER_INST_LASX);                                                                              \
        } else {lsassert(0);};                                                                                        \
    } while (0)
#endif
#ifdef CONFIG_DIFF
#define __NOT_IMPLEMENTED__ __NOT_IMPLEMENTED_EXIT__
#else
#define __NOT_IMPLEMENTED__ do {fprintf(stderr, "LA_EMU NOT IMPLEMENTED %s, pc:%lx\n", __func__, env->pc); env->pc += 4; return false;} while(0);
#endif
#define __NOT_CORRECTED_IMPLEMENTED__ do {fprintf(stderr, "LA_EMU NOT CORRECTED IMPLEMENTED %s, pc:%lx\n", __func__, env->pc);} while(0);
#define __NOT_IMPLEMENTED_EXIT__ do {fprintf(stderr, "LA_EMU NOT IMPLEMENTED %s, pc:%lx\n", __func__, env->pc); laemu_exit(1); return false;} while(0);

#define DisasContext CPULoongArchState
#define ctx env
#define TCGv int64_t
static inline int plus_1(DisasContext *ctx, int x)
{
    return x + 1;
}

static inline int shl_1(DisasContext *ctx, int x)
{
    return x << 1;
}

static inline int shl_2(DisasContext *ctx, int x)
{
    return x << 2;
}

static inline int shl_3(DisasContext *ctx, int x)
{
    return x << 3;
}
#include "trans_la.c.inc"

/*
 * If an operation is being performed on less than TARGET_LONG_BITS,
 * it may require the inputs to be sign- or zero-extended; which will
 * depend on the exact operation being performed.
 */
typedef enum {
    EXT_NONE,
    EXT_SIGN,
    EXT_ZERO,
} DisasExtend;

__attribute__((unused)) static inline int64_t gpr_src(CPULoongArchState *env, int reg_num, DisasExtend src_ext)
{
    if (reg_num == 0) {
        return 0;
    }

    switch (src_ext) {
    case EXT_NONE:
        return env->gpr[reg_num];
    case EXT_SIGN:
        return (int64_t)(int32_t)env->gpr[reg_num];
    case EXT_ZERO:
        return (uint64_t)(uint32_t)env->gpr[reg_num];
    }
    g_assert_not_reached();
}

static inline void gen_set_gpr(CPULoongArchState *env, int reg_num, int64_t t, DisasExtend dst_ext)
{
    if (reg_num != 0) {
        switch (dst_ext) {
        case EXT_NONE:
            env->gpr[reg_num] = t;
            break;
        case EXT_SIGN:
            env->gpr[reg_num] = (int64_t)(int32_t)t;
            break;
        case EXT_ZERO:
            env->gpr[reg_num] = (uint64_t)(uint32_t)t;
            break;
        default:
            g_assert_not_reached();
        }
    }
}
static inline int64_t get_fpr(CPULoongArchState *env, int reg_num) {
    return env->fpr[reg_num].vreg.D[0];
}
static inline void set_fpr(CPULoongArchState *env, int reg_num, int64_t val) {
    env->fpr[reg_num].vreg.D[0] = val;
}
static inline void set_fpr_s(CPULoongArchState *env, int reg_num, uint32_t val) {
    env->fpr[reg_num].vreg.UD[0] = 0xffffffff00000000ull | val;
}

/* bit0(signaling/quiet) bit1(lt) bit2(eq) bit3(un) bit4(neq) */
static uint32_t get_fcmp_flags(int cond)
{
    uint32_t flags = 0;

    if (cond & 0x1) {
        flags |= FCMP_LT;
    }
    if (cond & 0x2) {
        flags |= FCMP_EQ;
    }
    if (cond & 0x4) {
        flags |= FCMP_UN;
    }
    if (cond & 0x8) {
        flags |= FCMP_GT | FCMP_LT;
    }
    return flags;
}

/*
 * Instruction implementations are split by ISA class.  Keep them included in
 * this translation unit because trans_la.c.inc declares and calls static
 * trans_* handlers directly.
 */
#include "interpreter_int.c"
#include "interpreter_float.c"
#include "interpreter_vec.c"

bool interpreter(CPULoongArchState *env, uint32_t insn, INSCache* ic) {
    env->last_insn_name = NULL;

    if (ic) {
        env->last_insn_name = ic->name;
        ic->trans_func(env, ic->arg);
        if (lsx_insn_writes_vr(insn)) {
            uint32_t vd = insn & 0x1f;
            env->fpr[vd].vreg.UD[2] = 0;
            env->fpr[vd].vreg.UD[3] = 0;
        }
        env->gpr[0] = 0;
#ifdef CONFIG_INSN_STATS
        insn_stats_classify_and_record(insn, ic->name);
#endif
        return true;
    }
    if (decode(env, insn)) {
        if (lsx_insn_writes_vr(insn)) {
            uint32_t vd = insn & 0x1f;
            env->fpr[vd].vreg.UD[2] = 0;
            env->fpr[vd].vreg.UD[3] = 0;
        }
        env->gpr[0] = 0;
#ifdef CONFIG_INSN_STATS
        insn_stats_classify_and_record(insn, env->last_insn_name);
#endif
        return true;
    } else {
        return false;
    }
}
