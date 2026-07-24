/* SPDX-License-Identifier: GPL-2.0-or-later */
/* LoongArch scalar and vector FLOGB bit-level reference helpers. */

#ifndef LOONGARCH_FLOGB_H
#define LOONGARCH_FLOGB_H

static inline uint32_t la_flogb32(uint32_t src, float_status *status)
{
    uint32_t exp_field = (src >> 23) & 0xffU;
    uint32_t fraction = src & 0x7fffffU;
    int unbiased_exp;

    if (exp_field == 0xffU) {
        if (fraction == 0U) {
            return 0x7f800000U;
        }
        if ((fraction & 0x400000U) == 0U) {
            float_raise(float_flag_invalid, status);
        }
        return src | 0x00400000U;
    }
    if (exp_field == 0U) {
        if (fraction == 0U) {
            float_raise(float_flag_divbyzero, status);
            return 0xff800000U;
        }
        unbiased_exp = -126 - (__builtin_clz(fraction) - 8);
    } else {
        unbiased_exp = (int)exp_field - 127;
    }
    return int32_to_float32(unbiased_exp, status);
}

static inline uint64_t la_flogb64(uint64_t src, float_status *status)
{
    uint32_t exp_field = (src >> 52) & 0x7ffU;
    uint64_t fraction = src & UINT64_C(0x000fffffffffffff);
    int unbiased_exp;

    if (exp_field == 0x7ffU) {
        if (fraction == 0U) {
            return UINT64_C(0x7ff0000000000000);
        }
        if ((fraction & UINT64_C(0x0008000000000000)) == 0U) {
            float_raise(float_flag_invalid, status);
        }
        return src | UINT64_C(0x0008000000000000);
    }
    if (exp_field == 0U) {
        if (fraction == 0U) {
            float_raise(float_flag_divbyzero, status);
            return UINT64_C(0xfff0000000000000);
        }
        unbiased_exp = -1022 - (__builtin_clzll(fraction) - 11);
    } else {
        unbiased_exp = (int)exp_field - 1023;
    }
    return int32_to_float64(unbiased_exp, status);
}

#endif
