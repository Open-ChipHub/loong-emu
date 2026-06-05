/* Integer, memory, branch, CSR, and privileged instruction handlers.
 * Included by interpreter.c; do not compile this file separately.
 */

static bool trans_add_w(CPULoongArchState *env, arg_add_w *restrict a) {
    env->gpr[a->rd] = (int64_t)(int32_t)(env->gpr[a->rj] + env->gpr[a->rk]);
    env->pc += 4;
    return true;
}
static bool trans_add_d(CPULoongArchState *env, arg_add_d *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] + env->gpr[a->rk];
    env->pc += 4;
    return true;
}
static bool trans_sub_w(CPULoongArchState *env, arg_sub_w *restrict a) {
    env->gpr[a->rd] = (int64_t)(int32_t)(env->gpr[a->rj] - env->gpr[a->rk]);
    env->pc += 4;
    return true;
}
static bool trans_sub_d(CPULoongArchState *env, arg_sub_d *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] - env->gpr[a->rk];
    env->pc += 4;
    return true;
}
static bool trans_slt(CPULoongArchState *env, arg_slt *restrict a) {
    if (is_la64(env)) {
        env->gpr[a->rd] = (int64_t)env->gpr[a->rj] < (int64_t)env->gpr[a->rk];
    } else {
        env->gpr[a->rd] = (int32_t)env->gpr[a->rj] < (int32_t)env->gpr[a->rk];
    }
    env->pc += 4;
    return true;
}
static bool trans_sltu(CPULoongArchState *env, arg_sltu *restrict a) {
    if (is_la64(env)) {
        env->gpr[a->rd] = (uint64_t)env->gpr[a->rj] < (uint64_t)env->gpr[a->rk];
    } else {
        env->gpr[a->rd] = (uint32_t)env->gpr[a->rj] < (uint32_t)env->gpr[a->rk];
    }
    env->pc += 4;
    return true;
}
static bool trans_slti(CPULoongArchState *env, arg_slti *restrict a) {
    if (is_la64(env)) {
        env->gpr[a->rd] = (int64_t)env->gpr[a->rj] < (int64_t)a->imm;
    } else {
        env->gpr[a->rd] = (int32_t)env->gpr[a->rj] < (int32_t)a->imm;
    }
    env->pc += 4;
    return true;
}
static bool trans_sltui(CPULoongArchState *env, arg_sltui *restrict a) {
    if (is_la64(env)) {
        env->gpr[a->rd] = (uint64_t)env->gpr[a->rj] < (uint64_t)(int64_t)a->imm;
    } else {
        env->gpr[a->rd] = (uint32_t)env->gpr[a->rj] < (uint32_t)a->imm;
    }
    env->pc += 4;
    return true;
}
static bool trans_nor(CPULoongArchState *env, arg_nor *restrict a) {
    env->gpr[a->rd] = ~(env->gpr[a->rj] | env->gpr[a->rk]);
    env->pc += 4;
    return true;
}
static bool trans_and(CPULoongArchState *env, arg_and *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] & env->gpr[a->rk];
    env->pc += 4;
    return true;
}
static bool trans_or(CPULoongArchState *env, arg_or *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] | env->gpr[a->rk];
    env->pc += 4;
    return true;
}
static bool trans_xor(CPULoongArchState *env, arg_xor *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] ^ env->gpr[a->rk];
    env->pc += 4;
    return true;
}

static bool trans_orn(CPULoongArchState *env, arg_orn *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] | (~ env->gpr[a->rk]);
    env->pc += 4;
    return true;
}

static bool trans_andn(CPULoongArchState *env, arg_andn *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] & (~ env->gpr[a->rk]);
    env->pc += 4;
    return true;
}

