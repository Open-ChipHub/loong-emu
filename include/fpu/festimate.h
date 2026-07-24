/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * LoongArch reciprocal and reciprocal-square-root estimate model.
 *
 * This model derives the 128-segment Q1.23 interpolation coefficients from
 * the mathematical endpoint values.  It intentionally does not import the
 * RTL ROM, so the reference and implementation keep independent sources of
 * truth while agreeing on the documented estimate algorithm.
 */

#ifndef LOONGARCH_FESTIMATE_H
#define LOONGARCH_FESTIMATE_H

#include <math.h>

#define LA_FESTIMATE_SEGMENTS       128U
#define LA_FESTIMATE_DELTA_BITS     10U
#define LA_FESTIMATE_COEFF_SCALE    8388608.0
#define LA_FESTIMATE_COEFF_FRAC     23

static inline uint32_t la_festimate_coefficient(unsigned int mode,
                                                uint32_t index,
                                                uint32_t delta)
{
    double left = 1.0 + (double)index / LA_FESTIMATE_SEGMENTS;
    double right = 1.0 + (double)(index + 1U) / LA_FESTIMATE_SEGMENTS;
    double left_value;
    double right_value;
    uint32_t base;
    uint32_t span;
    uint64_t product;
    uint32_t correction;

    if (mode == 0U) {
        left_value = 1.0 / left;
        right_value = 1.0 / right;
    } else if (mode == 1U) {
        left_value = 1.0 / sqrt(left);
        right_value = 1.0 / sqrt(right);
    } else {
        left_value = 1.0 / sqrt(2.0 * left);
        right_value = 1.0 / sqrt(2.0 * right);
    }

    base = (uint32_t)llround(left_value * LA_FESTIMATE_COEFF_SCALE);
    span = (uint32_t)llround((left_value - right_value)
                             * LA_FESTIMATE_COEFF_SCALE);
    product = (uint64_t)span * delta;
    correction = product >> LA_FESTIMATE_DELTA_BITS;
    if ((product & (1U << (LA_FESTIMATE_DELTA_BITS - 1U)))
        && ((product & ((1U << (LA_FESTIMATE_DELTA_BITS - 1U)) - 1U))
            || (correction & 1U))) {
        correction++;
    }
    return base - correction;
}

static inline int la_festimate_half_exp(int exp)
{
    int half = exp / 2;

    if ((exp < 0) && (exp & 1)) {
        half--;
    }
    return half;
}

static inline uint32_t la_festimate32(uint32_t src, bool rsqrt,
                                      float_status *status)
{
    uint32_t sign = src >> 31;
    uint32_t exp_field = (src >> 23) & 0xffU;
    uint32_t fraction = src & 0x7fffffU;
    uint32_t norm_fraction;
    uint32_t index;
    uint32_t delta;
    uint32_t coefficient;
    unsigned int mode;
    int norm_exp;
    int power;
    int shift;
    int32_t signed_coefficient;

    if (exp_field == 0xffU) {
        if (fraction != 0U) {
            if ((fraction & 0x400000U) == 0U) {
                float_raise(float_flag_invalid, status);
            }
            return src | 0x00400000U;
        }
        if (rsqrt && sign) {
            float_raise(float_flag_invalid, status);
            return 0x7fc00000U;
        }
        return sign && !rsqrt ? 0x80000000U : 0U;
    }

    if ((exp_field == 0U) && (fraction == 0U)) {
        float_raise(float_flag_divbyzero, status);
        return (sign << 31) | 0x7f800000U;
    }
    if (rsqrt && sign) {
        float_raise(float_flag_invalid, status);
        return 0x7fc00000U;
    }

    if (exp_field == 0U) {
        shift = __builtin_clz(fraction) - 8;
        norm_fraction = (fraction << shift) & 0x7fffffU;
        norm_exp = -126 - shift;
    } else {
        norm_fraction = fraction;
        norm_exp = (int)exp_field - 127;
    }
    index = norm_fraction >> 16;
    delta = (norm_fraction >> 6) & 0x3ffU;
    mode = rsqrt ? ((norm_exp & 1) ? 2U : 1U) : 0U;
    coefficient = la_festimate_coefficient(mode, index, delta);
    power = rsqrt ? -la_festimate_half_exp(norm_exp) : -norm_exp;
    signed_coefficient = sign && !rsqrt
                         ? -(int32_t)coefficient
                         : (int32_t)coefficient;
    return int32_to_float32_scalbn(signed_coefficient,
                                   power - LA_FESTIMATE_COEFF_FRAC,
                                   status);
}

static inline uint64_t la_festimate64(uint64_t src, bool rsqrt,
                                      float_status *status)
{
    uint64_t sign = src >> 63;
    uint32_t exp_field = (src >> 52) & 0x7ffU;
    uint64_t fraction = src & UINT64_C(0x000fffffffffffff);
    uint64_t norm_fraction;
    uint32_t index;
    uint32_t delta;
    uint32_t coefficient;
    unsigned int mode;
    int norm_exp;
    int power;
    int shift;
    int32_t signed_coefficient;

    if (exp_field == 0x7ffU) {
        if (fraction != 0U) {
            if ((fraction & UINT64_C(0x0008000000000000)) == 0U) {
                float_raise(float_flag_invalid, status);
            }
            return src | UINT64_C(0x0008000000000000);
        }
        if (rsqrt && sign) {
            float_raise(float_flag_invalid, status);
            return UINT64_C(0x7ff8000000000000);
        }
        return sign && !rsqrt ? UINT64_C(0x8000000000000000) : 0U;
    }

    if ((exp_field == 0U) && (fraction == 0U)) {
        float_raise(float_flag_divbyzero, status);
        return (sign << 63) | UINT64_C(0x7ff0000000000000);
    }
    if (rsqrt && sign) {
        float_raise(float_flag_invalid, status);
        return UINT64_C(0x7ff8000000000000);
    }

    if (exp_field == 0U) {
        shift = __builtin_clzll(fraction) - 11;
        norm_fraction = (fraction << shift)
                        & UINT64_C(0x000fffffffffffff);
        norm_exp = -1022 - shift;
    } else {
        norm_fraction = fraction;
        norm_exp = (int)exp_field - 1023;
    }
    index = norm_fraction >> 45;
    delta = (norm_fraction >> 35) & 0x3ffU;
    mode = rsqrt ? ((norm_exp & 1) ? 2U : 1U) : 0U;
    coefficient = la_festimate_coefficient(mode, index, delta);
    power = rsqrt ? -la_festimate_half_exp(norm_exp) : -norm_exp;
    signed_coefficient = sign && !rsqrt
                         ? -(int32_t)coefficient
                         : (int32_t)coefficient;
    return int32_to_float64_scalbn(signed_coefficient,
                                   power - LA_FESTIMATE_COEFF_FRAC,
                                   status);
}

#endif
