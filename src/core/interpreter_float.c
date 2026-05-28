/* Scalar floating-point instruction handlers.
 * Included by interpreter.c; do not compile this file separately.
 */

static bool gen_fff(CPULoongArchState *env, arg_fff *restrict a, uint64_t (*func)(CPULoongArchState *, uint64_t, uint64_t)) {
    TCGv src1 = get_fpr(ctx, a->fj);
    TCGv src2 = get_fpr(ctx, a->fk);
    TCGv dest = func(env, src1, src2);
    set_fpr(env, a->fd, dest);
    env->pc += 4;
    return true;
}
static bool trans_fadd_s(CPULoongArchState *env, arg_fadd_s *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fadd_s);}
static bool trans_fadd_d(CPULoongArchState *env, arg_fadd_d *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fadd_d);}
static bool trans_fsub_s(CPULoongArchState *env, arg_fadd_s *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fsub_s);}
static bool trans_fsub_d(CPULoongArchState *env, arg_fadd_d *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fsub_d);}
static bool trans_fmul_s(CPULoongArchState *env, arg_fadd_s *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fmul_s);}
static bool trans_fmul_d(CPULoongArchState *env, arg_fadd_d *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fmul_d);}
static bool trans_fdiv_s(CPULoongArchState *env, arg_fadd_s *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fdiv_s);}
static bool trans_fdiv_d(CPULoongArchState *env, arg_fadd_d *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fdiv_d);}
static bool trans_fmax_s(CPULoongArchState *env, arg_fmax_s *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fmax_s);}
static bool trans_fmax_d(CPULoongArchState *env, arg_fmax_d *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fmax_d);}
static bool trans_fmin_s(CPULoongArchState *env, arg_fmin_s *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fmin_s);}
static bool trans_fmin_d(CPULoongArchState *env, arg_fmin_d *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fmin_d);}
static bool trans_fmaxa_s(CPULoongArchState *env, arg_fmaxa_s *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fmaxa_s);}
static bool trans_fmaxa_d(CPULoongArchState *env, arg_fmaxa_d *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fmaxa_d);}
static bool trans_fmina_s(CPULoongArchState *env, arg_fmina_s *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fmina_s);}
static bool trans_fmina_d(CPULoongArchState *env, arg_fmina_d *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fmina_d);}
static bool trans_fscaleb_s(CPULoongArchState *env, arg_fscaleb_s *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fscaleb_s);}
static bool trans_fscaleb_d(CPULoongArchState *env, arg_fscaleb_d *restrict a) {CHECK_FPE(8);return gen_fff(env, a, helper_fscaleb_d);}

static bool gen_ffff(CPULoongArchState *env, arg_ffff *restrict a, uint64_t (*func)(CPULoongArchState *, uint64_t, uint64_t, uint64_t, uint32_t), uint32_t flag) {
    TCGv src1 = get_fpr(ctx, a->fj);
    TCGv src2 = get_fpr(ctx, a->fk);
    TCGv src3 = get_fpr(ctx, a->fa);
    TCGv dest = func(env, src1, src2, src3, flag);
    set_fpr(env, a->fd, dest);
    env->pc += 4;
    return true;
}
static bool trans_fmadd_s(CPULoongArchState *env, arg_fmadd_s *restrict a) {CHECK_FPE(8);return gen_ffff(env, a, helper_fmuladd_s, 0);}
static bool trans_fmadd_d(CPULoongArchState *env, arg_fmadd_d *restrict a) {CHECK_FPE(8);return gen_ffff(env, a, helper_fmuladd_d, 0);}
static bool trans_fmsub_s(CPULoongArchState *env, arg_fmsub_s *restrict a) {CHECK_FPE(8);return gen_ffff(env, a, helper_fmuladd_s, float_muladd_negate_c);}
static bool trans_fmsub_d(CPULoongArchState *env, arg_fmsub_s *restrict a) {CHECK_FPE(8);return gen_ffff(env, a, helper_fmuladd_d, float_muladd_negate_c);}
static bool trans_fnmadd_s(CPULoongArchState *env, arg_fnmadd_s *restrict a) {CHECK_FPE(8);return gen_ffff(env, a, helper_fmuladd_s, float_muladd_negate_result);}
static bool trans_fnmadd_d(CPULoongArchState *env, arg_fnmadd_d *restrict a) {CHECK_FPE(8);return gen_ffff(env, a, helper_fmuladd_d, float_muladd_negate_result);}
static bool trans_fnmsub_s(CPULoongArchState *env, arg_fnmsub_s *restrict a) {CHECK_FPE(8);return gen_ffff(env, a, helper_fmuladd_s, float_muladd_negate_c | float_muladd_negate_result);}
static bool trans_fnmsub_d(CPULoongArchState *env, arg_fnmsub_d *restrict a) {CHECK_FPE(8);return gen_ffff(env, a, helper_fmuladd_d, float_muladd_negate_c | float_muladd_negate_result);}

