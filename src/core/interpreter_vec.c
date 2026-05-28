/* LSX/LASX vector instruction handlers.
 * Included by interpreter.c; do not compile this file separately.
 */

#define gen_trans_vvid(op, size, helper_name) \
static bool glue(trans_, op)(CPULoongArchState *env, arg_vv_i *restrict a) {      \
    CHECK_FPE(size);                                                    \
    int oprsz = size;                                                   \
    uint32_t desc = simd_desc(oprsz, oprsz, 0);                         \
    glue(helper_, helper_name)(&env->fpr[a->vd], &env->fpr[a->vj], a->imm, desc);   \
    env->pc += 4;                                                       \
    return true;                                                        \
}
#define gen_trans_vvvd(op, size, helper_name) \
static bool glue(trans_, op)(CPULoongArchState *env, arg_vvv *restrict a) {   \
    CHECK_FPE(size);                                                    \
    int oprsz = size;                                                   \
    uint32_t desc = simd_desc(oprsz, oprsz, 0);                         \
    glue(helper_, helper_name)(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc);          \
    env->pc += 4;                                                       \
    return true;                                                        \
}
#define gen_trans_vvvvd(op, size, helper_name) \
static bool glue(trans_, op)(CPULoongArchState *env, arg_vvvv *restrict a) {   \
    CHECK_FPE(size);                                                    \
    int oprsz = size;                                                   \
    uint32_t desc = simd_desc(oprsz, oprsz, 0);                         \
    glue(helper_, helper_name)(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], &env->fpr[a->va], desc);          \
    env->pc += 4;                                                       \
    return true;                                                        \
}
static inline bool vadd_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] + env->fpr[a->vk].vreg.B[i];}env->pc += 4;return true;}
static inline bool vadd_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] + env->fpr[a->vk].vreg.H[i];}env->pc += 4;return true;}
static inline bool vadd_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] + env->fpr[a->vk].vreg.W[i];}env->pc += 4;return true;}
static inline bool vadd_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] + env->fpr[a->vk].vreg.D[i];}env->pc += 4;return true;}
static bool trans_vadd_b(CPULoongArchState *env, arg_vadd_b *restrict a) {CHECK_FPE(16); return vadd_b(env, a, 16);}
static bool trans_vadd_h(CPULoongArchState *env, arg_vadd_h *restrict a) {CHECK_FPE(16); return vadd_h(env, a, 16);}
static bool trans_vadd_w(CPULoongArchState *env, arg_vadd_w *restrict a) {CHECK_FPE(16); return vadd_w(env, a, 16);}
static bool trans_vadd_d(CPULoongArchState *env, arg_vadd_d *restrict a) {CHECK_FPE(16); return vadd_d(env, a, 16);}
static bool trans_xvadd_b(CPULoongArchState *env, arg_vadd_b *restrict a) {CHECK_FPE(32); return vadd_b(env, a, 32);}
static bool trans_xvadd_h(CPULoongArchState *env, arg_vadd_h *restrict a) {CHECK_FPE(32); return vadd_h(env, a, 32);}
static bool trans_xvadd_w(CPULoongArchState *env, arg_vadd_w *restrict a) {CHECK_FPE(32); return vadd_w(env, a, 32);}
static bool trans_xvadd_d(CPULoongArchState *env, arg_vadd_d *restrict a) {CHECK_FPE(32); return vadd_d(env, a, 32);}
static bool trans_vadd_q(CPULoongArchState *env, arg_vadd_q *restrict a) {
    CHECK_FPE(16);
    env->fpr[a->vd].vreg.Q[0] = env->fpr[a->vj].vreg.Q[0] + env->fpr[a->vk].vreg.Q[0];
    env->pc += 4;
    return true;
}
static inline bool vsub_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] - env->fpr[a->vk].vreg.B[i];}env->pc += 4;return true;}
static inline bool vsub_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] - env->fpr[a->vk].vreg.H[i];}env->pc += 4;return true;}
static inline bool vsub_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] - env->fpr[a->vk].vreg.W[i];}env->pc += 4;return true;}
static inline bool vsub_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] - env->fpr[a->vk].vreg.D[i];}env->pc += 4;return true;}
static bool trans_vsub_b(CPULoongArchState *env, arg_vsub_b *restrict a) {CHECK_FPE(16); return vsub_b(env, a, 16);}
static bool trans_vsub_h(CPULoongArchState *env, arg_vsub_h *restrict a) {CHECK_FPE(16); return vsub_h(env, a, 16);}
static bool trans_vsub_w(CPULoongArchState *env, arg_vsub_w *restrict a) {CHECK_FPE(16); return vsub_w(env, a, 16);}
static bool trans_vsub_d(CPULoongArchState *env, arg_vsub_d *restrict a) {CHECK_FPE(16); return vsub_d(env, a, 16);}
static bool trans_xvsub_b(CPULoongArchState *env, arg_vsub_b *restrict a) {CHECK_FPE(32); return vsub_b(env, a, 32);}
static bool trans_xvsub_h(CPULoongArchState *env, arg_vsub_h *restrict a) {CHECK_FPE(32); return vsub_h(env, a, 32);}
static bool trans_xvsub_w(CPULoongArchState *env, arg_vsub_w *restrict a) {CHECK_FPE(32); return vsub_w(env, a, 32);}
static bool trans_xvsub_d(CPULoongArchState *env, arg_vsub_d *restrict a) {CHECK_FPE(32); return vsub_d(env, a, 32);}
static bool trans_vsub_q(CPULoongArchState *env, arg_vsub_q *restrict a) {
    CHECK_FPE(16);
    env->fpr[a->vd].vreg.Q[0] = env->fpr[a->vj].vreg.Q[0] - env->fpr[a->vk].vreg.Q[0];
    env->pc += 4;
    return true;
}
static inline bool vaddi_bu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] + a->imm;}env->pc += 4;return true;}
static inline bool vaddi_hu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] + a->imm;}env->pc += 4;return true;}
static inline bool vaddi_wu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] + a->imm;}env->pc += 4;return true;}
static inline bool vaddi_du(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] + a->imm;}env->pc += 4;return true;}
static bool trans_vaddi_bu(CPULoongArchState *env, arg_vaddi_bu *restrict a) {CHECK_FPE(16); return vaddi_bu(env, a, 16);}
static bool trans_vaddi_hu(CPULoongArchState *env, arg_vaddi_hu *restrict a) {CHECK_FPE(16); return vaddi_hu(env, a, 16);}
static bool trans_vaddi_wu(CPULoongArchState *env, arg_vaddi_wu *restrict a) {CHECK_FPE(16); return vaddi_wu(env, a, 16);}
static bool trans_vaddi_du(CPULoongArchState *env, arg_vaddi_du *restrict a) {CHECK_FPE(16); return vaddi_du(env, a, 16);}
static bool trans_xvaddi_bu(CPULoongArchState *env, arg_vaddi_bu *restrict a) {CHECK_FPE(32); return vaddi_bu(env, a, 32);}
static bool trans_xvaddi_hu(CPULoongArchState *env, arg_vaddi_hu *restrict a) {CHECK_FPE(32); return vaddi_hu(env, a, 32);}
static bool trans_xvaddi_wu(CPULoongArchState *env, arg_vaddi_wu *restrict a) {CHECK_FPE(32); return vaddi_wu(env, a, 32);}
static bool trans_xvaddi_du(CPULoongArchState *env, arg_vaddi_du *restrict a) {CHECK_FPE(32); return vaddi_du(env, a, 32);}
static inline bool vsubi_bu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] - a->imm;}env->pc += 4;return true;}
static inline bool vsubi_hu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] - a->imm;}env->pc += 4;return true;}
static inline bool vsubi_wu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] - a->imm;}env->pc += 4;return true;}
static inline bool vsubi_du(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] - a->imm;}env->pc += 4;return true;}
static bool trans_vsubi_bu(CPULoongArchState *env, arg_vsubi_bu *restrict a) {CHECK_FPE(16); return vsubi_bu(env, a, 16);}
static bool trans_vsubi_hu(CPULoongArchState *env, arg_vsubi_hu *restrict a) {CHECK_FPE(16); return vsubi_hu(env, a, 16);}
static bool trans_vsubi_wu(CPULoongArchState *env, arg_vsubi_wu *restrict a) {CHECK_FPE(16); return vsubi_wu(env, a, 16);}
static bool trans_vsubi_du(CPULoongArchState *env, arg_vsubi_du *restrict a) {CHECK_FPE(16); return vsubi_du(env, a, 16);}
static bool trans_xvsubi_bu(CPULoongArchState *env, arg_vsubi_bu *restrict a) {CHECK_FPE(32); return vsubi_bu(env, a, 32);}
static bool trans_xvsubi_hu(CPULoongArchState *env, arg_vsubi_hu *restrict a) {CHECK_FPE(32); return vsubi_hu(env, a, 32);}
static bool trans_xvsubi_wu(CPULoongArchState *env, arg_vsubi_wu *restrict a) {CHECK_FPE(32); return vsubi_wu(env, a, 32);}
static bool trans_xvsubi_du(CPULoongArchState *env, arg_vsubi_du *restrict a) {CHECK_FPE(32); return vsubi_du(env, a, 32);}
static inline bool vneg_b(CPULoongArchState *env, arg_vv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = - env->fpr[a->vj].vreg.B[i];}env->pc += 4;return true;}
static inline bool vneg_h(CPULoongArchState *env, arg_vv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = - env->fpr[a->vj].vreg.H[i];}env->pc += 4;return true;}
static inline bool vneg_w(CPULoongArchState *env, arg_vv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = - env->fpr[a->vj].vreg.W[i];}env->pc += 4;return true;}
static inline bool vneg_d(CPULoongArchState *env, arg_vv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = - env->fpr[a->vj].vreg.D[i];}env->pc += 4;return true;}
static bool trans_vneg_b(CPULoongArchState *env, arg_vneg_b *restrict a) {CHECK_FPE(16); return vneg_b(env, a, 16);}
static bool trans_vneg_h(CPULoongArchState *env, arg_vneg_h *restrict a) {CHECK_FPE(16); return vneg_h(env, a, 16);}
static bool trans_vneg_w(CPULoongArchState *env, arg_vneg_w *restrict a) {CHECK_FPE(16); return vneg_w(env, a, 16);}
static bool trans_vneg_d(CPULoongArchState *env, arg_vneg_d *restrict a) {CHECK_FPE(16); return vneg_d(env, a, 16);}
static bool trans_xvneg_b(CPULoongArchState *env, arg_vneg_b *restrict a) {CHECK_FPE(32); return vneg_b(env, a, 32);}
static bool trans_xvneg_h(CPULoongArchState *env, arg_vneg_h *restrict a) {CHECK_FPE(32); return vneg_h(env, a, 32);}
static bool trans_xvneg_w(CPULoongArchState *env, arg_vneg_w *restrict a) {CHECK_FPE(32); return vneg_w(env, a, 32);}
static bool trans_xvneg_d(CPULoongArchState *env, arg_vneg_d *restrict a) {CHECK_FPE(32); return vneg_d(env, a, 32);}
gen_trans_vvvd(vsadd_b, 16, gvec_ssadd8)
gen_trans_vvvd(vsadd_h, 16, gvec_ssadd16)
gen_trans_vvvd(vsadd_w, 16, gvec_ssadd32)
gen_trans_vvvd(vsadd_d, 16, gvec_ssadd64)
gen_trans_vvvd(vsadd_bu, 16, gvec_usadd8)
gen_trans_vvvd(vsadd_hu, 16, gvec_usadd16)
gen_trans_vvvd(vsadd_wu, 16, gvec_usadd32)
gen_trans_vvvd(vsadd_du, 16, gvec_usadd64)
gen_trans_vvvd(vssub_b, 16, gvec_sssub8)
gen_trans_vvvd(vssub_h, 16, gvec_sssub16)
gen_trans_vvvd(vssub_w, 16, gvec_sssub32)
gen_trans_vvvd(vssub_d, 16, gvec_sssub64)
gen_trans_vvvd(vssub_bu, 16, gvec_ussub8)
gen_trans_vvvd(vssub_hu, 16, gvec_ussub16)
gen_trans_vvvd(vssub_wu, 16, gvec_ussub32)
gen_trans_vvvd(vssub_du, 16, gvec_ussub64)
static bool trans_vhaddw_h_b(CPULoongArchState *env, arg_vhaddw_h_b *restrict a) {
    CHECK_FPE(16);
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vhaddw_h_b(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc);
    env->pc += 4;
    return true;
}
static bool trans_vhaddw_w_h(CPULoongArchState *env, arg_vhaddw_w_h *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vhaddw_w_h(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc);
    env->pc += 4;
    return true;
}
static bool trans_vhaddw_d_w(CPULoongArchState *env, arg_vhaddw_d_w *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vhaddw_d_w(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc);
    env->pc += 4;
    return true;
}
static bool trans_vhaddw_q_d(CPULoongArchState *env, arg_vhaddw_q_d *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vhaddw_q_d(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc);
    env->pc += 4;
    return true;
}
gen_trans_vvvd(vhaddw_hu_bu, 16, vhaddw_hu_bu)
gen_trans_vvvd(vhaddw_wu_hu, 16, vhaddw_wu_hu)
gen_trans_vvvd(vhaddw_du_wu, 16, vhaddw_du_wu)
gen_trans_vvvd(vhaddw_qu_du, 16, vhaddw_qu_du)
gen_trans_vvvd(vhsubw_h_b, 16, vhsubw_h_b)
gen_trans_vvvd(vhsubw_w_h, 16, vhsubw_w_h)
gen_trans_vvvd(vhsubw_d_w, 16, vhsubw_d_w)
gen_trans_vvvd(vhsubw_q_d, 16, vhsubw_q_d)
gen_trans_vvvd(vhsubw_hu_bu, 16, vhsubw_hu_bu)
gen_trans_vvvd(vhsubw_wu_hu, 16, vhsubw_wu_hu)
gen_trans_vvvd(vhsubw_du_wu, 16, vhsubw_du_wu)
gen_trans_vvvd(vhsubw_qu_du, 16, vhsubw_qu_du)
gen_trans_vvvd(vaddwev_h_b, 16, vaddwev_h_b)
gen_trans_vvvd(vaddwev_w_h, 16, vaddwev_w_h)
gen_trans_vvvd(vaddwev_d_w, 16, vaddwev_d_w)
gen_trans_vvvd(vaddwev_q_d, 16, vaddwev_q_d)
gen_trans_vvvd(vaddwod_h_b, 16, vaddwod_h_b)
gen_trans_vvvd(vaddwod_w_h, 16, vaddwod_w_h)
gen_trans_vvvd(vaddwod_d_w, 16, vaddwod_d_w)
gen_trans_vvvd(vaddwod_q_d, 16, vaddwod_q_d)
gen_trans_vvvd(vsubwev_h_b, 16, vsubwev_h_b)
gen_trans_vvvd(vsubwev_w_h, 16, vsubwev_w_h)
gen_trans_vvvd(vsubwev_d_w, 16, vsubwev_d_w)
gen_trans_vvvd(vsubwev_q_d, 16, vsubwev_q_d)
gen_trans_vvvd(vsubwod_h_b, 16, vsubwod_h_b)
gen_trans_vvvd(vsubwod_w_h, 16, vsubwod_w_h)
gen_trans_vvvd(vsubwod_d_w, 16, vsubwod_d_w)
gen_trans_vvvd(vsubwod_q_d, 16, vsubwod_q_d)
gen_trans_vvvd(vaddwev_h_bu, 16, vaddwev_h_bu)
gen_trans_vvvd(vaddwev_w_hu, 16, vaddwev_w_hu)
gen_trans_vvvd(vaddwev_d_wu, 16, vaddwev_d_wu)
gen_trans_vvvd(vaddwev_q_du, 16, vaddwev_q_du)
gen_trans_vvvd(vaddwod_h_bu, 16, vaddwod_h_bu)
gen_trans_vvvd(vaddwod_w_hu, 16, vaddwod_w_hu)
gen_trans_vvvd(vaddwod_d_wu, 16, vaddwod_d_wu)
gen_trans_vvvd(vaddwod_q_du, 16, vaddwod_q_du)
gen_trans_vvvd(vsubwev_h_bu, 16, vsubwev_h_bu)
gen_trans_vvvd(vsubwev_w_hu, 16, vsubwev_w_hu)
gen_trans_vvvd(vsubwev_d_wu, 16, vsubwev_d_wu)
gen_trans_vvvd(vsubwev_q_du, 16, vsubwev_q_du)
gen_trans_vvvd(vsubwod_h_bu, 16, vsubwod_h_bu)
gen_trans_vvvd(vsubwod_w_hu, 16, vsubwod_w_hu)
gen_trans_vvvd(vsubwod_d_wu, 16, vsubwod_d_wu)
gen_trans_vvvd(vsubwod_q_du, 16, vsubwod_q_du)
gen_trans_vvvd(vaddwev_h_bu_b, 16, vaddwev_h_bu_b)
gen_trans_vvvd(vaddwev_w_hu_h, 16, vaddwev_w_hu_h)
gen_trans_vvvd(vaddwev_d_wu_w, 16, vaddwev_d_wu_w)
gen_trans_vvvd(vaddwev_q_du_d, 16, vaddwev_q_du_d)
gen_trans_vvvd(vaddwod_h_bu_b, 16, vaddwod_h_bu_b)
gen_trans_vvvd(vaddwod_w_hu_h, 16, vaddwod_w_hu_h)
gen_trans_vvvd(vaddwod_d_wu_w, 16, vaddwod_d_wu_w)
gen_trans_vvvd(vaddwod_q_du_d, 16, vaddwod_q_du_d)
#define DO_VAVG(a, b)  ((a >> 1) + (b >> 1) + (a & b & 1))
#define DO_VAVGR(a, b) ((a >> 1) + (b >> 1) + ((a | b) & 1))
static inline bool vavg_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = DO_VAVG(env->fpr[a->vj].vreg.B[i], env->fpr[a->vk].vreg.B[i]) ;}env->pc += 4;return true;}
static inline bool vavg_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = DO_VAVG(env->fpr[a->vj].vreg.H[i], env->fpr[a->vk].vreg.H[i]) ;}env->pc += 4;return true;}
static inline bool vavg_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = DO_VAVG(env->fpr[a->vj].vreg.W[i], env->fpr[a->vk].vreg.W[i]) ;}env->pc += 4;return true;}
static inline bool vavg_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = DO_VAVG(env->fpr[a->vj].vreg.D[i], env->fpr[a->vk].vreg.D[i]) ;}env->pc += 4;return true;}
static bool trans_vavg_b(CPULoongArchState *env, arg_vavg_b *restrict a) {CHECK_FPE(16); return vavg_b(env, a, 16);}
static bool trans_vavg_h(CPULoongArchState *env, arg_vavg_h *restrict a) {CHECK_FPE(16); return vavg_h(env, a, 16);}
static bool trans_vavg_w(CPULoongArchState *env, arg_vavg_w *restrict a) {CHECK_FPE(16); return vavg_w(env, a, 16);}
static bool trans_vavg_d(CPULoongArchState *env, arg_vavg_d *restrict a) {CHECK_FPE(16); return vavg_d(env, a, 16);}
static bool trans_xvavg_b(CPULoongArchState *env, arg_vavg_b *restrict a) {CHECK_FPE(32); return vavg_b(env, a, 32);}
static bool trans_xvavg_h(CPULoongArchState *env, arg_vavg_h *restrict a) {CHECK_FPE(32); return vavg_h(env, a, 32);}
static bool trans_xvavg_w(CPULoongArchState *env, arg_vavg_w *restrict a) {CHECK_FPE(32); return vavg_w(env, a, 32);}
static bool trans_xvavg_d(CPULoongArchState *env, arg_vavg_d *restrict a) {CHECK_FPE(32); return vavg_d(env, a, 32);}
static inline bool vavg_bu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = DO_VAVG(env->fpr[a->vj].vreg.UB[i], env->fpr[a->vk].vreg.UB[i]) ;}env->pc += 4;return true;}
static inline bool vavg_hu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = DO_VAVG(env->fpr[a->vj].vreg.UH[i], env->fpr[a->vk].vreg.UH[i]) ;}env->pc += 4;return true;}
static inline bool vavg_wu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = DO_VAVG(env->fpr[a->vj].vreg.UW[i], env->fpr[a->vk].vreg.UW[i]) ;}env->pc += 4;return true;}
static inline bool vavg_du(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = DO_VAVG(env->fpr[a->vj].vreg.UD[i], env->fpr[a->vk].vreg.UD[i]) ;}env->pc += 4;return true;}
static bool trans_vavg_bu(CPULoongArchState *env, arg_vavg_bu *restrict a) {CHECK_FPE(16); return vavg_bu(env, a, 16);}
static bool trans_vavg_hu(CPULoongArchState *env, arg_vavg_hu *restrict a) {CHECK_FPE(16); return vavg_hu(env, a, 16);}
static bool trans_vavg_wu(CPULoongArchState *env, arg_vavg_wu *restrict a) {CHECK_FPE(16); return vavg_wu(env, a, 16);}
static bool trans_vavg_du(CPULoongArchState *env, arg_vavg_du *restrict a) {CHECK_FPE(16); return vavg_du(env, a, 16);}
static bool trans_xvavg_bu(CPULoongArchState *env, arg_vavg_bu *restrict a) {CHECK_FPE(32); return vavg_bu(env, a, 32);}
static bool trans_xvavg_hu(CPULoongArchState *env, arg_vavg_hu *restrict a) {CHECK_FPE(32); return vavg_hu(env, a, 32);}
static bool trans_xvavg_wu(CPULoongArchState *env, arg_vavg_wu *restrict a) {CHECK_FPE(32); return vavg_wu(env, a, 32);}
static bool trans_xvavg_du(CPULoongArchState *env, arg_vavg_du *restrict a) {CHECK_FPE(32); return vavg_du(env, a, 32);}
static inline bool vavgr_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = DO_VAVGR(env->fpr[a->vj].vreg.B[i], env->fpr[a->vk].vreg.B[i]) ;}env->pc += 4;return true;}
static inline bool vavgr_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = DO_VAVGR(env->fpr[a->vj].vreg.H[i], env->fpr[a->vk].vreg.H[i]) ;}env->pc += 4;return true;}
static inline bool vavgr_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = DO_VAVGR(env->fpr[a->vj].vreg.W[i], env->fpr[a->vk].vreg.W[i]) ;}env->pc += 4;return true;}
static inline bool vavgr_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = DO_VAVGR(env->fpr[a->vj].vreg.D[i], env->fpr[a->vk].vreg.D[i]) ;}env->pc += 4;return true;}
static bool trans_vavgr_b(CPULoongArchState *env, arg_vavgr_b *restrict a) {CHECK_FPE(16); return vavgr_b(env, a, 16);}
static bool trans_vavgr_h(CPULoongArchState *env, arg_vavgr_h *restrict a) {CHECK_FPE(16); return vavgr_h(env, a, 16);}
static bool trans_vavgr_w(CPULoongArchState *env, arg_vavgr_w *restrict a) {CHECK_FPE(16); return vavgr_w(env, a, 16);}
static bool trans_vavgr_d(CPULoongArchState *env, arg_vavgr_d *restrict a) {CHECK_FPE(16); return vavgr_d(env, a, 16);}
static bool trans_xvavgr_b(CPULoongArchState *env, arg_vavgr_b *restrict a) {CHECK_FPE(32); return vavgr_b(env, a, 32);}
static bool trans_xvavgr_h(CPULoongArchState *env, arg_vavgr_h *restrict a) {CHECK_FPE(32); return vavgr_h(env, a, 32);}
static bool trans_xvavgr_w(CPULoongArchState *env, arg_vavgr_w *restrict a) {CHECK_FPE(32); return vavgr_w(env, a, 32);}
static bool trans_xvavgr_d(CPULoongArchState *env, arg_vavgr_d *restrict a) {CHECK_FPE(32); return vavgr_d(env, a, 32);}
static inline bool vavgr_bu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = DO_VAVGR(env->fpr[a->vj].vreg.UB[i], env->fpr[a->vk].vreg.UB[i]) ;}env->pc += 4;return true;}
static inline bool vavgr_hu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = DO_VAVGR(env->fpr[a->vj].vreg.UH[i], env->fpr[a->vk].vreg.UH[i]) ;}env->pc += 4;return true;}
static inline bool vavgr_wu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = DO_VAVGR(env->fpr[a->vj].vreg.UW[i], env->fpr[a->vk].vreg.UW[i]) ;}env->pc += 4;return true;}
static inline bool vavgr_du(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = DO_VAVGR(env->fpr[a->vj].vreg.UD[i], env->fpr[a->vk].vreg.UD[i]) ;}env->pc += 4;return true;}
static bool trans_vavgr_bu(CPULoongArchState *env, arg_vavgr_bu *restrict a) {CHECK_FPE(16); return vavgr_bu(env, a, 16);}
static bool trans_vavgr_hu(CPULoongArchState *env, arg_vavgr_hu *restrict a) {CHECK_FPE(16); return vavgr_hu(env, a, 16);}
static bool trans_vavgr_wu(CPULoongArchState *env, arg_vavgr_wu *restrict a) {CHECK_FPE(16); return vavgr_wu(env, a, 16);}
static bool trans_vavgr_du(CPULoongArchState *env, arg_vavgr_du *restrict a) {CHECK_FPE(16); return vavgr_du(env, a, 16);}
static bool trans_xvavgr_bu(CPULoongArchState *env, arg_vavgr_bu *restrict a) {CHECK_FPE(32); return vavgr_bu(env, a, 32);}
static bool trans_xvavgr_hu(CPULoongArchState *env, arg_vavgr_hu *restrict a) {CHECK_FPE(32); return vavgr_hu(env, a, 32);}
static bool trans_xvavgr_wu(CPULoongArchState *env, arg_vavgr_wu *restrict a) {CHECK_FPE(32); return vavgr_wu(env, a, 32);}
static bool trans_xvavgr_du(CPULoongArchState *env, arg_vavgr_du *restrict a) {CHECK_FPE(32); return vavgr_du(env, a, 32);}
#define DO_VABSD(a, b)  ((a > b) ? (a -b) : (b-a))
static inline bool vabsd_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = DO_VABSD(env->fpr[a->vj].vreg.B[i], env->fpr[a->vk].vreg.B[i]) ;}env->pc += 4;return true;}
static inline bool vabsd_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = DO_VABSD(env->fpr[a->vj].vreg.H[i], env->fpr[a->vk].vreg.H[i]) ;}env->pc += 4;return true;}
static inline bool vabsd_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = DO_VABSD(env->fpr[a->vj].vreg.W[i], env->fpr[a->vk].vreg.W[i]) ;}env->pc += 4;return true;}
static inline bool vabsd_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = DO_VABSD(env->fpr[a->vj].vreg.D[i], env->fpr[a->vk].vreg.D[i]) ;}env->pc += 4;return true;}
static bool trans_vabsd_b(CPULoongArchState *env, arg_vabsd_b *restrict a) {CHECK_FPE(16); return vabsd_b(env, a, 16);}
static bool trans_vabsd_h(CPULoongArchState *env, arg_vabsd_h *restrict a) {CHECK_FPE(16); return vabsd_h(env, a, 16);}
static bool trans_vabsd_w(CPULoongArchState *env, arg_vabsd_w *restrict a) {CHECK_FPE(16); return vabsd_w(env, a, 16);}
static bool trans_vabsd_d(CPULoongArchState *env, arg_vabsd_d *restrict a) {CHECK_FPE(16); return vabsd_d(env, a, 16);}
static bool trans_xvabsd_b(CPULoongArchState *env, arg_vabsd_b *restrict a) {CHECK_FPE(32); return vabsd_b(env, a, 32);}
static bool trans_xvabsd_h(CPULoongArchState *env, arg_vabsd_h *restrict a) {CHECK_FPE(32); return vabsd_h(env, a, 32);}
static bool trans_xvabsd_w(CPULoongArchState *env, arg_vabsd_w *restrict a) {CHECK_FPE(32); return vabsd_w(env, a, 32);}
static bool trans_xvabsd_d(CPULoongArchState *env, arg_vabsd_d *restrict a) {CHECK_FPE(32); return vabsd_d(env, a, 32);}
static inline bool vabsd_bu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = DO_VABSD(env->fpr[a->vj].vreg.UB[i], env->fpr[a->vk].vreg.UB[i]) ;}env->pc += 4;return true;}
static inline bool vabsd_hu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = DO_VABSD(env->fpr[a->vj].vreg.UH[i], env->fpr[a->vk].vreg.UH[i]) ;}env->pc += 4;return true;}
static inline bool vabsd_wu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = DO_VABSD(env->fpr[a->vj].vreg.UW[i], env->fpr[a->vk].vreg.UW[i]) ;}env->pc += 4;return true;}
static inline bool vabsd_du(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = DO_VABSD(env->fpr[a->vj].vreg.UD[i], env->fpr[a->vk].vreg.UD[i]) ;}env->pc += 4;return true;}
static bool trans_vabsd_bu(CPULoongArchState *env, arg_vabsd_bu *restrict a) {CHECK_FPE(16); return vabsd_bu(env, a, 16);}
static bool trans_vabsd_hu(CPULoongArchState *env, arg_vabsd_hu *restrict a) {CHECK_FPE(16); return vabsd_hu(env, a, 16);}
static bool trans_vabsd_wu(CPULoongArchState *env, arg_vabsd_wu *restrict a) {CHECK_FPE(16); return vabsd_wu(env, a, 16);}
static bool trans_vabsd_du(CPULoongArchState *env, arg_vabsd_du *restrict a) {CHECK_FPE(16); return vabsd_du(env, a, 16);}
static bool trans_xvabsd_bu(CPULoongArchState *env, arg_vabsd_bu *restrict a) {CHECK_FPE(32); return vabsd_bu(env, a, 32);}
static bool trans_xvabsd_hu(CPULoongArchState *env, arg_vabsd_hu *restrict a) {CHECK_FPE(32); return vabsd_hu(env, a, 32);}
static bool trans_xvabsd_wu(CPULoongArchState *env, arg_vabsd_wu *restrict a) {CHECK_FPE(32); return vabsd_wu(env, a, 32);}
static bool trans_xvabsd_du(CPULoongArchState *env, arg_vabsd_du *restrict a) {CHECK_FPE(32); return vabsd_du(env, a, 32);}
#define DO_VABS(a)  ((a < 0) ? (-a) : (a))
static inline bool vadda_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = DO_VABS(env->fpr[a->vj].vreg.B[i]) + DO_VABS(env->fpr[a->vk].vreg.B[i]) ;}env->pc += 4;return true;}
static inline bool vadda_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = DO_VABS(env->fpr[a->vj].vreg.H[i]) + DO_VABS(env->fpr[a->vk].vreg.H[i]) ;}env->pc += 4;return true;}
static inline bool vadda_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = DO_VABS(env->fpr[a->vj].vreg.W[i]) + DO_VABS(env->fpr[a->vk].vreg.W[i]) ;}env->pc += 4;return true;}
static inline bool vadda_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = DO_VABS(env->fpr[a->vj].vreg.D[i]) + DO_VABS(env->fpr[a->vk].vreg.D[i]) ;}env->pc += 4;return true;}
static bool trans_vadda_b(CPULoongArchState *env, arg_vadda_b *restrict a) {CHECK_FPE(16); return vadda_b(env, a, 16);}
static bool trans_vadda_h(CPULoongArchState *env, arg_vadda_h *restrict a) {CHECK_FPE(16); return vadda_h(env, a, 16);}
static bool trans_vadda_w(CPULoongArchState *env, arg_vadda_w *restrict a) {CHECK_FPE(16); return vadda_w(env, a, 16);}
static bool trans_vadda_d(CPULoongArchState *env, arg_vadda_d *restrict a) {CHECK_FPE(16); return vadda_d(env, a, 16);}
static bool trans_xvadda_b(CPULoongArchState *env, arg_vadda_b *restrict a) {CHECK_FPE(32); return vadda_b(env, a, 32);}
static bool trans_xvadda_h(CPULoongArchState *env, arg_vadda_h *restrict a) {CHECK_FPE(32); return vadda_h(env, a, 32);}
static bool trans_xvadda_w(CPULoongArchState *env, arg_vadda_w *restrict a) {CHECK_FPE(32); return vadda_w(env, a, 32);}
static bool trans_xvadda_d(CPULoongArchState *env, arg_vadda_d *restrict a) {CHECK_FPE(32); return vadda_d(env, a, 32);}
static inline bool vmax_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = MAX(env->fpr[a->vj].vreg.B[i], env->fpr[a->vk].vreg.B[i]) ;}env->pc += 4;return true;}
static inline bool vmax_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = MAX(env->fpr[a->vj].vreg.H[i], env->fpr[a->vk].vreg.H[i]) ;}env->pc += 4;return true;}
static inline bool vmax_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = MAX(env->fpr[a->vj].vreg.W[i], env->fpr[a->vk].vreg.W[i]) ;}env->pc += 4;return true;}
static inline bool vmax_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = MAX(env->fpr[a->vj].vreg.D[i], env->fpr[a->vk].vreg.D[i]) ;}env->pc += 4;return true;}
static bool trans_vmax_b(CPULoongArchState *env, arg_vmax_b *restrict a) {CHECK_FPE(16); return vmax_b(env, a, 16);}
static bool trans_vmax_h(CPULoongArchState *env, arg_vmax_h *restrict a) {CHECK_FPE(16); return vmax_h(env, a, 16);}
static bool trans_vmax_w(CPULoongArchState *env, arg_vmax_w *restrict a) {CHECK_FPE(16); return vmax_w(env, a, 16);}
static bool trans_vmax_d(CPULoongArchState *env, arg_vmax_d *restrict a) {CHECK_FPE(16); return vmax_d(env, a, 16);}
static bool trans_xvmax_b(CPULoongArchState *env, arg_vmax_b *restrict a) {CHECK_FPE(32); return vmax_b(env, a, 32);}
static bool trans_xvmax_h(CPULoongArchState *env, arg_vmax_h *restrict a) {CHECK_FPE(32); return vmax_h(env, a, 32);}
static bool trans_xvmax_w(CPULoongArchState *env, arg_vmax_w *restrict a) {CHECK_FPE(32); return vmax_w(env, a, 32);}
static bool trans_xvmax_d(CPULoongArchState *env, arg_vmax_d *restrict a) {CHECK_FPE(32); return vmax_d(env, a, 32);}
static inline bool vmaxi_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = MAX(env->fpr[a->vj].vreg.B[i], a->imm) ;}env->pc += 4;return true;}
static inline bool vmaxi_h(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = MAX(env->fpr[a->vj].vreg.H[i], a->imm) ;}env->pc += 4;return true;}
static inline bool vmaxi_w(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = MAX(env->fpr[a->vj].vreg.W[i], a->imm) ;}env->pc += 4;return true;}
static inline bool vmaxi_d(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = MAX(env->fpr[a->vj].vreg.D[i], a->imm) ;}env->pc += 4;return true;}
static bool trans_vmaxi_b(CPULoongArchState *env, arg_vmaxi_b *restrict a) {CHECK_FPE(16); return vmaxi_b(env, a, 16);}
static bool trans_vmaxi_h(CPULoongArchState *env, arg_vmaxi_h *restrict a) {CHECK_FPE(16); return vmaxi_h(env, a, 16);}
static bool trans_vmaxi_w(CPULoongArchState *env, arg_vmaxi_w *restrict a) {CHECK_FPE(16); return vmaxi_w(env, a, 16);}
static bool trans_vmaxi_d(CPULoongArchState *env, arg_vmaxi_d *restrict a) {CHECK_FPE(16); return vmaxi_d(env, a, 16);}
static bool trans_xvmaxi_b(CPULoongArchState *env, arg_vmaxi_b *restrict a) {CHECK_FPE(32); return vmaxi_b(env, a, 32);}
static bool trans_xvmaxi_h(CPULoongArchState *env, arg_vmaxi_h *restrict a) {CHECK_FPE(32); return vmaxi_h(env, a, 32);}
static bool trans_xvmaxi_w(CPULoongArchState *env, arg_vmaxi_w *restrict a) {CHECK_FPE(32); return vmaxi_w(env, a, 32);}
static bool trans_xvmaxi_d(CPULoongArchState *env, arg_vmaxi_d *restrict a) {CHECK_FPE(32); return vmaxi_d(env, a, 32);}
static inline bool vmax_bu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = MAX(env->fpr[a->vj].vreg.UB[i], env->fpr[a->vk].vreg.UB[i]) ;}env->pc += 4;return true;}
static inline bool vmax_hu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = MAX(env->fpr[a->vj].vreg.UH[i], env->fpr[a->vk].vreg.UH[i]) ;}env->pc += 4;return true;}
static inline bool vmax_wu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = MAX(env->fpr[a->vj].vreg.UW[i], env->fpr[a->vk].vreg.UW[i]) ;}env->pc += 4;return true;}
static inline bool vmax_du(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = MAX(env->fpr[a->vj].vreg.UD[i], env->fpr[a->vk].vreg.UD[i]) ;}env->pc += 4;return true;}
static bool trans_vmax_bu(CPULoongArchState *env, arg_vmax_bu *restrict a) {CHECK_FPE(16); return vmax_bu(env, a, 16);}
static bool trans_vmax_hu(CPULoongArchState *env, arg_vmax_hu *restrict a) {CHECK_FPE(16); return vmax_hu(env, a, 16);}
static bool trans_vmax_wu(CPULoongArchState *env, arg_vmax_wu *restrict a) {CHECK_FPE(16); return vmax_wu(env, a, 16);}
static bool trans_vmax_du(CPULoongArchState *env, arg_vmax_du *restrict a) {CHECK_FPE(16); return vmax_du(env, a, 16);}
static bool trans_xvmax_bu(CPULoongArchState *env, arg_vmax_bu *restrict a) {CHECK_FPE(32); return vmax_bu(env, a, 32);}
static bool trans_xvmax_hu(CPULoongArchState *env, arg_vmax_hu *restrict a) {CHECK_FPE(32); return vmax_hu(env, a, 32);}
static bool trans_xvmax_wu(CPULoongArchState *env, arg_vmax_wu *restrict a) {CHECK_FPE(32); return vmax_wu(env, a, 32);}
static bool trans_xvmax_du(CPULoongArchState *env, arg_vmax_du *restrict a) {CHECK_FPE(32); return vmax_du(env, a, 32);}
static inline bool vmaxi_bu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = MAX(env->fpr[a->vj].vreg.UB[i], (uint64_t)a->imm) ;}env->pc += 4;return true;}
static inline bool vmaxi_hu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = MAX(env->fpr[a->vj].vreg.UH[i], (uint64_t)a->imm) ;}env->pc += 4;return true;}
static inline bool vmaxi_wu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = MAX(env->fpr[a->vj].vreg.UW[i], (uint64_t)a->imm) ;}env->pc += 4;return true;}
static inline bool vmaxi_du(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = MAX(env->fpr[a->vj].vreg.UD[i], (uint64_t)a->imm) ;}env->pc += 4;return true;}
static bool trans_vmaxi_bu(CPULoongArchState *env, arg_vmaxi_bu *restrict a) {CHECK_FPE(16); return vmaxi_bu(env, a, 16);}
static bool trans_vmaxi_hu(CPULoongArchState *env, arg_vmaxi_hu *restrict a) {CHECK_FPE(16); return vmaxi_hu(env, a, 16);}
static bool trans_vmaxi_wu(CPULoongArchState *env, arg_vmaxi_wu *restrict a) {CHECK_FPE(16); return vmaxi_wu(env, a, 16);}
static bool trans_vmaxi_du(CPULoongArchState *env, arg_vmaxi_du *restrict a) {CHECK_FPE(16); return vmaxi_du(env, a, 16);}
static bool trans_xvmaxi_bu(CPULoongArchState *env, arg_vmaxi_bu *restrict a) {CHECK_FPE(32); return vmaxi_bu(env, a, 32);}
static bool trans_xvmaxi_hu(CPULoongArchState *env, arg_vmaxi_hu *restrict a) {CHECK_FPE(32); return vmaxi_hu(env, a, 32);}
static bool trans_xvmaxi_wu(CPULoongArchState *env, arg_vmaxi_wu *restrict a) {CHECK_FPE(32); return vmaxi_wu(env, a, 32);}
static bool trans_xvmaxi_du(CPULoongArchState *env, arg_vmaxi_du *restrict a) {CHECK_FPE(32); return vmaxi_du(env, a, 32);}
static inline bool vmin_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = MIN(env->fpr[a->vj].vreg.B[i], env->fpr[a->vk].vreg.B[i]) ;}env->pc += 4;return true;}
static inline bool vmin_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = MIN(env->fpr[a->vj].vreg.H[i], env->fpr[a->vk].vreg.H[i]) ;}env->pc += 4;return true;}
static inline bool vmin_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = MIN(env->fpr[a->vj].vreg.W[i], env->fpr[a->vk].vreg.W[i]) ;}env->pc += 4;return true;}
static inline bool vmin_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = MIN(env->fpr[a->vj].vreg.D[i], env->fpr[a->vk].vreg.D[i]) ;}env->pc += 4;return true;}
static bool trans_vmin_b(CPULoongArchState *env, arg_vmin_b *restrict a) {CHECK_FPE(16); return vmin_b(env, a, 16);}
static bool trans_vmin_h(CPULoongArchState *env, arg_vmin_h *restrict a) {CHECK_FPE(16); return vmin_h(env, a, 16);}
static bool trans_vmin_w(CPULoongArchState *env, arg_vmin_w *restrict a) {CHECK_FPE(16); return vmin_w(env, a, 16);}
static bool trans_vmin_d(CPULoongArchState *env, arg_vmin_d *restrict a) {CHECK_FPE(16); return vmin_d(env, a, 16);}
static bool trans_xvmin_b(CPULoongArchState *env, arg_vmin_b *restrict a) {CHECK_FPE(32); return vmin_b(env, a, 32);}
static bool trans_xvmin_h(CPULoongArchState *env, arg_vmin_h *restrict a) {CHECK_FPE(32); return vmin_h(env, a, 32);}
static bool trans_xvmin_w(CPULoongArchState *env, arg_vmin_w *restrict a) {CHECK_FPE(32); return vmin_w(env, a, 32);}
static bool trans_xvmin_d(CPULoongArchState *env, arg_vmin_d *restrict a) {CHECK_FPE(32); return vmin_d(env, a, 32);}
static inline bool vmini_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = MIN(env->fpr[a->vj].vreg.B[i], a->imm) ;}env->pc += 4;return true;}
static inline bool vmini_h(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = MIN(env->fpr[a->vj].vreg.H[i], a->imm) ;}env->pc += 4;return true;}
static inline bool vmini_w(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = MIN(env->fpr[a->vj].vreg.W[i], a->imm) ;}env->pc += 4;return true;}
static inline bool vmini_d(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = MIN(env->fpr[a->vj].vreg.D[i], a->imm) ;}env->pc += 4;return true;}
static bool trans_vmini_b(CPULoongArchState *env, arg_vmini_b *restrict a) {CHECK_FPE(16); return vmini_b(env, a, 16);}
static bool trans_vmini_h(CPULoongArchState *env, arg_vmini_h *restrict a) {CHECK_FPE(16); return vmini_h(env, a, 16);}
static bool trans_vmini_w(CPULoongArchState *env, arg_vmini_w *restrict a) {CHECK_FPE(16); return vmini_w(env, a, 16);}
static bool trans_vmini_d(CPULoongArchState *env, arg_vmini_d *restrict a) {CHECK_FPE(16); return vmini_d(env, a, 16);}
static bool trans_xvmini_b(CPULoongArchState *env, arg_vmini_b *restrict a) {CHECK_FPE(32); return vmini_b(env, a, 32);}
static bool trans_xvmini_h(CPULoongArchState *env, arg_vmini_h *restrict a) {CHECK_FPE(32); return vmini_h(env, a, 32);}
static bool trans_xvmini_w(CPULoongArchState *env, arg_vmini_w *restrict a) {CHECK_FPE(32); return vmini_w(env, a, 32);}
static bool trans_xvmini_d(CPULoongArchState *env, arg_vmini_d *restrict a) {CHECK_FPE(32); return vmini_d(env, a, 32);}
static inline bool vmin_bu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = MIN(env->fpr[a->vj].vreg.UB[i], env->fpr[a->vk].vreg.UB[i]) ;}env->pc += 4;return true;}
static inline bool vmin_hu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = MIN(env->fpr[a->vj].vreg.UH[i], env->fpr[a->vk].vreg.UH[i]) ;}env->pc += 4;return true;}
static inline bool vmin_wu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = MIN(env->fpr[a->vj].vreg.UW[i], env->fpr[a->vk].vreg.UW[i]) ;}env->pc += 4;return true;}
static inline bool vmin_du(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = MIN(env->fpr[a->vj].vreg.UD[i], env->fpr[a->vk].vreg.UD[i]) ;}env->pc += 4;return true;}
static bool trans_vmin_bu(CPULoongArchState *env, arg_vmin_bu *restrict a) {CHECK_FPE(16); return vmin_bu(env, a, 16);}
static bool trans_vmin_hu(CPULoongArchState *env, arg_vmin_hu *restrict a) {CHECK_FPE(16); return vmin_hu(env, a, 16);}
static bool trans_vmin_wu(CPULoongArchState *env, arg_vmin_wu *restrict a) {CHECK_FPE(16); return vmin_wu(env, a, 16);}
static bool trans_vmin_du(CPULoongArchState *env, arg_vmin_du *restrict a) {CHECK_FPE(16); return vmin_du(env, a, 16);}
static bool trans_xvmin_bu(CPULoongArchState *env, arg_vmin_bu *restrict a) {CHECK_FPE(32); return vmin_bu(env, a, 32);}
static bool trans_xvmin_hu(CPULoongArchState *env, arg_vmin_hu *restrict a) {CHECK_FPE(32); return vmin_hu(env, a, 32);}
static bool trans_xvmin_wu(CPULoongArchState *env, arg_vmin_wu *restrict a) {CHECK_FPE(32); return vmin_wu(env, a, 32);}
static bool trans_xvmin_du(CPULoongArchState *env, arg_vmin_du *restrict a) {CHECK_FPE(32); return vmin_du(env, a, 32);}
static inline bool vmini_bu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = MIN(env->fpr[a->vj].vreg.UB[i], (uint64_t)a->imm) ;}env->pc += 4;return true;}
static inline bool vmini_hu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = MIN(env->fpr[a->vj].vreg.UH[i], (uint64_t)a->imm) ;}env->pc += 4;return true;}
static inline bool vmini_wu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = MIN(env->fpr[a->vj].vreg.UW[i], (uint64_t)a->imm) ;}env->pc += 4;return true;}
static inline bool vmini_du(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = MIN(env->fpr[a->vj].vreg.UD[i], (uint64_t)a->imm) ;}env->pc += 4;return true;}
static bool trans_vmini_bu(CPULoongArchState *env, arg_vmini_bu *restrict a) {CHECK_FPE(16); return vmini_bu(env, a, 16);}
static bool trans_vmini_hu(CPULoongArchState *env, arg_vmini_hu *restrict a) {CHECK_FPE(16); return vmini_hu(env, a, 16);}
static bool trans_vmini_wu(CPULoongArchState *env, arg_vmini_wu *restrict a) {CHECK_FPE(16); return vmini_wu(env, a, 16);}
static bool trans_vmini_du(CPULoongArchState *env, arg_vmini_du *restrict a) {CHECK_FPE(16); return vmini_du(env, a, 16);}
static bool trans_xvmini_bu(CPULoongArchState *env, arg_vmini_bu *restrict a) {CHECK_FPE(32); return vmini_bu(env, a, 32);}
static bool trans_xvmini_hu(CPULoongArchState *env, arg_vmini_hu *restrict a) {CHECK_FPE(32); return vmini_hu(env, a, 32);}
static bool trans_xvmini_wu(CPULoongArchState *env, arg_vmini_wu *restrict a) {CHECK_FPE(32); return vmini_wu(env, a, 32);}
static bool trans_xvmini_du(CPULoongArchState *env, arg_vmini_du *restrict a) {CHECK_FPE(32); return vmini_du(env, a, 32);}
static inline bool vmul_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = (env->fpr[a->vj].vreg.B[i] * env->fpr[a->vk].vreg.B[i]);}env->pc += 4;return true;}
static inline bool vmul_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = (env->fpr[a->vj].vreg.H[i] * env->fpr[a->vk].vreg.H[i]);}env->pc += 4;return true;}
static inline bool vmul_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = (env->fpr[a->vj].vreg.W[i] * env->fpr[a->vk].vreg.W[i]);}env->pc += 4;return true;}
static inline bool vmul_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = (env->fpr[a->vj].vreg.D[i] * env->fpr[a->vk].vreg.D[i]);}env->pc += 4;return true;}
static bool trans_vmul_b(CPULoongArchState *env, arg_vmul_b *restrict a) {CHECK_FPE(16); return vmul_b(env, a, 16);}
static bool trans_vmul_h(CPULoongArchState *env, arg_vmul_h *restrict a) {CHECK_FPE(16); return vmul_h(env, a, 16);}
static bool trans_vmul_w(CPULoongArchState *env, arg_vmul_w *restrict a) {CHECK_FPE(16); return vmul_w(env, a, 16);}
static bool trans_vmul_d(CPULoongArchState *env, arg_vmul_d *restrict a) {CHECK_FPE(16); return vmul_d(env, a, 16);}
static bool trans_xvmul_b(CPULoongArchState *env, arg_vmul_b *restrict a) {CHECK_FPE(32); return vmul_b(env, a, 32);}
static bool trans_xvmul_h(CPULoongArchState *env, arg_vmul_h *restrict a) {CHECK_FPE(32); return vmul_h(env, a, 32);}
static bool trans_xvmul_w(CPULoongArchState *env, arg_vmul_w *restrict a) {CHECK_FPE(32); return vmul_w(env, a, 32);}
static bool trans_xvmul_d(CPULoongArchState *env, arg_vmul_d *restrict a) {CHECK_FPE(32); return vmul_d(env, a, 32);}
gen_trans_vvvd(vmuh_b, 16, vmuh_b)
gen_trans_vvvd(vmuh_bu, 16, vmuh_bu)
gen_trans_vvvd(vmuh_d, 16, vmuh_d)
gen_trans_vvvd(vmuh_du, 16, vmuh_du)
gen_trans_vvvd(vmuh_h, 16, vmuh_h)
gen_trans_vvvd(vmuh_hu, 16, vmuh_hu)
gen_trans_vvvd(vmuh_w, 16, vmuh_w)
gen_trans_vvvd(vmuh_wu, 16, vmuh_wu)
gen_trans_vvvd(vmulwev_h_b, 16, vmulwev_h_b)
gen_trans_vvvd(vmulwev_w_h, 16, vmulwev_w_h)
gen_trans_vvvd(vmulwev_d_w, 16, vmulwev_d_w)
gen_trans_vvvd(vmulwod_h_b, 16, vmulwod_h_b)
gen_trans_vvvd(vmulwod_w_h, 16, vmulwod_w_h)
gen_trans_vvvd(vmulwod_d_w, 16, vmulwod_d_w)
gen_trans_vvvd(vmulwev_h_bu, 16, vmulwev_h_bu)
gen_trans_vvvd(vmulwev_w_hu, 16, vmulwev_w_hu)
gen_trans_vvvd(vmulwev_d_wu, 16, vmulwev_d_wu)
gen_trans_vvvd(vmulwod_h_bu, 16, vmulwod_h_bu)
gen_trans_vvvd(vmulwod_w_hu, 16, vmulwod_w_hu)
gen_trans_vvvd(vmulwod_d_wu, 16, vmulwod_d_wu)
gen_trans_vvvd(vmulwev_h_bu_b, 16, vmulwev_h_bu_b)
gen_trans_vvvd(vmulwev_w_hu_h, 16, vmulwev_w_hu_h)
gen_trans_vvvd(vmulwev_d_wu_w, 16, vmulwev_d_wu_w)
gen_trans_vvvd(vmulwod_h_bu_b, 16, vmulwod_h_bu_b)
gen_trans_vvvd(vmulwod_w_hu_h, 16, vmulwod_w_hu_h)
gen_trans_vvvd(vmulwod_d_wu_w, 16, vmulwod_d_wu_w)
static bool trans_vmulwev_q_d(CPULoongArchState *env, arg_vmulwev_q_d *restrict a) {
    int size = 16;
    int index = 0;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] = (__int128_t)env->fpr[a->vj].vreg.D[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_vmulwod_q_d(CPULoongArchState *env, arg_vmulwod_q_d *restrict a) {
    int size = 16;
    int index = 1;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] = (__int128_t)env->fpr[a->vj].vreg.D[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_vmulwev_q_du(CPULoongArchState *env, arg_vmulwev_q_du *restrict a) {
    int size = 16;
    int index = 0;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] = (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__uint128_t)env->fpr[a->vk].vreg.UD[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_vmulwod_q_du(CPULoongArchState *env, arg_vmulwod_q_du *restrict a) {
    int size = 16;
    int index = 1;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] = (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__uint128_t)env->fpr[a->vk].vreg.UD[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_vmulwev_q_du_d(CPULoongArchState *env, arg_vmulwev_q_du_d *restrict a) {
    int size = 16;
    int index = 0;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] = (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_vmulwod_q_du_d(CPULoongArchState *env, arg_vmulwod_q_du_d *restrict a) {
    int size = 16;
    int index = 1;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] = (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static inline bool vmadd_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vd].vreg.B[i] + (env->fpr[a->vj].vreg.B[i] * env->fpr[a->vk].vreg.B[i]);}env->pc += 4;return true;}
static inline bool vmadd_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vd].vreg.H[i] + (env->fpr[a->vj].vreg.H[i] * env->fpr[a->vk].vreg.H[i]);}env->pc += 4;return true;}
static inline bool vmadd_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vd].vreg.W[i] + (env->fpr[a->vj].vreg.W[i] * env->fpr[a->vk].vreg.W[i]);}env->pc += 4;return true;}
static inline bool vmadd_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vd].vreg.D[i] + (env->fpr[a->vj].vreg.D[i] * env->fpr[a->vk].vreg.D[i]);}env->pc += 4;return true;}
static bool trans_vmadd_b(CPULoongArchState *env, arg_vmadd_b *restrict a) {CHECK_FPE(16); return vmadd_b(env, a, 16);}
static bool trans_vmadd_h(CPULoongArchState *env, arg_vmadd_h *restrict a) {CHECK_FPE(16); return vmadd_h(env, a, 16);}
static bool trans_vmadd_w(CPULoongArchState *env, arg_vmadd_w *restrict a) {CHECK_FPE(16); return vmadd_w(env, a, 16);}
static bool trans_vmadd_d(CPULoongArchState *env, arg_vmadd_d *restrict a) {CHECK_FPE(16); return vmadd_d(env, a, 16);}
static bool trans_xvmadd_b(CPULoongArchState *env, arg_vmadd_b *restrict a) {CHECK_FPE(32); return vmadd_b(env, a, 32);}
static bool trans_xvmadd_h(CPULoongArchState *env, arg_vmadd_h *restrict a) {CHECK_FPE(32); return vmadd_h(env, a, 32);}
static bool trans_xvmadd_w(CPULoongArchState *env, arg_vmadd_w *restrict a) {CHECK_FPE(32); return vmadd_w(env, a, 32);}
static bool trans_xvmadd_d(CPULoongArchState *env, arg_vmadd_d *restrict a) {CHECK_FPE(32); return vmadd_d(env, a, 32);}
static inline bool vmsub_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vd].vreg.B[i] - (env->fpr[a->vj].vreg.B[i] * env->fpr[a->vk].vreg.B[i]);}env->pc += 4;return true;}
static inline bool vmsub_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vd].vreg.H[i] - (env->fpr[a->vj].vreg.H[i] * env->fpr[a->vk].vreg.H[i]);}env->pc += 4;return true;}
static inline bool vmsub_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vd].vreg.W[i] - (env->fpr[a->vj].vreg.W[i] * env->fpr[a->vk].vreg.W[i]);}env->pc += 4;return true;}
static inline bool vmsub_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vd].vreg.D[i] - (env->fpr[a->vj].vreg.D[i] * env->fpr[a->vk].vreg.D[i]);}env->pc += 4;return true;}
static bool trans_vmsub_b(CPULoongArchState *env, arg_vmsub_b *restrict a) {CHECK_FPE(16); return vmsub_b(env, a, 16);}
static bool trans_vmsub_h(CPULoongArchState *env, arg_vmsub_h *restrict a) {CHECK_FPE(16); return vmsub_h(env, a, 16);}
static bool trans_vmsub_w(CPULoongArchState *env, arg_vmsub_w *restrict a) {CHECK_FPE(16); return vmsub_w(env, a, 16);}
static bool trans_vmsub_d(CPULoongArchState *env, arg_vmsub_d *restrict a) {CHECK_FPE(16); return vmsub_d(env, a, 16);}
static bool trans_xvmsub_b(CPULoongArchState *env, arg_vmsub_b *restrict a) {CHECK_FPE(32); return vmsub_b(env, a, 32);}
static bool trans_xvmsub_h(CPULoongArchState *env, arg_vmsub_h *restrict a) {CHECK_FPE(32); return vmsub_h(env, a, 32);}
static bool trans_xvmsub_w(CPULoongArchState *env, arg_vmsub_w *restrict a) {CHECK_FPE(32); return vmsub_w(env, a, 32);}
static bool trans_xvmsub_d(CPULoongArchState *env, arg_vmsub_d *restrict a) {CHECK_FPE(32); return vmsub_d(env, a, 32);}

