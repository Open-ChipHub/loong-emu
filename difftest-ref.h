// LoongArch difftest reference model state layout.
// Shared between verif/difftest (refproxy) and verif/loong64-emu (difftest.c).
// Both sides must agree on this packed struct layout.

#ifndef __DIFFTEST_REF_H__
#define __DIFFTEST_REF_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t value[32];
} la_arch_int_reg_t;

typedef struct {
    uint64_t value[32];
} la_arch_fp_reg_t;

// Must match the RTL probe DifftestCSRRegState in difftest.v
// Fields: crmd, prmd, euen, ecfg, estat, era, badv, eentry,
//         tlbidx, tlbehi, tlbelo0, tlbelo1, asid, pgdl, pgdh,
//         save0, save1, save2, save3, tid, tcfg, tval, ticlr,
//         llbctl, tlbrentry, dmw0, dmw1
typedef struct __attribute__((packed)) {
    uint64_t crmd;
    uint64_t prmd;
    uint64_t euen;
    uint64_t ecfg;
    uint64_t estat;
    uint64_t era;
    uint64_t badv;
    uint64_t eentry;
    uint64_t tlbidx;
    uint64_t tlbehi;
    uint64_t tlbelo0;
    uint64_t tlbelo1;
    uint64_t asid;
    uint64_t pgdl;
    uint64_t pgdh;
    uint64_t save0;
    uint64_t save1;
    uint64_t save2;
    uint64_t save3;
    uint64_t save4;
    uint64_t save5;
    uint64_t save6;
    uint64_t save7;
    uint64_t tid;
    uint64_t tcfg;
    uint64_t tval;
    uint64_t ticlr;
    uint64_t llbctl;
    uint64_t tlbrentry;
    uint64_t dmw0;
    uint64_t dmw1;
    uint64_t dmw2;
    uint64_t dmw3;
} la_csr_state_t;

// The packed struct passed between difftest refproxy and REF .so
typedef struct __attribute__((packed)) {
    la_arch_int_reg_t xrf;
    la_arch_fp_reg_t  frf;
    la_csr_state_t    csr;
    uint64_t          pc;
} la_ref_state_t;

#ifdef __cplusplus
}
#endif

#endif // __DIFFTEST_REF_H__