static bool trans_fabs_s(CPULoongArchState *env, arg_fabs_s *restrict a) {
    CHECK_FPE(8);
    TCGv src = get_fpr(ctx, a->fj);
    TCGv dest = src & MAKE_64BIT_MASK(0, 31);
    set_fpr(env, a->fd, dest);
    env->pc += 4;
    return true;
}
static bool trans_fabs_d(CPULoongArchState *env, arg_fabs_d *restrict a) {
    CHECK_FPE(8);
    TCGv src = get_fpr(ctx, a->fj);
    TCGv dest = src & MAKE_64BIT_MASK(0, 63);
    set_fpr(env, a->fd, dest);
    env->pc += 4;
    return true;
}
static bool trans_fneg_s(CPULoongArchState *env, arg_fneg_s *restrict a) {
    CHECK_FPE(8);
    TCGv src = get_fpr(ctx, a->fj);
    TCGv dest = src ^ 0x80000000ULL;
    set_fpr(env, a->fd, dest);
    env->pc += 4;
    return true;
}
static bool trans_fneg_d(CPULoongArchState *env, arg_fneg_d *restrict a) {
    CHECK_FPE(8);
    TCGv src = get_fpr(ctx, a->fj);
    TCGv dest = src ^ 0x8000000000000000ULL;
    set_fpr(env, a->fd, dest);
    env->pc += 4;
    return true;
}
static bool gen_ff(CPULoongArchState *env, arg_ff *restrict a, uint64_t (*func)(CPULoongArchState *, uint64_t)) {
    TCGv src1 = get_fpr(ctx, a->fj);
    TCGv dest = func(env, src1);
    set_fpr(env, a->fd, dest);
    env->pc += 4;
    return true;
}
static bool trans_fsqrt_s(CPULoongArchState *env, arg_fsqrt_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_fsqrt_s);}
static bool trans_fsqrt_d(CPULoongArchState *env, arg_fsqrt_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_fsqrt_d);}
static bool trans_frecip_s(CPULoongArchState *env, arg_frecip_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_frecip_s);}
static bool trans_frecip_d(CPULoongArchState *env, arg_frecip_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_frecip_d);}
static bool trans_frsqrt_s(CPULoongArchState *env, arg_frsqrt_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_frsqrt_s);}
static bool trans_frsqrt_d(CPULoongArchState *env, arg_frsqrt_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_frsqrt_d);}
static bool trans_flogb_s(CPULoongArchState *env, arg_flogb_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_flogb_s);}
static bool trans_flogb_d(CPULoongArchState *env, arg_flogb_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_flogb_d);}
static bool trans_fclass_s(CPULoongArchState *env, arg_fclass_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_fclass_s);}
static bool trans_fclass_d(CPULoongArchState *env, arg_fclass_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_fclass_d);}
static bool trans_fcvt_s_d(CPULoongArchState *env, arg_fcvt_s_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_fcvt_s_d);}
static bool trans_fcvt_d_s(CPULoongArchState *env, arg_fcvt_d_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_fcvt_d_s);}
static bool trans_ftintrm_w_s(CPULoongArchState *env, arg_ftintrm_w_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrm_w_s);}
static bool trans_ftintrm_w_d(CPULoongArchState *env, arg_ftintrm_w_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrm_w_d);}
static bool trans_ftintrm_l_s(CPULoongArchState *env, arg_ftintrm_l_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrm_l_s);}
static bool trans_ftintrm_l_d(CPULoongArchState *env, arg_ftintrm_l_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrm_l_d);}
static bool trans_ftintrp_w_s(CPULoongArchState *env, arg_ftintrp_w_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrp_w_s);}
static bool trans_ftintrp_w_d(CPULoongArchState *env, arg_ftintrp_w_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrp_w_d);}
static bool trans_ftintrp_l_s(CPULoongArchState *env, arg_ftintrp_l_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrp_l_s);}
static bool trans_ftintrp_l_d(CPULoongArchState *env, arg_ftintrp_l_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrp_l_d);}
static bool trans_ftintrz_w_s(CPULoongArchState *env, arg_ftintrz_w_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrz_w_s);}
static bool trans_ftintrz_w_d(CPULoongArchState *env, arg_ftintrz_w_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrz_w_d);}
static bool trans_ftintrz_l_s(CPULoongArchState *env, arg_ftintrz_l_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrz_l_s);}
static bool trans_ftintrz_l_d(CPULoongArchState *env, arg_ftintrz_l_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrz_l_d);}
static bool trans_ftintrne_w_s(CPULoongArchState *env, arg_ftintrne_w_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrne_w_s);}
static bool trans_ftintrne_w_d(CPULoongArchState *env, arg_ftintrne_w_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrne_w_d);}
static bool trans_ftintrne_l_s(CPULoongArchState *env, arg_ftintrne_l_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrne_l_s);}
static bool trans_ftintrne_l_d(CPULoongArchState *env, arg_ftintrne_l_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftintrne_l_d);}
static bool trans_ftint_w_s(CPULoongArchState *env, arg_ftint_w_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftint_w_s);}
static bool trans_ftint_w_d(CPULoongArchState *env, arg_ftint_w_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftint_w_d);}
static bool trans_ftint_l_s(CPULoongArchState *env, arg_ftint_l_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftint_l_s);}
static bool trans_ftint_l_d(CPULoongArchState *env, arg_ftint_l_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ftint_l_d);}
static bool trans_ffint_s_w(CPULoongArchState *env, arg_ffint_s_w *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ffint_s_w);}
static bool trans_ffint_s_l(CPULoongArchState *env, arg_ffint_s_l *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ffint_s_l);}
static bool trans_ffint_d_w(CPULoongArchState *env, arg_ffint_d_w *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ffint_d_w);}
static bool trans_ffint_d_l(CPULoongArchState *env, arg_ffint_d_l *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_ffint_d_l);}
static bool trans_frint_s(CPULoongArchState *env, arg_frint_s *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_frint_s);}
static bool trans_frint_d(CPULoongArchState *env, arg_frint_d *restrict a) {CHECK_FPE(8); return gen_ff(env, a, helper_frint_d);}
static bool trans_fcopysign_s(CPULoongArchState *env, arg_fcopysign_s *restrict a) {
    CHECK_FPE(8);
    TCGv src1 = get_fpr(ctx, a->fj);
    TCGv src2 = get_fpr(ctx, a->fk);
    TCGv dest = deposit64(src2, 0, 31, src1);
    set_fpr(env, a->fd, dest);
    env->pc += 4;
    return true;
}
static bool trans_fcopysign_d(CPULoongArchState *env, arg_fcopysign_d *restrict a) {
    CHECK_FPE(8);
    TCGv src1 = get_fpr(ctx, a->fj);
    TCGv src2 = get_fpr(ctx, a->fk);
    TCGv dest = deposit64(src2, 0, 63, src1);
    set_fpr(env, a->fd, dest);
    env->pc += 4;
    return true;
}
static bool trans_fcmp_cond_s(CPULoongArchState *env, arg_fcmp_cond_s *restrict a) {
    CHECK_FPE(8);
    TCGv src1 = get_fpr(ctx, a->fj);
    TCGv src2 = get_fpr(ctx, a->fk);
    uint32_t flags = get_fcmp_flags(a->fcond >> 1);
    int r = (a->fcond & 1) ? helper_fcmp_s_s(env, src1, src2, flags) : helper_fcmp_c_s(env, src1, src2, flags);
    env->cf[a->cd] = r;
    env->pc += 4;
    return true;
}
static bool trans_fcmp_cond_d(CPULoongArchState *env, arg_fcmp_cond_d *restrict a) {
    CHECK_FPE(8);
    TCGv src1 = get_fpr(ctx, a->fj);
    TCGv src2 = get_fpr(ctx, a->fk);
    uint32_t flags = get_fcmp_flags(a->fcond >> 1);
    int r = (a->fcond & 1) ? helper_fcmp_s_d(env, src1, src2, flags) : helper_fcmp_c_d(env, src1, src2, flags);
    env->cf[a->cd] = r;
    env->pc += 4;
    return true;
}
static bool trans_fmov_s(CPULoongArchState *env, arg_fmov_s *restrict a) {
    CHECK_FPE(8);
    env->fpr[a->fd].vreg.W[0] = env->fpr[a->fj].vreg.W[0];
    env->pc += 4;
    return true;
}
static bool trans_fmov_d(CPULoongArchState *env, arg_fmov_d *restrict a) {
    CHECK_FPE(8);
    env->fpr[a->fd].vreg.D[0] = env->fpr[a->fj].vreg.D[0];
    env->pc += 4;
    return true;
}
static bool trans_fsel(CPULoongArchState *env, arg_fsel *restrict a) {
    CHECK_FPE(8);
    TCGv src1 = get_fpr(ctx, a->fj);
    TCGv src2 = get_fpr(ctx, a->fk);
    TCGv dest = env->cf[a->ca] == 0 ? src1 : src2;
    set_fpr(env, a->fd, dest);
    env->pc += 4;
    return true;
}
static bool trans_movgr2fr_w(CPULoongArchState *env, arg_movgr2fr_w *restrict a) {
    CHECK_FPE(8);
    env->fpr[a->fd].vreg.W[0] = env->gpr[a->rj];
    env->pc += 4;
    return true;
}
static bool trans_movgr2fr_d(CPULoongArchState *env, arg_movgr2fr_d *restrict a) {
    CHECK_FPE(8);
    env->fpr[a->fd].vreg.D[0] = env->gpr[a->rj];
    env->pc += 4;
    return true;
}
static bool trans_movgr2frh_w(CPULoongArchState *env, arg_movgr2frh_w *restrict a) {
    CHECK_FPE(8);
    env->fpr[a->fd].vreg.W[1] = env->gpr[a->rj];
    env->pc += 4;
    return true;
}
static bool trans_movfr2gr_s(CPULoongArchState *env, arg_movfr2gr_s *restrict a) {
    CHECK_FPE(8);
    env->gpr[a->rd] = (int64_t)env->fpr[a->fj].vreg.W[0];
    env->pc += 4;
    return true;
}
static bool trans_movfr2gr_d(CPULoongArchState *env, arg_movfr2gr_d *restrict a) {
    CHECK_FPE(8);
    TCGv src = get_fpr(ctx, a->fj);
    TCGv dest = src;
    gen_set_gpr(env, a->rd, dest, EXT_NONE);
    env->pc += 4;
    return true;
}
static bool trans_movfrh2gr_s(CPULoongArchState *env, arg_movfrh2gr_s *restrict a) {
    CHECK_FPE(8);
    TCGv src = get_fpr(ctx, a->fj);
    TCGv dest = src;
    gen_set_gpr(env, a->rd, dest, EXT_SIGN);
    env->pc += 4;
    return true;
}