gen_trans_vvvd(vmaddwev_h_b, 16, vmaddwev_h_b)
gen_trans_vvvd(vmaddwev_w_h, 16, vmaddwev_w_h)
gen_trans_vvvd(vmaddwev_d_w, 16, vmaddwev_d_w)
gen_trans_vvvd(vmaddwod_h_b, 16, vmaddwod_h_b)
gen_trans_vvvd(vmaddwod_w_h, 16, vmaddwod_w_h)
gen_trans_vvvd(vmaddwod_d_w, 16, vmaddwod_d_w)
gen_trans_vvvd(vmaddwev_h_bu, 16, vmaddwev_h_bu)
gen_trans_vvvd(vmaddwev_w_hu, 16, vmaddwev_w_hu)
gen_trans_vvvd(vmaddwev_d_wu, 16, vmaddwev_d_wu)
gen_trans_vvvd(vmaddwod_h_bu, 16, vmaddwod_h_bu)
gen_trans_vvvd(vmaddwod_w_hu, 16, vmaddwod_w_hu)
gen_trans_vvvd(vmaddwod_d_wu, 16, vmaddwod_d_wu)
gen_trans_vvvd(vmaddwev_h_bu_b, 16, vmaddwev_h_bu_b)
gen_trans_vvvd(vmaddwev_w_hu_h, 16, vmaddwev_w_hu_h)
gen_trans_vvvd(vmaddwev_d_wu_w, 16, vmaddwev_d_wu_w)
gen_trans_vvvd(vmaddwod_h_bu_b, 16, vmaddwod_h_bu_b)
gen_trans_vvvd(vmaddwod_w_hu_h, 16, vmaddwod_w_hu_h)
gen_trans_vvvd(vmaddwod_d_wu_w, 16, vmaddwod_d_wu_w)
static bool trans_vmaddwev_q_d(CPULoongArchState *env, arg_vmaddwev_q_d *restrict a) {
    int size = 16;
    int index = 0;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] += (__int128_t)env->fpr[a->vj].vreg.D[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_vmaddwod_q_d(CPULoongArchState *env, arg_vmaddwod_q_d *restrict a) {
    int size = 16;
    int index = 1;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] += (__int128_t)env->fpr[a->vj].vreg.D[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_vmaddwev_q_du(CPULoongArchState *env, arg_vmaddwev_q_du *restrict a) {
    int size = 16;
    int index = 0;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] += (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__uint128_t)env->fpr[a->vk].vreg.UD[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_vmaddwod_q_du(CPULoongArchState *env, arg_vmaddwod_q_du *restrict a) {
    int size = 16;
    int index = 1;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] += (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__uint128_t)env->fpr[a->vk].vreg.UD[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_vmaddwev_q_du_d(CPULoongArchState *env, arg_vmaddwev_q_du_d *restrict a) {
    int size = 16;
    int index = 0;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] += (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_vmaddwod_q_du_d(CPULoongArchState *env, arg_vmaddwod_q_du_d *restrict a) {
    int size = 16;
    int index = 1;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] += (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static inline bool vdiv_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] / env->fpr[a->vk].vreg.B[i];}env->pc += 4;return true;}
static inline bool vdiv_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] / env->fpr[a->vk].vreg.H[i];}env->pc += 4;return true;}
static inline bool vdiv_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] / env->fpr[a->vk].vreg.W[i];}env->pc += 4;return true;}
static inline bool vdiv_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] / env->fpr[a->vk].vreg.D[i];}env->pc += 4;return true;}
static bool trans_vdiv_b(CPULoongArchState *env, arg_vdiv_b *restrict a) {CHECK_FPE(16); return vdiv_b(env, a, 16);}
static bool trans_vdiv_h(CPULoongArchState *env, arg_vdiv_h *restrict a) {CHECK_FPE(16); return vdiv_h(env, a, 16);}
static bool trans_vdiv_w(CPULoongArchState *env, arg_vdiv_w *restrict a) {CHECK_FPE(16); return vdiv_w(env, a, 16);}
static bool trans_vdiv_d(CPULoongArchState *env, arg_vdiv_d *restrict a) {CHECK_FPE(16); return vdiv_d(env, a, 16);}
static bool trans_xvdiv_b(CPULoongArchState *env, arg_vdiv_b *restrict a) {CHECK_FPE(32); return vdiv_b(env, a, 32);}
static bool trans_xvdiv_h(CPULoongArchState *env, arg_vdiv_h *restrict a) {CHECK_FPE(32); return vdiv_h(env, a, 32);}
static bool trans_xvdiv_w(CPULoongArchState *env, arg_vdiv_w *restrict a) {CHECK_FPE(32); return vdiv_w(env, a, 32);}
static bool trans_xvdiv_d(CPULoongArchState *env, arg_vdiv_d *restrict a) {CHECK_FPE(32); return vdiv_d(env, a, 32);}
static inline bool vdiv_bu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] / env->fpr[a->vk].vreg.UB[i];}env->pc += 4;return true;}
static inline bool vdiv_hu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] / env->fpr[a->vk].vreg.UH[i];}env->pc += 4;return true;}
static inline bool vdiv_wu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] / env->fpr[a->vk].vreg.UW[i];}env->pc += 4;return true;}
static inline bool vdiv_du(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] / env->fpr[a->vk].vreg.UD[i];}env->pc += 4;return true;}
static bool trans_vdiv_bu(CPULoongArchState *env, arg_vdiv_bu *restrict a) {CHECK_FPE(16); return vdiv_bu(env, a, 16);}
static bool trans_vdiv_hu(CPULoongArchState *env, arg_vdiv_hu *restrict a) {CHECK_FPE(16); return vdiv_hu(env, a, 16);}
static bool trans_vdiv_wu(CPULoongArchState *env, arg_vdiv_wu *restrict a) {CHECK_FPE(16); return vdiv_wu(env, a, 16);}
static bool trans_vdiv_du(CPULoongArchState *env, arg_vdiv_du *restrict a) {CHECK_FPE(16); return vdiv_du(env, a, 16);}
static bool trans_xvdiv_bu(CPULoongArchState *env, arg_vdiv_bu *restrict a) {CHECK_FPE(32); return vdiv_bu(env, a, 32);}
static bool trans_xvdiv_hu(CPULoongArchState *env, arg_vdiv_hu *restrict a) {CHECK_FPE(32); return vdiv_hu(env, a, 32);}
static bool trans_xvdiv_wu(CPULoongArchState *env, arg_vdiv_wu *restrict a) {CHECK_FPE(32); return vdiv_wu(env, a, 32);}
static bool trans_xvdiv_du(CPULoongArchState *env, arg_vdiv_du *restrict a) {CHECK_FPE(32); return vdiv_du(env, a, 32);}
static inline bool vmod_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] % env->fpr[a->vk].vreg.B[i];}env->pc += 4;return true;}
static inline bool vmod_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] % env->fpr[a->vk].vreg.H[i];}env->pc += 4;return true;}
static inline bool vmod_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] % env->fpr[a->vk].vreg.W[i];}env->pc += 4;return true;}
static inline bool vmod_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] % env->fpr[a->vk].vreg.D[i];}env->pc += 4;return true;}
static bool trans_vmod_b(CPULoongArchState *env, arg_vmod_b *restrict a) {CHECK_FPE(16); return vmod_b(env, a, 16);}
static bool trans_vmod_h(CPULoongArchState *env, arg_vmod_h *restrict a) {CHECK_FPE(16); return vmod_h(env, a, 16);}
static bool trans_vmod_w(CPULoongArchState *env, arg_vmod_w *restrict a) {CHECK_FPE(16); return vmod_w(env, a, 16);}
static bool trans_vmod_d(CPULoongArchState *env, arg_vmod_d *restrict a) {CHECK_FPE(16); return vmod_d(env, a, 16);}
static bool trans_xvmod_b(CPULoongArchState *env, arg_vmod_b *restrict a) {CHECK_FPE(32); return vmod_b(env, a, 32);}
static bool trans_xvmod_h(CPULoongArchState *env, arg_vmod_h *restrict a) {CHECK_FPE(32); return vmod_h(env, a, 32);}
static bool trans_xvmod_w(CPULoongArchState *env, arg_vmod_w *restrict a) {CHECK_FPE(32); return vmod_w(env, a, 32);}
static bool trans_xvmod_d(CPULoongArchState *env, arg_vmod_d *restrict a) {CHECK_FPE(32); return vmod_d(env, a, 32);}
static inline bool vmod_bu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] % env->fpr[a->vk].vreg.UB[i];}env->pc += 4;return true;}
static inline bool vmod_hu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] % env->fpr[a->vk].vreg.UH[i];}env->pc += 4;return true;}
static inline bool vmod_wu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] % env->fpr[a->vk].vreg.UW[i];}env->pc += 4;return true;}
static inline bool vmod_du(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] % env->fpr[a->vk].vreg.UD[i];}env->pc += 4;return true;}
static bool trans_vmod_bu(CPULoongArchState *env, arg_vmod_bu *restrict a) {CHECK_FPE(16); return vmod_bu(env, a, 16);}
static bool trans_vmod_hu(CPULoongArchState *env, arg_vmod_hu *restrict a) {CHECK_FPE(16); return vmod_hu(env, a, 16);}
static bool trans_vmod_wu(CPULoongArchState *env, arg_vmod_wu *restrict a) {CHECK_FPE(16); return vmod_wu(env, a, 16);}
static bool trans_vmod_du(CPULoongArchState *env, arg_vmod_du *restrict a) {CHECK_FPE(16); return vmod_du(env, a, 16);}
static bool trans_xvmod_bu(CPULoongArchState *env, arg_vmod_bu *restrict a) {CHECK_FPE(32); return vmod_bu(env, a, 32);}
static bool trans_xvmod_hu(CPULoongArchState *env, arg_vmod_hu *restrict a) {CHECK_FPE(32); return vmod_hu(env, a, 32);}
static bool trans_xvmod_wu(CPULoongArchState *env, arg_vmod_wu *restrict a) {CHECK_FPE(32); return vmod_wu(env, a, 32);}
static bool trans_xvmod_du(CPULoongArchState *env, arg_vmod_du *restrict a) {CHECK_FPE(32); return vmod_du(env, a, 32);}
#define gen_trans_vvid_sat(op, size, helper_name) \
static bool glue(trans_, op)(CPULoongArchState *env, arg_vv_i *restrict a) {      \
    CHECK_FPE(size);                                                    \
    int oprsz = size;                                                   \
    uint32_t desc = simd_desc(oprsz, oprsz, 0);                         \
    glue(helper_, helper_name)(&env->fpr[a->vd], &env->fpr[a->vj], (1ll << a->imm) - 1, desc);   \
    env->pc += 4;                                                       \
    return true;                                                        \
}
#define gen_trans_vvid_satu(op, size, helper_name) \
static bool glue(trans_, op)(CPULoongArchState *env, arg_vv_i *restrict a) {      \
    CHECK_FPE(size);                                                    \
    int oprsz = size;                                                   \
    uint32_t desc = simd_desc(oprsz, oprsz, 0);                         \
    uint64_t max = (a->imm == 0x3f) ? UINT64_MAX : (1ull << (a->imm + 1)) - 1; \
    glue(helper_, helper_name)(&env->fpr[a->vd], &env->fpr[a->vj], max, desc);   \
    env->pc += 4;                                                       \
    return true;                                                        \
}
gen_trans_vvid_sat(vsat_b, 16, vsat_b)
gen_trans_vvid_sat(vsat_h, 16, vsat_h)
gen_trans_vvid_sat(vsat_w, 16, vsat_w)
gen_trans_vvid_sat(vsat_d, 16, vsat_d)
gen_trans_vvid_satu(vsat_bu, 16, vsat_bu)
gen_trans_vvid_satu(vsat_hu, 16, vsat_hu)
gen_trans_vvid_satu(vsat_wu, 16, vsat_wu)
gen_trans_vvid_satu(vsat_du, 16, vsat_du)
#define gen_trans_vvd(op, size, helper_name) \
static bool glue(trans_, op)(CPULoongArchState *env, arg_vv *restrict a) {      \
    CHECK_FPE(size);                                                      \
    int oprsz = size;                                                   \
    uint32_t desc = simd_desc(oprsz, oprsz, 0);                         \
    glue(helper_, helper_name)(&env->fpr[a->vd], &env->fpr[a->vj], desc);   \
    env->pc += 4;                                                       \
    return true;                                                        \
}
gen_trans_vvd(vexth_h_b, 16, vexth_h_b)
gen_trans_vvd(vexth_w_h, 16, vexth_w_h)
gen_trans_vvd(vexth_d_w, 16, vexth_d_w)
gen_trans_vvd(vexth_q_d, 16, vexth_q_d)
gen_trans_vvd(vextl_q_d, 16, vextl_q_d)
gen_trans_vvd(vexth_hu_bu, 16, vexth_hu_bu)
gen_trans_vvd(vexth_wu_hu, 16, vexth_wu_hu)
gen_trans_vvd(vexth_du_wu, 16, vexth_du_wu)
gen_trans_vvd(vexth_qu_du, 16, vexth_qu_du)
gen_trans_vvd(vextl_qu_du, 16, vextl_qu_du)