static bool trans_mul_w(CPULoongArchState *env, arg_mul_w *restrict a) {
    env->gpr[a->rd] = (int64_t)((int32_t)env->gpr[a->rj] * (int32_t)env->gpr[a->rk]);
    env->pc += 4;
    return true;
}
static bool trans_mulh_w(CPULoongArchState *env, arg_mulh_w *restrict a) {
    int64_t data = ((int64_t)(int32_t)env->gpr[a->rj] * (int64_t)(int32_t)env->gpr[a->rk]) >> 32;
    env->gpr[a->rd] = data;
    env->pc += 4;
    return true;
}
static bool trans_mulh_wu(CPULoongArchState *env, arg_mulh_wu *restrict a) {
    int64_t data = ((int64_t)((uint64_t)(uint32_t)env->gpr[a->rj] * (uint64_t)(uint32_t)env->gpr[a->rk])) >> 32;
    env->gpr[a->rd] = data;
    env->pc += 4;
    return true;
}
static bool trans_mul_d(CPULoongArchState *env, arg_mul_d *restrict a) {
    env->gpr[a->rd] = (int64_t)env->gpr[a->rj] * (int64_t)env->gpr[a->rk];
    env->pc += 4;
    return true;
}
static bool trans_mulh_d(CPULoongArchState *env, arg_mulh_d *restrict a) {
    uint64_t high,low;
    muls64(&low, &high, env->gpr[a->rj], env->gpr[a->rk]);
    env->gpr[a->rd] = high;
    env->pc += 4;
    return true;
}
static bool trans_mulh_du(CPULoongArchState *env, arg_mulh_du *restrict a) {
    uint64_t high,low;
    mulu64(&low, &high, env->gpr[a->rj], env->gpr[a->rk]);
    env->gpr[a->rd] = high;
    env->pc += 4;
    return true;
}
static bool trans_mulw_d_w(CPULoongArchState *env, arg_mulw_d_w *restrict a) {
    env->gpr[a->rd] = (int64_t)(int32_t)env->gpr[a->rj] * (int64_t)(int32_t)env->gpr[a->rk];
    env->pc += 4;
    return true;
}
static bool trans_mulw_d_wu(CPULoongArchState *env, arg_mulw_d_wu *restrict a) {
    env->gpr[a->rd] = (uint64_t)(uint32_t)env->gpr[a->rj] * (uint64_t)(uint32_t)env->gpr[a->rk];
    env->pc += 4;
    return true;
}
static bool trans_div_w(CPULoongArchState *env, arg_div_w *restrict a) {
    int32_t rj = (int32_t)env->gpr[a->rj], rk = (int32_t)env->gpr[a->rk];
    if (rk == 0) env->gpr[a->rd] = (int64_t)rj;
    else if (rj == INT32_MIN && rk == -1) env->gpr[a->rd] = (int64_t)(int32_t)rj;
    else env->gpr[a->rd] = (int64_t)(int32_t)(rj / rk);
    env->pc += 4;
    return true;
}
static bool trans_mod_w(CPULoongArchState *env, arg_mod_w *restrict a) {
    int32_t rj = (int32_t)env->gpr[a->rj], rk = (int32_t)env->gpr[a->rk];
    if (rk == 0) env->gpr[a->rd] = 0;
    else if (rj == INT32_MIN && rk == -1) env->gpr[a->rd] = 0;
    else env->gpr[a->rd] = (int64_t)(int32_t)(rj % rk);
    env->pc += 4;
    return true;
}
static bool trans_div_wu(CPULoongArchState *env, arg_div_wu *restrict a) {
    uint32_t rj = (uint32_t)env->gpr[a->rj], rk = (uint32_t)env->gpr[a->rk];
    env->gpr[a->rd] = (int64_t)(int32_t)(rk ? rj / rk : rj);
    env->pc += 4;
    return true;
}
static bool trans_mod_wu(CPULoongArchState *env, arg_mod_wu *restrict a) {
    uint32_t rj = (uint32_t)env->gpr[a->rj], rk = (uint32_t)env->gpr[a->rk];
    env->gpr[a->rd] = rk ? (int64_t)(int32_t)(rj % rk) : 0;
    env->pc += 4;
    return true;
}
static bool trans_div_d(CPULoongArchState *env, arg_div_d *restrict a) {
    int64_t rj = (int64_t)env->gpr[a->rj], rk = (int64_t)env->gpr[a->rk];
    if (rk == 0) env->gpr[a->rd] = rj;
    else if (rj == INT64_MIN && rk == -1) env->gpr[a->rd] = rj;
    else env->gpr[a->rd] = rj / rk;
    env->pc += 4;
    return true;
}
static bool trans_mod_d(CPULoongArchState *env, arg_mod_d *restrict a) {
    int64_t rj = (int64_t)env->gpr[a->rj], rk = (int64_t)env->gpr[a->rk];
    if (rk == 0) env->gpr[a->rd] = 0;
    else if (rj == INT64_MIN && rk == -1) env->gpr[a->rd] = 0;
    else env->gpr[a->rd] = rj % rk;
    env->pc += 4;
    return true;
}
static bool trans_div_du(CPULoongArchState *env, arg_div_du *restrict a) {
    uint64_t rj = env->gpr[a->rj], rk = env->gpr[a->rk];
    env->gpr[a->rd] = rk ? rj / rk : rj;
    env->pc += 4;
    return true;
}
static bool trans_mod_du(CPULoongArchState *env, arg_mod_du *restrict a) {
    uint64_t rj = env->gpr[a->rj], rk = env->gpr[a->rk];
    env->gpr[a->rd] = rk ? rj % rk : 0;
    env->pc += 4;
    return true;
}
static bool trans_alsl_w(CPULoongArchState *env, arg_alsl_w *restrict a) {
    env->gpr[a->rd] = (int64_t)(int32_t)((env->gpr[a->rj] << a->sa) + env->gpr[a->rk]);
    env->pc += 4;
    return true;
}
static bool trans_alsl_wu(CPULoongArchState *env, arg_alsl_wu *restrict a) {
    env->gpr[a->rd] = (uint32_t)((env->gpr[a->rj] << a->sa) + env->gpr[a->rk]);
    env->pc += 4;
    return true;
}
static bool trans_alsl_d(CPULoongArchState *env, arg_alsl_d *restrict a) {
    env->gpr[a->rd] = (env->gpr[a->rj] << a->sa) + env->gpr[a->rk];
    env->pc += 4;
    return true;
}
static bool trans_lu12i_w(CPULoongArchState *env, arg_lu12i_w *restrict a) {
    env->gpr[a->rd] = (int64_t)a->imm << 12;
    env->pc += 4;
    return true;
}
static bool trans_lu32i_d(CPULoongArchState *env, arg_lu32i_d *restrict a) {
    env->gpr[a->rd] = (uint64_t)(uint32_t)env->gpr[a->rd] | ((int64_t)a->imm << 32);
    env->pc += 4;
    return true;
}
static bool trans_lu52i_d(CPULoongArchState *env, arg_lu52i_d *restrict a) {
    env->gpr[a->rd] = deposit64(env->gpr[a->rj], 52, 12, a->imm);
    env->pc += 4;
    return true;
}
static bool trans_pcaddi(CPULoongArchState *env, arg_pcaddi *restrict a) {
    env->gpr[a->rd] = env->pc + ((int64_t)a->imm << 2);
    env->pc += 4;
    return true;
}
static bool trans_pcalau12i(CPULoongArchState *env, arg_pcalau12i *restrict a) {
    env->gpr[a->rd] = env->pc + ((int64_t)a->imm << 12);
    env->gpr[a->rd] &= ~0xfffull;
    env->pc += 4;
    return true;
}
static bool trans_pcaddu12i(CPULoongArchState *env, arg_pcaddu12i *restrict a) {
    env->gpr[a->rd] = env->pc + ((int64_t)a->imm << 12);
    env->pc += 4;
    return true;
}
static bool trans_pcaddu18i(CPULoongArchState *env, arg_pcaddu18i *restrict a) {
    env->gpr[a->rd] = env->pc + ((int64_t)a->imm << 18);
    env->pc += 4;
    return true;
}
static bool trans_addi_w(CPULoongArchState *env, arg_addi_w *restrict a) {
    env->gpr[a->rd] = (int64_t)(int32_t)(env->gpr[a->rj] + a->imm);
    env->pc += 4;
    return true;
}
static bool trans_addi_d(CPULoongArchState *env, arg_addi_d *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] + a->imm;
    env->pc += 4;
    return true;
}
static bool trans_addu16i_d(CPULoongArchState *env, arg_addu16i_d *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] + (a->imm << 16);
    env->pc += 4;
    return true;
}
static bool trans_andi(CPULoongArchState *env, arg_andi *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] & a->imm;
    env->pc += 4;
    return true;
}
static bool trans_ori(CPULoongArchState *env, arg_ori *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] | a->imm;
    env->pc += 4;
    return true;
}
static bool trans_xori(CPULoongArchState *env, arg_xori *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] ^ a->imm;
    env->pc += 4;
    return true;
}
static bool trans_sll_w(CPULoongArchState *env, arg_sll_w *restrict a) {
    env->gpr[a->rd] = (int64_t)((int32_t)env->gpr[a->rj] << (env->gpr[a->rk] & 0x1f));
    env->pc += 4;
    return true;
}
static bool trans_srl_w(CPULoongArchState *env, arg_srl_w *restrict a) {
    env->gpr[a->rd] = (int64_t)(int32_t)((uint32_t)env->gpr[a->rj] >> (env->gpr[a->rk] & 0x1f));
    env->pc += 4;
    return true;
}
static bool trans_sra_w(CPULoongArchState *env, arg_sra_w *restrict a) {
    env->gpr[a->rd] = (int64_t)((int32_t)env->gpr[a->rj] >> (env->gpr[a->rk] & 0x1f));
    env->pc += 4;
    return true;
}
static bool trans_sll_d(CPULoongArchState *env, arg_sll_d *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] << (env->gpr[a->rk] & 0x3f);
    env->pc += 4;
    return true;
}
static bool trans_srl_d(CPULoongArchState *env, arg_srl_d *restrict a) {
    env->gpr[a->rd] = (uint64_t)env->gpr[a->rj] >> (env->gpr[a->rk] & 0x3f);
    env->pc += 4;
    return true;
}
static bool trans_sra_d(CPULoongArchState *env, arg_sra_d *restrict a) {
    env->gpr[a->rd] = (int64_t)env->gpr[a->rj] >> (env->gpr[a->rk] & 0x3f);
    env->pc += 4;
    return true;
}
static bool trans_rotr_w(CPULoongArchState *env, arg_rotr_w *restrict a) {
    uint32_t rj = env->gpr[a->rj];
    int imm = env->gpr[a->rk] & 0x1f;
    env->gpr[a->rd] = (int64_t)(int32_t)((rj >> imm) | (rj << (32 - imm)));
    env->pc += 4;
    return true;
}
static bool trans_rotr_d(CPULoongArchState *env, arg_rotr_d *restrict a) {
    uint64_t rj = env->gpr[a->rj];
    int imm = env->gpr[a->rk] & 0x3f;
    env->gpr[a->rd] = (rj >> imm) | (rj << (64 - imm));
    env->pc += 4;
    return true;
}
static bool trans_slli_w(CPULoongArchState *env, arg_slli_w *restrict a) {
    env->gpr[a->rd] = (int64_t)((int32_t)env->gpr[a->rj] << a->imm);
    env->pc += 4;
    return true;
}
static bool trans_slli_d(CPULoongArchState *env, arg_slli_d *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] << a->imm;
    env->pc += 4;
    return true;
}
static bool trans_srli_w(CPULoongArchState *env, arg_srli_w *restrict a) {
    env->gpr[a->rd] = (int64_t)(int32_t)((uint32_t)env->gpr[a->rj] >> a->imm);
    env->pc += 4;
    return true;
}
static bool trans_srli_d(CPULoongArchState *env, arg_srli_d *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] >> a->imm;
    env->pc += 4;
    return true;
}
static bool trans_srai_w(CPULoongArchState *env, arg_srai_w *restrict a) {
    env->gpr[a->rd] = (int64_t)((int32_t)env->gpr[a->rj] >> a->imm);
    env->pc += 4;
    return true;
}
static bool trans_srai_d(CPULoongArchState *env, arg_srai_d *restrict a) {
    env->gpr[a->rd] = (int64_t)env->gpr[a->rj] >> a->imm;
    env->pc += 4;
    return true;
}
static bool trans_rotri_w(CPULoongArchState *env, arg_rotri_w *restrict a) {
    uint32_t rj = env->gpr[a->rj];
    int imm = a->imm & 0x1f;
    env->gpr[a->rd] = (int64_t)(int32_t)((rj >> imm) | (rj << (32 - imm)));
    env->pc += 4;
    return true;
}
static bool trans_rotri_d(CPULoongArchState *env, arg_rotri_d *restrict a) {
    uint64_t rj = env->gpr[a->rj];
    int imm = a->imm & 0x3f;
    env->gpr[a->rd] = (rj >> imm) | (rj << (64 - imm));
    env->pc += 4;
    return true;
}
static bool trans_ext_w_h(CPULoongArchState *env, arg_ext_w_h *restrict a) {
    env->gpr[a->rd] = (int64_t)(int16_t)env->gpr[a->rj];
    env->pc += 4;
    return true;
}
static bool trans_ext_w_b(CPULoongArchState *env, arg_ext_w_b *restrict a) {
    env->gpr[a->rd] = (int64_t)(int8_t)env->gpr[a->rj];
    env->pc += 4;
    return true;
}
static bool trans_clo_w(CPULoongArchState *env, arg_clo_w *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] ? clo32(env->gpr[a->rj]) : 0;
    env->pc += 4;
    return true;
}
static bool trans_clz_w(CPULoongArchState *env, arg_clz_w *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] ? clz32(env->gpr[a->rj]) : 32;
    env->pc += 4;
    return true;
}
static bool trans_cto_w(CPULoongArchState *env, arg_cto_w *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] ? cto32(env->gpr[a->rj]) : 0;
    env->pc += 4;
    return true;
}
static bool trans_ctz_w(CPULoongArchState *env, arg_ctz_w *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] ? ctz32(env->gpr[a->rj]) : 32;
    env->pc += 4;
    return true;
}
static bool trans_clo_d(CPULoongArchState *env, arg_clo_d *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] ? clo64(env->gpr[a->rj]) : 0;
    env->pc += 4;
    return true;
}
static bool trans_clz_d(CPULoongArchState *env, arg_clz_d *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] ? clz64(env->gpr[a->rj]) : 64;
    env->pc += 4;
    return true;
}
static bool trans_cto_d(CPULoongArchState *env, arg_cto_d *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] ? cto64(env->gpr[a->rj]) : 0;
    env->pc += 4;
    return true;
}
static bool trans_ctz_d(CPULoongArchState *env, arg_ctz_d *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rj] ? ctz64(env->gpr[a->rj]) : 64;
    env->pc += 4;
    return true;
}
static bool trans_revb_2h(CPULoongArchState *env, arg_revb_2h *restrict a) {
    uint32_t mask = 0x00FF00FF;
    uint32_t rj = env->gpr[a->rj];
    env->gpr[a->rd] = (int64_t)(int32_t)(((rj >> 8) & mask) | ((rj & mask ) << 8));
    env->pc += 4;
    return true;
}
static bool trans_revb_4h(CPULoongArchState *env, arg_revb_4h *restrict a) {
    uint64_t mask = 0x00FF00FF00FF00FFULL;
    uint64_t rj = env->gpr[a->rj];
    env->gpr[a->rd] = ((rj >> 8) & mask) | ((rj & mask ) << 8);
    env->pc += 4;
    return true;
}
static bool trans_revb_2w(CPULoongArchState *env, arg_revb_2w *restrict a) {
    uint64_t rj = env->gpr[a->rj];
    env->gpr[a->rd] =
        ((rj & 0xFF00000000000000u) >> 24u) |
        ((rj & 0x00FF000000000000u) >>  8u) |
        ((rj & 0x0000FF0000000000u) <<  8u) |
        ((rj & 0x000000FF00000000u) << 24u) |
        ((rj & 0x00000000FF000000u) >> 24u) |
        ((rj & 0x0000000000FF0000u) >>  8u) |
        ((rj & 0x000000000000FF00u) <<  8u) |
        ((rj & 0x00000000000000FFu) << 24u);
    env->pc += 4;
    return true;
}
static bool trans_revb_d(CPULoongArchState *env, arg_revb_d *restrict a) {
    uint64_t rj = env->gpr[a->rj];
    env->gpr[a->rd] =
        ((rj & 0xFF00000000000000u) >> 56u) |
        ((rj & 0x00FF000000000000u) >> 40u) |
        ((rj & 0x0000FF0000000000u) >> 24u) |
        ((rj & 0x000000FF00000000u) >>  8u) |
        ((rj & 0x00000000FF000000u) <<  8u) |
        ((rj & 0x0000000000FF0000u) << 24u) |
        ((rj & 0x000000000000FF00u) << 40u) |
        ((rj & 0x00000000000000FFu) << 56u);
    env->pc += 4;
    return true;
}
static bool trans_revh_2w(CPULoongArchState *env, arg_revh_2w *restrict a) {
    uint64_t rj = env->gpr[a->rj];
    env->gpr[a->rd] =
        ((rj & 0xFFFF000000000000u) >> 16u) |
        ((rj & 0x0000FFFF00000000u) << 16u) |
        ((rj & 0x00000000FFFF0000u) >> 16u) |
        ((rj & 0x000000000000FFFFu) << 16u);
    env->pc += 4;
    return true;
}
static bool trans_revh_d(CPULoongArchState *env, arg_revh_d *restrict a) {
    uint64_t mask = 0x0000FFFF0000FFFFULL;
    uint64_t rj = env->gpr[a->rj];
    uint64_t t = ((rj >> 16) & mask) | ((rj & mask ) << 16);
    env->gpr[a->rd] = (t >> 32) | (t << 32);
    env->pc += 4;
    return true;
}
target_ulong helper_bitswap(target_ulong v)
{
    v = ((v >> 1) & (target_ulong)0x5555555555555555ULL) |
        ((v & (target_ulong)0x5555555555555555ULL) << 1);
    v = ((v >> 2) & (target_ulong)0x3333333333333333ULL) |
        ((v & (target_ulong)0x3333333333333333ULL) << 2);
    v = ((v >> 4) & (target_ulong)0x0F0F0F0F0F0F0F0FULL) |
        ((v & (target_ulong)0x0F0F0F0F0F0F0F0FULL) << 4);
    return v;
}
static bool trans_bitrev_4b(CPULoongArchState *env, arg_bitrev_4b *restrict a) {
    gen_set_gpr(env, a->rd, helper_bitswap(env->gpr[a->rj]), EXT_SIGN);
    env->pc += 4;
    return true;
}
static bool trans_bitrev_8b(CPULoongArchState *env, arg_bitrev_8b *restrict a) {
    gen_set_gpr(env, a->rd, helper_bitswap(env->gpr[a->rj]), EXT_NONE);
    env->pc += 4;
    return true;
}
static bool trans_bitrev_w(CPULoongArchState *env, arg_bitrev_w *restrict a) {
    gen_set_gpr(env, a->rd, revbit32(env->gpr[a->rj]), EXT_SIGN);
    env->pc += 4;
    return true;
}
static bool trans_bitrev_d(CPULoongArchState *env, arg_bitrev_d *restrict a) {
    gen_set_gpr(env, a->rd, revbit64(env->gpr[a->rj]), EXT_NONE);
    env->pc += 4;
    return true;
}
static bool trans_bytepick_w(CPULoongArchState *env, arg_bytepick_w *restrict a) {
    uint64_t rj = (uint32_t)env->gpr[a->rj];
    uint64_t rk = (uint32_t)env->gpr[a->rk];
    uint64_t res = a->sa == 0 ? rk : ((rj >> (32 - a->sa * 8)) | (rk << (a->sa * 8)));
    env->gpr[a->rd] = (int64_t)(int32_t)res;
    env->pc += 4;
    return true;
}
static bool trans_bytepick_d(CPULoongArchState *env, arg_bytepick_d *restrict a) {
    uint64_t rj = env->gpr[a->rj];
    uint64_t rk = env->gpr[a->rk];
    env->gpr[a->rd] = a->sa == 0 ? rk : ((rj >> (64 - a->sa * 8)) | (rk << (a->sa * 8)));
    env->pc += 4;
    return true;
}
static bool trans_maskeqz(CPULoongArchState *env, arg_maskeqz *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rk] == 0 ? 0 : env->gpr[a->rj];
    env->pc += 4;
    return true;
}
static bool trans_masknez(CPULoongArchState *env, arg_masknez *restrict a) {
    env->gpr[a->rd] = env->gpr[a->rk] != 0 ? 0 : env->gpr[a->rj];
    env->pc += 4;
    return true;
}
static bool trans_bstrins_w(CPULoongArchState *env, arg_bstrins_w *restrict a)  {
    env->gpr[a->rd] = (int64_t)(int32_t)deposit32(env->gpr[a->rd], a->ls, a->ms - a->ls + 1, env->gpr[a->rj]);
    env->pc += 4;
    return true;
}
static bool trans_bstrpick_w(CPULoongArchState *env, arg_bstrpick_w *restrict a) {
    if (a->ls > a->ms) {
        return false;
    }
    env->gpr[a->rd] = (int64_t)(int32_t)extract32(env->gpr[a->rj], a->ls, a->ms - a->ls + 1);
    env->pc += 4;
    return true;
}
static bool trans_bstrins_d(CPULoongArchState *env, arg_bstrins_d *restrict a) {
    env->gpr[a->rd] = deposit64(env->gpr[a->rd], a->ls, a->ms - a->ls + 1, env->gpr[a->rj]);
    env->pc += 4;
    return true;
}
static bool trans_bstrpick_d(CPULoongArchState *env, arg_bstrpick_d *restrict a) {
    if (a->ls > a->ms) {
        return false;
    }
    env->gpr[a->rd] = extract64(env->gpr[a->rj], a->ls, a->ms - a->ls + 1);
    env->pc += 4;
    return true;
}

bool is_one_page(uint64_t addr, int bytes) {
    target_ulong pgsz = 0x4000;
    target_ulong pgmsk = pgsz - 1;
    return (addr & ~pgmsk) == ((addr + bytes - 1) & ~pgmsk);
}

bool is_two_page(uint64_t addr, int bytes) {
    return !is_one_page(addr, bytes);
}

/* Check if address addr is aligned for a memory access of size bytes.
   In system mode, alignment means addr and addr+bytes-1 lie within the same page. */
bool is_aligned(uint64_t addr, int bytes) {
#ifdef CONFIG_USER_ONLY
        return true;
#endif
    return is_one_page(addr, bytes);
}

bool is_unaligned(uint64_t addr, int bytes) {
    return !is_aligned(addr, bytes);
}
/* Translate guest virtual address to host physical address and load value. */
static hwaddr load_pa(CPULoongArchState *env, uint64_t addr) {
    PERF_INC(COUNTER_INST_LOAD);
#ifdef CONFIG_USER_ONLY
        return addr;
#endif
    if (!is_la64(env)) {
        addr = (uint32_t)addr;
    }
    hwaddr ha;
    int prot;

    int tc_index = TC_INDEX(addr);
    TLBCache* tc = env->tc_load + tc_index;
    uint64_t page_addr = addr & TARGET_PAGE_MASK;
    if (likely(page_addr == tc->va)) {
        uint64_t ha = (addr & (TARGET_PAGE_SIZE - 1)) | tc->pa;
        return ha;
    }
    int mmu_idx = FIELD_EX64(env->CSR_CRMD, CSR_CRMD, PLV) == 0 ? MMU_KERNEL_IDX : MMU_USER_IDX;
    check_get_physical_address(env, &ha, &prot, addr, MMU_DATA_LOAD, mmu_idx);
    tc->va = page_addr;
    tc->pa = ha & TARGET_PAGE_MASK;
    return ha;
}
/* Translate guest virtual address to host physical address and store value. */
static hwaddr store_pa(CPULoongArchState *env, uint64_t addr) {
    PERF_INC(COUNTER_INST_STORE);
#ifdef CONFIG_USER_ONLY
        return addr;
#endif
    if (!is_la64(env)) {
        addr = (uint32_t)addr;
    }
    hwaddr ha;
    int prot;
    int tc_index = TC_INDEX(addr);
    TLBCache* tc = env->tc_store + tc_index;
    uint64_t page_addr = addr & TARGET_PAGE_MASK;
    if (likely(page_addr == tc->va)) {
        ha = (addr & (TARGET_PAGE_SIZE - 1)) | tc->pa;
    } else {
        int mmu_idx = FIELD_EX64(env->CSR_CRMD, CSR_CRMD, PLV) == 0 ? MMU_KERNEL_IDX : MMU_USER_IDX;
        check_get_physical_address(env, &ha, &prot, addr, MMU_DATA_STORE, mmu_idx);
        tc->va = page_addr;
        tc->pa = ha & TARGET_PAGE_MASK;
    }
    return ha;
}
#if defined(CONFIG_USER_ONLY)
#define is_io(...) false
#else
// exclude 32MB bios
static bool is_io(hwaddr ha) {
    return (ha >= 0x10000000 && ha < 0x1c000000)
            || (ha > 0x1e000000 && ha < 0x20000000)
            || (ha >=0x100000000 && ha < 0x120000000);
}
#endif
static uint64_t add_addr(int64_t base, int64_t disp) {
    return (uint64_t)(base + disp);
}