static const uint32_t fcsr_mask[4] = {
    UINT32_MAX, FCSR0_M1, FCSR0_M2, FCSR0_M3
};
static bool trans_movgr2fcsr(CPULoongArchState *env, arg_movgr2fcsr *restrict a) {
    CHECK_FPE(8);
    uint32_t mask = fcsr_mask[a->fcsrd];
    uint64_t rj = env->gpr[a->rj];
    if (mask == UINT32_MAX) {
        env->fcsr0 = rj;
    } else {
        env->fcsr0 = (env->fcsr0 & ~mask) | (rj & mask);
    }

    if (mask & FCSR0_M3) {
        helper_set_rounding_mode(env);
    }
    env->pc += 4;
    return true;
}
static bool trans_movfcsr2gr(CPULoongArchState *env, arg_movfcsr2gr *restrict a) {
    CHECK_FPE(8);
    env->gpr[a->rd] = env->fcsr0 & fcsr_mask[a->fcsrs];
    env->pc += 4;
    return true;
}
static bool trans_movfr2cf(CPULoongArchState *env, arg_movfr2cf *restrict a) {
    CHECK_FPE(8);
    env->cf[a->cd] = env->fpr[a->fj].vreg.D[0] & 1;
    env->pc += 4;
    return true;
}
static bool trans_movcf2fr(CPULoongArchState *env, arg_movcf2fr *restrict a) {
    CHECK_FPE(8);
    env->fpr[a->fd].vreg.D[0] = env->cf[a->cj] & 1;
    env->pc += 4;
    return true;
}
static bool trans_movgr2cf(CPULoongArchState *env, arg_movgr2cf *restrict a) {
    CHECK_FPE(8);
    env->cf[a->cd] = env->gpr[a->rj] & 1;
    env->pc += 4;
    return true;
}
static bool trans_movcf2gr(CPULoongArchState *env, arg_movcf2gr *restrict a) {
    CHECK_FPE(8);
    env->gpr[a->rd] = env->cf[a->cj] & 1;
    env->pc += 4;
    return true;
}
static bool trans_fld_s(CPULoongArchState *env, arg_fld_s *restrict a) {
    CHECK_FPE(8);
    set_fpr_s(env, a->fd, ld_w(env, add_addr(env->gpr[a->rj], a->imm)));
    env->pc += 4;
    return true;
}
static bool trans_fst_s(CPULoongArchState *env, arg_fst_s *restrict a) {
    CHECK_FPE(8);
    st_w(env, add_addr(env->gpr[a->rj], a->imm), env->fpr[a->fd].vreg.W[0]);
    env->pc += 4;
    return true;
}
static bool trans_fld_d(CPULoongArchState *env, arg_fld_d *restrict a) {
    CHECK_FPE(8);
    set_fpr(env, a->fd, ld_d(env, add_addr(env->gpr[a->rj], a->imm)));
    env->pc += 4;
    return true;
}
static bool trans_fst_d(CPULoongArchState *env, arg_fst_d *restrict a) {
    CHECK_FPE(8);
    st_d(env, add_addr(env->gpr[a->rj], a->imm), env->fpr[a->fd].vreg.D[0]);
    env->pc += 4;
    return true;
}
static bool trans_fldx_s(CPULoongArchState *env, arg_fldx_s *restrict a) {
    CHECK_FPE(8);
    set_fpr_s(env, a->fd, ld_w(env, add_addr(env->gpr[a->rj], env->gpr[a->rk])));
    env->pc += 4;
    return true;
}
static bool trans_fldx_d(CPULoongArchState *env, arg_fldx_d *restrict a) {
    CHECK_FPE(8);
    set_fpr(env, a->fd, ld_d(env, add_addr(env->gpr[a->rj], env->gpr[a->rk])));
    env->pc += 4;
    return true;
}
static bool trans_fstx_s(CPULoongArchState *env, arg_fstx_s *restrict a) {
    CHECK_FPE(8);
    st_w(env, add_addr(env->gpr[a->rj], env->gpr[a->rk]), env->fpr[a->fd].vreg.W[0]);
    env->pc += 4;
    return true;
}
static bool trans_fstx_d(CPULoongArchState *env, arg_fstx_d *restrict a) {
    CHECK_FPE(8);
    st_d(env, add_addr(env->gpr[a->rj], env->gpr[a->rk]), env->fpr[a->fd].vreg.D[0]);
    env->pc += 4;
    return true;
}
static bool trans_fldgt_s(CPULoongArchState *env, arg_fldgt_s *restrict a) {
    CHECK_FPE(8);
    if ((int64_t)env->gpr[a->rj] > (int64_t)env->gpr[a->rk])
        set_fpr_s(env, a->fd, ld_w(env, env->gpr[a->rj]));
    env->pc += 4; return true;
}
static bool trans_fldgt_d(CPULoongArchState *env, arg_fldgt_d *restrict a) {
    CHECK_FPE(8);
    if ((int64_t)env->gpr[a->rj] > (int64_t)env->gpr[a->rk])
        set_fpr(env, a->fd, ld_d(env, env->gpr[a->rj]));
    env->pc += 4; return true;
}
static bool trans_fldle_s(CPULoongArchState *env, arg_fldle_s *restrict a) {
    CHECK_FPE(8);
    if ((int64_t)env->gpr[a->rj] <= (int64_t)env->gpr[a->rk])
        set_fpr_s(env, a->fd, ld_w(env, env->gpr[a->rj]));
    env->pc += 4; return true;
}
static bool trans_fldle_d(CPULoongArchState *env, arg_fldle_d *restrict a) {
    CHECK_FPE(8);
    if ((int64_t)env->gpr[a->rj] <= (int64_t)env->gpr[a->rk])
        set_fpr(env, a->fd, ld_d(env, env->gpr[a->rj]));
    env->pc += 4; return true;
}
static bool trans_fstgt_s(CPULoongArchState *env, arg_fstgt_s *restrict a) {
    CHECK_FPE(8);
    if ((int64_t)env->gpr[a->rj] > (int64_t)env->gpr[a->rk])
        st_w(env, env->gpr[a->rj], env->fpr[a->fd].vreg.W[0]);
    env->pc += 4; return true;
}
static bool trans_fstgt_d(CPULoongArchState *env, arg_fstgt_d *restrict a) {
    CHECK_FPE(8);
    if ((int64_t)env->gpr[a->rj] > (int64_t)env->gpr[a->rk])
        st_d(env, env->gpr[a->rj], env->fpr[a->fd].vreg.D[0]);
    env->pc += 4; return true;
}
static bool trans_fstle_s(CPULoongArchState *env, arg_fstle_s *restrict a) {
    CHECK_FPE(8);
    if ((int64_t)env->gpr[a->rj] <= (int64_t)env->gpr[a->rk])
        st_w(env, env->gpr[a->rj], env->fpr[a->fd].vreg.W[0]);
    env->pc += 4; return true;
}
static bool trans_fstle_d(CPULoongArchState *env, arg_fstle_d *restrict a) {
    CHECK_FPE(8);
    if ((int64_t)env->gpr[a->rj] <= (int64_t)env->gpr[a->rk])
        st_d(env, env->gpr[a->rj], env->fpr[a->fd].vreg.D[0]);
    env->pc += 4; return true;
}