gen_trans_vvvd(vsigncov_b, 16, vsigncov_b)
gen_trans_vvvd(vsigncov_h, 16, vsigncov_h)
gen_trans_vvvd(vsigncov_w, 16, vsigncov_w)
gen_trans_vvvd(vsigncov_d, 16, vsigncov_d)
gen_trans_vvd(vmskltz_b, 16, vmskltz_b)
gen_trans_vvd(vmskltz_h, 16, vmskltz_h)
gen_trans_vvd(vmskltz_w, 16, vmskltz_w)
gen_trans_vvd(vmskltz_d, 16, vmskltz_d)
static bool trans_vmskgez_b(CPULoongArchState *env, arg_vmskgez_b *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vmskgez_b(&env->fpr[a->vd], &env->fpr[a->vj], desc);
    env->pc += 4;
    return true;
}
static bool trans_vmsknz_b(CPULoongArchState *env, arg_vmsknz_b *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vmsknz_b(&env->fpr[a->vd], &env->fpr[a->vj], desc);
    env->pc += 4;
    return true;
}
#define EXPAND_BYTE(bit)  ((uint64_t)(bit ? 0xff : 0))

static uint64_t vldi_get_value(DisasContext *ctx, uint32_t imm)
{
    int mode;
    uint64_t data, t;

    /*
     * imm bit [11:8] is mode, mode value is 0-12.
     * other values are invalid.
     */
    mode = (imm >> 8) & 0xf;
    t =  imm & 0xff;
    switch (mode) {
    case 0:
        /* data: {2{24'0, imm[7:0]}} */
        data =  (t << 32) | t ;
        break;
    case 1:
        /* data: {2{16'0, imm[7:0], 8'0}} */
        data = (t << 24) | (t << 8);
        break;
    case 2:
        /* data: {2{8'0, imm[7:0], 16'0}} */
        data = (t << 48) | (t << 16);
        break;
    case 3:
        /* data: {2{imm[7:0], 24'0}} */
        data = (t << 56) | (t << 24);
        break;
    case 4:
        /* data: {4{8'0, imm[7:0]}} */
        data = (t << 48) | (t << 32) | (t << 16) | t;
        break;
    case 5:
        /* data: {4{imm[7:0], 8'0}} */
        data = (t << 56) |(t << 40) | (t << 24) | (t << 8);
        break;
    case 6:
        /* data: {2{16'0, imm[7:0], 8'1}} */
        data = (t << 40) | ((uint64_t)0xff << 32) | (t << 8) | 0xff;
        break;
    case 7:
        /* data: {2{8'0, imm[7:0], 16'1}} */
        data = (t << 48) | ((uint64_t)0xffff << 32) | (t << 16) | 0xffff;
        break;
    case 8:
        /* data: {8{imm[7:0]}} */
        data =(t << 56) | (t << 48) | (t << 40) | (t << 32) |
              (t << 24) | (t << 16) | (t << 8) | t;
        break;
    case 9:
        /* data: {{8{imm[7]}, ..., 8{imm[0]}}} */
        {
            uint64_t b0,b1,b2,b3,b4,b5,b6,b7;
            b0 = t& 0x1;
            b1 = (t & 0x2) >> 1;
            b2 = (t & 0x4) >> 2;
            b3 = (t & 0x8) >> 3;
            b4 = (t & 0x10) >> 4;
            b5 = (t & 0x20) >> 5;
            b6 = (t & 0x40) >> 6;
            b7 = (t & 0x80) >> 7;
            data = (EXPAND_BYTE(b7) << 56) |
                   (EXPAND_BYTE(b6) << 48) |
                   (EXPAND_BYTE(b5) << 40) |
                   (EXPAND_BYTE(b4) << 32) |
                   (EXPAND_BYTE(b3) << 24) |
                   (EXPAND_BYTE(b2) << 16) |
                   (EXPAND_BYTE(b1) <<  8) |
                   EXPAND_BYTE(b0);
        }
        break;
    case 10:
        /* data: {2{imm[7], ~imm[6], {5{imm[6]}}, imm[5:0], 19'0}} */
        {
            uint64_t b6, b7;
            uint64_t t0, t1;
            b6 = (imm & 0x40) >> 6;
            b7 = (imm & 0x80) >> 7;
            t0 = (imm & 0x3f);
            t1 = (b7 << 6) | ((1-b6) << 5) | (uint64_t)(b6 ? 0x1f : 0);
            data  = (t1 << 57) | (t0 << 51) | (t1 << 25) | (t0 << 19);
        }
        break;
    case 11:
        /* data: {32'0, imm[7], ~{imm[6]}, 5{imm[6]}, imm[5:0], 19'0} */
        {
            uint64_t b6,b7;
            uint64_t t0, t1;
            b6 = (imm & 0x40) >> 6;
            b7 = (imm & 0x80) >> 7;
            t0 = (imm & 0x3f);
            t1 = (b7 << 6) | ((1-b6) << 5) | (b6 ? 0x1f : 0);
            data = (t1 << 25) | (t0 << 19);
        }
        break;
    case 12:
        /* data: {imm[7], ~imm[6], 8{imm[6]}, imm[5:0], 48'0} */
        {
            uint64_t b6,b7;
            uint64_t t0, t1;
            b6 = (imm & 0x40) >> 6;
            b7 = (imm & 0x80) >> 7;
            t0 = (imm & 0x3f);
            t1 = (b7 << 9) | ((1-b6) << 8) | (b6 ? 0xff : 0);
            data = (t1 << 54) | (t0 << 48);
        }
        break;
    default:
        g_assert_not_reached();
    }
    return data;
}
static bool vldi(CPULoongArchState *env, arg_vldi *restrict a, uint32_t vlen) {
    int sel;
    uint64_t value;
    uint32_t ele_cnt = vlen / 8;

    sel = (a->imm >> 12) & 0x1;

    if (sel) {
        value = vldi_get_value(ctx, a->imm);
        for (uint32_t i = 0; i < ele_cnt; i++) {
            env->fpr[a->vd].vreg.UD[i] = value;
        }
    } else {
        value = ((int32_t)(a->imm << 22)) >> 22;
        switch ((a->imm >> 10) & 0x3) {
            case 0: for (uint32_t i = 0; i < (ele_cnt * 8); i++) {env->fpr[a->vd].vreg.B[i] = value;} break;
            case 1: for (uint32_t i = 0; i < (ele_cnt * 4); i++) {env->fpr[a->vd].vreg.H[i] = value;} break;
            case 2: for (uint32_t i = 0; i < (ele_cnt * 2); i++) {env->fpr[a->vd].vreg.W[i] = value;} break;
            case 3: for (uint32_t i = 0; i < (ele_cnt * 1); i++) {env->fpr[a->vd].vreg.D[i] = value;} break;
            default: g_assert_not_reached();
        }
    }
    env->pc += 4;
    return true;
}

static inline bool trans_vldi(CPULoongArchState *env, arg_vldi *restrict a) {CHECK_FPE(16); return vldi(env, a, 16); }
static inline bool trans_xvldi(CPULoongArchState *env, arg_vldi *restrict a) {CHECK_FPE(32); return vldi(env, a, 32); }

static inline bool vand_v(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] & env->fpr[a->vk].vreg.D[i] ;}env->pc += 4;return true;}
static inline bool vor_v(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] | env->fpr[a->vk].vreg.D[i] ;}env->pc += 4;return true;}
static inline bool vxor_v(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] ^ env->fpr[a->vk].vreg.D[i] ;}env->pc += 4;return true;}
static inline bool vnor_v(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = ~(env->fpr[a->vj].vreg.D[i] | env->fpr[a->vk].vreg.D[i]) ;}env->pc += 4;return true;}
static inline bool vandn_v(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = (~env->fpr[a->vj].vreg.D[i]) & env->fpr[a->vk].vreg.D[i] ;}env->pc += 4;return true;}
static inline bool vorn_v(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] | (~env->fpr[a->vk].vreg.D[i]) ;}env->pc += 4;return true;}
static bool trans_vand_v(CPULoongArchState *env, arg_vand_v *restrict a) {CHECK_FPE(16); return vand_v(env, a, 16);}
static bool trans_vor_v(CPULoongArchState *env, arg_vor_v *restrict a) {CHECK_FPE(16); return vor_v(env, a, 16);}
static bool trans_vxor_v(CPULoongArchState *env, arg_vxor_v *restrict a) {CHECK_FPE(16); return vxor_v(env, a, 16);}
static bool trans_vnor_v(CPULoongArchState *env, arg_vnor_v *restrict a) {CHECK_FPE(16); return vnor_v(env, a, 16);}
static bool trans_vandn_v(CPULoongArchState *env, arg_vandn_v *restrict a) {CHECK_FPE(16); return vandn_v(env, a, 16);}
static bool trans_vorn_v(CPULoongArchState *env, arg_vorn_v *restrict a) {CHECK_FPE(16); return vorn_v(env, a, 16);}
static bool trans_xvand_v(CPULoongArchState *env, arg_vand_v *restrict a) {CHECK_FPE(32); return vand_v(env, a, 32);}
static bool trans_xvor_v(CPULoongArchState *env, arg_vor_v *restrict a) {CHECK_FPE(32); return vor_v(env, a, 32);}
static bool trans_xvxor_v(CPULoongArchState *env, arg_vxor_v *restrict a) {CHECK_FPE(32); return vxor_v(env, a, 32);}
static bool trans_xvnor_v(CPULoongArchState *env, arg_vnor_v *restrict a) {CHECK_FPE(32); return vnor_v(env, a, 32);}
static bool trans_xvandn_v(CPULoongArchState *env, arg_vandn_v *restrict a) {CHECK_FPE(32); return vandn_v(env, a, 32);}
static bool trans_xvorn_v(CPULoongArchState *env, arg_vorn_v *restrict a) {CHECK_FPE(32); return vorn_v(env, a, 32);}
static inline bool vandi_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] & a->imm;}env->pc += 4;return true;}
static inline bool vori_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] | a->imm;}env->pc += 4;return true;}
static inline bool vxori_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] ^ a->imm;}env->pc += 4;return true;}
static inline bool vnori_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = ~(env->fpr[a->vj].vreg.B[i] | a->imm);}env->pc += 4;return true;}
static bool trans_vandi_b(CPULoongArchState *env, arg_vandi_b *restrict a) {CHECK_FPE(16); return vandi_b(env, a, 16);}
static bool trans_vori_b(CPULoongArchState *env, arg_vori_b *restrict a) {CHECK_FPE(16); return vori_b(env, a, 16);}
static bool trans_vxori_b(CPULoongArchState *env, arg_vxori_b *restrict a) {CHECK_FPE(16); return vxori_b(env, a, 16);}
static bool trans_vnori_b(CPULoongArchState *env, arg_vnori_b *restrict a) {CHECK_FPE(16); return vnori_b(env, a, 16);}
static bool trans_xvandi_b(CPULoongArchState *env, arg_vandi_b *restrict a) {CHECK_FPE(32); return vandi_b(env, a, 32);}
static bool trans_xvori_b(CPULoongArchState *env, arg_vori_b *restrict a) {CHECK_FPE(32); return vori_b(env, a, 32);}
static bool trans_xvxori_b(CPULoongArchState *env, arg_vxori_b *restrict a) {CHECK_FPE(32); return vxori_b(env, a, 32);}
static bool trans_xvnori_b(CPULoongArchState *env, arg_vnori_b *restrict a) {CHECK_FPE(32); return vnori_b(env, a, 32);}
static inline bool vsll_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] << (env->fpr[a->vk].vreg.B[i] & 0x7);}env->pc += 4;return true;}
static inline bool vsll_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] << (env->fpr[a->vk].vreg.H[i] & 0xf);}env->pc += 4;return true;}
static inline bool vsll_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] << (env->fpr[a->vk].vreg.W[i] & 0x1f);}env->pc += 4;return true;}
static inline bool vsll_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] << (env->fpr[a->vk].vreg.D[i] & 0x3f);}env->pc += 4;return true;}
static bool trans_vsll_b(CPULoongArchState *env, arg_vsll_b *restrict a) {CHECK_FPE(16); return vsll_b(env, a, 16);}
static bool trans_vsll_h(CPULoongArchState *env, arg_vsll_h *restrict a) {CHECK_FPE(16); return vsll_h(env, a, 16);}
static bool trans_vsll_w(CPULoongArchState *env, arg_vsll_w *restrict a) {CHECK_FPE(16); return vsll_w(env, a, 16);}
static bool trans_vsll_d(CPULoongArchState *env, arg_vsll_d *restrict a) {CHECK_FPE(16); return vsll_d(env, a, 16);}
static bool trans_xvsll_b(CPULoongArchState *env, arg_vsll_b *restrict a) {CHECK_FPE(32); return vsll_b(env, a, 32);}
static bool trans_xvsll_h(CPULoongArchState *env, arg_vsll_h *restrict a) {CHECK_FPE(32); return vsll_h(env, a, 32);}
static bool trans_xvsll_w(CPULoongArchState *env, arg_vsll_w *restrict a) {CHECK_FPE(32); return vsll_w(env, a, 32);}
static bool trans_xvsll_d(CPULoongArchState *env, arg_vsll_d *restrict a) {CHECK_FPE(32); return vsll_d(env, a, 32);}
static inline bool vslli_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] << a->imm;}env->pc += 4;return true;}
static inline bool vslli_h(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] << a->imm;}env->pc += 4;return true;}
static inline bool vslli_w(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] << a->imm;}env->pc += 4;return true;}
static inline bool vslli_d(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] << a->imm;}env->pc += 4;return true;}
static bool trans_vslli_b(CPULoongArchState *env, arg_vslli_b *restrict a) {CHECK_FPE(16); return vslli_b(env, a, 16);}
static bool trans_vslli_h(CPULoongArchState *env, arg_vslli_h *restrict a) {CHECK_FPE(16); return vslli_h(env, a, 16);}
static bool trans_vslli_w(CPULoongArchState *env, arg_vslli_w *restrict a) {CHECK_FPE(16); return vslli_w(env, a, 16);}
static bool trans_vslli_d(CPULoongArchState *env, arg_vslli_d *restrict a) {CHECK_FPE(16); return vslli_d(env, a, 16);}
static bool trans_xvslli_b(CPULoongArchState *env, arg_vslli_b *restrict a) {CHECK_FPE(32); return vslli_b(env, a, 32);}
static bool trans_xvslli_h(CPULoongArchState *env, arg_vslli_h *restrict a) {CHECK_FPE(32); return vslli_h(env, a, 32);}
static bool trans_xvslli_w(CPULoongArchState *env, arg_vslli_w *restrict a) {CHECK_FPE(32); return vslli_w(env, a, 32);}
static bool trans_xvslli_d(CPULoongArchState *env, arg_vslli_d *restrict a) {CHECK_FPE(32); return vslli_d(env, a, 32);}
static inline bool vsrl_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] >> (env->fpr[a->vk].vreg.B[i] & 0x7);}env->pc += 4;return true;}
static inline bool vsrl_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] >> (env->fpr[a->vk].vreg.H[i] & 0xf);}env->pc += 4;return true;}
static inline bool vsrl_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] >> (env->fpr[a->vk].vreg.W[i] & 0x1f);}env->pc += 4;return true;}
static inline bool vsrl_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] >> (env->fpr[a->vk].vreg.D[i] & 0x3f);}env->pc += 4;return true;}
static bool trans_vsrl_b(CPULoongArchState *env, arg_vsrl_b *restrict a) {CHECK_FPE(16); return vsrl_b(env, a, 16);}
static bool trans_vsrl_h(CPULoongArchState *env, arg_vsrl_h *restrict a) {CHECK_FPE(16); return vsrl_h(env, a, 16);}
static bool trans_vsrl_w(CPULoongArchState *env, arg_vsrl_w *restrict a) {CHECK_FPE(16); return vsrl_w(env, a, 16);}
static bool trans_vsrl_d(CPULoongArchState *env, arg_vsrl_d *restrict a) {CHECK_FPE(16); return vsrl_d(env, a, 16);}
static bool trans_xvsrl_b(CPULoongArchState *env, arg_vsrl_b *restrict a) {CHECK_FPE(32); return vsrl_b(env, a, 32);}
static bool trans_xvsrl_h(CPULoongArchState *env, arg_vsrl_h *restrict a) {CHECK_FPE(32); return vsrl_h(env, a, 32);}
static bool trans_xvsrl_w(CPULoongArchState *env, arg_vsrl_w *restrict a) {CHECK_FPE(32); return vsrl_w(env, a, 32);}
static bool trans_xvsrl_d(CPULoongArchState *env, arg_vsrl_d *restrict a) {CHECK_FPE(32); return vsrl_d(env, a, 32);}
static inline bool vsrli_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] >> a->imm;}env->pc += 4;return true;}
static inline bool vsrli_h(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] >> a->imm;}env->pc += 4;return true;}
static inline bool vsrli_w(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] >> a->imm;}env->pc += 4;return true;}
static inline bool vsrli_d(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] >> a->imm;}env->pc += 4;return true;}
static bool trans_vsrli_b(CPULoongArchState *env, arg_vsrli_b *restrict a) {CHECK_FPE(16); return vsrli_b(env, a, 16);}
static bool trans_vsrli_h(CPULoongArchState *env, arg_vsrli_h *restrict a) {CHECK_FPE(16); return vsrli_h(env, a, 16);}
static bool trans_vsrli_w(CPULoongArchState *env, arg_vsrli_w *restrict a) {CHECK_FPE(16); return vsrli_w(env, a, 16);}
static bool trans_vsrli_d(CPULoongArchState *env, arg_vsrli_d *restrict a) {CHECK_FPE(16); return vsrli_d(env, a, 16);}
static bool trans_xvsrli_b(CPULoongArchState *env, arg_vsrli_b *restrict a) {CHECK_FPE(32); return vsrli_b(env, a, 32);}
static bool trans_xvsrli_h(CPULoongArchState *env, arg_vsrli_h *restrict a) {CHECK_FPE(32); return vsrli_h(env, a, 32);}
static bool trans_xvsrli_w(CPULoongArchState *env, arg_vsrli_w *restrict a) {CHECK_FPE(32); return vsrli_w(env, a, 32);}
static bool trans_xvsrli_d(CPULoongArchState *env, arg_vsrli_d *restrict a) {CHECK_FPE(32); return vsrli_d(env, a, 32);}
static inline bool vsra_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] >> (env->fpr[a->vk].vreg.B[i] & 0x7);}env->pc += 4;return true;}
static inline bool vsra_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] >> (env->fpr[a->vk].vreg.H[i] & 0xf);}env->pc += 4;return true;}
static inline bool vsra_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] >> (env->fpr[a->vk].vreg.W[i] & 0x1f);}env->pc += 4;return true;}
static inline bool vsra_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] >> (env->fpr[a->vk].vreg.D[i] & 0x3f);}env->pc += 4;return true;}
static bool trans_vsra_b(CPULoongArchState *env, arg_vsra_b *restrict a) {CHECK_FPE(16); return vsra_b(env, a, 16);}
static bool trans_vsra_h(CPULoongArchState *env, arg_vsra_h *restrict a) {CHECK_FPE(16); return vsra_h(env, a, 16);}
static bool trans_vsra_w(CPULoongArchState *env, arg_vsra_w *restrict a) {CHECK_FPE(16); return vsra_w(env, a, 16);}
static bool trans_vsra_d(CPULoongArchState *env, arg_vsra_d *restrict a) {CHECK_FPE(16); return vsra_d(env, a, 16);}