static int8_t ld_b(CPULoongArchState *env, uint64_t va) {
    hwaddr ha = load_pa(env, va);
#if defined(CONFIG_USER_ONLY)
    return ram_ldb(ha);
#else
    return is_io(ha) ? do_io_ld(ha, 1) : ram_ldb(ha);
#endif
}

static int16_t ld_h(CPULoongArchState *env, uint64_t va) {
    uint64_t data;
    const int data_size = 2;
    hwaddr ha = load_pa(env, va);
    if (is_io(ha)) {
#if !defined(CONFIG_USER_ONLY)
        data = do_io_ld(ha, data_size);
#endif
    } else {
        if (is_aligned(va, data_size)) {
            data = ram_ldh(ha);
        } else {
            PERF_INC(COUNTER_INST_CROSS_PAGE_LOAD);
            data = 0;
            for (int i = (data_size - 1); i >= 0; i--){
                data |= (((uint16_t)ld_b(env, va + i) & 0xff) << (i * 8)) ;
            }
        }
    }
    return data;
}

static int32_t ld_w(CPULoongArchState *env, uint64_t va) {
    uint64_t data;
    const int data_size = 4;
    hwaddr ha = load_pa(env, va);
    if (is_io(ha)) {
#if !defined(CONFIG_USER_ONLY)
        data = do_io_ld(ha, data_size);
#endif
    } else {
        if (is_aligned(va, data_size)) {
            data = ram_ldw(ha);
        } else {
            PERF_INC(COUNTER_INST_CROSS_PAGE_LOAD);
            data = 0;
            for (int i = (data_size - 1); i >= 0; i--){
                data |= (((uint32_t)ld_b(env, va + i) & 0xff) << (i * 8)) ;
            }
        }
    }
    return data;
}

static int64_t ld_d(CPULoongArchState *env, uint64_t va) {
    uint64_t data;
    const int data_size = 8;
    hwaddr ha = load_pa(env, va);
    if (is_io(ha)) {
#if !defined(CONFIG_USER_ONLY)
        data = do_io_ld(ha, data_size);
#endif
    } else {
        if (is_aligned(va, data_size)) {
            data = ram_ldd(ha);
        } else {
            PERF_INC(COUNTER_INST_CROSS_PAGE_LOAD);
            data = 0;
            for (int i = (data_size - 1); i >= 0; i--){
                data |= (((uint64_t)ld_b(env, va + i) & 0xff) << (i * 8)) ;
            }
        }
    }
    return data;
}

static void st_b(CPULoongArchState *env, uint64_t va, uint8_t data) {
    hwaddr ha = store_pa(env, va);
#if defined(CONFIG_USER_ONLY)
    ram_stb(ha, data);
#else
    is_io(ha) ? do_io_st(ha, data, 1) : ram_stb(ha, data);
#endif
}

static void st_h(CPULoongArchState *env, uint64_t va, uint16_t data) {
    const int data_size = 2;
    hwaddr ha = store_pa(env, va);
    if (is_io(ha)) {
#if !defined(CONFIG_USER_ONLY)
        do_io_st(ha, data, data_size);
#endif
    } else {
        if (is_aligned(va, data_size)) {
            ram_sth(ha, data);
        } else {
            PERF_INC(COUNTER_INST_CROSS_PAGE_LOAD);
            for (int i = (data_size - 1); i >= 0; i--){
                st_b(env, va + i, (data >> (i * 8)) & 0xff);
            }
        }
    }
}

static void st_w(CPULoongArchState *env, uint64_t va, uint32_t data) {
    const int data_size = 4;
    hwaddr ha = store_pa(env, va);
    if (is_io(ha)) {
#if !defined(CONFIG_USER_ONLY)
        do_io_st(ha, data, data_size);
#endif
    } else {
        if (is_aligned(va, data_size)) {
            ram_stw(ha, data);
        } else {
            PERF_INC(COUNTER_INST_CROSS_PAGE_LOAD);
            for (int i = (data_size - 1); i >= 0; i--){
                st_b(env, va + i, (data >> (i * 8)) & 0xff);
            }
        }
    }
}

static void st_d(CPULoongArchState *env, uint64_t va, uint64_t data) {
    const int data_size = 8;
    hwaddr ha = store_pa(env, va);
    if (is_io(ha)) {
#if !defined(CONFIG_USER_ONLY)
        do_io_st(ha, data, data_size);
#endif
    } else {
        if (is_aligned(va, data_size)) {
            ram_std(ha, data);
        } else {
            PERF_INC(COUNTER_INST_CROSS_PAGE_LOAD);
            for (int i = (data_size - 1); i >= 0; i--){
                st_b(env, va + i, (data >> (i * 8)) & 0xff);
            }
        }
    }
}

static bool trans_ld_b(CPULoongArchState *env, arg_ld_b *restrict a) {
    env->gpr[a->rd] = (int64_t)ld_b(env, add_addr(env->gpr[a->rj], a->imm));
    env->pc += 4;
    return true;
}
static bool trans_ld_h(CPULoongArchState *env, arg_ld_h *restrict a) {
    env->gpr[a->rd] = (int64_t)ld_h(env, add_addr(env->gpr[a->rj], a->imm));
    env->pc += 4;
    return true;
}
static bool trans_ld_w(CPULoongArchState *env, arg_ld_w *restrict a) {
    env->gpr[a->rd] = (int64_t)ld_w(env, add_addr(env->gpr[a->rj], a->imm));
    env->pc += 4;
    return true;
}
static bool trans_ld_d(CPULoongArchState *env, arg_ld_d *restrict a) {
    env->gpr[a->rd] = (int64_t)ld_d(env, add_addr(env->gpr[a->rj], a->imm));
    env->pc += 4;
    return true;
}

static bool trans_st_b(CPULoongArchState *env, arg_st_b *restrict a) {
    st_b(env, add_addr(env->gpr[a->rj], a->imm), env->gpr[a->rd]);
    env->pc += 4;
    return true;
}
static bool trans_st_h(CPULoongArchState *env, arg_st_h *restrict a) {
    st_h(env, add_addr(env->gpr[a->rj], a->imm), env->gpr[a->rd]);
    env->pc += 4;
    return true;
}