static bool trans_xvsra_b(CPULoongArchState *env, arg_vsra_b *restrict a) {CHECK_FPE(32); return vsra_b(env, a, 32);}
static bool trans_xvsra_h(CPULoongArchState *env, arg_vsra_h *restrict a) {CHECK_FPE(32); return vsra_h(env, a, 32);}
static bool trans_xvsra_w(CPULoongArchState *env, arg_vsra_w *restrict a) {CHECK_FPE(32); return vsra_w(env, a, 32);}
static bool trans_xvsra_d(CPULoongArchState *env, arg_vsra_d *restrict a) {CHECK_FPE(32); return vsra_d(env, a, 32);}
static inline bool vsrai_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] >> a->imm;}env->pc += 4;return true;}
static inline bool vsrai_h(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] >> a->imm;}env->pc += 4;return true;}
static inline bool vsrai_w(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] >> a->imm;}env->pc += 4;return true;}
static inline bool vsrai_d(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] >> a->imm;}env->pc += 4;return true;}
static bool trans_vsrai_b(CPULoongArchState *env, arg_vsrai_b *restrict a) {CHECK_FPE(16); return vsrai_b(env, a, 16);}
static bool trans_vsrai_h(CPULoongArchState *env, arg_vsrai_h *restrict a) {CHECK_FPE(16); return vsrai_h(env, a, 16);}
static bool trans_vsrai_w(CPULoongArchState *env, arg_vsrai_w *restrict a) {CHECK_FPE(16); return vsrai_w(env, a, 16);}
static bool trans_vsrai_d(CPULoongArchState *env, arg_vsrai_d *restrict a) {CHECK_FPE(16); return vsrai_d(env, a, 16);}
static bool trans_xvsrai_b(CPULoongArchState *env, arg_vsrai_b *restrict a) {CHECK_FPE(32); return vsrai_b(env, a, 32);}
static bool trans_xvsrai_h(CPULoongArchState *env, arg_vsrai_h *restrict a) {CHECK_FPE(32); return vsrai_h(env, a, 32);}
static bool trans_xvsrai_w(CPULoongArchState *env, arg_vsrai_w *restrict a) {CHECK_FPE(32); return vsrai_w(env, a, 32);}
static bool trans_xvsrai_d(CPULoongArchState *env, arg_vsrai_d *restrict a) {CHECK_FPE(32); return vsrai_d(env, a, 32);}
gen_trans_vvvd(vrotr_b, 16, gvec_rotr8v)
gen_trans_vvvd(vrotr_h, 16, gvec_rotr16v)
gen_trans_vvvd(vrotr_w, 16, gvec_rotr32v)
gen_trans_vvvd(vrotr_d, 16, gvec_rotr64v)
static bool trans_vrotri_b(CPULoongArchState *env, arg_vrotri_b *restrict a) {
    int size = 16;
    CHECK_FPE(size);
    int oprsz = size;
    uint32_t desc = simd_desc(oprsz, oprsz, 8 - a->imm);
    helper_gvec_rotl8i(&env->fpr[a->vd], &env->fpr[a->vj], desc);
    env->pc += 4;
    return true;
}
static bool trans_vrotri_h(CPULoongArchState *env, arg_vrotri_h *restrict a) {
    int size = 16;
    CHECK_FPE(size);
    int oprsz = size;
    uint32_t desc = simd_desc(oprsz, oprsz, 16 - a->imm);
    helper_gvec_rotl16i(&env->fpr[a->vd], &env->fpr[a->vj], desc);
    env->pc += 4;
    return true;
}
static bool trans_vrotri_w(CPULoongArchState *env, arg_vrotri_w *restrict a) {
    int size = 16;
    CHECK_FPE(size);
    int oprsz = size;
    uint32_t desc = simd_desc(oprsz, oprsz, 32 - a->imm);
    helper_gvec_rotl32i(&env->fpr[a->vd], &env->fpr[a->vj], desc);
    env->pc += 4;
    return true;
}
static bool trans_vrotri_d(CPULoongArchState *env, arg_vrotri_d *restrict a) {
    int size = 16;
    CHECK_FPE(size);
    int oprsz = size;
    uint32_t desc = simd_desc(oprsz, oprsz, 64 - a->imm);
    helper_gvec_rotl64i(&env->fpr[a->vd], &env->fpr[a->vj], desc);
    env->pc += 4;
    return true;
}
gen_trans_vvid(vsllwil_h_b, 16, vsllwil_h_b)
gen_trans_vvid(vsllwil_w_h, 16, vsllwil_w_h)
gen_trans_vvid(vsllwil_d_w, 16, vsllwil_d_w)
gen_trans_vvid(vsllwil_hu_bu, 16, vsllwil_hu_bu)
gen_trans_vvid(vsllwil_wu_hu, 16, vsllwil_wu_hu)
gen_trans_vvid(vsllwil_du_wu, 16, vsllwil_du_wu)
gen_trans_vvvd(vsrlr_b, 16, vsrlr_b)
gen_trans_vvvd(vsrlr_h, 16, vsrlr_h)
gen_trans_vvvd(vsrlr_w, 16, vsrlr_w)
gen_trans_vvvd(vsrlr_d, 16, vsrlr_d)
gen_trans_vvid(vsrlri_b, 16, vsrlri_b)
gen_trans_vvid(vsrlri_h, 16, vsrlri_h)
gen_trans_vvid(vsrlri_w, 16, vsrlri_w)
gen_trans_vvid(vsrlri_d, 16, vsrlri_d)
gen_trans_vvvd(vsrar_b, 16, vsrar_b)
gen_trans_vvvd(vsrar_h, 16, vsrar_h)
gen_trans_vvvd(vsrar_w, 16, vsrar_w)
gen_trans_vvvd(vsrar_d, 16, vsrar_d)
gen_trans_vvid(vsrari_b, 16, vsrari_b)
gen_trans_vvid(vsrari_h, 16, vsrari_h)
gen_trans_vvid(vsrari_w, 16, vsrari_w)
gen_trans_vvid(vsrari_d, 16, vsrari_d)

gen_trans_vvvd(vsrln_b_h, 16, vsrln_b_h)
gen_trans_vvvd(vsrln_h_w, 16, vsrln_h_w)
gen_trans_vvvd(vsrln_w_d, 16, vsrln_w_d)
gen_trans_vvvd(vsran_b_h, 16, vsran_b_h)
gen_trans_vvvd(vsran_h_w, 16, vsran_h_w)
gen_trans_vvvd(vsran_w_d, 16, vsran_w_d)

gen_trans_vvid(vsrlni_b_h, 16, vsrlni_b_h)
gen_trans_vvid(vsrlni_h_w, 16, vsrlni_h_w)
gen_trans_vvid(vsrlni_w_d, 16, vsrlni_w_d)
gen_trans_vvid(vsrlni_d_q, 16, vsrlni_d_q)
gen_trans_vvid(vsrani_b_h, 16, vsrani_b_h)
gen_trans_vvid(vsrani_h_w, 16, vsrani_h_w)
gen_trans_vvid(vsrani_w_d, 16, vsrani_w_d)
gen_trans_vvid(vsrani_d_q, 16, vsrani_d_q)


gen_trans_vvvd(vsrlrn_b_h, 16, vsrlrn_b_h)
gen_trans_vvvd(vsrlrn_h_w, 16, vsrlrn_h_w)
gen_trans_vvvd(vsrlrn_w_d, 16, vsrlrn_w_d)
gen_trans_vvvd(vsrarn_b_h, 16, vsrarn_b_h)
gen_trans_vvvd(vsrarn_h_w, 16, vsrarn_h_w)
gen_trans_vvvd(vsrarn_w_d, 16, vsrarn_w_d)

gen_trans_vvid(vsrlrni_b_h, 16, vsrlrni_b_h)
gen_trans_vvid(vsrlrni_h_w, 16, vsrlrni_h_w)
gen_trans_vvid(vsrlrni_w_d, 16, vsrlrni_w_d)
gen_trans_vvid(vsrlrni_d_q, 16, vsrlrni_d_q)
gen_trans_vvid(vsrarni_b_h, 16, vsrarni_b_h)
gen_trans_vvid(vsrarni_h_w, 16, vsrarni_h_w)
gen_trans_vvid(vsrarni_w_d, 16, vsrarni_w_d)
gen_trans_vvid(vsrarni_d_q, 16, vsrarni_d_q)

gen_trans_vvvd(vssrln_b_h, 16, vssrln_b_h)
gen_trans_vvvd(vssrln_h_w, 16, vssrln_h_w)
gen_trans_vvvd(vssrln_w_d, 16, vssrln_w_d)
gen_trans_vvvd(vssran_b_h, 16, vssran_b_h)
gen_trans_vvvd(vssran_h_w, 16, vssran_h_w)
gen_trans_vvvd(vssran_w_d, 16, vssran_w_d)
gen_trans_vvvd(vssrln_bu_h, 16, vssrln_bu_h)
gen_trans_vvvd(vssrln_hu_w, 16, vssrln_hu_w)
gen_trans_vvvd(vssrln_wu_d, 16, vssrln_wu_d)
gen_trans_vvvd(vssran_bu_h, 16, vssran_bu_h)
gen_trans_vvvd(vssran_hu_w, 16, vssran_hu_w)
gen_trans_vvvd(vssran_wu_d, 16, vssran_wu_d)

gen_trans_vvid(vssrlni_b_h, 16, vssrlni_b_h)
gen_trans_vvid(vssrlni_h_w, 16, vssrlni_h_w)
gen_trans_vvid(vssrlni_w_d, 16, vssrlni_w_d)
gen_trans_vvid(vssrlni_d_q, 16, vssrlni_d_q)
gen_trans_vvid(vssrani_b_h, 16, vssrani_b_h)
gen_trans_vvid(vssrani_h_w, 16, vssrani_h_w)
gen_trans_vvid(vssrani_w_d, 16, vssrani_w_d)
gen_trans_vvid(vssrani_d_q, 16, vssrani_d_q)
gen_trans_vvid(vssrlni_bu_h, 16, vssrlni_bu_h)
gen_trans_vvid(vssrlni_hu_w, 16, vssrlni_hu_w)
gen_trans_vvid(vssrlni_wu_d, 16, vssrlni_wu_d)
gen_trans_vvid(vssrlni_du_q, 16, vssrlni_du_q)
gen_trans_vvid(vssrani_bu_h, 16, vssrani_bu_h)
gen_trans_vvid(vssrani_hu_w, 16, vssrani_hu_w)
gen_trans_vvid(vssrani_wu_d, 16, vssrani_wu_d)
gen_trans_vvid(vssrani_du_q, 16, vssrani_du_q)

gen_trans_vvvd(vssrlrn_b_h, 16, vssrlrn_b_h)
gen_trans_vvvd(vssrlrn_h_w, 16, vssrlrn_h_w)
gen_trans_vvvd(vssrlrn_w_d, 16, vssrlrn_w_d)
gen_trans_vvvd(vssrarn_b_h, 16, vssrarn_b_h)
gen_trans_vvvd(vssrarn_h_w, 16, vssrarn_h_w)
gen_trans_vvvd(vssrarn_w_d, 16, vssrarn_w_d)
gen_trans_vvvd(vssrlrn_bu_h, 16, vssrlrn_bu_h)
gen_trans_vvvd(vssrlrn_hu_w, 16, vssrlrn_hu_w)
gen_trans_vvvd(vssrlrn_wu_d, 16, vssrlrn_wu_d)
gen_trans_vvvd(vssrarn_bu_h, 16, vssrarn_bu_h)
gen_trans_vvvd(vssrarn_hu_w, 16, vssrarn_hu_w)
gen_trans_vvvd(vssrarn_wu_d, 16, vssrarn_wu_d)

gen_trans_vvid(vssrlrni_b_h, 16, vssrlrni_b_h)
gen_trans_vvid(vssrlrni_h_w, 16, vssrlrni_h_w)
gen_trans_vvid(vssrlrni_w_d, 16, vssrlrni_w_d)
gen_trans_vvid(vssrlrni_d_q, 16, vssrlrni_d_q)
gen_trans_vvid(vssrarni_b_h, 16, vssrarni_b_h)
gen_trans_vvid(vssrarni_h_w, 16, vssrarni_h_w)
gen_trans_vvid(vssrarni_w_d, 16, vssrarni_w_d)
gen_trans_vvid(vssrarni_d_q, 16, vssrarni_d_q)
gen_trans_vvid(vssrlrni_bu_h, 16, vssrlrni_bu_h)
gen_trans_vvid(vssrlrni_hu_w, 16, vssrlrni_hu_w)
gen_trans_vvid(vssrlrni_wu_d, 16, vssrlrni_wu_d)
gen_trans_vvid(vssrlrni_du_q, 16, vssrlrni_du_q)
gen_trans_vvid(vssrarni_bu_h, 16, vssrarni_bu_h)
gen_trans_vvid(vssrarni_hu_w, 16, vssrarni_hu_w)
gen_trans_vvid(vssrarni_wu_d, 16, vssrarni_wu_d)
gen_trans_vvid(vssrarni_du_q, 16, vssrarni_du_q)

gen_trans_vvd(vclo_b, 16, vclo_b)
gen_trans_vvd(vclo_h, 16, vclo_h)
gen_trans_vvd(vclo_w, 16, vclo_w)
gen_trans_vvd(vclo_d, 16, vclo_d)
gen_trans_vvd(vclz_b, 16, vclz_b)
gen_trans_vvd(vclz_h, 16, vclz_h)
gen_trans_vvd(vclz_w, 16, vclz_w)
gen_trans_vvd(vclz_d, 16, vclz_d)
gen_trans_vvd(vpcnt_b, 16, vpcnt_b)
gen_trans_vvd(vpcnt_h, 16, vpcnt_h)
gen_trans_vvd(vpcnt_w, 16, vpcnt_w)
gen_trans_vvd(vpcnt_d, 16, vpcnt_d)

static inline bool vbitclr_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] & (~(1ull << (env->fpr[a->vk].vreg.UB[i] & 0x7)));}env->pc += 4;return true;}
static inline bool vbitclr_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] & (~(1ull << (env->fpr[a->vk].vreg.UH[i] & 0xf)));}env->pc += 4;return true;}
static inline bool vbitclr_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] & (~(1ull << (env->fpr[a->vk].vreg.UW[i] & 0x1f)));}env->pc += 4;return true;}
static inline bool vbitclr_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] & (~(1ull << (env->fpr[a->vk].vreg.UD[i] & 0x3f)));}env->pc += 4;return true;}
static bool trans_vbitclr_b(CPULoongArchState *env, arg_vbitclr_b *restrict a) {CHECK_FPE(16); return vbitclr_b(env, a, 16);}
static bool trans_vbitclr_h(CPULoongArchState *env, arg_vbitclr_h *restrict a) {CHECK_FPE(16); return vbitclr_h(env, a, 16);}
static bool trans_vbitclr_w(CPULoongArchState *env, arg_vbitclr_w *restrict a) {CHECK_FPE(16); return vbitclr_w(env, a, 16);}
static bool trans_vbitclr_d(CPULoongArchState *env, arg_vbitclr_d *restrict a) {CHECK_FPE(16); return vbitclr_d(env, a, 16);}
static bool trans_xvbitclr_b(CPULoongArchState *env, arg_vbitclr_b *restrict a) {CHECK_FPE(32); return vbitclr_b(env, a, 32);}
static bool trans_xvbitclr_h(CPULoongArchState *env, arg_vbitclr_h *restrict a) {CHECK_FPE(32); return vbitclr_h(env, a, 32);}
static bool trans_xvbitclr_w(CPULoongArchState *env, arg_vbitclr_w *restrict a) {CHECK_FPE(32); return vbitclr_w(env, a, 32);}
static bool trans_xvbitclr_d(CPULoongArchState *env, arg_vbitclr_d *restrict a) {CHECK_FPE(32); return vbitclr_d(env, a, 32);}
static inline bool vbitclri_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] & (~(1ull << a->imm));}env->pc += 4;return true;}
static inline bool vbitclri_h(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] & (~(1ull << a->imm));}env->pc += 4;return true;}
static inline bool vbitclri_w(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] & (~(1ull << a->imm));}env->pc += 4;return true;}
static inline bool vbitclri_d(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] & (~(1ull << a->imm));}env->pc += 4;return true;}
static bool trans_vbitclri_b(CPULoongArchState *env, arg_vbitclri_b *restrict a) {CHECK_FPE(16); return vbitclri_b(env, a, 16);}
static bool trans_vbitclri_h(CPULoongArchState *env, arg_vbitclri_h *restrict a) {CHECK_FPE(16); return vbitclri_h(env, a, 16);}
static bool trans_vbitclri_w(CPULoongArchState *env, arg_vbitclri_w *restrict a) {CHECK_FPE(16); return vbitclri_w(env, a, 16);}
static bool trans_vbitclri_d(CPULoongArchState *env, arg_vbitclri_d *restrict a) {CHECK_FPE(16); return vbitclri_d(env, a, 16);}
static bool trans_xvbitclri_b(CPULoongArchState *env, arg_vbitclri_b *restrict a) {CHECK_FPE(32); return vbitclri_b(env, a, 32);}
static bool trans_xvbitclri_h(CPULoongArchState *env, arg_vbitclri_h *restrict a) {CHECK_FPE(32); return vbitclri_h(env, a, 32);}
static bool trans_xvbitclri_w(CPULoongArchState *env, arg_vbitclri_w *restrict a) {CHECK_FPE(32); return vbitclri_w(env, a, 32);}
static bool trans_xvbitclri_d(CPULoongArchState *env, arg_vbitclri_d *restrict a) {CHECK_FPE(32); return vbitclri_d(env, a, 32);}
static inline bool vbitset_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] | ((1ull << (env->fpr[a->vk].vreg.UB[i] & 0x7)));}env->pc += 4;return true;}
static inline bool vbitset_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] | ((1ull << (env->fpr[a->vk].vreg.UH[i] & 0xf)));}env->pc += 4;return true;}
static inline bool vbitset_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] | ((1ull << (env->fpr[a->vk].vreg.UW[i] & 0x1f)));}env->pc += 4;return true;}
static inline bool vbitset_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] | ((1ull << (env->fpr[a->vk].vreg.UD[i] & 0x3f)));}env->pc += 4;return true;}
static bool trans_vbitset_b(CPULoongArchState *env, arg_vbitset_b *restrict a) {CHECK_FPE(16); return vbitset_b(env, a, 16);}
static bool trans_vbitset_h(CPULoongArchState *env, arg_vbitset_h *restrict a) {CHECK_FPE(16); return vbitset_h(env, a, 16);}
static bool trans_vbitset_w(CPULoongArchState *env, arg_vbitset_w *restrict a) {CHECK_FPE(16); return vbitset_w(env, a, 16);}
static bool trans_vbitset_d(CPULoongArchState *env, arg_vbitset_d *restrict a) {CHECK_FPE(16); return vbitset_d(env, a, 16);}
static bool trans_xvbitset_b(CPULoongArchState *env, arg_vbitset_b *restrict a) {CHECK_FPE(32); return vbitset_b(env, a, 32);}
static bool trans_xvbitset_h(CPULoongArchState *env, arg_vbitset_h *restrict a) {CHECK_FPE(32); return vbitset_h(env, a, 32);}
static bool trans_xvbitset_w(CPULoongArchState *env, arg_vbitset_w *restrict a) {CHECK_FPE(32); return vbitset_w(env, a, 32);}
static bool trans_xvbitset_d(CPULoongArchState *env, arg_vbitset_d *restrict a) {CHECK_FPE(32); return vbitset_d(env, a, 32);}
static inline bool vbitseti_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] | ((1ull << a->imm));}env->pc += 4;return true;}
static inline bool vbitseti_h(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] | ((1ull << a->imm));}env->pc += 4;return true;}
static inline bool vbitseti_w(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] | ((1ull << a->imm));}env->pc += 4;return true;}
static inline bool vbitseti_d(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] | ((1ull << a->imm));}env->pc += 4;return true;}
static bool trans_vbitseti_b(CPULoongArchState *env, arg_vbitseti_b *restrict a) {CHECK_FPE(16); return vbitseti_b(env, a, 16);}
static bool trans_vbitseti_h(CPULoongArchState *env, arg_vbitseti_h *restrict a) {CHECK_FPE(16); return vbitseti_h(env, a, 16);}
static bool trans_vbitseti_w(CPULoongArchState *env, arg_vbitseti_w *restrict a) {CHECK_FPE(16); return vbitseti_w(env, a, 16);}
static bool trans_vbitseti_d(CPULoongArchState *env, arg_vbitseti_d *restrict a) {CHECK_FPE(16); return vbitseti_d(env, a, 16);}
static bool trans_xvbitseti_b(CPULoongArchState *env, arg_vbitseti_b *restrict a) {CHECK_FPE(32); return vbitseti_b(env, a, 32);}
static bool trans_xvbitseti_h(CPULoongArchState *env, arg_vbitseti_h *restrict a) {CHECK_FPE(32); return vbitseti_h(env, a, 32);}
static bool trans_xvbitseti_w(CPULoongArchState *env, arg_vbitseti_w *restrict a) {CHECK_FPE(32); return vbitseti_w(env, a, 32);}
static bool trans_xvbitseti_d(CPULoongArchState *env, arg_vbitseti_d *restrict a) {CHECK_FPE(32); return vbitseti_d(env, a, 32);}
static inline bool vbitrev_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] ^ ((1ull << (env->fpr[a->vk].vreg.UB[i] & 0x7)));}env->pc += 4;return true;}
static inline bool vbitrev_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] ^ ((1ull << (env->fpr[a->vk].vreg.UH[i] & 0xf)));}env->pc += 4;return true;}
static inline bool vbitrev_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] ^ ((1ull << (env->fpr[a->vk].vreg.UW[i] & 0x1f)));}env->pc += 4;return true;}
static inline bool vbitrev_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] ^ ((1ull << (env->fpr[a->vk].vreg.UD[i] & 0x3f)));}env->pc += 4;return true;}
static bool trans_vbitrev_b(CPULoongArchState *env, arg_vbitrev_b *restrict a) {CHECK_FPE(16); return vbitrev_b(env, a, 16);}
static bool trans_vbitrev_h(CPULoongArchState *env, arg_vbitrev_h *restrict a) {CHECK_FPE(16); return vbitrev_h(env, a, 16);}
static bool trans_vbitrev_w(CPULoongArchState *env, arg_vbitrev_w *restrict a) {CHECK_FPE(16); return vbitrev_w(env, a, 16);}
static bool trans_vbitrev_d(CPULoongArchState *env, arg_vbitrev_d *restrict a) {CHECK_FPE(16); return vbitrev_d(env, a, 16);}
static bool trans_xvbitrev_b(CPULoongArchState *env, arg_vbitrev_b *restrict a) {CHECK_FPE(32); return vbitrev_b(env, a, 32);}
static bool trans_xvbitrev_h(CPULoongArchState *env, arg_vbitrev_h *restrict a) {CHECK_FPE(32); return vbitrev_h(env, a, 32);}
static bool trans_xvbitrev_w(CPULoongArchState *env, arg_vbitrev_w *restrict a) {CHECK_FPE(32); return vbitrev_w(env, a, 32);}
static bool trans_xvbitrev_d(CPULoongArchState *env, arg_vbitrev_d *restrict a) {CHECK_FPE(32); return vbitrev_d(env, a, 32);}
static inline bool vbitrevi_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] ^ (1 << a->imm);}env->pc += 4;return true;}
static inline bool vbitrevi_h(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] ^ (1 << a->imm);}env->pc += 4;return true;}
static inline bool vbitrevi_w(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] ^ (1 << a->imm);}env->pc += 4;return true;}
static inline bool vbitrevi_d(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] ^ (1ull << a->imm);}env->pc += 4;return true;}
static bool trans_vbitrevi_b(CPULoongArchState *env, arg_vbitrevi_b *restrict a) {CHECK_FPE(16); return vbitrevi_b(env, a, 16);}
static bool trans_vbitrevi_h(CPULoongArchState *env, arg_vbitrevi_h *restrict a) {CHECK_FPE(16); return vbitrevi_h(env, a, 16);}
static bool trans_vbitrevi_w(CPULoongArchState *env, arg_vbitrevi_w *restrict a) {CHECK_FPE(16); return vbitrevi_w(env, a, 16);}
static bool trans_vbitrevi_d(CPULoongArchState *env, arg_vbitrevi_d *restrict a) {CHECK_FPE(16); return vbitrevi_d(env, a, 16);}
static bool trans_xvbitrevi_b(CPULoongArchState *env, arg_vbitrevi_b *restrict a) {CHECK_FPE(32); return vbitrevi_b(env, a, 32);}
static bool trans_xvbitrevi_h(CPULoongArchState *env, arg_vbitrevi_h *restrict a) {CHECK_FPE(32); return vbitrevi_h(env, a, 32);}
static bool trans_xvbitrevi_w(CPULoongArchState *env, arg_vbitrevi_w *restrict a) {CHECK_FPE(32); return vbitrevi_w(env, a, 32);}
static bool trans_xvbitrevi_d(CPULoongArchState *env, arg_vbitrevi_d *restrict a) {CHECK_FPE(32); return vbitrevi_d(env, a, 32);}
gen_trans_vvvd(vfrstp_b, 16, vfrstp_b)
gen_trans_vvvd(vfrstp_h, 16, vfrstp_h)
gen_trans_vvid(vfrstpi_b, 16, vfrstpi_b)
gen_trans_vvid(vfrstpi_h, 16, vfrstpi_h)