static bool trans_st_w(CPULoongArchState *env, arg_st_w *restrict a) {
    st_w(env, add_addr(env->gpr[a->rj], a->imm), env->gpr[a->rd]);
    env->pc += 4;
    return true;
}
static bool trans_st_d(CPULoongArchState *env, arg_st_d *restrict a) {
    st_d(env, add_addr(env->gpr[a->rj], a->imm), env->gpr[a->rd]);
    env->pc += 4;
    return true;
}
static bool trans_ld_bu(CPULoongArchState *env, arg_ld_bu *restrict a) {
    env->gpr[a->rd] = (uint8_t)ld_b(env, add_addr(env->gpr[a->rj], a->imm));
    env->pc += 4;
    return true;
}
static bool trans_ld_hu(CPULoongArchState *env, arg_ld_hu *restrict a) {
    env->gpr[a->rd] = (uint16_t)ld_h(env, add_addr(env->gpr[a->rj], a->imm));
    env->pc += 4;
    return true;
}
static bool trans_ld_wu(CPULoongArchState *env, arg_ld_wu *restrict a) {
    env->gpr[a->rd] = (uint32_t)ld_w(env, add_addr(env->gpr[a->rj], a->imm));
    env->pc += 4;
    return true;
}
static bool trans_ldx_b(CPULoongArchState *env, arg_ldx_b *restrict a) {
    env->gpr[a->rd] = (int64_t)ld_b(env, add_addr(env->gpr[a->rj], env->gpr[a->rk]));
    env->pc += 4;
    return true;
}
static bool trans_ldx_h(CPULoongArchState *env, arg_ldx_h *restrict a) {
    env->gpr[a->rd] = (int64_t)ld_h(env, add_addr(env->gpr[a->rj], env->gpr[a->rk]));
    env->pc += 4;
    return true;
}
static bool trans_ldx_w(CPULoongArchState *env, arg_ldx_w *restrict a) {
    env->gpr[a->rd] = (int64_t)ld_w(env, add_addr(env->gpr[a->rj], env->gpr[a->rk]));
    env->pc += 4;
    return true;
}
static bool trans_ldx_d(CPULoongArchState *env, arg_ldx_d *restrict a) {
    env->gpr[a->rd] = (int64_t)ld_d(env, add_addr(env->gpr[a->rj], env->gpr[a->rk]));
    env->pc += 4;
    return true;
}
static bool trans_stx_b(CPULoongArchState *env, arg_stx_b *restrict a) {
    st_b(env, add_addr(env->gpr[a->rj], env->gpr[a->rk]), env->gpr[a->rd]);
    env->pc += 4;
    return true;
}
static bool trans_stx_h(CPULoongArchState *env, arg_stx_h *restrict a) {
    st_h(env, add_addr(env->gpr[a->rj], env->gpr[a->rk]), env->gpr[a->rd]);
    env->pc += 4;
    return true;
}
static bool trans_stx_w(CPULoongArchState *env, arg_stx_w *restrict a) {
    st_w(env, add_addr(env->gpr[a->rj], env->gpr[a->rk]), env->gpr[a->rd]);
    env->pc += 4;
    return true;
}
static bool trans_stx_d(CPULoongArchState *env, arg_stx_d *restrict a) {
    st_d(env, add_addr(env->gpr[a->rj], env->gpr[a->rk]), env->gpr[a->rd]);
    env->pc += 4;
    return true;
}
static bool trans_ldx_bu(CPULoongArchState *env, arg_ldx_bu *restrict a) {
    env->gpr[a->rd] = (uint8_t)ld_b(env, add_addr(env->gpr[a->rj], env->gpr[a->rk]));
    env->pc += 4;
    return true;
}
static bool trans_ldx_hu(CPULoongArchState *env, arg_ldx_hu *restrict a) {
    env->gpr[a->rd] = (uint16_t)ld_h(env, add_addr(env->gpr[a->rj], env->gpr[a->rk]));
    env->pc += 4;
    return true;
}
static bool trans_ldx_wu(CPULoongArchState *env, arg_ldx_wu *restrict a) {
    env->gpr[a->rd] = (uint32_t)ld_w(env, add_addr(env->gpr[a->rj], env->gpr[a->rk]));
    env->pc += 4;
    return true;
}
static bool trans_preld(CPULoongArchState *env, arg_preld *restrict a) {
    env->pc += 4;
    return true;
}
static bool trans_preldx(CPULoongArchState *env, arg_preldx *restrict a) {
    env->pc += 4;
    return true;
}
static bool trans_dbar(CPULoongArchState *env, arg_dbar *restrict a) {
    env->pc += 4;
    return true;
}
static bool trans_ibar(CPULoongArchState *env, arg_ibar *restrict a) {
#ifndef CONFIG_DIFF
    static double begin_timestamp;
    if (a->imm == 64) {
        begin_timestamp = second();
        fprintf(stderr, "[INST HACK] ibar 64 begin %f\n", begin_timestamp);
    } else if (a->imm == 65) {
        fprintf(stderr, "[INST HACK] ibar 65 end %f\n", second() - begin_timestamp);
        dump_exec_info(env, stderr);
        laemu_exit(0);
    }
#endif
    env->pc += 4;
    return true;
}
static bool trans_ldptr_w(CPULoongArchState *env, arg_ldptr_w *restrict a) {
    env->gpr[a->rd] = (int64_t)ld_w(env, add_addr(env->gpr[a->rj], a->imm));
    env->pc += 4;
    return true;
}
static bool trans_stptr_w(CPULoongArchState *env, arg_stptr_w *restrict a) {
    st_w(env, add_addr(env->gpr[a->rj], a->imm), env->gpr[a->rd]);
    env->pc += 4;
    return true;
}
static bool trans_ldptr_d(CPULoongArchState *env, arg_ldptr_d *restrict a) {
    env->gpr[a->rd] = (int64_t)ld_d(env, add_addr(env->gpr[a->rj], a->imm));
    env->pc += 4;
    return true;
}
static bool trans_stptr_d(CPULoongArchState *env, arg_stptr_d *restrict a) {
    st_d(env, add_addr(env->gpr[a->rj], a->imm), env->gpr[a->rd]);
    env->pc += 4;
    return true;
}
/* Load/Store with boundary check: assert first, then always access memory. */
static bool trans_ldgt_b(CPULoongArchState *env, arg_ldgt_b *restrict a) {
    if ((uint64_t)env->gpr[a->rj] <= (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    env->gpr[a->rd] = (int64_t)ld_b(env, env->gpr[a->rj]);
    env->pc += 4; return true;
}
static bool trans_ldgt_h(CPULoongArchState *env, arg_ldgt_h *restrict a) {
    if ((uint64_t)env->gpr[a->rj] <= (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    env->gpr[a->rd] = (int64_t)ld_h(env, env->gpr[a->rj]);
    env->pc += 4; return true;
}
static bool trans_ldgt_w(CPULoongArchState *env, arg_ldgt_w *restrict a) {
    if ((uint64_t)env->gpr[a->rj] <= (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    env->gpr[a->rd] = (int64_t)ld_w(env, env->gpr[a->rj]);
    env->pc += 4; return true;
}
static bool trans_ldgt_d(CPULoongArchState *env, arg_ldgt_d *restrict a) {
    if ((uint64_t)env->gpr[a->rj] <= (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    env->gpr[a->rd] = ld_d(env, env->gpr[a->rj]);
    env->pc += 4; return true;
}
static bool trans_ldle_b(CPULoongArchState *env, arg_ldle_b *restrict a) {
    if ((uint64_t)env->gpr[a->rj] > (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    env->gpr[a->rd] = (int64_t)ld_b(env, env->gpr[a->rj]);
    env->pc += 4; return true;
}
static bool trans_ldle_h(CPULoongArchState *env, arg_ldle_h *restrict a) {
    if ((uint64_t)env->gpr[a->rj] > (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    env->gpr[a->rd] = (int64_t)ld_h(env, env->gpr[a->rj]);
    env->pc += 4; return true;
}
static bool trans_ldle_w(CPULoongArchState *env, arg_ldle_w *restrict a) {
    if ((uint64_t)env->gpr[a->rj] > (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    env->gpr[a->rd] = (int64_t)ld_w(env, env->gpr[a->rj]);
    env->pc += 4; return true;
}
static bool trans_ldle_d(CPULoongArchState *env, arg_ldle_d *restrict a) {
    if ((uint64_t)env->gpr[a->rj] > (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    env->gpr[a->rd] = ld_d(env, env->gpr[a->rj]);
    env->pc += 4; return true;
}
static bool trans_stgt_b(CPULoongArchState *env, arg_stgt_b *restrict a) {
    if ((uint64_t)env->gpr[a->rj] <= (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    st_b(env, env->gpr[a->rj], env->gpr[a->rd]);
    env->pc += 4; return true;
}
static bool trans_stgt_h(CPULoongArchState *env, arg_stgt_h *restrict a) {
    if ((uint64_t)env->gpr[a->rj] <= (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    st_h(env, env->gpr[a->rj], env->gpr[a->rd]);
    env->pc += 4; return true;
}
static bool trans_stgt_w(CPULoongArchState *env, arg_stgt_w *restrict a) {
    if ((uint64_t)env->gpr[a->rj] <= (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    st_w(env, env->gpr[a->rj], env->gpr[a->rd]);
    env->pc += 4; return true;
}
static bool trans_stgt_d(CPULoongArchState *env, arg_stgt_d *restrict a) {
    if ((uint64_t)env->gpr[a->rj] <= (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    st_d(env, env->gpr[a->rj], env->gpr[a->rd]);
    env->pc += 4; return true;
}
static bool trans_stle_b(CPULoongArchState *env, arg_stle_b *restrict a) {
    if ((uint64_t)env->gpr[a->rj] > (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    st_b(env, env->gpr[a->rj], env->gpr[a->rd]);
    env->pc += 4; return true;
}
static bool trans_stle_h(CPULoongArchState *env, arg_stle_h *restrict a) {
    if ((uint64_t)env->gpr[a->rj] > (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    st_h(env, env->gpr[a->rj], env->gpr[a->rd]);
    env->pc += 4; return true;
}
static bool trans_stle_w(CPULoongArchState *env, arg_stle_w *restrict a) {
    if ((uint64_t)env->gpr[a->rj] > (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    st_w(env, env->gpr[a->rj], env->gpr[a->rd]);
    env->pc += 4; return true;
}
static bool trans_stle_d(CPULoongArchState *env, arg_stle_d *restrict a) {
    if ((uint64_t)env->gpr[a->rj] > (uint64_t)env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    st_d(env, env->gpr[a->rj], env->gpr[a->rd]);
    env->pc += 4; return true;
}
static bool trans_ll_w(CPULoongArchState *env, arg_ll_w *restrict a) {
    hwaddr ha = load_pa(env, env->gpr[a->rj] + a->imm);
    env->gpr[a->rd] = ram_ldw(ha);
    env->lladdr = ha;
    env->llval = env->gpr[a->rd];
    env->CSR_LLBCTL = FIELD_DP64(env->CSR_LLBCTL, CSR_LLBCTL, ROLLB, 1);
    env->pc += 4;
    return true;
}
static bool trans_sc_w(CPULoongArchState *env, arg_sc_w *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj] + a->imm);
    if (FIELD_EX64(env->CSR_LLBCTL, CSR_LLBCTL, ROLLB) &&
        env->lladdr == ha && env->llval == ram_ldw(ha)) {
        ram_stw(ha, env->gpr[a->rd]);
        env->gpr[a->rd] = 1;
    } else {
        env->gpr[a->rd] = 0;
    }
    env->CSR_LLBCTL = FIELD_DP64(env->CSR_LLBCTL, CSR_LLBCTL, ROLLB, 0);
    env->pc += 4;
    return true;
}
static bool trans_ll_d(CPULoongArchState *env, arg_ll_d *restrict a) {
    hwaddr ha = load_pa(env, env->gpr[a->rj] + a->imm);
    env->gpr[a->rd] = ram_ldd(ha);
    env->lladdr = ha;
    env->llval = env->gpr[a->rd];
    env->CSR_LLBCTL = FIELD_DP64(env->CSR_LLBCTL, CSR_LLBCTL, ROLLB, 1);
    env->pc += 4;
    return true;
}
static bool trans_sc_d(CPULoongArchState *env, arg_sc_d *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj] + a->imm);
    if (FIELD_EX64(env->CSR_LLBCTL, CSR_LLBCTL, ROLLB) &&
        env->lladdr == ha && env->llval == ram_ldd(ha)) {
        ram_std(ha, env->gpr[a->rd]);
        env->gpr[a->rd] = 1;
    } else {
        env->gpr[a->rd] = 0;
    }
    env->CSR_LLBCTL = FIELD_DP64(env->CSR_LLBCTL, CSR_LLBCTL, ROLLB, 0);
    env->pc += 4;
    return true;
}
static bool trans_amswap_w(CPULoongArchState *env, arg_amswap_w *restrict a) {return trans_amswap_db_w(env, a);}
static bool trans_amswap_d(CPULoongArchState *env, arg_amswap_d *restrict a) {return trans_amswap_db_d(env, a);}
static bool trans_amadd_w(CPULoongArchState *env, arg_amadd_w *restrict a) {return trans_amadd_db_w(env, a);}
static bool trans_amadd_d(CPULoongArchState *env, arg_amadd_d *restrict a) {return trans_amadd_db_d(env, a);}
static bool trans_amand_w(CPULoongArchState *env, arg_amand_w *restrict a) {return trans_amand_db_w(env, a);}
static bool trans_amand_d(CPULoongArchState *env, arg_amand_d *restrict a) {return trans_amand_db_d(env, a);}
static bool trans_amor_w(CPULoongArchState *env, arg_amor_w *restrict a) {return trans_amor_db_w(env, a);}
static bool trans_amor_d(CPULoongArchState *env, arg_amor_d *restrict a) {return trans_amor_db_d(env, a);}
static bool trans_amxor_w(CPULoongArchState *env, arg_amxor_w *restrict a) {return trans_amxor_db_w(env, a);}
static bool trans_amxor_d(CPULoongArchState *env, arg_amxor_d *restrict a) {return trans_amxor_db_d(env, a);}
static bool trans_ammax_w(CPULoongArchState *env, arg_ammax_w *restrict a) {return trans_ammax_db_w(env, a);}
static bool trans_ammax_d(CPULoongArchState *env, arg_ammax_d *restrict a) {return trans_ammax_db_d(env, a);}
static bool trans_ammin_w(CPULoongArchState *env, arg_ammin_w *restrict a) {return trans_ammin_db_w(env, a);}
static bool trans_ammin_d(CPULoongArchState *env, arg_ammin_d *restrict a) {return trans_ammin_db_d(env, a);}
static bool trans_ammax_wu(CPULoongArchState *env, arg_ammax_wu *restrict a) {return trans_ammax_db_wu(env, a);}
static bool trans_ammax_du(CPULoongArchState *env, arg_ammax_du *restrict a) {return trans_ammax_db_du(env, a);}
static bool trans_ammin_wu(CPULoongArchState *env, arg_ammin_wu *restrict a) {return trans_ammin_db_wu(env, a);}
static bool trans_ammin_du(CPULoongArchState *env, arg_ammin_du *restrict a) {return trans_ammin_db_du(env, a);}
static bool trans_amswap_db_w(CPULoongArchState *env, arg_amswap_db_w *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int32_t old_v = ram_ldw(ha);
    ram_stw(ha, env->gpr[a->rk]);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_amswap_db_d(CPULoongArchState *env, arg_amswap_db_d *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int64_t old_v = ram_ldd(ha);
    ram_std(ha, env->gpr[a->rk]);
    env->gpr[a->rd] = old_v;
    env->pc += 4;
    return true;
}
static bool trans_amadd_db_w(CPULoongArchState *env, arg_amadd_db_w *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int32_t old_v = ram_ldw(ha);
    int32_t new_v = env->gpr[a->rk] + old_v;
    ram_stw(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_amadd_db_d(CPULoongArchState *env, arg_amadd_db_d *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int64_t old_v = ram_ldd(ha);
    int64_t new_v = env->gpr[a->rk] + old_v;
    ram_std(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_amand_db_w(CPULoongArchState *env, arg_amand_db_w *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int32_t old_v = ram_ldw(ha);
    int32_t new_v = env->gpr[a->rk] & old_v;
    ram_stw(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_amand_db_d(CPULoongArchState *env, arg_amand_db_d *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int64_t old_v = ram_ldd(ha);
    int64_t new_v = env->gpr[a->rk] & old_v;
    ram_std(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_amor_db_w(CPULoongArchState *env, arg_amor_db_w *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int32_t old_v = ram_ldw(ha);
    int32_t new_v = env->gpr[a->rk] | old_v;
    ram_stw(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_amor_db_d(CPULoongArchState *env, arg_amor_db_d *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int64_t old_v = ram_ldd(ha);
    int64_t new_v = env->gpr[a->rk] | old_v;
    ram_std(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_amxor_db_w(CPULoongArchState *env, arg_amxor_db_w *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int32_t old_v = ram_ldw(ha);
    int32_t new_v = env->gpr[a->rk] ^ old_v;
    ram_stw(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_amxor_db_d(CPULoongArchState *env, arg_amxor_db_d *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int64_t old_v = ram_ldd(ha);
    int64_t new_v = env->gpr[a->rk] ^ old_v;
    ram_std(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_ammax_db_w(CPULoongArchState *env, arg_ammax_db_w *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int32_t old_v = ram_ldw(ha);
    int32_t new_v = MAX((int32_t)env->gpr[a->rk], old_v);
    ram_stw(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_ammax_db_d(CPULoongArchState *env, arg_ammax_db_d *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int64_t old_v = ram_ldd(ha);
    int64_t new_v = MAX((int64_t)env->gpr[a->rk], old_v);
    ram_std(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_ammin_db_w(CPULoongArchState *env, arg_ammin_db_w *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int32_t old_v = ram_ldw(ha);
    int32_t new_v = MIN((int32_t)env->gpr[a->rk], old_v);
    ram_stw(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_ammin_db_d(CPULoongArchState *env, arg_ammin_db_d *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int64_t old_v = ram_ldd(ha);
    int64_t new_v = MIN((int64_t)env->gpr[a->rk], old_v);
    ram_std(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_ammax_db_wu(CPULoongArchState *env, arg_ammax_db_wu *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int32_t old_v = ram_ldw(ha);
    int32_t new_v = MAX((uint32_t)env->gpr[a->rk], (uint32_t)old_v);
    ram_stw(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_ammax_db_du(CPULoongArchState *env, arg_ammax_db_du *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int64_t old_v = ram_ldd(ha);
    int64_t new_v = MAX((uint64_t)env->gpr[a->rk], (uint64_t)old_v);
    ram_std(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_ammin_db_wu(CPULoongArchState *env, arg_ammin_db_wu *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int32_t old_v = ram_ldw(ha);
    int32_t new_v = MIN((uint32_t)env->gpr[a->rk], (uint32_t)old_v);
    ram_stw(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static bool trans_ammin_db_du(CPULoongArchState *env, arg_ammin_db_du *restrict a) {
    hwaddr ha = store_pa(env, env->gpr[a->rj]);
    int64_t old_v = ram_ldd(ha);
    int64_t new_v = MIN((uint64_t)env->gpr[a->rk], (uint64_t)old_v);
    ram_std(ha, new_v);
    env->gpr[a->rd] = (int64_t)old_v;
    env->pc += 4;
    return true;
}
static const unsigned long crc_table[256] =
{
    0x00000000UL, 0x77073096UL, 0xee0e612cUL, 0x990951baUL, 0x076dc419UL,
    0x706af48fUL, 0xe963a535UL, 0x9e6495a3UL, 0x0edb8832UL, 0x79dcb8a4UL,
    0xe0d5e91eUL, 0x97d2d988UL, 0x09b64c2bUL, 0x7eb17cbdUL, 0xe7b82d07UL,
    0x90bf1d91UL, 0x1db71064UL, 0x6ab020f2UL, 0xf3b97148UL, 0x84be41deUL,
    0x1adad47dUL, 0x6ddde4ebUL, 0xf4d4b551UL, 0x83d385c7UL, 0x136c9856UL,
    0x646ba8c0UL, 0xfd62f97aUL, 0x8a65c9ecUL, 0x14015c4fUL, 0x63066cd9UL,
    0xfa0f3d63UL, 0x8d080df5UL, 0x3b6e20c8UL, 0x4c69105eUL, 0xd56041e4UL,
    0xa2677172UL, 0x3c03e4d1UL, 0x4b04d447UL, 0xd20d85fdUL, 0xa50ab56bUL,
    0x35b5a8faUL, 0x42b2986cUL, 0xdbbbc9d6UL, 0xacbcf940UL, 0x32d86ce3UL,
    0x45df5c75UL, 0xdcd60dcfUL, 0xabd13d59UL, 0x26d930acUL, 0x51de003aUL,
    0xc8d75180UL, 0xbfd06116UL, 0x21b4f4b5UL, 0x56b3c423UL, 0xcfba9599UL,
    0xb8bda50fUL, 0x2802b89eUL, 0x5f058808UL, 0xc60cd9b2UL, 0xb10be924UL,
    0x2f6f7c87UL, 0x58684c11UL, 0xc1611dabUL, 0xb6662d3dUL, 0x76dc4190UL,
    0x01db7106UL, 0x98d220bcUL, 0xefd5102aUL, 0x71b18589UL, 0x06b6b51fUL,
    0x9fbfe4a5UL, 0xe8b8d433UL, 0x7807c9a2UL, 0x0f00f934UL, 0x9609a88eUL,
    0xe10e9818UL, 0x7f6a0dbbUL, 0x086d3d2dUL, 0x91646c97UL, 0xe6635c01UL,
    0x6b6b51f4UL, 0x1c6c6162UL, 0x856530d8UL, 0xf262004eUL, 0x6c0695edUL,
    0x1b01a57bUL, 0x8208f4c1UL, 0xf50fc457UL, 0x65b0d9c6UL, 0x12b7e950UL,
    0x8bbeb8eaUL, 0xfcb9887cUL, 0x62dd1ddfUL, 0x15da2d49UL, 0x8cd37cf3UL,
    0xfbd44c65UL, 0x4db26158UL, 0x3ab551ceUL, 0xa3bc0074UL, 0xd4bb30e2UL,
    0x4adfa541UL, 0x3dd895d7UL, 0xa4d1c46dUL, 0xd3d6f4fbUL, 0x4369e96aUL,
    0x346ed9fcUL, 0xad678846UL, 0xda60b8d0UL, 0x44042d73UL, 0x33031de5UL,
    0xaa0a4c5fUL, 0xdd0d7cc9UL, 0x5005713cUL, 0x270241aaUL, 0xbe0b1010UL,
    0xc90c2086UL, 0x5768b525UL, 0x206f85b3UL, 0xb966d409UL, 0xce61e49fUL,
    0x5edef90eUL, 0x29d9c998UL, 0xb0d09822UL, 0xc7d7a8b4UL, 0x59b33d17UL,
    0x2eb40d81UL, 0xb7bd5c3bUL, 0xc0ba6cadUL, 0xedb88320UL, 0x9abfb3b6UL,
    0x03b6e20cUL, 0x74b1d29aUL, 0xead54739UL, 0x9dd277afUL, 0x04db2615UL,
    0x73dc1683UL, 0xe3630b12UL, 0x94643b84UL, 0x0d6d6a3eUL, 0x7a6a5aa8UL,
    0xe40ecf0bUL, 0x9309ff9dUL, 0x0a00ae27UL, 0x7d079eb1UL, 0xf00f9344UL,
    0x8708a3d2UL, 0x1e01f268UL, 0x6906c2feUL, 0xf762575dUL, 0x806567cbUL,
    0x196c3671UL, 0x6e6b06e7UL, 0xfed41b76UL, 0x89d32be0UL, 0x10da7a5aUL,
    0x67dd4accUL, 0xf9b9df6fUL, 0x8ebeeff9UL, 0x17b7be43UL, 0x60b08ed5UL,
    0xd6d6a3e8UL, 0xa1d1937eUL, 0x38d8c2c4UL, 0x4fdff252UL, 0xd1bb67f1UL,
    0xa6bc5767UL, 0x3fb506ddUL, 0x48b2364bUL, 0xd80d2bdaUL, 0xaf0a1b4cUL,
    0x36034af6UL, 0x41047a60UL, 0xdf60efc3UL, 0xa867df55UL, 0x316e8eefUL,
    0x4669be79UL, 0xcb61b38cUL, 0xbc66831aUL, 0x256fd2a0UL, 0x5268e236UL,
    0xcc0c7795UL, 0xbb0b4703UL, 0x220216b9UL, 0x5505262fUL, 0xc5ba3bbeUL,
    0xb2bd0b28UL, 0x2bb45a92UL, 0x5cb36a04UL, 0xc2d7ffa7UL, 0xb5d0cf31UL,
    0x2cd99e8bUL, 0x5bdeae1dUL, 0x9b64c2b0UL, 0xec63f226UL, 0x756aa39cUL,
    0x026d930aUL, 0x9c0906a9UL, 0xeb0e363fUL, 0x72076785UL, 0x05005713UL,
    0x95bf4a82UL, 0xe2b87a14UL, 0x7bb12baeUL, 0x0cb61b38UL, 0x92d28e9bUL,
    0xe5d5be0dUL, 0x7cdcefb7UL, 0x0bdbdf21UL, 0x86d3d2d4UL, 0xf1d4e242UL,
    0x68ddb3f8UL, 0x1fda836eUL, 0x81be16cdUL, 0xf6b9265bUL, 0x6fb077e1UL,
    0x18b74777UL, 0x88085ae6UL, 0xff0f6a70UL, 0x66063bcaUL, 0x11010b5cUL,
    0x8f659effUL, 0xf862ae69UL, 0x616bffd3UL, 0x166ccf45UL, 0xa00ae278UL,
    0xd70dd2eeUL, 0x4e048354UL, 0x3903b3c2UL, 0xa7672661UL, 0xd06016f7UL,
    0x4969474dUL, 0x3e6e77dbUL, 0xaed16a4aUL, 0xd9d65adcUL, 0x40df0b66UL,
    0x37d83bf0UL, 0xa9bcae53UL, 0xdebb9ec5UL, 0x47b2cf7fUL, 0x30b5ffe9UL,
    0xbdbdf21cUL, 0xcabac28aUL, 0x53b39330UL, 0x24b4a3a6UL, 0xbad03605UL,
    0xcdd70693UL, 0x54de5729UL, 0x23d967bfUL, 0xb3667a2eUL, 0xc4614ab8UL,
    0x5d681b02UL, 0x2a6f2b94UL, 0xb40bbe37UL, 0xc30c8ea1UL, 0x5a05df1bUL,
    0x2d02ef8dUL
};

static unsigned long crc32(uint32_t crc, const unsigned char *buf, size_t len)
{
    if (buf == NULL) return 0UL;

    crc = crc ^ 0xffffffffUL;
    if (len) do {
        crc = crc_table[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
    } while (--len);
    return crc ^ 0xffffffffUL;
}

target_ulong helper_crc32(target_ulong val, target_ulong m, uint64_t sz)
{
    uint8_t buf[8];
    target_ulong mask = ((sz * 8) == 64) ? -1ULL : ((1ULL << (sz * 8)) - 1);

    m &= mask;
    stq_le_p(buf, m);
    return (int32_t) (crc32(val ^ 0xffffffff, buf, sz) ^ 0xffffffff);
}
static const uint32_t crc32c_table[256] = {
    0x00000000L, 0xF26B8303L, 0xE13B70F7L, 0x1350F3F4L,
    0xC79A971FL, 0x35F1141CL, 0x26A1E7E8L, 0xD4CA64EBL,
    0x8AD958CFL, 0x78B2DBCCL, 0x6BE22838L, 0x9989AB3BL,
    0x4D43CFD0L, 0xBF284CD3L, 0xAC78BF27L, 0x5E133C24L,
    0x105EC76FL, 0xE235446CL, 0xF165B798L, 0x030E349BL,
    0xD7C45070L, 0x25AFD373L, 0x36FF2087L, 0xC494A384L,
    0x9A879FA0L, 0x68EC1CA3L, 0x7BBCEF57L, 0x89D76C54L,
    0x5D1D08BFL, 0xAF768BBCL, 0xBC267848L, 0x4E4DFB4BL,
    0x20BD8EDEL, 0xD2D60DDDL, 0xC186FE29L, 0x33ED7D2AL,
    0xE72719C1L, 0x154C9AC2L, 0x061C6936L, 0xF477EA35L,
    0xAA64D611L, 0x580F5512L, 0x4B5FA6E6L, 0xB93425E5L,
    0x6DFE410EL, 0x9F95C20DL, 0x8CC531F9L, 0x7EAEB2FAL,
    0x30E349B1L, 0xC288CAB2L, 0xD1D83946L, 0x23B3BA45L,
    0xF779DEAEL, 0x05125DADL, 0x1642AE59L, 0xE4292D5AL,
    0xBA3A117EL, 0x4851927DL, 0x5B016189L, 0xA96AE28AL,
    0x7DA08661L, 0x8FCB0562L, 0x9C9BF696L, 0x6EF07595L,
    0x417B1DBCL, 0xB3109EBFL, 0xA0406D4BL, 0x522BEE48L,
    0x86E18AA3L, 0x748A09A0L, 0x67DAFA54L, 0x95B17957L,
    0xCBA24573L, 0x39C9C670L, 0x2A993584L, 0xD8F2B687L,
    0x0C38D26CL, 0xFE53516FL, 0xED03A29BL, 0x1F682198L,
    0x5125DAD3L, 0xA34E59D0L, 0xB01EAA24L, 0x42752927L,
    0x96BF4DCCL, 0x64D4CECFL, 0x77843D3BL, 0x85EFBE38L,
    0xDBFC821CL, 0x2997011FL, 0x3AC7F2EBL, 0xC8AC71E8L,
    0x1C661503L, 0xEE0D9600L, 0xFD5D65F4L, 0x0F36E6F7L,
    0x61C69362L, 0x93AD1061L, 0x80FDE395L, 0x72966096L,
    0xA65C047DL, 0x5437877EL, 0x4767748AL, 0xB50CF789L,
    0xEB1FCBADL, 0x197448AEL, 0x0A24BB5AL, 0xF84F3859L,
    0x2C855CB2L, 0xDEEEDFB1L, 0xCDBE2C45L, 0x3FD5AF46L,
    0x7198540DL, 0x83F3D70EL, 0x90A324FAL, 0x62C8A7F9L,
    0xB602C312L, 0x44694011L, 0x5739B3E5L, 0xA55230E6L,
    0xFB410CC2L, 0x092A8FC1L, 0x1A7A7C35L, 0xE811FF36L,
    0x3CDB9BDDL, 0xCEB018DEL, 0xDDE0EB2AL, 0x2F8B6829L,
    0x82F63B78L, 0x709DB87BL, 0x63CD4B8FL, 0x91A6C88CL,
    0x456CAC67L, 0xB7072F64L, 0xA457DC90L, 0x563C5F93L,
    0x082F63B7L, 0xFA44E0B4L, 0xE9141340L, 0x1B7F9043L,
    0xCFB5F4A8L, 0x3DDE77ABL, 0x2E8E845FL, 0xDCE5075CL,
    0x92A8FC17L, 0x60C37F14L, 0x73938CE0L, 0x81F80FE3L,
    0x55326B08L, 0xA759E80BL, 0xB4091BFFL, 0x466298FCL,
    0x1871A4D8L, 0xEA1A27DBL, 0xF94AD42FL, 0x0B21572CL,
    0xDFEB33C7L, 0x2D80B0C4L, 0x3ED04330L, 0xCCBBC033L,
    0xA24BB5A6L, 0x502036A5L, 0x4370C551L, 0xB11B4652L,
    0x65D122B9L, 0x97BAA1BAL, 0x84EA524EL, 0x7681D14DL,
    0x2892ED69L, 0xDAF96E6AL, 0xC9A99D9EL, 0x3BC21E9DL,
    0xEF087A76L, 0x1D63F975L, 0x0E330A81L, 0xFC588982L,
    0xB21572C9L, 0x407EF1CAL, 0x532E023EL, 0xA145813DL,
    0x758FE5D6L, 0x87E466D5L, 0x94B49521L, 0x66DF1622L,
    0x38CC2A06L, 0xCAA7A905L, 0xD9F75AF1L, 0x2B9CD9F2L,
    0xFF56BD19L, 0x0D3D3E1AL, 0x1E6DCDEEL, 0xEC064EEDL,
    0xC38D26C4L, 0x31E6A5C7L, 0x22B65633L, 0xD0DDD530L,
    0x0417B1DBL, 0xF67C32D8L, 0xE52CC12CL, 0x1747422FL,
    0x49547E0BL, 0xBB3FFD08L, 0xA86F0EFCL, 0x5A048DFFL,
    0x8ECEE914L, 0x7CA56A17L, 0x6FF599E3L, 0x9D9E1AE0L,
    0xD3D3E1ABL, 0x21B862A8L, 0x32E8915CL, 0xC083125FL,
    0x144976B4L, 0xE622F5B7L, 0xF5720643L, 0x07198540L,
    0x590AB964L, 0xAB613A67L, 0xB831C993L, 0x4A5A4A90L,
    0x9E902E7BL, 0x6CFBAD78L, 0x7FAB5E8CL, 0x8DC0DD8FL,
    0xE330A81AL, 0x115B2B19L, 0x020BD8EDL, 0xF0605BEEL,
    0x24AA3F05L, 0xD6C1BC06L, 0xC5914FF2L, 0x37FACCF1L,
    0x69E9F0D5L, 0x9B8273D6L, 0x88D28022L, 0x7AB90321L,
    0xAE7367CAL, 0x5C18E4C9L, 0x4F48173DL, 0xBD23943EL,
    0xF36E6F75L, 0x0105EC76L, 0x12551F82L, 0xE03E9C81L,
    0x34F4F86AL, 0xC69F7B69L, 0xD5CF889DL, 0x27A40B9EL,
    0x79B737BAL, 0x8BDCB4B9L, 0x988C474DL, 0x6AE7C44EL,
    0xBE2DA0A5L, 0x4C4623A6L, 0x5F16D052L, 0xAD7D5351L
};
static uint32_t crc32c(uint32_t crc, const uint8_t *data, unsigned int length)
{
    while (length--) {
        crc = crc32c_table[(crc ^ *data++) & 0xFFL] ^ (crc >> 8);
    }
    return crc^0xffffffff;
}

target_ulong helper_crc32c(target_ulong val, target_ulong m, uint64_t sz)
{
    uint8_t buf[8];
    target_ulong mask = ((sz * 8) == 64) ? -1ULL : ((1ULL << (sz * 8)) - 1);
    m &= mask;
    stq_le_p(buf, m);
    return (int32_t) (crc32c(val, buf, sz) ^ 0xffffffff);
}
static bool trans_crc_w_b_w(CPULoongArchState *env, arg_crc_w_b_w *restrict a) {env->gpr[a->rd] = helper_crc32(env->gpr[a->rk], env->gpr[a->rj], 1);env->pc += 4;return true;}
static bool trans_crc_w_h_w(CPULoongArchState *env, arg_crc_w_h_w *restrict a) {env->gpr[a->rd] = helper_crc32(env->gpr[a->rk], env->gpr[a->rj], 2);env->pc += 4;return true;}
static bool trans_crc_w_w_w(CPULoongArchState *env, arg_crc_w_w_w *restrict a) {env->gpr[a->rd] = helper_crc32(env->gpr[a->rk], env->gpr[a->rj], 4);env->pc += 4;return true;}
static bool trans_crc_w_d_w(CPULoongArchState *env, arg_crc_w_d_w *restrict a) {env->gpr[a->rd] = helper_crc32(env->gpr[a->rk], env->gpr[a->rj], 8);env->pc += 4;return true;}
static bool trans_crcc_w_b_w(CPULoongArchState *env, arg_crcc_w_b_w *restrict a) {env->gpr[a->rd] = helper_crc32c(env->gpr[a->rk], env->gpr[a->rj], 1);env->pc += 4;return true;}
static bool trans_crcc_w_h_w(CPULoongArchState *env, arg_crcc_w_h_w *restrict a) {env->gpr[a->rd] = helper_crc32c(env->gpr[a->rk], env->gpr[a->rj], 2);env->pc += 4;return true;}
static bool trans_crcc_w_w_w(CPULoongArchState *env, arg_crcc_w_w_w *restrict a) {env->gpr[a->rd] = helper_crc32c(env->gpr[a->rk], env->gpr[a->rj], 4);env->pc += 4;return true;}
static bool trans_crcc_w_d_w(CPULoongArchState *env, arg_crcc_w_d_w *restrict a) {env->gpr[a->rd] = helper_crc32c(env->gpr[a->rk], env->gpr[a->rj], 8);env->pc += 4;return true;}
static bool trans_break(CPULoongArchState *env, arg_break *restrict a) {

#if defined(CONFIG_USER_ONLY)
    fprintf(stderr, "trans_break\n");
    laemu_exit(0);
#else
    do_raise_exception(env, EXCCODE_BRK, 0);
#endif
    return true;
}


static bool trans_syscall(CPULoongArchState *env, arg_syscall *restrict a) {
    env->syscall_count ++;
#if defined(CONFIG_USER_ONLY)
    target_long ret = do_syscall(env, env->gpr[11],
                        env->gpr[4], env->gpr[5],
                        env->gpr[6], env->gpr[7],
                        env->gpr[8], env->gpr[9],
                        -1, -1);
    env->gpr[4] = ret;
    env->pc += 4;
#else
    do_raise_exception(env, EXCCODE_SYS, 0);
#endif
    return true;
}
static bool trans_asrtle_d(CPULoongArchState *env, arg_asrtle_d *restrict a) {
    if (env->gpr[a->rj] > env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    env->pc += 4;
    return true;
}
static bool trans_asrtgt_d(CPULoongArchState *env, arg_asrtgt_d *restrict a) {
    if (env->gpr[a->rj] <= env->gpr[a->rk]) {
        env->CSR_BADV = env->gpr[a->rj];
        do_raise_exception(env, EXCCODE_BCE, 0);
    }
    env->pc += 4;
    return true;
}
static bool trans_rdtimel_w(CPULoongArchState *env, arg_rdtimel_w *restrict a) {
#ifndef CONFIG_DIFF
    long long tval = la_get_tval(env);
    gen_set_gpr(env, a->rd, tval, EXT_SIGN);
#else
    gen_set_gpr(env, a->rd, 0, EXT_SIGN);
#endif
    env->gpr[a->rj] = 0;
    env->pc += 4;
    return true;
}
static bool trans_rdtimeh_w(CPULoongArchState *env, arg_rdtimeh_w *restrict a) {
#ifndef CONFIG_DIFF
    long long tval = la_get_tval(env);
    gen_set_gpr(env, a->rd, tval >> 32, EXT_SIGN);
#else
    gen_set_gpr(env, a->rd, 0, EXT_SIGN);
#endif
    env->gpr[a->rj] = 0;
    env->pc += 4;
    return true;
}
static bool trans_rdtime_d(CPULoongArchState *env, arg_rdtime_d *restrict a) {
#ifndef CONFIG_DIFF
    env->gpr[a->rd] = la_get_tval(env);
#else
    env->gpr[a->rd] = 0;
#endif
    env->gpr[a->rj] = 0;
    env->pc += 4;
    return true;
}
static bool trans_cpucfg(CPULoongArchState *env, arg_cpucfg *restrict a) {
    int index = env->gpr[a->rj];
    env->gpr[a->rd] = index >= ARRAY_SIZE(env->cpucfg) ? 0 : env->cpucfg[index];
    env->pc += 4;
    return true;
}
static bool trans_beqz(CPULoongArchState *env, arg_beqz *restrict a) {
    PERF_INC(COUNTER_INST_BRANCH);
    if ((is_la64(env) ? env->gpr[a->rj] : (uint32_t)env->gpr[a->rj]) == 0) {
        env->pc += a->offs;
    } else {
        env->pc += 4;
    }
    return true;
}
static bool trans_bnez(CPULoongArchState *env, arg_bnez *restrict a) {
    PERF_INC(COUNTER_INST_BRANCH);
    if ((is_la64(env) ? env->gpr[a->rj] : (uint32_t)env->gpr[a->rj]) != 0) {
        env->pc += a->offs;
    } else {
        env->pc += 4;
    }
    return true;
}
static bool trans_bceqz(CPULoongArchState *env, arg_bceqz *restrict a) {
    PERF_INC(COUNTER_INST_BRANCH);
    CHECK_FPE(8);
    if (env->cf[a->cj] == 0) {
        env->pc += a->offs;
    } else {
        env->pc += 4;
    }
    return true;
}
static bool trans_bcnez(CPULoongArchState *env, arg_bcnez *restrict a) {
    PERF_INC(COUNTER_INST_BRANCH);
    CHECK_FPE(8);
    if (env->cf[a->cj] != 0) {
        env->pc += a->offs;
    } else {
        env->pc += 4;
    }
    return true;
}
static bool trans_jirl(CPULoongArchState *env, arg_jirl *restrict a) {
    PERF_INC(COUNTER_INST_BRANCH);
    uint64_t old_pc = env->pc;
    env->pc = env->gpr[a->rj] + a->imm;
    env->gpr[a->rd] = old_pc + 4;
    return true;
}
static bool trans_b(CPULoongArchState *env, arg_b *restrict a) {
    PERF_INC(COUNTER_INST_BRANCH);
#ifndef CONFIG_DIFF
    if (!a->offs) {
        laemu_exit(EXIT_SUCCESS);
    }
#endif
    env->pc += a->offs;
    return true;
}
static bool trans_bl(CPULoongArchState *env, arg_bl *restrict a) {
    PERF_INC(COUNTER_INST_BRANCH);
    env->gpr[1] = env->pc + 4;
    env->pc += a->offs;
    return true;
}
static bool trans_beq(CPULoongArchState *env, arg_beq *restrict a) {
    PERF_INC(COUNTER_INST_BRANCH);
    if (is_la64(env) ? (env->gpr[a->rj] == env->gpr[a->rd])
                     : ((uint32_t)env->gpr[a->rj] == (uint32_t)env->gpr[a->rd])) {
        env->pc += a->offs;
    } else {
        env->pc += 4;
    }
    return true;
}
static bool trans_bne(CPULoongArchState *env, arg_bne *restrict a) {
    PERF_INC(COUNTER_INST_BRANCH);
    if (is_la64(env) ? (env->gpr[a->rj] != env->gpr[a->rd])
                     : ((uint32_t)env->gpr[a->rj] != (uint32_t)env->gpr[a->rd])) {
        env->pc += a->offs;
    } else {
        env->pc += 4;
    }
    return true;
}
static bool trans_blt(CPULoongArchState *env, arg_blt *restrict a) {
    PERF_INC(COUNTER_INST_BRANCH);
    if (is_la64(env) ? ((int64_t)env->gpr[a->rj] < (int64_t)env->gpr[a->rd])
                     : ((int32_t)env->gpr[a->rj] < (int32_t)env->gpr[a->rd])) {
        env->pc += a->offs;
    } else {
        env->pc += 4;
    }
    return true;
}
static bool trans_bge(CPULoongArchState *env, arg_bge *restrict a) {
    PERF_INC(COUNTER_INST_BRANCH);
    if (is_la64(env) ? ((int64_t)env->gpr[a->rj] >= (int64_t)env->gpr[a->rd])
                     : ((int32_t)env->gpr[a->rj] >= (int32_t)env->gpr[a->rd])) {
        env->pc += a->offs;
    } else {
        env->pc += 4;
    }
    return true;
}
static bool trans_bltu(CPULoongArchState *env, arg_bltu *restrict a) {
    PERF_INC(COUNTER_INST_BRANCH);
    if (is_la64(env) ? ((uint64_t)env->gpr[a->rj] < (uint64_t)env->gpr[a->rd])
                     : ((uint32_t)env->gpr[a->rj] < (uint32_t)env->gpr[a->rd])) {
        env->pc += a->offs;
    } else {
        env->pc += 4;
    }
    return true;
}
static bool trans_bgeu(CPULoongArchState *env, arg_bgeu *restrict a) {
    PERF_INC(COUNTER_INST_BRANCH);
    if (is_la64(env) ? ((uint64_t)env->gpr[a->rj] >= (uint64_t)env->gpr[a->rd])
                     : ((uint32_t)env->gpr[a->rj] >= (uint32_t)env->gpr[a->rd])) {
        env->pc += a->offs;
    } else {
        env->pc += 4;
    }
    return true;
}

#ifdef CONFIG_USER_ONLY
static bool trans_csrrd(CPULoongArchState *env, arg_csrrd *restrict a) {return false;}
static bool trans_csrwr(CPULoongArchState *env, arg_csrwr *restrict a) {return false;}
static bool trans_csrxchg(CPULoongArchState *env, arg_csrxchg *restrict a) {return false;}
static bool trans_iocsrrd_b(CPULoongArchState *env, arg_iocsrrd_b *restrict a) {return false;}
static bool trans_iocsrrd_h(CPULoongArchState *env, arg_iocsrrd_h *restrict a) {return false;}
static bool trans_iocsrrd_w(CPULoongArchState *env, arg_iocsrrd_w *restrict a) {return false;}
static bool trans_iocsrrd_d(CPULoongArchState *env, arg_iocsrrd_d *restrict a) {return false;}
static bool trans_iocsrwr_b(CPULoongArchState *env, arg_iocsrwr_b *restrict a) {return false;}
static bool trans_iocsrwr_h(CPULoongArchState *env, arg_iocsrwr_h *restrict a) {return false;}
static bool trans_iocsrwr_w(CPULoongArchState *env, arg_iocsrwr_w *restrict a) {return false;}
static bool trans_iocsrwr_d(CPULoongArchState *env, arg_iocsrwr_d *restrict a) {return false;}
static bool trans_tlbsrch(CPULoongArchState *env, arg_tlbsrch *restrict a) {return false;}
static bool trans_tlbrd(CPULoongArchState *env, arg_tlbrd *restrict a) {return false;}
static bool trans_tlbwr(CPULoongArchState *env, arg_tlbwr *restrict a) {return false;}
static bool trans_tlbfill(CPULoongArchState *env, arg_tlbfill *restrict a) {return false;}
static bool trans_tlbclr(CPULoongArchState *env, arg_tlbclr *restrict a) {return false;}
static bool trans_tlbflush(CPULoongArchState *env, arg_tlbflush *restrict a) {return false;}
static bool trans_invtlb(CPULoongArchState *env, arg_invtlb *restrict a) {return false;}
static bool trans_cacop(CPULoongArchState *env, arg_cacop *restrict a) {return false;}
static bool trans_ldpte(CPULoongArchState *env, arg_ldpte *restrict a) {return false;}
static bool trans_lddir(CPULoongArchState *env, arg_lddir *restrict a) {return false;}
static bool trans_ertn(CPULoongArchState *env, arg_ertn *restrict a) {return false;}
static bool trans_dbcl(CPULoongArchState *env, arg_dbcl *restrict a) {return false;}
static bool trans_idle(CPULoongArchState *env, arg_idle *restrict a) {return false;}

#else

/* Read a CSR register by index. Returns the current value. */
uint64_t helper_read_csr(CPULoongArchState *env, int csr_index) {
    uint64_t old_v = 0;
    switch (csr_index) {
        case LOONGARCH_CSR_CRMD           :old_v = env->CSR_CRMD; break;
        case LOONGARCH_CSR_PRMD           :old_v = env->CSR_PRMD; break;
        case LOONGARCH_CSR_EUEN           :old_v = env->CSR_EUEN; break;
        case LOONGARCH_CSR_MISC           :old_v = env->CSR_MISC; break;
        case LOONGARCH_CSR_ECFG           :old_v = env->CSR_ECFG; break;
        case LOONGARCH_CSR_ESTAT          :old_v = env->CSR_ESTAT; break;
        case LOONGARCH_CSR_ERA            :old_v = env->CSR_ERA; break;
        case LOONGARCH_CSR_BADV           :old_v = env->CSR_BADV; break;
        case LOONGARCH_CSR_BADI           :old_v = env->CSR_BADI; break;
        case LOONGARCH_CSR_EENTRY         :old_v = env->CSR_EENTRY; break;
        case LOONGARCH_CSR_TLBIDX         :old_v = sextract64(env->CSR_TLBIDX, 0, 32); break;
        case LOONGARCH_CSR_TLBEHI         :old_v = sextract64(env->CSR_TLBEHI, 0, FIELD_EX64(env->cpucfg[1], CPUCFG1, VALEN) + 1); break;
        case LOONGARCH_CSR_TLBELO0        :old_v = env->CSR_TLBELO0; break;
        case LOONGARCH_CSR_TLBELO1        :old_v = env->CSR_TLBELO1; break;
        case LOONGARCH_CSR_ASID           :old_v = env->CSR_ASID; break;
        case LOONGARCH_CSR_PGDL           :old_v = env->CSR_PGDL; break;
        case LOONGARCH_CSR_PGDH           :old_v = env->CSR_PGDH; break;
        case LOONGARCH_CSR_PGD            :old_v = helper_csrrd_pgd(env); break;
        case LOONGARCH_CSR_PWCL           :old_v = sextract64(env->CSR_PWCL, 0, 32); break;
        case LOONGARCH_CSR_PWCH           :old_v = env->CSR_PWCH; break;
        case LOONGARCH_CSR_STLBPS         :old_v = env->CSR_STLBPS; break;
        case LOONGARCH_CSR_RVACFG         :old_v = env->CSR_RVACFG; break;
        case LOONGARCH_CSR_CPUID          :old_v = env->CSR_CPUID; break;
        case LOONGARCH_CSR_PRCFG1         :old_v = env->CSR_PRCFG1; break;
        case LOONGARCH_CSR_PRCFG2         :old_v = env->CSR_PRCFG2; break;
        case LOONGARCH_CSR_PRCFG3         :old_v = env->CSR_PRCFG3; break;
        case LOONGARCH_CSR_SAVE(0)        :old_v = env->CSR_SAVE[0]; break;
        case LOONGARCH_CSR_SAVE(1)        :old_v = env->CSR_SAVE[1]; break;
        case LOONGARCH_CSR_SAVE(2)        :old_v = env->CSR_SAVE[2]; break;
        case LOONGARCH_CSR_SAVE(3)        :old_v = env->CSR_SAVE[3]; break;
        case LOONGARCH_CSR_SAVE(4)        :old_v = env->CSR_SAVE[4]; break;
        case LOONGARCH_CSR_SAVE(5)        :old_v = env->CSR_SAVE[5]; break;
        case LOONGARCH_CSR_SAVE(6)        :old_v = env->CSR_SAVE[6]; break;
        case LOONGARCH_CSR_SAVE(7)        :old_v = env->CSR_SAVE[7]; break;
        case LOONGARCH_CSR_SAVE(8)        :old_v = env->CSR_SAVE[8]; break;
        case LOONGARCH_CSR_SAVE(9)        :old_v = env->CSR_SAVE[9]; break;
        case LOONGARCH_CSR_SAVE(10)       :old_v = env->CSR_SAVE[10]; break;
        case LOONGARCH_CSR_SAVE(11)       :old_v = env->CSR_SAVE[11]; break;
        case LOONGARCH_CSR_SAVE(12)       :old_v = env->CSR_SAVE[12]; break;
        case LOONGARCH_CSR_SAVE(13)       :old_v = env->CSR_SAVE[13]; break;
        case LOONGARCH_CSR_SAVE(14)       :old_v = env->CSR_SAVE[14]; break;
        case LOONGARCH_CSR_SAVE(15)       :old_v = env->CSR_SAVE[15]; break;
        case LOONGARCH_CSR_TID            :old_v = sextract64(env->CSR_TID, 0, 32); break;
        case LOONGARCH_CSR_TCFG           :old_v = env->CSR_TCFG; break;
        case LOONGARCH_CSR_TVAL           :old_v = env->timer_counter; break;
        case LOONGARCH_CSR_CNTC           :old_v = env->CSR_CNTC; break;
        case LOONGARCH_CSR_TICLR          :old_v = 0; break;
        case LOONGARCH_CSR_LLBCTL         :old_v = env->CSR_LLBCTL; break;
        case LOONGARCH_CSR_IMPCTL1        :old_v = env->CSR_IMPCTL1; break;
        case LOONGARCH_CSR_IMPCTL2        :old_v = env->CSR_IMPCTL2; break;
        case LOONGARCH_CSR_TLBRENTRY      :old_v = env->CSR_TLBRENTRY; break;
        case LOONGARCH_CSR_TLBRBADV       :old_v = env->CSR_TLBRBADV; break;
        case LOONGARCH_CSR_TLBRERA        :old_v = env->CSR_TLBRERA; break;
        case LOONGARCH_CSR_TLBRSAVE       :old_v = env->CSR_TLBRSAVE; break;
        case LOONGARCH_CSR_TLBRELO0       :old_v = env->CSR_TLBRELO0; break;
        case LOONGARCH_CSR_TLBRELO1       :old_v = env->CSR_TLBRELO1; break;
        case LOONGARCH_CSR_TLBREHI        :old_v = sextract64(env->CSR_TLBREHI, 0, FIELD_EX64(env->cpucfg[1], CPUCFG1, VALEN) + 1); break;
        case LOONGARCH_CSR_TLBRPRMD       :old_v = env->CSR_TLBRPRMD; break;
        case LOONGARCH_CSR_MERRCTL        :old_v = env->CSR_MERRCTL; break;
        case LOONGARCH_CSR_MERRINFO1      :old_v = env->CSR_MERRINFO1; break;
        case LOONGARCH_CSR_MERRINFO2      :old_v = env->CSR_MERRINFO2; break;
        case LOONGARCH_CSR_MERRENTRY      :old_v = env->CSR_MERRENTRY; break;
        case LOONGARCH_CSR_MERRERA        :old_v = env->CSR_MERRERA; break;
        case LOONGARCH_CSR_MERRSAVE       :old_v = env->CSR_MERRSAVE; break;
        case LOONGARCH_CSR_CTAG           :old_v = env->CSR_CTAG; break;
        case LOONGARCH_CSR_DMW(0)         :old_v = env->CSR_DMW[0]; break;
        case LOONGARCH_CSR_DMW(1)         :old_v = env->CSR_DMW[1]; break;
        case LOONGARCH_CSR_DMW(2)         :old_v = env->CSR_DMW[2]; break;
        case LOONGARCH_CSR_DMW(3)         :old_v = env->CSR_DMW[3]; break;
        case LOONGARCH_CSR_DBG            :old_v = env->CSR_DBG; break;
        case LOONGARCH_CSR_DERA           :old_v = env->CSR_DERA; break;
        case LOONGARCH_CSR_DSAVE          :old_v = env->CSR_DSAVE; break;
        default:
            fprintf(stderr, "NOT IMPLEMENTED %s %x\n", __func__, csr_index);
    }
    return old_v;
}

static bool trans_csrrd(CPULoongArchState *env, arg_csrrd *restrict a) {
    CHECK_PLV(0);
    env->gpr[a->rd] = helper_read_csr(env, a->csr);
    switch (a->csr)
    {
    case LOONGARCH_CSR_TID:    qemu_log_mask(CPU_LOG_TIMER, "%s pc:%lx CSR_TID:%lx\n", __func__, env->pc, env->CSR_TID); break;
    case LOONGARCH_CSR_TCFG:   qemu_log_mask(CPU_LOG_TIMER, "%s pc:%lx CSR_TCFG:%lx\n", __func__, env->pc, env->CSR_TCFG); break;
    case LOONGARCH_CSR_TVAL:   qemu_log_mask(CPU_LOG_TIMER, "%s pc:%lx CSR_TVAL:%lx\n", __func__, env->pc, env->CSR_TVAL); break;
    case LOONGARCH_CSR_CNTC:   qemu_log_mask(CPU_LOG_TIMER, "%s pc:%lx CSR_CNTC:%lx\n", __func__, env->pc, env->CSR_CNTC); break;
    case LOONGARCH_CSR_TICLR:  qemu_log_mask(CPU_LOG_TIMER, "%s pc:%lx CSR_TICLR:%lx\n", __func__, env->pc, env->CSR_TICLR); break;
    default:
        break;
    }
    env->pc += 4;
    return true;
}

uint64_t mask_write(uint64_t old, uint64_t new, uint64_t mask) {
    return (old & ~mask) | (new & mask);
}

/* Write a CSR register by index with a mask. Returns the old value before the write. */
uint64_t helper_write_csr(CPULoongArchState *env, int csr_index, uint64_t new_v, uint64_t mask) {
    uint64_t old_v = 0;
    switch (csr_index) {
        case LOONGARCH_CSR_CRMD           :old_v = env->CSR_CRMD; env->CSR_CRMD = mask_write(env->CSR_CRMD, new_v, mask & LOONGARCH_CSR_CRMD_WMASK); cpu_clear_tc(env); break;
        case LOONGARCH_CSR_PRMD           :old_v = env->CSR_PRMD; env->CSR_PRMD = mask_write(env->CSR_PRMD, new_v, mask & LOONGARCH_CSR_PRMD_WMASK); break;
        case LOONGARCH_CSR_EUEN: {
            uint64_t wmask = LOONGARCH_CSR_EUEN_WMASK;
            if (!is_la64(env)) wmask &= ~(R_CSR_EUEN_SXE_MASK | R_CSR_EUEN_ASXE_MASK);
            old_v = env->CSR_EUEN; env->CSR_EUEN = mask_write(env->CSR_EUEN, new_v, mask & wmask);
            break;
        }
        case LOONGARCH_CSR_MISC           :old_v = env->CSR_MISC; env->CSR_MISC = mask_write(env->CSR_MISC, new_v, mask & LOONGARCH_CSR_MISC_WMASK); break;
        case LOONGARCH_CSR_ECFG           :old_v = env->CSR_ECFG; env->CSR_ECFG = mask_write(env->CSR_ECFG, new_v, mask & LOONGARCH_CSR_ECFG_WMASK); break;
        case LOONGARCH_CSR_ESTAT          :old_v = env->CSR_ESTAT; env->CSR_ESTAT = mask_write(env->CSR_ESTAT, new_v, mask & LOONGARCH_CSR_ESTAT_WMASK); break;
        case LOONGARCH_CSR_ERA            :old_v = env->CSR_ERA; env->CSR_ERA = mask_write(env->CSR_ERA, new_v, mask); break;
        case LOONGARCH_CSR_BADV           :old_v = env->CSR_BADV; env->CSR_BADV = mask_write(env->CSR_BADV, new_v, mask); break;
        case LOONGARCH_CSR_BADI           :old_v = env->CSR_BADI; break;
        case LOONGARCH_CSR_EENTRY         :old_v = env->CSR_EENTRY; env->CSR_EENTRY = mask_write(env->CSR_EENTRY, new_v, mask & LOONGARCH_CSR_EENTRY_WMASK); break;
        case LOONGARCH_CSR_TLBIDX         :old_v = sextract64(env->CSR_TLBIDX, 0, 32); env->CSR_TLBIDX = mask_write(env->CSR_TLBIDX, new_v, mask & LOONGARCH_CSR_TLBIDX_WMASK); break;
        case LOONGARCH_CSR_TLBEHI         :old_v = sextract64(env->CSR_TLBEHI, 0, FIELD_EX64(env->cpucfg[1], CPUCFG1, VALEN) + 1); env->CSR_TLBEHI = mask_write(env->CSR_TLBEHI, new_v, mask & (is_la64(env) ? LOONGARCH_CSR_TLBEHI_64_WMASK : LOONGARCH_CSR_TLBEHI_32_WMASK)); break;
        case LOONGARCH_CSR_TLBELO0        :old_v = env->CSR_TLBELO0; env->CSR_TLBELO0 = mask_write(env->CSR_TLBELO0, new_v, mask); break;
        case LOONGARCH_CSR_TLBELO1        :old_v = env->CSR_TLBELO1; env->CSR_TLBELO1 = mask_write(env->CSR_TLBELO1, new_v, mask); break;
        case LOONGARCH_CSR_ASID           :old_v = env->CSR_ASID; env->CSR_ASID = mask_write(env->CSR_ASID, new_v, mask & LOONGARCH_CSR_ASID_WMASK); cpu_clear_tc(env); break;
        case LOONGARCH_CSR_PGDL           :old_v = env->CSR_PGDL; env->CSR_PGDL = mask_write(env->CSR_PGDL, new_v, mask & LOONGARCH_CSR_PGDL_WMASK); break;
        case LOONGARCH_CSR_PGDH           :old_v = env->CSR_PGDH; env->CSR_PGDH = mask_write(env->CSR_PGDH, new_v, mask & LOONGARCH_CSR_PGDH_WMASK); break;
        case LOONGARCH_CSR_PGD            :old_v = helper_csrrd_pgd(env); break;
        case LOONGARCH_CSR_PWCL           :old_v = sextract64(env->CSR_PWCL, 0, 32); env->CSR_PWCL = mask_write(env->CSR_PWCL, new_v, mask & LOONGARCH_CSR_PWCL_WMASK); break;
        case LOONGARCH_CSR_PWCH           :old_v = env->CSR_PWCH; env->CSR_PWCH = mask_write(env->CSR_PWCH, new_v, mask & LOONGARCH_CSR_PWCH_WMASK); break;
        case LOONGARCH_CSR_STLBPS         :old_v = env->CSR_STLBPS; env->CSR_STLBPS = mask_write(env->CSR_STLBPS, new_v, mask & LOONGARCH_CSR_STLBPS_WMASK); cpu_clear_tc(env); break;
        case LOONGARCH_CSR_RVACFG         :old_v = env->CSR_RVACFG; env->CSR_RVACFG = mask_write(env->CSR_RVACFG, new_v, mask & LOONGARCH_CSR_RVACFG_WMASK); break;
        case LOONGARCH_CSR_CPUID          :old_v = env->CSR_CPUID; break;
        case LOONGARCH_CSR_PRCFG1         :old_v = env->CSR_PRCFG1; break;
        case LOONGARCH_CSR_PRCFG2         :old_v = env->CSR_PRCFG2; break;
        case LOONGARCH_CSR_PRCFG3         :old_v = env->CSR_PRCFG3; break;
        case LOONGARCH_CSR_SAVE(0)        :old_v = env->CSR_SAVE[0]; env->CSR_SAVE[0] = mask_write(env->CSR_SAVE[0], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(1)        :old_v = env->CSR_SAVE[1]; env->CSR_SAVE[1] = mask_write(env->CSR_SAVE[1], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(2)        :old_v = env->CSR_SAVE[2]; env->CSR_SAVE[2] = mask_write(env->CSR_SAVE[2], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(3)        :old_v = env->CSR_SAVE[3]; env->CSR_SAVE[3] = mask_write(env->CSR_SAVE[3], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(4)        :old_v = env->CSR_SAVE[4]; env->CSR_SAVE[4] = mask_write(env->CSR_SAVE[4], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(5)        :old_v = env->CSR_SAVE[5]; env->CSR_SAVE[5] = mask_write(env->CSR_SAVE[5], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(6)        :old_v = env->CSR_SAVE[6]; env->CSR_SAVE[6] = mask_write(env->CSR_SAVE[6], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(7)        :old_v = env->CSR_SAVE[7]; env->CSR_SAVE[7] = mask_write(env->CSR_SAVE[7], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(8)        :old_v = env->CSR_SAVE[8]; env->CSR_SAVE[8] = mask_write(env->CSR_SAVE[8], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(9)        :old_v = env->CSR_SAVE[9]; env->CSR_SAVE[9] = mask_write(env->CSR_SAVE[9], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(10)       :old_v = env->CSR_SAVE[10]; env->CSR_SAVE[10] = mask_write(env->CSR_SAVE[10], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(11)       :old_v = env->CSR_SAVE[11]; env->CSR_SAVE[11] = mask_write(env->CSR_SAVE[11], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(12)       :old_v = env->CSR_SAVE[12]; env->CSR_SAVE[12] = mask_write(env->CSR_SAVE[12], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(13)       :old_v = env->CSR_SAVE[13]; env->CSR_SAVE[13] = mask_write(env->CSR_SAVE[13], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(14)       :old_v = env->CSR_SAVE[14]; env->CSR_SAVE[14] = mask_write(env->CSR_SAVE[14], new_v, mask); break;
        case LOONGARCH_CSR_SAVE(15)       :old_v = env->CSR_SAVE[15]; env->CSR_SAVE[15] = mask_write(env->CSR_SAVE[15], new_v, mask); break;
        case LOONGARCH_CSR_TID            :old_v = sextract64(env->CSR_TID, 0, 32); env->CSR_TID = mask_write(env->CSR_TID, new_v, mask & LOONGARCH_CSR_TID_WMASK); break;
        case LOONGARCH_CSR_TCFG           :old_v = env->CSR_TCFG; env->CSR_TCFG = mask_write(env->CSR_TCFG, new_v, mask);
#ifndef CONFIG_DIFF
            if (env->CSR_TCFG & 1) {
                if (determined) {
                    env->timer_counter = (env->CSR_TCFG & CONSTANT_TIMER_TICK_MASK) / TIME_SCALE;
                } else {
                    cpu_settimer(env, env->CSR_TCFG & CONSTANT_TIMER_TICK_MASK);
                }
            } else {
                if (determined) {
                    env->timer_counter = -1;
                } else {
                    cpu_disable_timer(env);
                }
            }
#endif
            break;
        case LOONGARCH_CSR_TVAL           :old_v = env->CSR_TVAL; break;
        case LOONGARCH_CSR_CNTC           :old_v = env->CSR_CNTC; env->CSR_CNTC = mask_write(env->CSR_CNTC, new_v, mask); break;
        case LOONGARCH_CSR_TICLR          :old_v = 0;
            if (new_v & mask & 1) {
                env->timer_int = 0;
                loongarch_cpu_set_irq(env, IRQ_TIMER, 0);
            }
        break;
        case LOONGARCH_CSR_LLBCTL         :old_v = env->CSR_LLBCTL;
            if (new_v & mask & R_CSR_LLBCTL_WCLLB_MASK) {
                env->CSR_LLBCTL = FIELD_DP64(env->CSR_LLBCTL, CSR_LLBCTL, ROLLB, 0);
            }
            if (mask & R_CSR_LLBCTL_KLO_MASK) {
                env->CSR_LLBCTL = FIELD_DP64(env->CSR_LLBCTL, CSR_LLBCTL, KLO, (new_v & R_CSR_LLBCTL_KLO_MASK) >> R_CSR_LLBCTL_KLO_SHIFT);
            }
        break;
        case LOONGARCH_CSR_IMPCTL1        :old_v = env->CSR_IMPCTL1; env->CSR_IMPCTL1 = mask_write(env->CSR_IMPCTL1, new_v, mask); break;
        case LOONGARCH_CSR_IMPCTL2        :old_v = env->CSR_IMPCTL2; env->CSR_IMPCTL2 = mask_write(env->CSR_IMPCTL2, new_v, mask); break;
        case LOONGARCH_CSR_TLBRENTRY      :old_v = env->CSR_TLBRENTRY; env->CSR_TLBRENTRY = mask_write(env->CSR_TLBRENTRY, new_v, mask & (is_la64(env) ? (is_la64(env)?LOONGARCH_CSR_TLBRENTRY_64_WMASK:LOONGARCH_CSR_TLBRENTRY_32_WMASK) : LOONGARCH_CSR_TLBRENTRY_32_WMASK)); break;
        case LOONGARCH_CSR_TLBRBADV       :old_v = env->CSR_TLBRBADV; env->CSR_TLBRBADV = mask_write(env->CSR_TLBRBADV, new_v, mask); break;
        case LOONGARCH_CSR_TLBRERA        :old_v = env->CSR_TLBRERA; env->CSR_TLBRERA = mask_write(env->CSR_TLBRERA, new_v, mask & LOONGARCH_CSR_TLBRERA_WMASK); break;
        case LOONGARCH_CSR_TLBRSAVE       :old_v = env->CSR_TLBRSAVE; env->CSR_TLBRSAVE = mask_write(env->CSR_TLBRSAVE, new_v, mask); break;
        case LOONGARCH_CSR_TLBRELO0       :old_v = env->CSR_TLBRELO0; env->CSR_TLBRELO0 = mask_write(env->CSR_TLBRELO0, new_v, mask); break;
        case LOONGARCH_CSR_TLBRELO1       :old_v = env->CSR_TLBRELO1; env->CSR_TLBRELO1 = mask_write(env->CSR_TLBRELO1, new_v, mask); break;
        case LOONGARCH_CSR_TLBREHI        :old_v = sextract64(env->CSR_TLBREHI, 0, FIELD_EX64(env->cpucfg[1], CPUCFG1, VALEN) + 1); env->CSR_TLBREHI = mask_write(env->CSR_TLBREHI, new_v, mask & (is_la64(env) ? LOONGARCH_CSR_TLBREHI_64_WMASK : LOONGARCH_CSR_TLBREHI_32_WMASK)); break;
        case LOONGARCH_CSR_TLBRPRMD       :old_v = env->CSR_TLBRPRMD; env->CSR_TLBRPRMD = mask_write(env->CSR_TLBRPRMD, new_v, mask & LOONGARCH_CSR_TLBRPRMD_WMASK); break;
        case LOONGARCH_CSR_MERRCTL        :old_v = env->CSR_MERRCTL; env->CSR_MERRCTL = mask_write(env->CSR_MERRCTL, new_v, mask); break;
        case LOONGARCH_CSR_MERRINFO1      :old_v = env->CSR_MERRINFO1; env->CSR_MERRINFO1 = mask_write(env->CSR_MERRINFO1, new_v, mask); break;
        case LOONGARCH_CSR_MERRINFO2      :old_v = env->CSR_MERRINFO2; env->CSR_MERRINFO2 = mask_write(env->CSR_MERRINFO2, new_v, mask); break;
        case LOONGARCH_CSR_MERRENTRY      :old_v = env->CSR_MERRENTRY; env->CSR_MERRENTRY = mask_write(env->CSR_MERRENTRY, new_v, mask); break;
        case LOONGARCH_CSR_MERRERA        :old_v = env->CSR_MERRERA; env->CSR_MERRERA = mask_write(env->CSR_MERRERA, new_v, mask); break;
        case LOONGARCH_CSR_MERRSAVE       :old_v = env->CSR_MERRSAVE; env->CSR_MERRSAVE = mask_write(env->CSR_MERRSAVE, new_v, mask); break;
        case LOONGARCH_CSR_CTAG           :old_v = env->CSR_CTAG; env->CSR_CTAG = mask_write(env->CSR_CTAG, new_v, mask); break;
        case LOONGARCH_CSR_DMW(0)         :old_v = env->CSR_DMW[0]; env->CSR_DMW[0] = mask_write(env->CSR_DMW[0], new_v, mask & (is_la64(env) ? (is_la64(env)?LOONGARCH_CSR_DMW_64_WMASK:LOONGARCH_CSR_DMW_32_WMASK) : LOONGARCH_CSR_DMW_32_WMASK)); cpu_clear_tc(env); break;
        case LOONGARCH_CSR_DMW(1)         :old_v = env->CSR_DMW[1]; env->CSR_DMW[1] = mask_write(env->CSR_DMW[1], new_v, mask & (is_la64(env) ? (is_la64(env)?LOONGARCH_CSR_DMW_64_WMASK:LOONGARCH_CSR_DMW_32_WMASK) : LOONGARCH_CSR_DMW_32_WMASK)); cpu_clear_tc(env); break;
        case LOONGARCH_CSR_DMW(2)         :old_v = env->CSR_DMW[2]; env->CSR_DMW[2] = mask_write(env->CSR_DMW[2], new_v, mask & (is_la64(env) ? (is_la64(env)?LOONGARCH_CSR_DMW_64_WMASK:LOONGARCH_CSR_DMW_32_WMASK) : LOONGARCH_CSR_DMW_32_WMASK)); cpu_clear_tc(env); break;
        case LOONGARCH_CSR_DMW(3)         :old_v = env->CSR_DMW[3]; env->CSR_DMW[3] = mask_write(env->CSR_DMW[3], new_v, mask & (is_la64(env) ? (is_la64(env)?LOONGARCH_CSR_DMW_64_WMASK:LOONGARCH_CSR_DMW_32_WMASK) : LOONGARCH_CSR_DMW_32_WMASK)); cpu_clear_tc(env); break;
        case LOONGARCH_CSR_DBG            :old_v = env->CSR_DBG; break;
        case LOONGARCH_CSR_DERA           :old_v = env->CSR_DERA; env->CSR_DERA = mask_write(env->CSR_DERA, new_v, mask); break;
        case LOONGARCH_CSR_DSAVE          :old_v = env->CSR_DSAVE; env->CSR_DSAVE = mask_write(env->CSR_DSAVE, new_v, mask); break;
        case 0x132                        :old_v = 0; break;
        default:
            fprintf(stderr, "NOT IMPLEMENTED %s %x\n", __func__, csr_index);
    }
    switch (csr_index)
    {
    case LOONGARCH_CSR_TID:    qemu_log_mask(CPU_LOG_TIMER, "%s pc:%lx CSR_TID:   %016lx old:%016lx new:%016lx mask:%016lx\n", __func__, env->pc, env->CSR_TID, old_v, new_v, mask); break;
    case LOONGARCH_CSR_TCFG:   qemu_log_mask(CPU_LOG_TIMER, "%s pc:%lx CSR_TCFG:  %016lx old:%016lx new:%016lx mask:%016lx\n", __func__, env->pc, env->CSR_TCFG, old_v, new_v, mask); break;
    case LOONGARCH_CSR_TVAL:   qemu_log_mask(CPU_LOG_TIMER, "%s pc:%lx CSR_TVAL:  %016lx old:%016lx new:%016lx mask:%016lx\n", __func__, env->pc, env->CSR_TVAL, old_v, new_v, mask); break;
    case LOONGARCH_CSR_CNTC:   qemu_log_mask(CPU_LOG_TIMER, "%s pc:%lx CSR_CNTC:  %016lx old:%016lx new:%016lx mask:%016lx\n", __func__, env->pc, env->CSR_CNTC, old_v, new_v, mask); break;
    case LOONGARCH_CSR_TICLR:  qemu_log_mask(CPU_LOG_TIMER, "%s pc:%lx CSR_TICLR: %016lx old:%016lx new:%016lx mask:%016lx\n", __func__, env->pc, env->CSR_TICLR, old_v, new_v, mask); break;
    default:
        break;
    }
    return old_v;
}

static bool trans_csrwr(CPULoongArchState *env, arg_csrwr *restrict a) {
    CHECK_PLV(0);
    env->gpr[a->rd] = helper_write_csr(env, a->csr, env->gpr[a->rd], -1);
    env->pc += 4;
    return true;
}
static bool trans_csrxchg(CPULoongArchState *env, arg_csrxchg *restrict a) {
    CHECK_PLV(0);
    env->gpr[a->rd] = helper_write_csr(env, a->csr, env->gpr[a->rd], env->gpr[a->rj]);
    env->pc += 4;
    return true;
}
static bool trans_iocsrrd_b(CPULoongArchState *env, arg_iocsrrd_b *restrict a) {
    CHECK_PLV(0);
    env->gpr[a->rd] = loongarch_iocsr_read(env, env->gpr[a->rj], 1);
    env->pc += 4;
    return true;
}
static bool trans_iocsrrd_h(CPULoongArchState *env, arg_iocsrrd_h *restrict a) {
    CHECK_PLV(0);
    env->gpr[a->rd] = loongarch_iocsr_read(env, env->gpr[a->rj], 2);
    env->pc += 4;
    return true;
}
static bool trans_iocsrrd_w(CPULoongArchState *env, arg_iocsrrd_w *restrict a) {
    CHECK_PLV(0);
    env->gpr[a->rd] = loongarch_iocsr_read(env, env->gpr[a->rj], 4);
    env->pc += 4;
    return true;
}
static bool trans_iocsrrd_d(CPULoongArchState *env, arg_iocsrrd_d *restrict a) {
    CHECK_PLV(0);
    CHECK_LA64();
    env->gpr[a->rd] = loongarch_iocsr_read(env, env->gpr[a->rj], 8);
    env->pc += 4;
    return true;
}
static bool trans_iocsrwr_b(CPULoongArchState *env, arg_iocsrwr_b *restrict a) {
    CHECK_PLV(0);
    loongarch_iocsr_write(env, env->gpr[a->rj], env->gpr[a->rd], 1);
    env->pc += 4;
    return true;
}
static bool trans_iocsrwr_h(CPULoongArchState *env, arg_iocsrwr_h *restrict a) {
    CHECK_PLV(0);
    loongarch_iocsr_write(env, env->gpr[a->rj], env->gpr[a->rd], 2);
    env->pc += 4;
    return true;
}
static bool trans_iocsrwr_w(CPULoongArchState *env, arg_iocsrwr_w *restrict a) {
    CHECK_PLV(0);
    loongarch_iocsr_write(env, env->gpr[a->rj], env->gpr[a->rd], 4);
    env->pc += 4;
    return true;
}
static bool trans_iocsrwr_d(CPULoongArchState *env, arg_iocsrwr_d *restrict a) {
    CHECK_PLV(0);
    CHECK_LA64();
    loongarch_iocsr_write(env, env->gpr[a->rj], env->gpr[a->rd], 8);
    env->pc += 4;
    return true;
}
static bool trans_tlbsrch(CPULoongArchState *env, arg_tlbsrch *restrict a) {
    CHECK_PLV(0);
    helper_tlbsrch(env);
    env->pc += 4;
    return true;
}
static bool trans_tlbrd(CPULoongArchState *env, arg_tlbrd *restrict a) {
    CHECK_PLV(0);
    helper_tlbrd(env);
    env->pc += 4;
    return true;
}
static bool trans_tlbwr(CPULoongArchState *env, arg_tlbwr *restrict a) {
    CHECK_PLV(0);
    helper_tlbwr(env);
    cpu_clear_tc(env);
    env->pc += 4;
    return true;
}
static bool trans_tlbfill(CPULoongArchState *env, arg_tlbfill *restrict a) {
    CHECK_PLV(0);
    helper_tlbfill(env);
    env->pc += 4;
    return true;
}
static bool trans_tlbclr(CPULoongArchState *env, arg_tlbclr *restrict a) {
    CHECK_PLV(0);
    helper_tlbclr(env);
    cpu_clear_tc(env);
    env->pc += 4;
    return true;
}
static bool trans_tlbflush(CPULoongArchState *env, arg_tlbflush *restrict a) {
    CHECK_PLV(0);
    helper_tlbflush(env);
    cpu_clear_tc(env);
    env->pc += 4;
    return true;
}
static bool trans_invtlb(CPULoongArchState *env, arg_invtlb *restrict a) {
    CHECK_PLV(0);
    helper_invtlb_all(env);
    cpu_clear_tc(env);
    env->pc += 4;
    return true;
}
static bool trans_cacop(CPULoongArchState *env, arg_cacop *restrict a) {
    CHECK_PLV(0);
    env->pc += 4;
    return true;
}
static bool trans_lddir(CPULoongArchState *env, arg_lddir *restrict a) {
    CHECK_PLV(0);
    uint64_t dir_phys_addr;
    env->gpr[a->rd] = helper_lddir(env, env->gpr[a->rj], a->imm, 0, &dir_phys_addr);
    env->pc += 4;
    return true;
}
static bool trans_ldpte(CPULoongArchState *env, arg_ldpte *restrict a) {
    CHECK_PLV(0);
    uint64_t pte_phys_addr;
    helper_ldpte(env, env->gpr[a->rj], a->imm, 0, &pte_phys_addr);
    env->pc += 4;
    return true;
}
static bool trans_ertn(CPULoongArchState *env, arg_ertn *restrict a) {
    CHECK_PLV(0);
    helper_ertn(env);
    cpu_clear_tc(env);
    return true;
}
static bool trans_idle(CPULoongArchState *env, arg_idle *restrict a) {
    CHECK_PLV(0);
#ifndef CONFIG_DIFF
    if (loongarch_smp_cpu_count() > 1 && !loongarch_cpu_has_irq(env)) {
        env_cpu(env)->halted = 1;
        env->pc += 4;
        return true;
    }
    if (!determined) {
        while (loongarch_cpu_check_irq(env), !loongarch_cpu_has_irq(env)) {
            sleep(1);
        }
    }
#endif
    env->pc += 4;
    return true;
}
static bool trans_dbcl(CPULoongArchState *env, arg_dbcl *restrict a) {
    CHECK_PLV(0);
    env->CSR_DBG = FIELD_DP64(env->CSR_DBG, CSR_DBG, DST, 0);
    env->pc += 4;
    return true;
}
#endif