#define gen_trans_vvved(op, size, helper_name) \
static bool glue(trans_, op)(CPULoongArchState *env, arg_vvv *restrict a) {   \
    CHECK_FPE(size);                                                   \
    int oprsz = size;                                                   \
    uint32_t desc = simd_desc(oprsz, oprsz, 0);                         \
    glue(helper_, helper_name)(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], env, desc);          \
    env->pc += 4;                                                       \
    return true;                                                        \
}

gen_trans_vvved(vfadd_s, 16, vfadd_s)
gen_trans_vvved(vfadd_d, 16, vfadd_d)
gen_trans_vvved(vfsub_s, 16, vfsub_s)
gen_trans_vvved(vfsub_d, 16, vfsub_d)
gen_trans_vvved(vfmul_s, 16, vfmul_s)
gen_trans_vvved(vfmul_d, 16, vfmul_d)
gen_trans_vvved(vfdiv_s, 16, vfdiv_s)
gen_trans_vvved(vfdiv_d, 16, vfdiv_d)

gen_trans_vvved(xvfadd_s, 32, vfadd_s)
gen_trans_vvved(xvfadd_d, 32, vfadd_d)
gen_trans_vvved(xvfsub_s, 32, vfsub_s)
gen_trans_vvved(xvfsub_d, 32, vfsub_d)
gen_trans_vvved(xvfmul_s, 32, vfmul_s)
gen_trans_vvved(xvfmul_d, 32, vfmul_d)
gen_trans_vvved(xvfdiv_s, 32, vfdiv_s)
gen_trans_vvved(xvfdiv_d, 32, vfdiv_d)

#define gen_trans_vvvv(op, size, helper_name) \
static bool glue(trans_, op)(CPULoongArchState *env, arg_vvvv *restrict a) {   \
    CHECK_FPE(size);                                                   \
    int oprsz = size;                                                   \
    uint32_t desc = simd_desc(oprsz, oprsz, 0);                         \
    glue(helper_, helper_name)(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], &env->fpr[a->va], env, desc);          \
    env->pc += 4;                                                       \
    return true;                                                        \
}

gen_trans_vvvv(vfmadd_s, 16, vfmadd_s)
gen_trans_vvvv(vfmadd_d, 16, vfmadd_d)
gen_trans_vvvv(vfmsub_s, 16, vfmsub_s)
gen_trans_vvvv(vfmsub_d, 16, vfmsub_d)
gen_trans_vvvv(vfnmadd_s, 16, vfnmadd_s)
gen_trans_vvvv(vfnmadd_d, 16, vfnmadd_d)
gen_trans_vvvv(vfnmsub_s, 16, vfnmsub_s)
gen_trans_vvvv(vfnmsub_d, 16, vfnmsub_d)

gen_trans_vvved(vfmax_s, 16, vfmax_s)
gen_trans_vvved(vfmax_d, 16, vfmax_d)
gen_trans_vvved(vfmin_s, 16, vfmin_s)
gen_trans_vvved(vfmin_d, 16, vfmin_d)
gen_trans_vvved(vfmaxa_s, 16, vfmaxa_s)
gen_trans_vvved(vfmaxa_d, 16, vfmaxa_d)
gen_trans_vvved(vfmina_s, 16, vfmina_s)
gen_trans_vvved(vfmina_d, 16, vfmina_d)

gen_trans_vvvv(xvfmadd_s, 32, vfmadd_s)
gen_trans_vvvv(xvfmadd_d, 32, vfmadd_d)
gen_trans_vvvv(xvfmsub_s, 32, vfmsub_s)
gen_trans_vvvv(xvfmsub_d, 32, vfmsub_d)
gen_trans_vvvv(xvfnmadd_s, 32, vfnmadd_s)
gen_trans_vvvv(xvfnmadd_d, 32, vfnmadd_d)
gen_trans_vvvv(xvfnmsub_s, 32, vfnmsub_s)
gen_trans_vvvv(xvfnmsub_d, 32, vfnmsub_d)

gen_trans_vvved(xvfmax_s, 32, vfmax_s)
gen_trans_vvved(xvfmax_d, 32, vfmax_d)
gen_trans_vvved(xvfmin_s, 32, vfmin_s)
gen_trans_vvved(xvfmin_d, 32, vfmin_d)
gen_trans_vvved(xvfmaxa_s, 32, vfmaxa_s)
gen_trans_vvved(xvfmaxa_d, 32, vfmaxa_d)
gen_trans_vvved(xvfmina_s, 32, vfmina_s)
gen_trans_vvved(xvfmina_d, 32, vfmina_d)

#define gen_trans_vved(op, size, helper_name) \
static bool glue(trans_, op)(CPULoongArchState *env, arg_vv *restrict a) {      \
    CHECK_FPE(size);                                                   \
    int oprsz = size;                                                   \
    uint32_t desc = simd_desc(oprsz, oprsz, 0);                         \
    glue(helper_, helper_name)(&env->fpr[a->vd], &env->fpr[a->vj], env, desc);   \
    env->pc += 4;                                                       \
    return true;                                                        \
}
gen_trans_vved(vflogb_s, 16, vflogb_s)
gen_trans_vved(vflogb_d, 16, vflogb_d)
gen_trans_vved(vfclass_s, 16, vfclass_s)
gen_trans_vved(vfclass_d, 16, vfclass_d)
gen_trans_vved(vfsqrt_s, 16, vfsqrt_s)
gen_trans_vved(vfsqrt_d, 16, vfsqrt_d)
gen_trans_vved(vfrecip_s, 16, vfrecip_s)
gen_trans_vved(vfrecip_d, 16, vfrecip_d)
gen_trans_vved(vfrsqrt_s, 16, vfrsqrt_s)
gen_trans_vved(vfrsqrt_d, 16, vfrsqrt_d)

gen_trans_vved(vfcvtl_s_h, 16, vfcvtl_s_h)
gen_trans_vved(vfcvth_s_h, 16, vfcvth_s_h)
gen_trans_vved(vfcvtl_d_s, 16, vfcvtl_d_s)
gen_trans_vved(vfcvth_d_s, 16, vfcvth_d_s)

gen_trans_vvved(vfcvt_h_s, 16, vfcvt_h_s)
gen_trans_vvved(vfcvt_s_d, 16, vfcvt_s_d)

gen_trans_vved(vfrint_s, 16, vfrint_s)
gen_trans_vved(vfrint_d, 16, vfrint_d)
gen_trans_vved(vfrintrm_s, 16, vfrintrm_s)
gen_trans_vved(vfrintrm_d, 16, vfrintrm_d)
gen_trans_vved(vfrintrp_s, 16, vfrintrp_s)
gen_trans_vved(vfrintrp_d, 16, vfrintrp_d)
gen_trans_vved(vfrintrz_s, 16, vfrintrz_s)
gen_trans_vved(vfrintrz_d, 16, vfrintrz_d)
gen_trans_vved(vfrintrne_s, 16, vfrintrne_s)
gen_trans_vved(vfrintrne_d, 16, vfrintrne_d)
gen_trans_vved(vftint_w_s, 16, vftint_w_s)
gen_trans_vved(vftint_l_d, 16, vftint_l_d)
gen_trans_vved(vftintrm_w_s, 16, vftintrm_w_s)
gen_trans_vved(vftintrm_l_d, 16, vftintrm_l_d)
gen_trans_vved(vftintrp_w_s, 16, vftintrp_w_s)
gen_trans_vved(vftintrp_l_d, 16, vftintrp_l_d)
gen_trans_vved(vftintrz_w_s, 16, vftintrz_w_s)
gen_trans_vved(vftintrz_l_d, 16, vftintrz_l_d)
gen_trans_vved(vftintrne_w_s, 16, vftintrne_w_s)
gen_trans_vved(vftintrne_l_d, 16, vftintrne_l_d)
gen_trans_vved(vftint_wu_s, 16, vftint_wu_s)
gen_trans_vved(vftint_lu_d, 16, vftint_lu_d)
gen_trans_vved(vftintrz_wu_s, 16, vftintrz_wu_s)
gen_trans_vved(vftintrz_lu_d, 16, vftintrz_lu_d)
gen_trans_vvved(vftint_w_d, 16, vftint_w_d)
gen_trans_vvved(vftintrm_w_d, 16, vftintrm_w_d)
gen_trans_vvved(vftintrp_w_d, 16, vftintrp_w_d)
gen_trans_vvved(vftintrz_w_d, 16, vftintrz_w_d)
gen_trans_vvved(vftintrne_w_d, 16, vftintrne_w_d)
gen_trans_vved(vftintl_l_s, 16, vftintl_l_s)
gen_trans_vved(vftinth_l_s, 16, vftinth_l_s)
gen_trans_vved(vftintrml_l_s, 16, vftintrml_l_s)
gen_trans_vved(vftintrmh_l_s, 16, vftintrmh_l_s)
gen_trans_vved(vftintrpl_l_s, 16, vftintrpl_l_s)
gen_trans_vved(vftintrph_l_s, 16, vftintrph_l_s)
gen_trans_vved(vftintrzl_l_s, 16, vftintrzl_l_s)
gen_trans_vved(vftintrzh_l_s, 16, vftintrzh_l_s)
gen_trans_vved(vftintrnel_l_s, 16, vftintrnel_l_s)
gen_trans_vved(vftintrneh_l_s, 16, vftintrneh_l_s)

gen_trans_vved(vffint_s_w, 16, vffint_s_w)
gen_trans_vved(vffint_s_wu, 16, vffint_s_wu)
gen_trans_vved(vffint_d_l, 16, vffint_d_l)
gen_trans_vved(vffint_d_lu, 16, vffint_d_lu)
gen_trans_vved(vffintl_d_w, 16, vffintl_d_w)
gen_trans_vved(vffinth_d_w, 16, vffinth_d_w)
gen_trans_vvved(vffint_s_l, 16, vffint_s_l)


static inline bool vseq_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] == env->fpr[a->vk].vreg.B[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vseq_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] == env->fpr[a->vk].vreg.H[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vseq_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] == env->fpr[a->vk].vreg.W[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vseq_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] == env->fpr[a->vk].vreg.D[i] ? -1 : 0;}env->pc += 4;return true;}
static bool trans_vseq_b(CPULoongArchState *env, arg_vseq_b *restrict a) {CHECK_FPE(16); return vseq_b(env, a, 16);}
static bool trans_vseq_h(CPULoongArchState *env, arg_vseq_h *restrict a) {CHECK_FPE(16); return vseq_h(env, a, 16);}
static bool trans_vseq_w(CPULoongArchState *env, arg_vseq_w *restrict a) {CHECK_FPE(16); return vseq_w(env, a, 16);}
static bool trans_vseq_d(CPULoongArchState *env, arg_vseq_d *restrict a) {CHECK_FPE(16); return vseq_d(env, a, 16);}
static bool trans_xvseq_b(CPULoongArchState *env, arg_vseq_b *restrict a) {CHECK_FPE(32); return vseq_b(env, a, 32);}
static bool trans_xvseq_h(CPULoongArchState *env, arg_vseq_h *restrict a) {CHECK_FPE(32); return vseq_h(env, a, 32);}
static bool trans_xvseq_w(CPULoongArchState *env, arg_vseq_w *restrict a) {CHECK_FPE(32); return vseq_w(env, a, 32);}
static bool trans_xvseq_d(CPULoongArchState *env, arg_vseq_d *restrict a) {CHECK_FPE(32); return vseq_d(env, a, 32);}
static inline bool vseqi_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] == a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vseqi_h(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] == a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vseqi_w(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] == a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vseqi_d(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] == a->imm ? -1: 0;}env->pc += 4;return true;}
static bool trans_vseqi_b(CPULoongArchState *env, arg_vseqi_b *restrict a) {CHECK_FPE(16); return vseqi_b(env, a, 16);}
static bool trans_vseqi_h(CPULoongArchState *env, arg_vseqi_h *restrict a) {CHECK_FPE(16); return vseqi_h(env, a, 16);}
static bool trans_vseqi_w(CPULoongArchState *env, arg_vseqi_w *restrict a) {CHECK_FPE(16); return vseqi_w(env, a, 16);}
static bool trans_vseqi_d(CPULoongArchState *env, arg_vseqi_d *restrict a) {CHECK_FPE(16); return vseqi_d(env, a, 16);}

static bool trans_xvseqi_b(CPULoongArchState *env, arg_vseqi_b *restrict a) {CHECK_FPE(32); return vseqi_b(env, a, 32);}
static bool trans_xvseqi_h(CPULoongArchState *env, arg_vseqi_h *restrict a) {CHECK_FPE(32); return vseqi_h(env, a, 32);}
static bool trans_xvseqi_w(CPULoongArchState *env, arg_vseqi_w *restrict a) {CHECK_FPE(32); return vseqi_w(env, a, 32);}
static bool trans_xvseqi_d(CPULoongArchState *env, arg_vseqi_d *restrict a) {CHECK_FPE(32); return vseqi_d(env, a, 32);}
static inline bool vsle_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] <= env->fpr[a->vk].vreg.B[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vsle_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] <= env->fpr[a->vk].vreg.H[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vsle_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] <= env->fpr[a->vk].vreg.W[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vsle_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] <= env->fpr[a->vk].vreg.D[i] ? -1 : 0;}env->pc += 4;return true;}
static bool trans_vsle_b(CPULoongArchState *env, arg_vsle_b *restrict a) {CHECK_FPE(16); return vsle_b(env, a, 16);}
static bool trans_vsle_h(CPULoongArchState *env, arg_vsle_h *restrict a) {CHECK_FPE(16); return vsle_h(env, a, 16);}
static bool trans_vsle_w(CPULoongArchState *env, arg_vsle_w *restrict a) {CHECK_FPE(16); return vsle_w(env, a, 16);}
static bool trans_vsle_d(CPULoongArchState *env, arg_vsle_d *restrict a) {CHECK_FPE(16); return vsle_d(env, a, 16);}
static bool trans_xvsle_b(CPULoongArchState *env, arg_vsle_b *restrict a) {CHECK_FPE(32); return vsle_b(env, a, 32);}
static bool trans_xvsle_h(CPULoongArchState *env, arg_vsle_h *restrict a) {CHECK_FPE(32); return vsle_h(env, a, 32);}
static bool trans_xvsle_w(CPULoongArchState *env, arg_vsle_w *restrict a) {CHECK_FPE(32); return vsle_w(env, a, 32);}
static bool trans_xvsle_d(CPULoongArchState *env, arg_vsle_d *restrict a) {CHECK_FPE(32); return vsle_d(env, a, 32);}
static inline bool vslei_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] <= a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vslei_h(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] <= a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vslei_w(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] <= a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vslei_d(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] <= a->imm ? -1: 0;}env->pc += 4;return true;}
static bool trans_vslei_b(CPULoongArchState *env, arg_vslei_b *restrict a) {CHECK_FPE(16); return vslei_b(env, a, 16);}
static bool trans_vslei_h(CPULoongArchState *env, arg_vslei_h *restrict a) {CHECK_FPE(16); return vslei_h(env, a, 16);}
static bool trans_vslei_w(CPULoongArchState *env, arg_vslei_w *restrict a) {CHECK_FPE(16); return vslei_w(env, a, 16);}
static bool trans_vslei_d(CPULoongArchState *env, arg_vslei_d *restrict a) {CHECK_FPE(16); return vslei_d(env, a, 16);}
static bool trans_xvslei_b(CPULoongArchState *env, arg_vslei_b *restrict a) {CHECK_FPE(32); return vslei_b(env, a, 32);}
static bool trans_xvslei_h(CPULoongArchState *env, arg_vslei_h *restrict a) {CHECK_FPE(32); return vslei_h(env, a, 32);}
static bool trans_xvslei_w(CPULoongArchState *env, arg_vslei_w *restrict a) {CHECK_FPE(32); return vslei_w(env, a, 32);}
static bool trans_xvslei_d(CPULoongArchState *env, arg_vslei_d *restrict a) {CHECK_FPE(32); return vslei_d(env, a, 32);}
static inline bool vsle_bu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] <= env->fpr[a->vk].vreg.UB[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vsle_hu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] <= env->fpr[a->vk].vreg.UH[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vsle_wu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] <= env->fpr[a->vk].vreg.UW[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vsle_du(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] <= env->fpr[a->vk].vreg.UD[i] ? -1 : 0;}env->pc += 4;return true;}
static bool trans_vsle_bu(CPULoongArchState *env, arg_vsle_bu *restrict a) {CHECK_FPE(16); return vsle_bu(env, a, 16);}
static bool trans_vsle_hu(CPULoongArchState *env, arg_vsle_hu *restrict a) {CHECK_FPE(16); return vsle_hu(env, a, 16);}
static bool trans_vsle_wu(CPULoongArchState *env, arg_vsle_wu *restrict a) {CHECK_FPE(16); return vsle_wu(env, a, 16);}
static bool trans_vsle_du(CPULoongArchState *env, arg_vsle_du *restrict a) {CHECK_FPE(16); return vsle_du(env, a, 16);}
static bool trans_xvsle_bu(CPULoongArchState *env, arg_vsle_bu *restrict a) {CHECK_FPE(32); return vsle_bu(env, a, 32);}
static bool trans_xvsle_hu(CPULoongArchState *env, arg_vsle_hu *restrict a) {CHECK_FPE(32); return vsle_hu(env, a, 32);}
static bool trans_xvsle_wu(CPULoongArchState *env, arg_vsle_wu *restrict a) {CHECK_FPE(32); return vsle_wu(env, a, 32);}
static bool trans_xvsle_du(CPULoongArchState *env, arg_vsle_du *restrict a) {CHECK_FPE(32); return vsle_du(env, a, 32);}
static inline bool vslei_bu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] <= (uint64_t)a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vslei_hu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] <= (uint64_t)a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vslei_wu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] <= (uint64_t)a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vslei_du(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] <= (uint64_t)a->imm ? -1: 0;}env->pc += 4;return true;}
static bool trans_vslei_bu(CPULoongArchState *env, arg_vslei_bu *restrict a) {CHECK_FPE(16); return vslei_bu(env, a, 16);}
static bool trans_vslei_hu(CPULoongArchState *env, arg_vslei_hu *restrict a) {CHECK_FPE(16); return vslei_hu(env, a, 16);}
static bool trans_vslei_wu(CPULoongArchState *env, arg_vslei_wu *restrict a) {CHECK_FPE(16); return vslei_wu(env, a, 16);}
static bool trans_vslei_du(CPULoongArchState *env, arg_vslei_du *restrict a) {CHECK_FPE(16); return vslei_du(env, a, 16);}
static bool trans_xvslei_bu(CPULoongArchState *env, arg_vslei_bu *restrict a) {CHECK_FPE(32); return vslei_bu(env, a, 32);}
static bool trans_xvslei_hu(CPULoongArchState *env, arg_vslei_hu *restrict a) {CHECK_FPE(32); return vslei_hu(env, a, 32);}
static bool trans_xvslei_wu(CPULoongArchState *env, arg_vslei_wu *restrict a) {CHECK_FPE(32); return vslei_wu(env, a, 32);}
static bool trans_xvslei_du(CPULoongArchState *env, arg_vslei_du *restrict a) {CHECK_FPE(32); return vslei_du(env, a, 32);}
static inline bool vslt_b(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] < env->fpr[a->vk].vreg.B[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vslt_h(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] < env->fpr[a->vk].vreg.H[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vslt_w(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] < env->fpr[a->vk].vreg.W[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vslt_d(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] < env->fpr[a->vk].vreg.D[i] ? -1 : 0;}env->pc += 4;return true;}
static bool trans_vslt_b(CPULoongArchState *env, arg_vslt_b *restrict a) {CHECK_FPE(16); return vslt_b(env, a, 16);}
static bool trans_vslt_h(CPULoongArchState *env, arg_vslt_h *restrict a) {CHECK_FPE(16); return vslt_h(env, a, 16);}
static bool trans_vslt_w(CPULoongArchState *env, arg_vslt_w *restrict a) {CHECK_FPE(16); return vslt_w(env, a, 16);}
static bool trans_vslt_d(CPULoongArchState *env, arg_vslt_d *restrict a) {CHECK_FPE(16); return vslt_d(env, a, 16);}
static bool trans_xvslt_b(CPULoongArchState *env, arg_vslt_b *restrict a) {CHECK_FPE(32); return vslt_b(env, a, 32);}
static bool trans_xvslt_h(CPULoongArchState *env, arg_vslt_h *restrict a) {CHECK_FPE(32); return vslt_h(env, a, 32);}
static bool trans_xvslt_w(CPULoongArchState *env, arg_vslt_w *restrict a) {CHECK_FPE(32); return vslt_w(env, a, 32);}
static bool trans_xvslt_d(CPULoongArchState *env, arg_vslt_d *restrict a) {CHECK_FPE(32); return vslt_d(env, a, 32);}
static inline bool vslti_b(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i] < a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vslti_h(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[i] < a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vslti_w(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[i] < a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vslti_d(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[i] < a->imm ? -1: 0;}env->pc += 4;return true;}
static bool trans_vslti_b(CPULoongArchState *env, arg_vslti_b *restrict a) {CHECK_FPE(16); return vslti_b(env, a, 16);}
static bool trans_vslti_h(CPULoongArchState *env, arg_vslti_h *restrict a) {CHECK_FPE(16); return vslti_h(env, a, 16);}
static bool trans_vslti_w(CPULoongArchState *env, arg_vslti_w *restrict a) {CHECK_FPE(16); return vslti_w(env, a, 16);}
static bool trans_vslti_d(CPULoongArchState *env, arg_vslti_d *restrict a) {CHECK_FPE(16); return vslti_d(env, a, 16);}
static bool trans_xvslti_b(CPULoongArchState *env, arg_vslti_b *restrict a) {CHECK_FPE(32); return vslti_b(env, a, 32);}
static bool trans_xvslti_h(CPULoongArchState *env, arg_vslti_h *restrict a) {CHECK_FPE(32); return vslti_h(env, a, 32);}
static bool trans_xvslti_w(CPULoongArchState *env, arg_vslti_w *restrict a) {CHECK_FPE(32); return vslti_w(env, a, 32);}
static bool trans_xvslti_d(CPULoongArchState *env, arg_vslti_d *restrict a) {CHECK_FPE(32); return vslti_d(env, a, 32);}
static inline bool vslt_bu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] < env->fpr[a->vk].vreg.UB[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vslt_hu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] < env->fpr[a->vk].vreg.UH[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vslt_wu(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] < env->fpr[a->vk].vreg.UW[i] ? -1 : 0;}env->pc += 4;return true;}
static inline bool vslt_du(CPULoongArchState *env, arg_vvv *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8; for (size_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] < env->fpr[a->vk].vreg.UD[i] ? -1 : 0;}env->pc += 4;return true;}
static bool trans_vslt_bu(CPULoongArchState *env, arg_vslt_bu *restrict a) {CHECK_FPE(16); return vslt_bu(env, a, 16);}
static bool trans_vslt_hu(CPULoongArchState *env, arg_vslt_hu *restrict a) {CHECK_FPE(16); return vslt_hu(env, a, 16);}
static bool trans_vslt_wu(CPULoongArchState *env, arg_vslt_wu *restrict a) {CHECK_FPE(16); return vslt_wu(env, a, 16);}
static bool trans_vslt_du(CPULoongArchState *env, arg_vslt_du *restrict a) {CHECK_FPE(16); return vslt_du(env, a, 16);}
static bool trans_xvslt_bu(CPULoongArchState *env, arg_vslt_bu *restrict a) {CHECK_FPE(32); return vslt_bu(env, a, 32);}
static bool trans_xvslt_hu(CPULoongArchState *env, arg_vslt_hu *restrict a) {CHECK_FPE(32); return vslt_hu(env, a, 32);}
static bool trans_xvslt_wu(CPULoongArchState *env, arg_vslt_wu *restrict a) {CHECK_FPE(32); return vslt_wu(env, a, 32);}
static bool trans_xvslt_du(CPULoongArchState *env, arg_vslt_du *restrict a) {CHECK_FPE(32); return vslt_du(env, a, 32);}
static inline bool vslti_bu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 1;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UB[i] = env->fpr[a->vj].vreg.UB[i] < (uint64_t)a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vslti_hu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 2;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UH[i] = env->fpr[a->vj].vreg.UH[i] < (uint64_t)a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vslti_wu(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 4;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UW[i] = env->fpr[a->vj].vreg.UW[i] < (uint64_t)a->imm ? -1: 0;}env->pc += 4;return true;}
static inline bool vslti_du(CPULoongArchState *env, arg_vv_i *restrict a, uint32_t vlen) {uint32_t ele_cnt = vlen / 8;for (uint32_t i = 0; i < ele_cnt; i++) {env->fpr[a->vd].vreg.UD[i] = env->fpr[a->vj].vreg.UD[i] < (uint64_t)a->imm ? -1: 0;}env->pc += 4;return true;}
static bool trans_vslti_bu(CPULoongArchState *env, arg_vslti_bu *restrict a) {CHECK_FPE(16); return vslti_bu(env, a, 16);}
static bool trans_vslti_hu(CPULoongArchState *env, arg_vslti_hu *restrict a) {CHECK_FPE(16); return vslti_hu(env, a, 16);}
static bool trans_vslti_wu(CPULoongArchState *env, arg_vslti_wu *restrict a) {CHECK_FPE(16); return vslti_wu(env, a, 16);}
static bool trans_vslti_du(CPULoongArchState *env, arg_vslti_du *restrict a) {CHECK_FPE(16); return vslti_du(env, a, 16);}
static bool trans_xvslti_bu(CPULoongArchState *env, arg_vslti_bu *restrict a) {CHECK_FPE(32); return vslti_bu(env, a, 32);}
static bool trans_xvslti_hu(CPULoongArchState *env, arg_vslti_hu *restrict a) {CHECK_FPE(32); return vslti_hu(env, a, 32);}
static bool trans_xvslti_wu(CPULoongArchState *env, arg_vslti_wu *restrict a) {CHECK_FPE(32); return vslti_wu(env, a, 32);}
static bool trans_xvslti_du(CPULoongArchState *env, arg_vslti_du *restrict a) {CHECK_FPE(32); return vslti_du(env, a, 32);}
static bool trans_vfcmp_cond_s(CPULoongArchState *env, arg_vfcmp_cond_s *restrict a) {
    CHECK_FPE(16);
    uint32_t flags = get_fcmp_flags(a->fcond >> 1);
    (a->fcond & 1) ? helper_vfcmp_s_s(env, 16, a->vd, a->vj, a->vk, flags) : helper_vfcmp_c_s(env, 16, a->vd, a->vj, a->vk, flags);
    env->pc += 4;
    return true;
}
static bool trans_vfcmp_cond_d(CPULoongArchState *env, arg_vfcmp_cond_d *restrict a) {
    CHECK_FPE(16);
    uint32_t flags = get_fcmp_flags(a->fcond >> 1);
    (a->fcond & 1) ? helper_vfcmp_s_d(env, 16, a->vd, a->vj, a->vk, flags) : helper_vfcmp_c_d(env, 16, a->vd, a->vj, a->vk, flags);
    env->pc += 4;
    return true;
}
static bool trans_vbitsel_v(CPULoongArchState *env, arg_vbitsel_v *restrict a) {
    CHECK_FPE(16);
    for (size_t i = 0; i < 2; i++) {
        env->fpr[a->vd].vreg.D[i] = ((~env->fpr[a->va].vreg.D[i]) & env->fpr[a->vj].vreg.D[i]) | (env->fpr[a->va].vreg.D[i] & env->fpr[a->vk].vreg.D[i]);
    }
    env->pc += 4;
    return true;
}
static bool trans_vbitseli_b(CPULoongArchState *env, arg_vbitseli_b *restrict a) {
    CHECK_FPE(16);
    for (size_t i = 0; i < 16; i++) {
        env->fpr[a->vd].vreg.B[i] = ((~env->fpr[a->vd].vreg.B[i]) & env->fpr[a->vj].vreg.B[i]) | (env->fpr[a->vd].vreg.B[i] & a->imm);
    }
    env->pc += 4;
    return true;
}
static bool trans_vseteqz_v(CPULoongArchState *env, arg_vseteqz_v *restrict a) {
    CHECK_FPE(16);
    uint32_t ele_cnt = 16 / 8;
    bool r = 1;
    for (uint32_t i = 0; i < ele_cnt; i++) {
        r &= (env->fpr[a->vj].vreg.D[i] == 0);
    }
    env->cf[a->cd] = r;
    env->pc += 4;
    return true;
}
static bool trans_vsetnez_v(CPULoongArchState *env, arg_vsetnez_v *restrict a) {
    CHECK_FPE(16);
    uint32_t ele_cnt = 16 / 8;
    bool r = 0;
    for (uint32_t i = 0; i < ele_cnt; i++) {
        r |= (env->fpr[a->vj].vreg.D[i] != 0);
    }
    env->cf[a->cd] = r;
    env->pc += 4;
    return true;
}
static inline bool vsetanyeqz_b(CPULoongArchState *env, arg_cv *restrict a, uint32_t vlen) { uint32_t ele_cnt = vlen / 1; bool r = 0; for (uint32_t i = 0; i < ele_cnt; i++) { r |= (env->fpr[a->vj].vreg.B[i]==0); } env->cf[a->cd] = r; env->pc += 4; return true;}
static inline bool vsetanyeqz_h(CPULoongArchState *env, arg_cv *restrict a, uint32_t vlen) { uint32_t ele_cnt = vlen / 2; bool r = 0; for (uint32_t i = 0; i < ele_cnt; i++) { r |= (env->fpr[a->vj].vreg.H[i]==0); } env->cf[a->cd] = r; env->pc += 4; return true;}
static inline bool vsetanyeqz_w(CPULoongArchState *env, arg_cv *restrict a, uint32_t vlen) { uint32_t ele_cnt = vlen / 4; bool r = 0; for (uint32_t i = 0; i < ele_cnt; i++) { r |= (env->fpr[a->vj].vreg.W[i]==0); } env->cf[a->cd] = r; env->pc += 4; return true;}
static inline bool vsetanyeqz_d(CPULoongArchState *env, arg_cv *restrict a, uint32_t vlen) { uint32_t ele_cnt = vlen / 8; bool r = 0; for (uint32_t i = 0; i < ele_cnt; i++) { r |= (env->fpr[a->vj].vreg.D[i]==0); } env->cf[a->cd] = r; env->pc += 4; return true;}
static bool trans_vsetanyeqz_b(CPULoongArchState *env, arg_vsetanyeqz_b *restrict a) {CHECK_FPE(16); return vsetanyeqz_b(env, a, 16);}
static bool trans_vsetanyeqz_h(CPULoongArchState *env, arg_vsetanyeqz_h *restrict a) {CHECK_FPE(16); return vsetanyeqz_h(env, a, 16);}
static bool trans_vsetanyeqz_w(CPULoongArchState *env, arg_vsetanyeqz_w *restrict a) {CHECK_FPE(16); return vsetanyeqz_w(env, a, 16);}
static bool trans_vsetanyeqz_d(CPULoongArchState *env, arg_vsetanyeqz_d *restrict a) {CHECK_FPE(16); return vsetanyeqz_d(env, a, 16);}
static bool trans_xvsetanyeqz_b(CPULoongArchState *env, arg_vsetanyeqz_b *restrict a) {CHECK_FPE(32); return vsetanyeqz_b(env, a, 32);}
static bool trans_xvsetanyeqz_h(CPULoongArchState *env, arg_vsetanyeqz_h *restrict a) {CHECK_FPE(32); return vsetanyeqz_h(env, a, 32);}
static bool trans_xvsetanyeqz_w(CPULoongArchState *env, arg_vsetanyeqz_w *restrict a) {CHECK_FPE(32); return vsetanyeqz_w(env, a, 32);}
static bool trans_xvsetanyeqz_d(CPULoongArchState *env, arg_vsetanyeqz_d *restrict a) {CHECK_FPE(32); return vsetanyeqz_d(env, a, 32);}
static inline bool vsetallnez_b(CPULoongArchState *env, arg_cv *restrict a, uint32_t vlen) { uint32_t ele_cnt = vlen / 1; bool r = 1; for (uint32_t i = 0; i < ele_cnt; i++) { r &= (env->fpr[a->vj].vreg.B[i]!=0); } env->cf[a->cd] = r; env->pc += 4; return true;}
static inline bool vsetallnez_h(CPULoongArchState *env, arg_cv *restrict a, uint32_t vlen) { uint32_t ele_cnt = vlen / 2; bool r = 1; for (uint32_t i = 0; i < ele_cnt; i++) { r &= (env->fpr[a->vj].vreg.H[i]!=0); } env->cf[a->cd] = r; env->pc += 4; return true;}
static inline bool vsetallnez_w(CPULoongArchState *env, arg_cv *restrict a, uint32_t vlen) { uint32_t ele_cnt = vlen / 4; bool r = 1; for (uint32_t i = 0; i < ele_cnt; i++) { r &= (env->fpr[a->vj].vreg.W[i]!=0); } env->cf[a->cd] = r; env->pc += 4; return true;}
static inline bool vsetallnez_d(CPULoongArchState *env, arg_cv *restrict a, uint32_t vlen) { uint32_t ele_cnt = vlen / 8; bool r = 1; for (uint32_t i = 0; i < ele_cnt; i++) { r &= (env->fpr[a->vj].vreg.D[i]!=0); } env->cf[a->cd] = r; env->pc += 4; return true;}
static bool trans_vsetallnez_b(CPULoongArchState *env, arg_vsetallnez_b *restrict a) {CHECK_FPE(16); return vsetallnez_b(env, a, 16);}
static bool trans_vsetallnez_h(CPULoongArchState *env, arg_vsetallnez_h *restrict a) {CHECK_FPE(16); return vsetallnez_h(env, a, 16);}
static bool trans_vsetallnez_w(CPULoongArchState *env, arg_vsetallnez_w *restrict a) {CHECK_FPE(16); return vsetallnez_w(env, a, 16);}
static bool trans_vsetallnez_d(CPULoongArchState *env, arg_vsetallnez_d *restrict a) {CHECK_FPE(16); return vsetallnez_d(env, a, 16);}
static bool trans_xvsetallnez_b(CPULoongArchState *env, arg_vsetallnez_b *restrict a) {CHECK_FPE(32); return vsetallnez_b(env, a, 32);}
static bool trans_xvsetallnez_h(CPULoongArchState *env, arg_vsetallnez_h *restrict a) {CHECK_FPE(32); return vsetallnez_h(env, a, 32);}
static bool trans_xvsetallnez_w(CPULoongArchState *env, arg_vsetallnez_w *restrict a) {CHECK_FPE(32); return vsetallnez_w(env, a, 32);}
static bool trans_xvsetallnez_d(CPULoongArchState *env, arg_vsetallnez_d *restrict a) {CHECK_FPE(32); return vsetallnez_d(env, a, 32);}
static bool trans_vinsgr2vr_b(CPULoongArchState *env, arg_vinsgr2vr_b *restrict a) {CHECK_FPE(16); env->fpr[a->vd].vreg.UB[a->imm] = env->gpr[a->rj]; env->pc += 4; return true;}
static bool trans_vinsgr2vr_h(CPULoongArchState *env, arg_vinsgr2vr_h *restrict a) {CHECK_FPE(16); env->fpr[a->vd].vreg.UH[a->imm] = env->gpr[a->rj]; env->pc += 4; return true;}
static bool trans_vinsgr2vr_w(CPULoongArchState *env, arg_vinsgr2vr_w *restrict a) {CHECK_FPE(16); env->fpr[a->vd].vreg.UW[a->imm] = env->gpr[a->rj]; env->pc += 4; return true;}
static bool trans_vinsgr2vr_d(CPULoongArchState *env, arg_vinsgr2vr_d *restrict a) {CHECK_FPE(16); env->fpr[a->vd].vreg.UD[a->imm] = env->gpr[a->rj]; env->pc += 4; return true;}
static bool trans_vpickve2gr_b(CPULoongArchState *env, arg_vpickve2gr_b *restrict a) {CHECK_FPE(16); env->gpr[a->rd] = (int64_t)env->fpr[a->vj].vreg.B[a->imm]; env->pc += 4; return true;}
static bool trans_vpickve2gr_h(CPULoongArchState *env, arg_vpickve2gr_h *restrict a) {CHECK_FPE(16); env->gpr[a->rd] = (int64_t)env->fpr[a->vj].vreg.H[a->imm]; env->pc += 4; return true;}
static bool trans_vpickve2gr_w(CPULoongArchState *env, arg_vpickve2gr_w *restrict a) {CHECK_FPE(16); env->gpr[a->rd] = (int64_t)env->fpr[a->vj].vreg.W[a->imm]; env->pc += 4; return true;}
static bool trans_vpickve2gr_d(CPULoongArchState *env, arg_vpickve2gr_d *restrict a) {CHECK_FPE(16); env->gpr[a->rd] = (int64_t)env->fpr[a->vj].vreg.D[a->imm]; env->pc += 4; return true;}
static bool trans_vpickve2gr_bu(CPULoongArchState *env, arg_vpickve2gr_bu *restrict a) {CHECK_FPE(16); env->gpr[a->rd] = (uint64_t)env->fpr[a->vj].vreg.UB[a->imm]; env->pc += 4; return true;}
static bool trans_vpickve2gr_hu(CPULoongArchState *env, arg_vpickve2gr_hu *restrict a) {CHECK_FPE(16); env->gpr[a->rd] = (uint64_t)env->fpr[a->vj].vreg.UH[a->imm]; env->pc += 4; return true;}
static bool trans_vpickve2gr_wu(CPULoongArchState *env, arg_vpickve2gr_wu *restrict a) {CHECK_FPE(16); env->gpr[a->rd] = (uint64_t)env->fpr[a->vj].vreg.UW[a->imm]; env->pc += 4; return true;}
static bool trans_vpickve2gr_du(CPULoongArchState *env, arg_vpickve2gr_du *restrict a) {CHECK_FPE(16); env->gpr[a->rd] = (uint64_t)env->fpr[a->vj].vreg.UD[a->imm]; env->pc += 4; return true;}

static bool trans_xvinsgr2vr_w(CPULoongArchState *env, arg_vinsgr2vr_w *restrict a) {CHECK_FPE(32); env->fpr[a->vd].vreg.UW[a->imm] = env->gpr[a->rj]; env->pc += 4; return true;}
static bool trans_xvinsgr2vr_d(CPULoongArchState *env, arg_vinsgr2vr_d *restrict a) {CHECK_FPE(32); env->fpr[a->vd].vreg.UD[a->imm] = env->gpr[a->rj]; env->pc += 4; return true;}
static bool trans_xvpickve2gr_w(CPULoongArchState *env, arg_vpickve2gr_w *restrict a) {CHECK_FPE(32); env->gpr[a->rd] = (int64_t)env->fpr[a->vj].vreg.W[a->imm]; env->pc += 4; return true;}
static bool trans_xvpickve2gr_d(CPULoongArchState *env, arg_vpickve2gr_d *restrict a) {CHECK_FPE(32); env->gpr[a->rd] = (int64_t)env->fpr[a->vj].vreg.D[a->imm]; env->pc += 4; return true;}
static bool trans_xvpickve2gr_wu(CPULoongArchState *env, arg_vpickve2gr_wu *restrict a) {CHECK_FPE(32); env->gpr[a->rd] = (uint64_t)env->fpr[a->vj].vreg.UW[a->imm]; env->pc += 4; return true;}
static bool trans_xvpickve2gr_du(CPULoongArchState *env, arg_vpickve2gr_du *restrict a) {CHECK_FPE(32); env->gpr[a->rd] = (uint64_t)env->fpr[a->vj].vreg.UD[a->imm]; env->pc += 4; return true;}
static bool vreplgr2vr_b(CPULoongArchState *env, arg_vr *restrict a, uint32_t vlen) { uint32_t ele_cnt = vlen / 1; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.UB[i] = env->gpr[a->rj]; } env->pc += 4;return true;}
static bool vreplgr2vr_h(CPULoongArchState *env, arg_vr *restrict a, uint32_t vlen) { uint32_t ele_cnt = vlen / 2; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.UH[i] = env->gpr[a->rj]; } env->pc += 4;return true;}
static bool vreplgr2vr_w(CPULoongArchState *env, arg_vr *restrict a, uint32_t vlen) { uint32_t ele_cnt = vlen / 4; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.UW[i] = env->gpr[a->rj]; } env->pc += 4;return true;}
static bool vreplgr2vr_d(CPULoongArchState *env, arg_vr *restrict a, uint32_t vlen) { uint32_t ele_cnt = vlen / 8; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.UD[i] = env->gpr[a->rj]; } env->pc += 4;return true;}
static bool trans_vreplgr2vr_b(CPULoongArchState *env, arg_vreplgr2vr_b *restrict a) {CHECK_FPE(16); return vreplgr2vr_b(env, a, 16);}
static bool trans_vreplgr2vr_h(CPULoongArchState *env, arg_vreplgr2vr_h *restrict a) {CHECK_FPE(16); return vreplgr2vr_h(env, a, 16);}
static bool trans_vreplgr2vr_w(CPULoongArchState *env, arg_vreplgr2vr_w *restrict a) {CHECK_FPE(16); return vreplgr2vr_w(env, a, 16);}
static bool trans_vreplgr2vr_d(CPULoongArchState *env, arg_vreplgr2vr_d *restrict a) {CHECK_FPE(16); return vreplgr2vr_d(env, a, 16);}
static bool trans_xvreplgr2vr_b(CPULoongArchState *env, arg_vreplgr2vr_b *restrict a) {CHECK_FPE(32); return vreplgr2vr_b(env, a, 32);}
static bool trans_xvreplgr2vr_h(CPULoongArchState *env, arg_vreplgr2vr_h *restrict a) {CHECK_FPE(32); return vreplgr2vr_h(env, a, 32);}
static bool trans_xvreplgr2vr_w(CPULoongArchState *env, arg_vreplgr2vr_w *restrict a) {CHECK_FPE(32); return vreplgr2vr_w(env, a, 32);}
static bool trans_xvreplgr2vr_d(CPULoongArchState *env, arg_vreplgr2vr_d *restrict a) {CHECK_FPE(32); return vreplgr2vr_d(env, a, 32);}
static bool trans_vreplve_b(CPULoongArchState *env, arg_vreplve_b *restrict a) {CHECK_FPE(16); int32_t ele_cnt = 16 / 1; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[env->gpr[a->rk] & (ele_cnt - 1)]; } env->pc += 4; return true;}
static bool trans_vreplve_h(CPULoongArchState *env, arg_vreplve_h *restrict a) {CHECK_FPE(16); int32_t ele_cnt = 16 / 2; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[env->gpr[a->rk] & (ele_cnt - 1)]; } env->pc += 4; return true;}
static bool trans_vreplve_w(CPULoongArchState *env, arg_vreplve_w *restrict a) {CHECK_FPE(16); int32_t ele_cnt = 16 / 4; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[env->gpr[a->rk] & (ele_cnt - 1)]; } env->pc += 4; return true;}
static bool trans_vreplve_d(CPULoongArchState *env, arg_vreplve_d *restrict a) {CHECK_FPE(16); int32_t ele_cnt = 16 / 8; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[env->gpr[a->rk] & (ele_cnt - 1)]; } env->pc += 4; return true;}
static bool trans_vreplvei_b(CPULoongArchState *env, arg_vreplvei_b *restrict a) {CHECK_FPE(16); uint32_t ele_cnt = 16 / 1; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[a->imm]; } env->pc += 4; return true;}
static bool trans_vreplvei_h(CPULoongArchState *env, arg_vreplvei_h *restrict a) {CHECK_FPE(16); uint32_t ele_cnt = 16 / 2; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[a->imm]; } env->pc += 4; return true;}
static bool trans_vreplvei_w(CPULoongArchState *env, arg_vreplvei_w *restrict a) {CHECK_FPE(16); uint32_t ele_cnt = 16 / 4; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[a->imm]; } env->pc += 4; return true;}
static bool trans_vreplvei_d(CPULoongArchState *env, arg_vreplvei_d *restrict a) {CHECK_FPE(16); uint32_t ele_cnt = 16 / 8; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[a->imm]; } env->pc += 4; return true;}
static bool trans_vbsll_v(CPULoongArchState *env, arg_vbsll_v *restrict a) {
    CHECK_FPE(16);
    int imm = a->imm & 0xf;
    for (int i = 0; i < (16 - imm); i ++) {
        env->fpr[a->vd].vreg.B[15 - i] = env->fpr[a->vj].vreg.B[15 - (i + imm)];
    }
    for (int i = 0; i < imm; i ++) {
        env->fpr[a->vd].vreg.B[i] = 0;
    }
    env->pc += 4;
    return true;
}
static bool trans_vbsrl_v(CPULoongArchState *env, arg_vbsrl_v *restrict a) {
    CHECK_FPE(16);
    int imm = a->imm & 0xf;
    for (int i = 0; i < (16 - imm); i ++) {
        env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i + imm];
    }
    for (int i = (16 - imm); i < 16; i ++) {
        env->fpr[a->vd].vreg.B[i] = 0;
    }
    env->pc += 4;
    return true;
}
gen_trans_vvvd(vpackev_b, 16, vpackev_b)
gen_trans_vvvd(vpackev_h, 16, vpackev_h)
gen_trans_vvvd(vpackev_w, 16, vpackev_w)
gen_trans_vvvd(vpackev_d, 16, vpackev_d)
gen_trans_vvvd(vpackod_b, 16, vpackod_b)
gen_trans_vvvd(vpackod_h, 16, vpackod_h)
gen_trans_vvvd(vpackod_w, 16, vpackod_w)
gen_trans_vvvd(vpackod_d, 16, vpackod_d)
static bool trans_vpickev_b(CPULoongArchState *env, arg_vpickev_b *restrict a) {CHECK_FPE(16); int oprsz = 16; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickev_b(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_vpickev_h(CPULoongArchState *env, arg_vpickev_h *restrict a) {CHECK_FPE(16); int oprsz = 16; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickev_h(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_vpickev_w(CPULoongArchState *env, arg_vpickev_w *restrict a) {CHECK_FPE(16); int oprsz = 16; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickev_w(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_vpickev_d(CPULoongArchState *env, arg_vpickev_d *restrict a) {CHECK_FPE(16); int oprsz = 16; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickev_d(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_vpickod_b(CPULoongArchState *env, arg_vpickod_b *restrict a) {CHECK_FPE(16); int oprsz = 16; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickod_b(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_vpickod_h(CPULoongArchState *env, arg_vpickod_h *restrict a) {CHECK_FPE(16); int oprsz = 16; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickod_h(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_vpickod_w(CPULoongArchState *env, arg_vpickod_w *restrict a) {CHECK_FPE(16); int oprsz = 16; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickod_w(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_vpickod_d(CPULoongArchState *env, arg_vpickod_d *restrict a) {CHECK_FPE(16); int oprsz = 16; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickod_d(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_xvpickev_b(CPULoongArchState *env, arg_vpickev_b *restrict a) {CHECK_FPE(32); int oprsz = 32; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickev_b(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_xvpickev_h(CPULoongArchState *env, arg_vpickev_h *restrict a) {CHECK_FPE(32); int oprsz = 32; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickev_h(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_xvpickev_w(CPULoongArchState *env, arg_vpickev_w *restrict a) {CHECK_FPE(32); int oprsz = 32; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickev_w(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_xvpickev_d(CPULoongArchState *env, arg_vpickev_d *restrict a) {CHECK_FPE(32); int oprsz = 32; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickev_d(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_xvpickod_b(CPULoongArchState *env, arg_vpickod_b *restrict a) {CHECK_FPE(32); int oprsz = 32; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickod_b(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_xvpickod_h(CPULoongArchState *env, arg_vpickod_h *restrict a) {CHECK_FPE(32); int oprsz = 32; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickod_h(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_xvpickod_w(CPULoongArchState *env, arg_vpickod_w *restrict a) {CHECK_FPE(32); int oprsz = 32; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickod_w(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_xvpickod_d(CPULoongArchState *env, arg_vpickod_d *restrict a) {CHECK_FPE(32); int oprsz = 32; uint32_t desc = simd_desc(oprsz, oprsz, 0); helper_vpickod_d(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc); env->pc += 4; return true;}
static bool trans_vilvl_b(CPULoongArchState *env, arg_vilvl_b *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vilvl_b(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc);
    env->pc += 4;
    return true;
}
static bool trans_vilvl_h(CPULoongArchState *env, arg_vilvl_h *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vilvl_h(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc);
    env->pc += 4;
    return true;
}
static bool trans_vilvl_w(CPULoongArchState *env, arg_vilvl_w *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vilvl_w(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc);
    env->pc += 4;
    return true;
}
static bool trans_vilvl_d(CPULoongArchState *env, arg_vilvl_d *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vilvl_d(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc);
    env->pc += 4;
    return true;
}
static bool trans_vilvh_b(CPULoongArchState *env, arg_vilvh_b *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vilvh_b(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc);
    env->pc += 4;
    return true;
}
static bool trans_vilvh_h(CPULoongArchState *env, arg_vilvh_h *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vilvh_h(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc);
    env->pc += 4;
    return true;
}
static bool trans_vilvh_w(CPULoongArchState *env, arg_vilvh_w *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vilvh_w(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc);
    env->pc += 4;
    return true;
}
static bool trans_vilvh_d(CPULoongArchState *env, arg_vilvh_d *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vilvh_d(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], desc);
    env->pc += 4;
    return true;
}
static bool trans_vshuf_b(CPULoongArchState *env, arg_vshuf_b *restrict a) {
    CHECK_FPE(16);
    int oprsz = 16;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vshuf_b(&env->fpr[a->vd], &env->fpr[a->vj], &env->fpr[a->vk], &env->fpr[a->va], desc);
    env->pc += 4;
    return true;
}
static bool trans_vshuf_h(CPULoongArchState *env, arg_vshuf_h *restrict a) {CHECK_FPE(16); const uint32_t ele_cnt = 16 / 2;int16_t vv[ele_cnt * 2];for (size_t i = 0; i < ele_cnt; i++) {vv[i] = env->fpr[a->vk].vreg.H[i];vv[ele_cnt + i] = env->fpr[a->vj].vreg.H[i];}for (size_t i = 0; i < ele_cnt; i++) { env->fpr[a->vd].vreg.H[i] = vv[env->fpr[a->vd].vreg.H[i] & (ele_cnt * 2 - 1)];}env->pc += 4;return true;}
static bool trans_vshuf_w(CPULoongArchState *env, arg_vshuf_w *restrict a) {CHECK_FPE(16); const uint32_t ele_cnt = 16 / 4;int32_t vv[ele_cnt * 2];for (size_t i = 0; i < ele_cnt; i++) {vv[i] = env->fpr[a->vk].vreg.W[i];vv[ele_cnt + i] = env->fpr[a->vj].vreg.W[i];}for (size_t i = 0; i < ele_cnt; i++) { env->fpr[a->vd].vreg.W[i] = vv[env->fpr[a->vd].vreg.W[i] & (ele_cnt * 2 - 1)];}env->pc += 4;return true;}
static bool trans_vshuf_d(CPULoongArchState *env, arg_vshuf_d *restrict a) {CHECK_FPE(16); const uint32_t ele_cnt = 16 / 8;int64_t vv[ele_cnt * 2];for (size_t i = 0; i < ele_cnt; i++) {vv[i] = env->fpr[a->vk].vreg.D[i];vv[ele_cnt + i] = env->fpr[a->vj].vreg.D[i];}for (size_t i = 0; i < ele_cnt; i++) { env->fpr[a->vd].vreg.D[i] = vv[env->fpr[a->vd].vreg.D[i] & (ele_cnt * 2 - 1)];}env->pc += 4;return true;}
gen_trans_vvid(vshuf4i_b, 16, vshuf4i_b)
gen_trans_vvid(vshuf4i_h, 16, vshuf4i_h)
gen_trans_vvid(vshuf4i_w, 16, vshuf4i_w)
gen_trans_vvid(vshuf4i_d, 16, vshuf4i_d)
gen_trans_vvid(vpermi_w, 16, vpermi_w)

gen_trans_vvid(vextrins_d, 16, vextrins_d)
gen_trans_vvid(vextrins_w, 16, vextrins_w)
gen_trans_vvid(vextrins_h, 16, vextrins_h)
gen_trans_vvid(vextrins_b, 16, vextrins_b)
static bool trans_vld(CPULoongArchState *env, arg_vld *restrict a) {
    CHECK_FPE(16);
    uint64_t va = add_addr(env->gpr[a->rj], a->imm);
    lsassert(!is_io(load_pa(env, va)));
    env->fpr[a->vd].vreg.D[0] = ld_d(env, va);
    env->fpr[a->vd].vreg.D[1] = ld_d(env, va + 8);
    env->pc += 4;
    return true;
}
static bool trans_vst(CPULoongArchState *env, arg_vst *restrict a) {
    CHECK_FPE(16);
    uint64_t va = add_addr(env->gpr[a->rj], a->imm);
    lsassert(!is_io(store_pa(env, va)));
    st_d(env, va, env->fpr[a->vd].vreg.D[0]);
    st_d(env, va + 8, env->fpr[a->vd].vreg.D[1]);
    env->pc += 4;
    return true;
}
static bool trans_vldx(CPULoongArchState *env, arg_vldx *restrict a) {
    CHECK_FPE(16);
    uint64_t va = add_addr(env->gpr[a->rj], env->gpr[a->rk]);
    lsassert(!is_io(load_pa(env, va)));
    env->fpr[a->vd].vreg.D[0] = ld_d(env, va);
    env->fpr[a->vd].vreg.D[1] = ld_d(env, va + 8);
    env->pc += 4;
    return true;
}
static bool trans_vstx(CPULoongArchState *env, arg_vstx *restrict a) {
    CHECK_FPE(16);
    uint64_t va = add_addr(env->gpr[a->rj], env->gpr[a->rk]);
    lsassert(!is_io(store_pa(env, va)));
    st_d(env, va, env->fpr[a->vd].vreg.D[0]);
    st_d(env, va + 8, env->fpr[a->vd].vreg.D[1]);
    env->pc += 4;
    return true;
}
static bool trans_vldrepl_b(CPULoongArchState *env, arg_vldrepl_b *restrict a) {CHECK_FPE(16); int8_t data = ld_b(env, add_addr(env->gpr[a->rj], a->imm));for (size_t i = 0; i < 16; i++){env->fpr[a->vd].vreg.B[i] = data;}env->pc += 4;return true;}
static bool trans_vldrepl_h(CPULoongArchState *env, arg_vldrepl_h *restrict a) {CHECK_FPE(16); int16_t data = ld_h(env, add_addr(env->gpr[a->rj], a->imm));for (size_t i = 0; i < 8; i++){env->fpr[a->vd].vreg.H[i] = data;}env->pc += 4;return true;}
static bool trans_vldrepl_w(CPULoongArchState *env, arg_vldrepl_w *restrict a) {CHECK_FPE(16); int32_t data = ld_w(env, add_addr(env->gpr[a->rj], a->imm));for (size_t i = 0; i < 4; i++){env->fpr[a->vd].vreg.W[i] = data;}env->pc += 4;return true;}
static bool trans_vldrepl_d(CPULoongArchState *env, arg_vldrepl_d *restrict a) {CHECK_FPE(16); int64_t data = ld_d(env, add_addr(env->gpr[a->rj], a->imm));for (size_t i = 0; i < 2; i++){env->fpr[a->vd].vreg.D[i] = data;}env->pc += 4;return true;}
static bool trans_vstelm_b(CPULoongArchState *env, arg_vstelm_b *restrict a) {CHECK_FPE(16); st_b(env, add_addr(env->gpr[a->rj], a->imm), env->fpr[a->vd].vreg.B[a->imm2]);env->pc += 4;return true;}
static bool trans_vstelm_h(CPULoongArchState *env, arg_vstelm_h *restrict a) {CHECK_FPE(16); st_h(env, add_addr(env->gpr[a->rj], a->imm), env->fpr[a->vd].vreg.H[a->imm2]);env->pc += 4;return true;}
static bool trans_vstelm_w(CPULoongArchState *env, arg_vstelm_w *restrict a) {CHECK_FPE(16); st_w(env, add_addr(env->gpr[a->rj], a->imm), env->fpr[a->vd].vreg.W[a->imm2]);env->pc += 4;return true;}
static bool trans_vstelm_d(CPULoongArchState *env, arg_vstelm_d *restrict a) {CHECK_FPE(16); st_d(env, add_addr(env->gpr[a->rj], a->imm), env->fpr[a->vd].vreg.D[a->imm2]);env->pc += 4;return true;}


gen_trans_vvd(vext2xv_d_b, 32, vext2xv_d_b)
gen_trans_vvd(vext2xv_d_h, 32, vext2xv_d_h)
gen_trans_vvd(vext2xv_du_bu, 32, vext2xv_du_bu)
gen_trans_vvd(vext2xv_du_hu, 32, vext2xv_du_hu)
gen_trans_vvd(vext2xv_du_wu, 32, vext2xv_du_wu)
gen_trans_vvd(vext2xv_d_w, 32, vext2xv_d_w)
gen_trans_vvd(vext2xv_h_b, 32, vext2xv_h_b)
gen_trans_vvd(vext2xv_hu_bu, 32, vext2xv_hu_bu)
gen_trans_vvd(vext2xv_w_b, 32, vext2xv_w_b)
gen_trans_vvd(vext2xv_w_h, 32, vext2xv_w_h)
gen_trans_vvd(vext2xv_wu_bu, 32, vext2xv_wu_bu)
gen_trans_vvd(vext2xv_wu_hu, 32, vext2xv_wu_hu)

static bool trans_xvadd_q(CPULoongArchState *env, arg_xvadd_q *restrict a) {
    CHECK_FPE(32);
    env->fpr[a->vd].vreg.Q[0] = env->fpr[a->vj].vreg.Q[0] + env->fpr[a->vk].vreg.Q[0];
    env->fpr[a->vd].vreg.Q[1] = env->fpr[a->vj].vreg.Q[1] + env->fpr[a->vk].vreg.Q[1];
    env->pc += 4;
    return true;
}
gen_trans_vvvd(xvaddwev_d_w, 32, vaddwev_d_w)
gen_trans_vvvd(xvaddwev_d_wu, 32, vaddwev_d_wu)
gen_trans_vvvd(xvaddwev_d_wu_w, 32, vaddwev_d_wu_w)
gen_trans_vvvd(xvaddwev_h_b, 32, vaddwev_h_b)
gen_trans_vvvd(xvaddwev_h_bu_b, 32, vaddwev_h_bu_b)
gen_trans_vvvd(xvaddwev_h_bu, 32, vaddwev_h_bu)
gen_trans_vvvd(xvaddwev_q_d, 32, vaddwev_q_d)
gen_trans_vvvd(xvaddwev_q_du_d, 32, vaddwev_q_du_d)
gen_trans_vvvd(xvaddwev_q_du, 32, vaddwev_q_du)
gen_trans_vvvd(xvaddwev_w_h, 32, vaddwev_w_h)
gen_trans_vvvd(xvaddwev_w_hu, 32, vaddwev_w_hu)
gen_trans_vvvd(xvaddwev_w_hu_h, 32, vaddwev_w_hu_h)
gen_trans_vvvd(xvaddwod_d_w, 32, vaddwod_d_w)
gen_trans_vvvd(xvaddwod_d_wu, 32, vaddwod_d_wu)
gen_trans_vvvd(xvaddwod_d_wu_w, 32, vaddwod_d_wu_w)
gen_trans_vvvd(xvaddwod_h_b, 32, vaddwod_h_b)
gen_trans_vvvd(xvaddwod_h_bu_b, 32, vaddwod_h_bu_b)
gen_trans_vvvd(xvaddwod_h_bu, 32, vaddwod_h_bu)
gen_trans_vvvd(xvaddwod_q_d, 32, vaddwod_q_d)
gen_trans_vvvd(xvaddwod_q_du_d, 32, vaddwod_q_du_d)
gen_trans_vvvd(xvaddwod_q_du, 32, vaddwod_q_du)
gen_trans_vvvd(xvaddwod_w_h, 32, vaddwod_w_h)
gen_trans_vvvd(xvaddwod_w_hu, 32, vaddwod_w_hu)
gen_trans_vvvd(xvaddwod_w_hu_h, 32, vaddwod_w_hu_h)
static bool trans_xvbitseli_b(CPULoongArchState *env, arg_xvbitseli_b *restrict a) {
    CHECK_FPE(32);
    for (size_t i = 0; i < 32; i++) {
        env->fpr[a->vd].vreg.B[i] = ((~env->fpr[a->vd].vreg.B[i]) & env->fpr[a->vj].vreg.B[i]) | (env->fpr[a->vd].vreg.B[i] & a->imm);
    }
    env->pc += 4;
    return true;
}
static bool trans_xvbitsel_v(CPULoongArchState *env, arg_xvbitsel_v *restrict a) {
    CHECK_FPE(32);
    for (size_t i = 0; i < 4; i++) {
        env->fpr[a->vd].vreg.D[i] = ((~env->fpr[a->va].vreg.D[i]) & env->fpr[a->vj].vreg.D[i]) | (env->fpr[a->va].vreg.D[i] & env->fpr[a->vk].vreg.D[i]);
    }
    env->pc += 4;
    return true;
}
static bool trans_xvbsll_v(CPULoongArchState *env, arg_xvbsll_v *restrict a) {
    CHECK_FPE(32);
    int vlen = 16;
    int imm = a->imm & (vlen - 1);
    for (int i = 0; i < (vlen - imm); i ++) {
        env->fpr[a->vd].vreg.B[vlen - 1 - i] = env->fpr[a->vj].vreg.B[vlen - 1 - (i + imm)];
        env->fpr[a->vd].vreg.B[vlen + vlen - 1 - i] = env->fpr[a->vj].vreg.B[vlen + vlen - 1 - (i + imm)];
    }
    for (int i = 0; i < imm; i ++) {
        env->fpr[a->vd].vreg.B[i] = 0;
        env->fpr[a->vd].vreg.B[i + vlen] = 0;
    }
    env->pc += 4;
    return true;
}
static bool trans_xvbsrl_v(CPULoongArchState *env, arg_xvbsrl_v *restrict a) {
    CHECK_FPE(32);
    int vlen = 16;
    int imm = a->imm & (vlen - 1);
    for (int i = 0; i < (vlen - imm); i ++) {
        env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[i + imm];
        env->fpr[a->vd].vreg.B[i + vlen] = env->fpr[a->vj].vreg.B[i + imm + vlen];    }
    for (int i = (vlen - imm); i < vlen; i ++) {
        env->fpr[a->vd].vreg.B[i] = 0;
        env->fpr[a->vd].vreg.B[i + vlen] = 0;
    }
    env->pc += 4;
    return true;
}
gen_trans_vvd(xvclo_b, 32, vclo_b)
gen_trans_vvd(xvclo_h, 32, vclo_h)
gen_trans_vvd(xvclo_w, 32, vclo_w)
gen_trans_vvd(xvclo_d, 32, vclo_d)
gen_trans_vvd(xvclz_b, 32, vclz_b)
gen_trans_vvd(xvclz_h, 32, vclz_h)
gen_trans_vvd(xvclz_w, 32, vclz_w)
gen_trans_vvd(xvclz_d, 32, vclz_d)
gen_trans_vvd(xvpcnt_b, 32, vpcnt_b)
gen_trans_vvd(xvpcnt_h, 32, vpcnt_h)
gen_trans_vvd(xvpcnt_w, 32, vpcnt_w)
gen_trans_vvd(xvpcnt_d, 32, vpcnt_d)

gen_trans_vvd(xvexth_du_wu, 32, vexth_du_wu)
gen_trans_vvd(xvexth_d_w, 32, vexth_d_w)
gen_trans_vvd(xvexth_h_b, 32, vexth_h_b)
gen_trans_vvd(xvexth_hu_bu, 32, vexth_hu_bu)
gen_trans_vvd(xvexth_q_d, 32, vexth_q_d)
gen_trans_vvd(xvexth_qu_du, 32, vexth_qu_du)
gen_trans_vvd(xvexth_w_h, 32, vexth_w_h)
gen_trans_vvd(xvexth_wu_hu, 32, vexth_wu_hu)
gen_trans_vvd(xvextl_q_d, 32, vextl_q_d)
gen_trans_vvd(xvextl_qu_du, 32, vextl_qu_du)
gen_trans_vvid(xvextrins_d, 32, vextrins_d)
gen_trans_vvid(xvextrins_w, 32, vextrins_w)
gen_trans_vvid(xvextrins_h, 32, vextrins_h)
gen_trans_vvid(xvextrins_b, 32, vextrins_b)
gen_trans_vved(xvfclass_d, 32, vfclass_d)
gen_trans_vved(xvfclass_s, 32, vfclass_s)
static bool trans_xvfcmp_cond_d(CPULoongArchState *env, arg_xvfcmp_cond_d *restrict a) {
    CHECK_FPE(32);
    uint32_t flags = get_fcmp_flags(a->fcond >> 1);
    (a->fcond & 1) ? helper_vfcmp_s_d(env, 32, a->vd, a->vj, a->vk, flags) : helper_vfcmp_c_d(env, 32, a->vd, a->vj, a->vk, flags);
    env->pc += 4;
    return true;
}
static bool trans_xvfcmp_cond_s(CPULoongArchState *env, arg_xvfcmp_cond_s *restrict a) {
    CHECK_FPE(32);
    uint32_t flags = get_fcmp_flags(a->fcond >> 1);
    (a->fcond & 1) ? helper_vfcmp_s_s(env, 32, a->vd, a->vj, a->vk, flags) : helper_vfcmp_c_s(env, 32, a->vd, a->vj, a->vk, flags);
    env->pc += 4;
    return true;
}
gen_trans_vved(xvfcvtl_s_h, 32, vfcvtl_s_h)
gen_trans_vved(xvfcvth_s_h, 32, vfcvth_s_h)
gen_trans_vved(xvfcvtl_d_s, 32, vfcvtl_d_s)
gen_trans_vved(xvfcvth_d_s, 32, vfcvth_d_s)
gen_trans_vvved(xvfcvt_h_s, 32, vfcvt_h_s)
gen_trans_vvved(xvfcvt_s_d, 32, vfcvt_s_d)
gen_trans_vved(xvffint_s_w, 32, vffint_s_w)
gen_trans_vved(xvffint_s_wu, 32, vffint_s_wu)
gen_trans_vved(xvffint_d_l, 32, vffint_d_l)
gen_trans_vved(xvffint_d_lu, 32, vffint_d_lu)
gen_trans_vved(xvffintl_d_w, 32, vffintl_d_w)
gen_trans_vved(xvffinth_d_w, 32, vffinth_d_w)
gen_trans_vvved(xvffint_s_l, 32, vffint_s_l)
gen_trans_vved(xvflogb_d, 32, vflogb_d)
gen_trans_vved(xvflogb_s, 32, vflogb_s)
gen_trans_vved(xvfrecip_d, 32, vfrecip_d)
gen_trans_vved(xvfrecip_s, 32, vfrecip_s)
gen_trans_vved(xvfrsqrt_d, 32, vfrsqrt_d)
gen_trans_vved(xvfrsqrt_s, 32, vfrsqrt_s)
gen_trans_vvvd(xvfrstp_b, 32, vfrstp_b)
gen_trans_vvvd(xvfrstp_h, 32, vfrstp_h)
gen_trans_vvid(xvfrstpi_b, 32, vfrstpi_b)
gen_trans_vvid(xvfrstpi_h, 32, vfrstpi_h)
gen_trans_vved(xvfsqrt_d, 32, vfsqrt_d)
gen_trans_vved(xvfsqrt_s, 32, vfsqrt_s)
gen_trans_vved(xvfrint_s, 32, vfrint_s)
gen_trans_vved(xvfrint_d, 32, vfrint_d)
gen_trans_vved(xvfrintrm_s, 32, vfrintrm_s)
gen_trans_vved(xvfrintrm_d, 32, vfrintrm_d)
gen_trans_vved(xvfrintrp_s, 32, vfrintrp_s)
gen_trans_vved(xvfrintrp_d, 32, vfrintrp_d)
gen_trans_vved(xvfrintrz_s, 32, vfrintrz_s)
gen_trans_vved(xvfrintrz_d, 32, vfrintrz_d)
gen_trans_vved(xvfrintrne_s, 32, vfrintrne_s)
gen_trans_vved(xvfrintrne_d, 32, vfrintrne_d)
gen_trans_vved(xvftint_w_s, 32, vftint_w_s)
gen_trans_vved(xvftint_l_d, 32, vftint_l_d)
gen_trans_vved(xvftintrm_w_s, 32, vftintrm_w_s)
gen_trans_vved(xvftintrm_l_d, 32, vftintrm_l_d)
gen_trans_vved(xvftintrp_w_s, 32, vftintrp_w_s)
gen_trans_vved(xvftintrp_l_d, 32, vftintrp_l_d)
gen_trans_vved(xvftintrz_w_s, 32, vftintrz_w_s)
gen_trans_vved(xvftintrz_l_d, 32, vftintrz_l_d)
gen_trans_vved(xvftintrne_w_s, 32, vftintrne_w_s)
gen_trans_vved(xvftintrne_l_d, 32, vftintrne_l_d)
gen_trans_vved(xvftint_wu_s, 32, vftint_wu_s)
gen_trans_vved(xvftint_lu_d, 32, vftint_lu_d)
gen_trans_vved(xvftintrz_wu_s, 32, vftintrz_wu_s)
gen_trans_vved(xvftintrz_lu_d, 32, vftintrz_lu_d)
gen_trans_vvved(xvftint_w_d, 32, vftint_w_d)
gen_trans_vvved(xvftintrm_w_d, 32, vftintrm_w_d)
gen_trans_vvved(xvftintrp_w_d, 32, vftintrp_w_d)
gen_trans_vvved(xvftintrz_w_d, 32, vftintrz_w_d)
gen_trans_vvved(xvftintrne_w_d, 32, vftintrne_w_d)
gen_trans_vved(xvftintl_l_s, 32, vftintl_l_s)
gen_trans_vved(xvftinth_l_s, 32, vftinth_l_s)
gen_trans_vved(xvftintrml_l_s, 32, vftintrml_l_s)
gen_trans_vved(xvftintrmh_l_s, 32, vftintrmh_l_s)
gen_trans_vved(xvftintrpl_l_s, 32, vftintrpl_l_s)
gen_trans_vved(xvftintrph_l_s, 32, vftintrph_l_s)
gen_trans_vved(xvftintrzl_l_s, 32, vftintrzl_l_s)
gen_trans_vved(xvftintrzh_l_s, 32, vftintrzh_l_s)
gen_trans_vved(xvftintrnel_l_s, 32, vftintrnel_l_s)
gen_trans_vved(xvftintrneh_l_s, 32, vftintrneh_l_s)
gen_trans_vvvd(xvhaddw_du_wu, 32, vhaddw_du_wu)
gen_trans_vvvd(xvhaddw_d_w, 32, vhaddw_d_w)
gen_trans_vvvd(xvhaddw_h_b, 32, vhaddw_h_b)
gen_trans_vvvd(xvhaddw_hu_bu, 32, vhaddw_hu_bu)
gen_trans_vvvd(xvhaddw_q_d, 32, vhaddw_q_d)
gen_trans_vvvd(xvhaddw_qu_du, 32, vhaddw_qu_du)
gen_trans_vvvd(xvhaddw_w_h, 32, vhaddw_w_h)
gen_trans_vvvd(xvhaddw_wu_hu, 32, vhaddw_wu_hu)
gen_trans_vvvd(xvhsubw_du_wu, 32, vhsubw_du_wu)
gen_trans_vvvd(xvhsubw_d_w, 32, vhsubw_d_w)
gen_trans_vvvd(xvhsubw_h_b, 32, vhsubw_h_b)
gen_trans_vvvd(xvhsubw_hu_bu, 32, vhsubw_hu_bu)
gen_trans_vvvd(xvhsubw_q_d, 32, vhsubw_q_d)
gen_trans_vvvd(xvhsubw_qu_du, 32, vhsubw_qu_du)
gen_trans_vvvd(xvhsubw_w_h, 32, vhsubw_w_h)
gen_trans_vvvd(xvhsubw_wu_hu, 32, vhsubw_wu_hu)
gen_trans_vvvd(xvilvh_b, 32, vilvh_b)
gen_trans_vvvd(xvilvh_d, 32, vilvh_d)
gen_trans_vvvd(xvilvh_h, 32, vilvh_h)
gen_trans_vvvd(xvilvh_w, 32, vilvh_w)
gen_trans_vvvd(xvilvl_b, 32, vilvl_b)
gen_trans_vvvd(xvilvl_d, 32, vilvl_d)
gen_trans_vvvd(xvilvl_h, 32, vilvl_h)
gen_trans_vvvd(xvilvl_w, 32, vilvl_w)
static bool trans_xvinsve0_d(CPULoongArchState *env, arg_xvinsve0_d *restrict a) {
    CHECK_FPE(32);
    env->fpr[a->vd].vreg.D[a->imm] = env->fpr[a->vj].vreg.D[0];
    env->pc += 4;
    return true;
}
static bool trans_xvinsve0_w(CPULoongArchState *env, arg_xvinsve0_w *restrict a) {
    CHECK_FPE(32);
    env->fpr[a->vd].vreg.W[a->imm] = env->fpr[a->vj].vreg.W[0];
    env->pc += 4;
    return true;
}
static bool trans_xvld(CPULoongArchState *env, arg_xvld *restrict a) {
    CHECK_FPE(32);
    int32_t ele_cnt = 32 / 8;
    for (int32_t i = 0; i < ele_cnt; i++) {
        env->fpr[a->vd].vreg.D[i] = ld_d(env, add_addr(env->gpr[a->rj], a->imm + (i * 8)));
    }
    env->pc += 4;
    return true;
}
static bool trans_xvldrepl_b(CPULoongArchState *env, arg_xvldrepl_b *restrict a) {CHECK_FPE(32); int8_t data = ld_b(env, add_addr(env->gpr[a->rj], a->imm));for (size_t i = 0; i < 32; i++){env->fpr[a->vd].vreg.B[i] = data;}env->pc += 4;return true;}
static bool trans_xvldrepl_h(CPULoongArchState *env, arg_xvldrepl_h *restrict a) {CHECK_FPE(32); int16_t data = ld_h(env, add_addr(env->gpr[a->rj], a->imm));for (size_t i = 0; i < 16; i++){env->fpr[a->vd].vreg.H[i] = data;}env->pc += 4;return true;}
static bool trans_xvldrepl_w(CPULoongArchState *env, arg_xvldrepl_w *restrict a) {CHECK_FPE(32); int32_t data = ld_w(env, add_addr(env->gpr[a->rj], a->imm));for (size_t i = 0; i < 8; i++){env->fpr[a->vd].vreg.W[i] = data;}env->pc += 4;return true;}
static bool trans_xvldrepl_d(CPULoongArchState *env, arg_xvldrepl_d *restrict a) {CHECK_FPE(32); int64_t data = ld_d(env, add_addr(env->gpr[a->rj], a->imm));for (size_t i = 0; i < 4; i++){env->fpr[a->vd].vreg.D[i] = data;}env->pc += 4;return true;}
static bool trans_xvldx(CPULoongArchState *env, arg_xvldx *restrict a) {
    CHECK_FPE(32);
    int32_t ele_cnt = 32 / 8;
    for (int32_t i = 0; i < ele_cnt; i++) {
        env->fpr[a->vd].vreg.D[i] = ld_d(env, add_addr(env->gpr[a->rj], env->gpr[a->rk] + (i * 8)));
    }
    env->pc += 4;
    return true;
}
gen_trans_vvvd(xvmaddwev_h_b, 32, vmaddwev_h_b)
gen_trans_vvvd(xvmaddwev_w_h, 32, vmaddwev_w_h)
gen_trans_vvvd(xvmaddwev_d_w, 32, vmaddwev_d_w)
gen_trans_vvvd(xvmaddwod_h_b, 32, vmaddwod_h_b)
gen_trans_vvvd(xvmaddwod_w_h, 32, vmaddwod_w_h)
gen_trans_vvvd(xvmaddwod_d_w, 32, vmaddwod_d_w)
gen_trans_vvvd(xvmaddwev_h_bu, 32, vmaddwev_h_bu)
gen_trans_vvvd(xvmaddwev_w_hu, 32, vmaddwev_w_hu)
gen_trans_vvvd(xvmaddwev_d_wu, 32, vmaddwev_d_wu)
gen_trans_vvvd(xvmaddwod_h_bu, 32, vmaddwod_h_bu)
gen_trans_vvvd(xvmaddwod_w_hu, 32, vmaddwod_w_hu)
gen_trans_vvvd(xvmaddwod_d_wu, 32, vmaddwod_d_wu)
gen_trans_vvvd(xvmaddwev_h_bu_b, 32, vmaddwev_h_bu_b)
gen_trans_vvvd(xvmaddwev_w_hu_h, 32, vmaddwev_w_hu_h)
gen_trans_vvvd(xvmaddwev_d_wu_w, 32, vmaddwev_d_wu_w)
gen_trans_vvvd(xvmaddwod_h_bu_b, 32, vmaddwod_h_bu_b)
gen_trans_vvvd(xvmaddwod_w_hu_h, 32, vmaddwod_w_hu_h)
gen_trans_vvvd(xvmaddwod_d_wu_w, 32, vmaddwod_d_wu_w)
static bool trans_xvmaddwev_q_d(CPULoongArchState *env, arg_xvmaddwev_q_d *restrict a) {
    int size = 32;
    int index = 0;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] += (__int128_t)env->fpr[a->vj].vreg.D[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_xvmaddwod_q_d(CPULoongArchState *env, arg_xvmaddwod_q_d *restrict a) {
    int size = 32;
    int index = 1;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] += (__int128_t)env->fpr[a->vj].vreg.D[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_xvmaddwev_q_du(CPULoongArchState *env, arg_xvmaddwev_q_du *restrict a) {
    int size = 32;
    int index = 0;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] += (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__uint128_t)env->fpr[a->vk].vreg.UD[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_xvmaddwod_q_du(CPULoongArchState *env, arg_xvmaddwod_q_du *restrict a) {
    int size = 32;
    int index = 1;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] += (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__uint128_t)env->fpr[a->vk].vreg.UD[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_xvmaddwev_q_du_d(CPULoongArchState *env, arg_xvmaddwev_q_du_d *restrict a) {
    int size = 32;
    int index = 0;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] += (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_xvmaddwod_q_du_d(CPULoongArchState *env, arg_xvmaddwod_q_du_d *restrict a) {
    int size = 32;
    int index = 1;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] += (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_xvmskgez_b(CPULoongArchState *env, arg_xvmskgez_b *restrict a) {
    CHECK_FPE(32);
    int oprsz = 32;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vmskgez_b(&env->fpr[a->vd], &env->fpr[a->vj], desc);
    env->pc += 4;
    return true;
}
gen_trans_vvd(xvmskltz_b, 32, vmskltz_b)
gen_trans_vvd(xvmskltz_d, 32, vmskltz_d)
gen_trans_vvd(xvmskltz_h, 32, vmskltz_h)
gen_trans_vvd(xvmskltz_w, 32, vmskltz_w)
static bool trans_xvmsknz_b(CPULoongArchState *env, arg_xvmsknz_b *restrict a) {
    CHECK_FPE(32);
    int oprsz = 32;
    uint32_t desc = simd_desc(oprsz, oprsz, 0);
    helper_vmsknz_b(&env->fpr[a->vd], &env->fpr[a->vj], desc);
    env->pc += 4;
    return true;
}
gen_trans_vvvd(xvmuh_b, 32, vmuh_b)
gen_trans_vvvd(xvmuh_bu, 32, vmuh_bu)
gen_trans_vvvd(xvmuh_d, 32, vmuh_d)
gen_trans_vvvd(xvmuh_du, 32, vmuh_du)
gen_trans_vvvd(xvmuh_h, 32, vmuh_h)
gen_trans_vvvd(xvmuh_hu, 32, vmuh_hu)
gen_trans_vvvd(xvmuh_w, 32, vmuh_w)
gen_trans_vvvd(xvmuh_wu, 32, vmuh_wu)
gen_trans_vvvd(xvmulwev_d_w, 32, vmulwev_d_w)
gen_trans_vvvd(xvmulwev_d_wu, 32, vmulwev_d_wu)
gen_trans_vvvd(xvmulwev_d_wu_w, 32, vmulwev_d_wu_w)
gen_trans_vvvd(xvmulwev_h_b, 32, vmulwev_h_b)
gen_trans_vvvd(xvmulwev_h_bu_b, 32, vmulwev_h_bu_b)
gen_trans_vvvd(xvmulwev_h_bu, 32, vmulwev_h_bu)
gen_trans_vvvd(xvmulwev_w_h, 32, vmulwev_w_h)
gen_trans_vvvd(xvmulwev_w_hu, 32, vmulwev_w_hu)
gen_trans_vvvd(xvmulwev_w_hu_h, 32, vmulwev_w_hu_h)
gen_trans_vvvd(xvmulwod_d_w, 32, vmulwod_d_w)
gen_trans_vvvd(xvmulwod_d_wu, 32, vmulwod_d_wu)
gen_trans_vvvd(xvmulwod_d_wu_w, 32, vmulwod_d_wu_w)
gen_trans_vvvd(xvmulwod_h_b, 32, vmulwod_h_b)
gen_trans_vvvd(xvmulwod_h_bu_b, 32, vmulwod_h_bu_b)
gen_trans_vvvd(xvmulwod_h_bu, 32, vmulwod_h_bu)
gen_trans_vvvd(xvmulwod_w_h, 32, vmulwod_w_h)
gen_trans_vvvd(xvmulwod_w_hu, 32, vmulwod_w_hu)
gen_trans_vvvd(xvmulwod_w_hu_h, 32, vmulwod_w_hu_h)
static bool trans_xvmulwev_q_d(CPULoongArchState *env, arg_xvmulwev_q_d *restrict a) {
    int size = 32;
    int index = 0;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] = (__int128_t)env->fpr[a->vj].vreg.D[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_xvmulwod_q_d(CPULoongArchState *env, arg_xvmulwod_q_d *restrict a) {
    int size = 32;
    int index = 1;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] = (__int128_t)env->fpr[a->vj].vreg.D[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_xvmulwev_q_du(CPULoongArchState *env, arg_xvmulwev_q_du *restrict a) {
    int size = 32;
    int index = 0;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] = (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__uint128_t)env->fpr[a->vk].vreg.UD[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_xvmulwod_q_du(CPULoongArchState *env, arg_xvmulwod_q_du *restrict a) {
    int size = 32;
    int index = 1;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] = (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__uint128_t)env->fpr[a->vk].vreg.UD[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_xvmulwev_q_du_d(CPULoongArchState *env, arg_xvmulwev_q_du_d *restrict a) {
    int size = 32;
    int index = 0;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] = (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
static bool trans_xvmulwod_q_du_d(CPULoongArchState *env, arg_xvmulwod_q_du_d *restrict a) {
    int size = 32;
    int index = 1;
    CHECK_FPE(size);
    for (int i = 0; i < (size / 16); i++) {
        env->fpr[a->vd].vreg.Q[i] = (__uint128_t)env->fpr[a->vj].vreg.UD[i * 2 + index] * (__int128_t)env->fpr[a->vk].vreg.D[i *2 + index];
    }
    env->pc += 4;
    return true;
}
gen_trans_vvvd(xvpackev_b, 32, vpackev_b)
gen_trans_vvvd(xvpackev_h, 32, vpackev_h)
gen_trans_vvvd(xvpackev_w, 32, vpackev_w)
gen_trans_vvvd(xvpackev_d, 32, vpackev_d)
gen_trans_vvvd(xvpackod_b, 32, vpackod_b)
gen_trans_vvvd(xvpackod_h, 32, vpackod_h)
gen_trans_vvvd(xvpackod_w, 32, vpackod_w)
gen_trans_vvvd(xvpackod_d, 32, vpackod_d)
gen_trans_vvid(xvpermi_d, 32, vpermi_d)
gen_trans_vvid(xvpermi_w, 32, vpermi_w)
gen_trans_vvid(xvpermi_q, 32, vpermi_q)
gen_trans_vvvd(xvperm_w, 32, vperm_w)
gen_trans_vvid(xvpickve_d, 32, xvpickve_d)
gen_trans_vvid(xvpickve_w, 32, xvpickve_w)
static bool trans_xvrepl128vei_b(CPULoongArchState *env, arg_xvrepl128vei_b *restrict a) {CHECK_FPE(32); uint32_t ele_cnt = 16 / 1; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[a->imm]; env->fpr[a->vd].vreg.B[i + ele_cnt] = env->fpr[a->vj].vreg.B[a->imm + ele_cnt]; } env->pc += 4; return true;}
static bool trans_xvrepl128vei_h(CPULoongArchState *env, arg_xvrepl128vei_h *restrict a) {CHECK_FPE(32); uint32_t ele_cnt = 16 / 2; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[a->imm]; env->fpr[a->vd].vreg.H[i + ele_cnt] = env->fpr[a->vj].vreg.H[a->imm + ele_cnt]; } env->pc += 4; return true;}
static bool trans_xvrepl128vei_w(CPULoongArchState *env, arg_xvrepl128vei_w *restrict a) {CHECK_FPE(32); uint32_t ele_cnt = 16 / 4; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[a->imm]; env->fpr[a->vd].vreg.W[i + ele_cnt] = env->fpr[a->vj].vreg.W[a->imm + ele_cnt]; } env->pc += 4; return true;}
static bool trans_xvrepl128vei_d(CPULoongArchState *env, arg_xvrepl128vei_d *restrict a) {CHECK_FPE(32); uint32_t ele_cnt = 16 / 8; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[a->imm]; env->fpr[a->vd].vreg.D[i + ele_cnt] = env->fpr[a->vj].vreg.D[a->imm + ele_cnt]; } env->pc += 4; return true;}
static bool trans_xvreplve0_b(CPULoongArchState *env, arg_xvreplve0_b *restrict a) {CHECK_FPE(32); int32_t ele_cnt = 32 / 1; for (int32_t i = 0; i < ele_cnt; i++) { env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[0]; } env->pc += 4; return true;}
static bool trans_xvreplve0_h(CPULoongArchState *env, arg_xvreplve0_h *restrict a) {CHECK_FPE(32); int32_t ele_cnt = 32 / 2; for (int32_t i = 0; i < ele_cnt; i++) { env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[0]; } env->pc += 4; return true;}
static bool trans_xvreplve0_w(CPULoongArchState *env, arg_xvreplve0_w *restrict a) {CHECK_FPE(32); int32_t ele_cnt = 32 / 4; for (int32_t i = 0; i < ele_cnt; i++) { env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[0]; } env->pc += 4; return true;}
static bool trans_xvreplve0_d(CPULoongArchState *env, arg_xvreplve0_d *restrict a) {CHECK_FPE(32); int32_t ele_cnt = 32 / 8; for (int32_t i = 0; i < ele_cnt; i++) { env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[0]; } env->pc += 4; return true;}
static bool trans_xvreplve0_q(CPULoongArchState *env, arg_xvreplve0_q *restrict a) {
    CHECK_FPE(32);
    env->fpr[a->vd].vreg.D[0] = env->fpr[a->vj].vreg.D[0];
    env->fpr[a->vd].vreg.D[1] = env->fpr[a->vj].vreg.D[1];
    env->fpr[a->vd].vreg.D[2] = env->fpr[a->vj].vreg.D[0];
    env->fpr[a->vd].vreg.D[3] = env->fpr[a->vj].vreg.D[1];
    env->pc += 4;
    return true;
}
static bool trans_xvreplve_b(CPULoongArchState *env, arg_xvreplve_b *restrict a) {CHECK_FPE(32); int32_t ele_cnt = 16 / 1; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.B[i] = env->fpr[a->vj].vreg.B[env->gpr[a->rk] & (ele_cnt - 1)]; env->fpr[a->vd].vreg.B[i + ele_cnt] = env->fpr[a->vj].vreg.B[(env->gpr[a->rk] & (ele_cnt - 1)) + ele_cnt]; } env->pc += 4; return true;}
static bool trans_xvreplve_h(CPULoongArchState *env, arg_xvreplve_h *restrict a) {CHECK_FPE(32); int32_t ele_cnt = 16 / 2; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.H[i] = env->fpr[a->vj].vreg.H[env->gpr[a->rk] & (ele_cnt - 1)]; env->fpr[a->vd].vreg.H[i + ele_cnt] = env->fpr[a->vj].vreg.H[(env->gpr[a->rk] & (ele_cnt - 1)) + ele_cnt]; } env->pc += 4; return true;}
static bool trans_xvreplve_w(CPULoongArchState *env, arg_xvreplve_w *restrict a) {CHECK_FPE(32); int32_t ele_cnt = 16 / 4; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.W[i] = env->fpr[a->vj].vreg.W[env->gpr[a->rk] & (ele_cnt - 1)]; env->fpr[a->vd].vreg.W[i + ele_cnt] = env->fpr[a->vj].vreg.W[(env->gpr[a->rk] & (ele_cnt - 1)) + ele_cnt]; } env->pc += 4; return true;}
static bool trans_xvreplve_d(CPULoongArchState *env, arg_xvreplve_d *restrict a) {CHECK_FPE(32); int32_t ele_cnt = 16 / 8; for (int i = 0; i < ele_cnt; i ++) { env->fpr[a->vd].vreg.D[i] = env->fpr[a->vj].vreg.D[env->gpr[a->rk] & (ele_cnt - 1)]; env->fpr[a->vd].vreg.D[i + ele_cnt] = env->fpr[a->vj].vreg.D[(env->gpr[a->rk] & (ele_cnt - 1)) + ele_cnt]; } env->pc += 4; return true;}
gen_trans_vvvd(xvrotr_b, 32, gvec_rotr8v)
gen_trans_vvvd(xvrotr_h, 32, gvec_rotr16v)
gen_trans_vvvd(xvrotr_w, 32, gvec_rotr32v)
gen_trans_vvvd(xvrotr_d, 32, gvec_rotr64v)
static bool trans_xvrotri_b(CPULoongArchState *env, arg_xvrotri_b *restrict a) {
    int size = 32;
    CHECK_FPE(size);
    int oprsz = size;
    uint32_t desc = simd_desc(oprsz, oprsz, 8 - a->imm);
    helper_gvec_rotl8i(&env->fpr[a->vd], &env->fpr[a->vj], desc);
    env->pc += 4;
    return true;
}
static bool trans_xvrotri_h(CPULoongArchState *env, arg_xvrotri_h *restrict a) {
    int size = 32;
    CHECK_FPE(size);
    int oprsz = size;
    uint32_t desc = simd_desc(oprsz, oprsz, 16 - a->imm);
    helper_gvec_rotl16i(&env->fpr[a->vd], &env->fpr[a->vj], desc);
    env->pc += 4;
    return true;
}
static bool trans_xvrotri_w(CPULoongArchState *env, arg_xvrotri_w *restrict a) {
    int size = 32;
    CHECK_FPE(size);
    int oprsz = size;
    uint32_t desc = simd_desc(oprsz, oprsz, 32 - a->imm);
    helper_gvec_rotl32i(&env->fpr[a->vd], &env->fpr[a->vj], desc);
    env->pc += 4;
    return true;
}
static bool trans_xvrotri_d(CPULoongArchState *env, arg_xvrotri_d *restrict a) {
    int size = 32;
    CHECK_FPE(size);
    int oprsz = size;
    uint32_t desc = simd_desc(oprsz, oprsz, 64 - a->imm);
    helper_gvec_rotl64i(&env->fpr[a->vd], &env->fpr[a->vj], desc);
    env->pc += 4;
    return true;
}
gen_trans_vvvd(xvsadd_b, 32, gvec_ssadd8)
gen_trans_vvvd(xvsadd_h, 32, gvec_ssadd16)
gen_trans_vvvd(xvsadd_w, 32, gvec_ssadd32)
gen_trans_vvvd(xvsadd_d, 32, gvec_ssadd64)
gen_trans_vvvd(xvsadd_bu, 32, gvec_usadd8)
gen_trans_vvvd(xvsadd_hu, 32, gvec_usadd16)
gen_trans_vvvd(xvsadd_wu, 32, gvec_usadd32)
gen_trans_vvvd(xvsadd_du, 32, gvec_usadd64)
gen_trans_vvid_sat(xvsat_b, 32, vsat_b)
gen_trans_vvid_sat(xvsat_h, 32, vsat_h)
gen_trans_vvid_sat(xvsat_w, 32, vsat_w)
gen_trans_vvid_sat(xvsat_d, 32, vsat_d)
gen_trans_vvid_satu(xvsat_bu, 32, vsat_bu)
gen_trans_vvid_satu(xvsat_hu, 32, vsat_hu)
gen_trans_vvid_satu(xvsat_wu, 32, vsat_wu)
gen_trans_vvid_satu(xvsat_du, 32, vsat_du)
static bool trans_xvseteqz_v(CPULoongArchState *env, arg_xvseteqz_v *restrict a) {
    CHECK_FPE(32);
    uint32_t ele_cnt = 32 / 8;
    bool r = 1;
    for (uint32_t i = 0; i < ele_cnt; i++) {
        r &= (env->fpr[a->vj].vreg.D[i] == 0);
    }
    env->cf[a->cd] = r;
    env->pc += 4;
    return true;
}
static bool trans_xvsetnez_v(CPULoongArchState *env, arg_xvsetnez_v *restrict a) {
    CHECK_FPE(32);
    uint32_t ele_cnt = 32 / 8;
    bool r = 0;
    for (uint32_t i = 0; i < ele_cnt; i++) {
        r |= (env->fpr[a->vj].vreg.D[i] != 0);
    }
    env->cf[a->cd] = r;
    env->pc += 4;
    return true;
}
gen_trans_vvid(xvshuf4i_b, 32, vshuf4i_b)
gen_trans_vvid(xvshuf4i_d, 32, vshuf4i_d)
gen_trans_vvid(xvshuf4i_h, 32, vshuf4i_h)
gen_trans_vvid(xvshuf4i_w, 32, vshuf4i_w)
gen_trans_vvvvd(xvshuf_b, 32, vshuf_b)
gen_trans_vvvd(xvshuf_h, 32, vshuf_h)
gen_trans_vvvd(xvshuf_w, 32, vshuf_w)
gen_trans_vvvd(xvshuf_d, 32, vshuf_d)
gen_trans_vvvd(xvsigncov_b, 32, vsigncov_b)
gen_trans_vvvd(xvsigncov_h, 32, vsigncov_h)
gen_trans_vvvd(xvsigncov_w, 32, vsigncov_w)
gen_trans_vvvd(xvsigncov_d, 32, vsigncov_d)
gen_trans_vvid(xvsllwil_du_wu, 32, vsllwil_du_wu)
gen_trans_vvid(xvsllwil_d_w, 32, vsllwil_d_w)
gen_trans_vvid(xvsllwil_h_b, 32, vsllwil_h_b)
gen_trans_vvid(xvsllwil_hu_bu, 32, vsllwil_hu_bu)
gen_trans_vvid(xvsllwil_w_h, 32, vsllwil_w_h)
gen_trans_vvid(xvsllwil_wu_hu, 32, vsllwil_wu_hu)
gen_trans_vvvd(xvsran_b_h, 32, vsran_b_h)
gen_trans_vvvd(xvsran_h_w, 32, vsran_h_w)
gen_trans_vvid(xvsrani_b_h, 32, vsrani_b_h)
gen_trans_vvid(xvsrani_d_q, 32, vsrani_d_q)
gen_trans_vvid(xvsrani_h_w, 32, vsrani_h_w)
gen_trans_vvid(xvsrani_w_d, 32, vsrani_w_d)
gen_trans_vvvd(xvsran_w_d, 32, vsran_w_d)
gen_trans_vvvd(xvsrar_b, 32, vsrar_b)
gen_trans_vvvd(xvsrar_d, 32, vsrar_d)
gen_trans_vvvd(xvsrar_h, 32, vsrar_h)
gen_trans_vvid(xvsrari_b, 32, vsrari_b)
gen_trans_vvid(xvsrari_d, 32, vsrari_d)
gen_trans_vvid(xvsrari_h, 32, vsrari_h)
gen_trans_vvid(xvsrari_w, 32, vsrari_w)
gen_trans_vvvd(xvsrarn_b_h, 32, vsrarn_b_h)
gen_trans_vvvd(xvsrarn_h_w, 32, vsrarn_h_w)
gen_trans_vvid(xvsrarni_b_h, 32, vsrarni_b_h)
gen_trans_vvid(xvsrarni_d_q, 32, vsrarni_d_q)
gen_trans_vvid(xvsrarni_h_w, 32, vsrarni_h_w)
gen_trans_vvid(xvsrarni_w_d, 32, vsrarni_w_d)
gen_trans_vvvd(xvsrarn_w_d, 32, vsrarn_w_d)
gen_trans_vvvd(xvsrar_w, 32, vsrar_w)
gen_trans_vvvd(xvsrln_b_h, 32, vsrln_b_h)
gen_trans_vvvd(xvsrln_h_w, 32, vsrln_h_w)
gen_trans_vvid(xvsrlni_b_h, 32, vsrlni_b_h)
gen_trans_vvid(xvsrlni_d_q, 32, vsrlni_d_q)
gen_trans_vvid(xvsrlni_h_w, 32, vsrlni_h_w)
gen_trans_vvid(xvsrlni_w_d, 32, vsrlni_w_d)
gen_trans_vvvd(xvsrln_w_d, 32, vsrln_w_d)
gen_trans_vvvd(xvsrlr_b, 32, vsrlr_b)
gen_trans_vvvd(xvsrlr_d, 32, vsrlr_d)
gen_trans_vvvd(xvsrlr_h, 32, vsrlr_h)
gen_trans_vvid(xvsrlri_b, 32, vsrlri_b)
gen_trans_vvid(xvsrlri_d, 32, vsrlri_d)
gen_trans_vvid(xvsrlri_h, 32, vsrlri_h)
gen_trans_vvid(xvsrlri_w, 32, vsrlri_w)
gen_trans_vvvd(xvsrlrn_b_h, 32, vsrlrn_b_h)
gen_trans_vvvd(xvsrlrn_h_w, 32, vsrlrn_h_w)
gen_trans_vvid(xvsrlrni_b_h, 32, vsrlrni_b_h)
gen_trans_vvid(xvsrlrni_d_q, 32, vsrlrni_d_q)
gen_trans_vvid(xvsrlrni_h_w, 32, vsrlrni_h_w)
gen_trans_vvid(xvsrlrni_w_d, 32, vsrlrni_w_d)
gen_trans_vvvd(xvsrlrn_w_d, 32, vsrlrn_w_d)
gen_trans_vvvd(xvsrlr_w, 32, vsrlr_w)
gen_trans_vvvd(xvssran_b_h, 32, vssran_b_h)
gen_trans_vvvd(xvssran_bu_h, 32, vssran_bu_h)
gen_trans_vvvd(xvssran_hu_w, 32, vssran_hu_w)
gen_trans_vvvd(xvssran_h_w, 32, vssran_h_w)
gen_trans_vvid(xvssrani_b_h, 32, vssrani_b_h)
gen_trans_vvid(xvssrani_bu_h, 32, vssrani_bu_h)
gen_trans_vvid(xvssrani_d_q, 32, vssrani_d_q)
gen_trans_vvid(xvssrani_du_q, 32, vssrani_du_q)
gen_trans_vvid(xvssrani_hu_w, 32, vssrani_hu_w)
gen_trans_vvid(xvssrani_h_w, 32, vssrani_h_w)
gen_trans_vvid(xvssrani_w_d, 32, vssrani_w_d)
gen_trans_vvid(xvssrani_wu_d, 32, vssrani_wu_d)
gen_trans_vvvd(xvssran_w_d, 32, vssran_w_d)
gen_trans_vvvd(xvssran_wu_d, 32, vssran_wu_d)
gen_trans_vvvd(xvssrarn_b_h, 32, vssrarn_b_h)
gen_trans_vvvd(xvssrarn_bu_h, 32, vssrarn_bu_h)
gen_trans_vvvd(xvssrarn_hu_w, 32, vssrarn_hu_w)
gen_trans_vvvd(xvssrarn_h_w, 32, vssrarn_h_w)
gen_trans_vvid(xvssrarni_b_h, 32, vssrarni_b_h)
gen_trans_vvid(xvssrarni_bu_h, 32, vssrarni_bu_h)
gen_trans_vvid(xvssrarni_d_q, 32, vssrarni_d_q)
gen_trans_vvid(xvssrarni_du_q, 32, vssrarni_du_q)
gen_trans_vvid(xvssrarni_hu_w, 32, vssrarni_hu_w)
gen_trans_vvid(xvssrarni_h_w, 32, vssrarni_h_w)
gen_trans_vvid(xvssrarni_w_d, 32, vssrarni_w_d)
gen_trans_vvid(xvssrarni_wu_d, 32, vssrarni_wu_d)
gen_trans_vvvd(xvssrarn_w_d, 32, vssrarn_w_d)
gen_trans_vvvd(xvssrarn_wu_d, 32, vssrarn_wu_d)
gen_trans_vvvd(xvssrln_b_h, 32, vssrln_b_h)
gen_trans_vvvd(xvssrln_bu_h, 32, vssrln_bu_h)
gen_trans_vvvd(xvssrln_hu_w, 32, vssrln_hu_w)
gen_trans_vvvd(xvssrln_h_w, 32, vssrln_h_w)
gen_trans_vvid(xvssrlni_b_h, 32, vssrlni_b_h)
gen_trans_vvid(xvssrlni_bu_h, 32, vssrlni_bu_h)
gen_trans_vvid(xvssrlni_d_q, 32, vssrlni_d_q)
gen_trans_vvid(xvssrlni_du_q, 32, vssrlni_du_q)
gen_trans_vvid(xvssrlni_hu_w, 32, vssrlni_hu_w)
gen_trans_vvid(xvssrlni_h_w, 32, vssrlni_h_w)
gen_trans_vvid(xvssrlni_w_d, 32, vssrlni_w_d)
gen_trans_vvid(xvssrlni_wu_d, 32, vssrlni_wu_d)
gen_trans_vvvd(xvssrln_w_d, 32, vssrln_w_d)
gen_trans_vvvd(xvssrln_wu_d, 32, vssrln_wu_d)
gen_trans_vvvd(xvssrlrn_b_h, 32, vssrlrn_b_h)
gen_trans_vvvd(xvssrlrn_bu_h, 32, vssrlrn_bu_h)
gen_trans_vvvd(xvssrlrn_hu_w, 32, vssrlrn_hu_w)
gen_trans_vvvd(xvssrlrn_h_w, 32, vssrlrn_h_w)
gen_trans_vvid(xvssrlrni_b_h, 32, vssrlrni_b_h)
gen_trans_vvid(xvssrlrni_bu_h, 32, vssrlrni_bu_h)
gen_trans_vvid(xvssrlrni_d_q, 32, vssrlrni_d_q)
gen_trans_vvid(xvssrlrni_du_q, 32, vssrlrni_du_q)
gen_trans_vvid(xvssrlrni_hu_w, 32, vssrlrni_hu_w)
gen_trans_vvid(xvssrlrni_h_w, 32, vssrlrni_h_w)
gen_trans_vvid(xvssrlrni_w_d, 32, vssrlrni_w_d)
gen_trans_vvid(xvssrlrni_wu_d, 32, vssrlrni_wu_d)
gen_trans_vvvd(xvssrlrn_w_d, 32, vssrlrn_w_d)
gen_trans_vvvd(xvssrlrn_wu_d, 32, vssrlrn_wu_d)
gen_trans_vvvd(xvssub_b, 32, gvec_sssub8)
gen_trans_vvvd(xvssub_h, 32, gvec_sssub16)
gen_trans_vvvd(xvssub_w, 32, gvec_sssub32)
gen_trans_vvvd(xvssub_d, 32, gvec_sssub64)
gen_trans_vvvd(xvssub_bu, 32, gvec_ussub8)
gen_trans_vvvd(xvssub_hu, 32, gvec_ussub16)
gen_trans_vvvd(xvssub_wu, 32, gvec_ussub32)
gen_trans_vvvd(xvssub_du, 32, gvec_ussub64)
static bool trans_xvst(CPULoongArchState *env, arg_xvst *restrict a) {
    CHECK_FPE(32);
    int32_t ele_cnt = 32 / 8;
    for (int32_t i = 0; i < ele_cnt; i++) {
        st_d(env, add_addr(env->gpr[a->rj], a->imm + (i * 8)), env->fpr[a->vd].vreg.D[i]);
    }
    env->pc += 4;
    return true;
}
static bool trans_xvstelm_b(CPULoongArchState *env, arg_xvstelm_b *restrict a) {CHECK_FPE(32); st_b(env, add_addr(env->gpr[a->rj], a->imm), env->fpr[a->vd].vreg.B[a->imm2]);env->pc += 4;return true;}
static bool trans_xvstelm_h(CPULoongArchState *env, arg_xvstelm_h *restrict a) {CHECK_FPE(32); st_h(env, add_addr(env->gpr[a->rj], a->imm), env->fpr[a->vd].vreg.H[a->imm2]);env->pc += 4;return true;}
static bool trans_xvstelm_w(CPULoongArchState *env, arg_xvstelm_w *restrict a) {CHECK_FPE(32); st_w(env, add_addr(env->gpr[a->rj], a->imm), env->fpr[a->vd].vreg.W[a->imm2]);env->pc += 4;return true;}
static bool trans_xvstelm_d(CPULoongArchState *env, arg_xvstelm_d *restrict a) {CHECK_FPE(32); st_d(env, add_addr(env->gpr[a->rj], a->imm), env->fpr[a->vd].vreg.D[a->imm2]);env->pc += 4;return true;}
static bool trans_xvstx(CPULoongArchState *env, arg_xvstx *restrict a) {
    CHECK_FPE(32);
    int32_t ele_cnt = 32 / 8;
    for (int32_t i = 0; i < ele_cnt; i++) {
        st_d(env, add_addr(env->gpr[a->rj], env->gpr[a->rk] + (i * 8)), env->fpr[a->vd].vreg.D[i]);
    }
    env->pc += 4;
    return true;
}
static bool trans_xvsub_q(CPULoongArchState *env, arg_xvsub_q *restrict a) {
    CHECK_FPE(32);
    env->fpr[a->vd].vreg.Q[0] = env->fpr[a->vj].vreg.Q[0] - env->fpr[a->vk].vreg.Q[0];
    env->fpr[a->vd].vreg.Q[1] = env->fpr[a->vj].vreg.Q[1] - env->fpr[a->vk].vreg.Q[1];
    env->pc += 4;
    return true;
}

gen_trans_vvvd(xvsubwev_d_w, 32, vsubwev_d_w)
gen_trans_vvvd(xvsubwev_d_wu, 32, vsubwev_d_wu)
gen_trans_vvvd(xvsubwev_h_b, 32, vsubwev_h_b)
gen_trans_vvvd(xvsubwev_h_bu, 32, vsubwev_h_bu)
gen_trans_vvvd(xvsubwev_q_d, 32, vsubwev_q_d)
gen_trans_vvvd(xvsubwev_q_du, 32, vsubwev_q_du)
gen_trans_vvvd(xvsubwev_w_h, 32, vsubwev_w_h)
gen_trans_vvvd(xvsubwev_w_hu, 32, vsubwev_w_hu)
gen_trans_vvvd(xvsubwod_d_w, 32, vsubwod_d_w)
gen_trans_vvvd(xvsubwod_d_wu, 32, vsubwod_d_wu)
gen_trans_vvvd(xvsubwod_h_b, 32, vsubwod_h_b)
gen_trans_vvvd(xvsubwod_h_bu, 32, vsubwod_h_bu)
gen_trans_vvvd(xvsubwod_q_d, 32, vsubwod_q_d)
gen_trans_vvvd(xvsubwod_q_du, 32, vsubwod_q_du)
gen_trans_vvvd(xvsubwod_w_h, 32, vsubwod_w_h)
gen_trans_vvvd(xvsubwod_w_hu, 32, vsubwod_w_hu)

static inline bool insn_match_bits(uint32_t insn, uint32_t value, uint32_t mask)
{
    return (insn & mask) == value;
}

static bool lsx_insn_writes_vr(uint32_t insn)
{
    if (insn_match_bits(insn, 0x30100000, 0xfff80000) ||
        insn_match_bits(insn, 0x30200000, 0xfff00000) ||
        insn_match_bits(insn, 0x30400000, 0xffe00000) ||
        insn_match_bits(insn, 0x30800000, 0xffc00000)) {
        return true; /* QEMU vldrepl.* gvec forms clear the LASX tail. */
    }

    if (insn_match_bits(insn, 0x0d100000, 0xfff00000) ||
        insn_match_bits(insn, 0x70000000, 0xfff00000) ||
        insn_match_bits(insn, 0x70100000, 0xffff8000) ||
        insn_match_bits(insn, 0x70700000, 0xfff00000) ||
        insn_match_bits(insn, 0x70e00000, 0xfff00000) ||
        insn_match_bits(insn, 0x710e8000, 0xffff8000) ||
        insn_match_bits(insn, 0x710f0000, 0xffff8000) ||
        insn_match_bits(insn, 0x710f8000, 0xffff8000) ||
        insn_match_bits(insn, 0x72800000, 0xfff80000) ||
        insn_match_bits(insn, 0x72880000, 0xfffe0000) ||
        insn_match_bits(insn, 0x728a0000, 0xfffe0000) ||
        insn_match_bits(insn, 0x728c0000, 0xfffe0000) ||
        insn_match_bits(insn, 0x72900000, 0xfff80000) ||
        insn_match_bits(insn, 0x71260000, 0xffff8000) ||
        insn_match_bits(insn, 0x71268000, 0xffff8000) ||
        insn_match_bits(insn, 0x71270000, 0xffff8000) ||
        insn_match_bits(insn, 0x71278000, 0xffff8000) ||
        insn_match_bits(insn, 0x71280000, 0xffff8000) ||
        insn_match_bits(insn, 0x71288000, 0xffff8000) ||
        insn_match_bits(insn, 0x0d500000, 0xfff00000) ||
        insn_match_bits(insn, 0x717a8000, 0xffff8000) ||
        insn_match_bits(insn, 0x717b0000, 0xffff8000) ||
        insn_match_bits(insn, 0x717b8000, 0xffff8000) ||
        insn_match_bits(insn, 0x73100000, 0xfff00000) ||
        insn_match_bits(insn, 0x73200000, 0xfff00000) ||
        insn_match_bits(insn, 0x73300000, 0xfff00000) ||
        insn_match_bits(insn, 0x72a02000, 0xffffe000) ||
        insn_match_bits(insn, 0x72a04000, 0xffffc000) ||
        insn_match_bits(insn, 0x72a08000, 0xffff8000) ||
        insn_match_bits(insn, 0x72a10000, 0xffff0000) ||
        insn_match_bits(insn, 0x72f78000, 0xffffc000) ||
        insn_match_bits(insn, 0x72f7c000, 0xffffe000) ||
        insn_match_bits(insn, 0x72f7e000, 0xfffff000) ||
        insn_match_bits(insn, 0x72f7f000, 0xfffff800) ||
        insn_match_bits(insn, 0x73c40000, 0xfffc0000) ||
        insn_match_bits(insn, 0x73d00000, 0xfff00000) ||
        insn_match_bits(insn, 0x73e00000, 0xfffc0000)) {
        return true; /* QEMU gvec LSX forms clear the LASX tail. */
    }

    return false;
}
