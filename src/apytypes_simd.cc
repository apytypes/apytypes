// Generates code for every target that this compiler can support.
#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "src/apytypes_simd.cc" // this file
#include <hwy/foreach_target.h>                   // must come before highway.h
#include <hwy/highway.h>

#include <fmt/format.h>
#include <string>
#include <vector>

#include "apybuffer.h"
#include "apytypes_util.h"

namespace simd {
namespace HWY_NAMESPACE { // required: unique per target

    // Can skip hn:: prefixes if already inside hwy::HWY_NAMESPACE.
    namespace hn = hwy::HWY_NAMESPACE;

    HWY_ATTR void _hwy_vector_shift_add(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        const apy_limb_t* HWY_RESTRICT src2,
        unsigned src1_shift_amount,
        unsigned src2_shift_amount,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1
                = hn::ShiftLeftSame(hn::LoadU(d, src1 + i), src1_shift_amount);
            const auto v2
                = hn::ShiftLeftSame(hn::LoadU(d, src2 + i), src2_shift_amount);
            const auto res = hn::Add(v1, v2);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = (src1[i] << src1_shift_amount) + (src2[i] << src2_shift_amount);
        }
    }

    HWY_ATTR void _hwy_vector_shift_add_const(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        apy_limb_t constant,
        unsigned src1_shift_amount,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        std::size_t i = 0;
        const auto c1 = hn::Set(d, constant);
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1
                = hn::ShiftLeftSame(hn::LoadU(d, src1 + i), src1_shift_amount);
            const auto res = hn::Add(v1, c1);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = (src1[i] << src1_shift_amount) + constant;
        }
    }

    HWY_ATTR void _hwy_vector_shift_add_const_even_odd(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        apy_limb_t even_constant,
        apy_limb_t odd_constant,
        unsigned src1_shift_amount,
        unsigned src2_shift_amount,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t lanes = hn::Lanes(d);
        const std::size_t size_simd = size - size % lanes;

        auto shifted_even_constant = even_constant << src2_shift_amount;
        auto shifted_odd_constant = odd_constant << src2_shift_amount;
        const auto even = (hn::Set(d, shifted_even_constant));
        const auto odd = (hn::Set(d, shifted_odd_constant));
        const auto constants_even_start = hn::OddEven(odd, even);
        std::size_t i = 0;
        if ((lanes & 1U) == 0U) {
            for (; i < size_simd; i += lanes) {
                const auto v1
                    = hn::ShiftLeftSame(hn::LoadU(d, src1 + i), src1_shift_amount);
                const auto res = hn::Add(v1, constants_even_start);
                hn::StoreU(res, d, dst + i);
            }
            for (; i < size; i += 2) {
                dst[i] = (src1[i] << src1_shift_amount) + shifted_even_constant;
                dst[i + 1] = (src1[i + 1] << src1_shift_amount) + shifted_odd_constant;
            }
        } else {
            const auto constants_odd_start = hn::OddEven(even, odd);
            for (; i < size_simd; i += lanes) {
                const auto constants
                    = (i & 1) ? constants_odd_start : constants_even_start;
                const auto v1
                    = hn::ShiftLeftSame(hn::LoadU(d, src1 + i), src1_shift_amount);
                const auto res = hn::Add(v1, constants);
                hn::StoreU(res, d, dst + i);
            }
            for (; i < size; i++) {
                dst[i] = (src1[i] << src1_shift_amount)
                    + (((i & 1) ? shifted_odd_constant : shifted_even_constant));
            }
        }
    }

    HWY_ATTR void _hwy_vector_shift_sub_const_even_odd(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        apy_limb_t even_constant,
        apy_limb_t odd_constant,
        unsigned src1_shift_amount,
        unsigned src2_shift_amount,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t lanes = hn::Lanes(d);
        const std::size_t size_simd = size - size % lanes;

        auto shifted_even_constant = even_constant << src2_shift_amount;
        auto shifted_odd_constant = odd_constant << src2_shift_amount;
        const auto even = hn::Set(d, shifted_even_constant);
        const auto odd = hn::Set(d, shifted_odd_constant);
        const auto constants_even_start = hn::OddEven(odd, even);
        std::size_t i = 0;
        if ((lanes & 1U) == 0U) {
            for (; i < size_simd; i += lanes) {
                const auto v1
                    = hn::ShiftLeftSame(hn::LoadU(d, src1 + i), src1_shift_amount);
                const auto res = hn::Sub(v1, constants_even_start);
                hn::StoreU(res, d, dst + i);
            }
            for (; i < size; i += 2) {
                dst[i] = (src1[i] << src1_shift_amount) - shifted_even_constant;
                dst[i + 1] = (src1[i + 1] << src1_shift_amount) - shifted_odd_constant;
            }
        } else {
            const auto constants_odd_start = hn::OddEven(even, odd);
            for (; i < size_simd; i += lanes) {
                const auto constants
                    = (i & 1) ? constants_odd_start : constants_even_start;
                const auto v1
                    = hn::ShiftLeftSame(hn::LoadU(d, src1 + i), src1_shift_amount);
                const auto res = hn::Sub(v1, constants);
                hn::StoreU(res, d, dst + i);
            }
            for (; i < size; i++) {
                dst[i] = (src1[i] << src1_shift_amount)
                    - (((i & 1) ? shifted_odd_constant : shifted_even_constant));
            }
        }
    }

    HWY_ATTR void _hwy_vector_shift_sub(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        const apy_limb_t* HWY_RESTRICT src2,
        unsigned src1_shift_amount,
        unsigned src2_shift_amount,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1
                = hn::ShiftLeftSame(hn::LoadU(d, src1 + i), src1_shift_amount);
            const auto v2
                = hn::ShiftLeftSame(hn::LoadU(d, src2 + i), src2_shift_amount);
            const auto res = hn::Sub(v1, v2);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = (src1[i] << src1_shift_amount) - (src2[i] << src2_shift_amount);
        }
    }

    HWY_ATTR void _hwy_vector_shift_sub_const(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        apy_limb_t constant,
        unsigned src1_shift_amount,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        std::size_t i = 0;
        const auto c1 = hn::Set(d, constant);
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1
                = hn::ShiftLeftSame(hn::LoadU(d, src1 + i), src1_shift_amount);
            const auto res = hn::Sub(v1, c1);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = (src1[i] << src1_shift_amount) - constant;
        }
    }

    HWY_ATTR void _hwy_vector_neg(
        apy_limb_signed_t* HWY_RESTRICT dst,
        const apy_limb_signed_t* HWY_RESTRICT src,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_signed_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto res = hn::Neg(hn::LoadU(d, src + i));
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = -apy_limb_signed_t(src[i]);
        }
    }

    HWY_ATTR void _hwy_vector_conj(
        apy_limb_signed_t* HWY_RESTRICT dst,
        const apy_limb_signed_t* HWY_RESTRICT src,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_signed_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            auto a = hn::LoadU(d, src + i);
            const auto res = hn::OddEven(hn::Neg(a), a);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i += 2) {
            dst[i] = src[i];
            dst[i + 1] = -src[i + 1];
        }
    }

    HWY_ATTR void _hwy_vector_not(
        apy_limb_signed_t* HWY_RESTRICT dst,
        const apy_limb_signed_t* HWY_RESTRICT src,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_signed_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto res = hn::Not(hn::LoadU(d, src + i));
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = ~apy_limb_signed_t(src[i]);
        }
    }

    HWY_ATTR void _hwy_vector_abs(
        apy_limb_signed_t* HWY_RESTRICT dst,
        const apy_limb_signed_t* HWY_RESTRICT src,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_signed_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto res = hn::Abs(hn::LoadU(d, src + i));
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = std::abs(apy_limb_signed_t(src[i]));
        }
    }

    HWY_ATTR void _hwy_vector_shift_div_signed(
        apy_limb_signed_t* HWY_RESTRICT dst,
        const apy_limb_signed_t* HWY_RESTRICT src1,
        const apy_limb_signed_t* HWY_RESTRICT src2,
        unsigned src1_shift_amount,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_signed_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1
                = hn::ShiftLeftSame(hn::LoadU(d, src1 + i), src1_shift_amount);
            const auto v2 = hn::LoadU(d, src2 + i);
            const auto res = hn::Div(v1, v2);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = apy_limb_signed_t(src1[i] << src1_shift_amount)
                / apy_limb_signed_t(src2[i]);
        }
    }

    HWY_ATTR void _hwy_vector_shift_div_const_signed(
        apy_limb_signed_t* HWY_RESTRICT dst,
        const apy_limb_signed_t* HWY_RESTRICT src1,
        apy_limb_t constant,
        unsigned src1_shift_amount,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_signed_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        std::size_t i = 0;
        const auto c1 = hn::Set(d, constant);
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1
                = hn::ShiftLeftSame(hn::LoadU(d, src1 + i), src1_shift_amount);
            const auto res = hn::Div(v1, c1);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = apy_limb_signed_t(src1[i] << src1_shift_amount)
                / apy_limb_signed_t(constant);
        }
    }

    HWY_ATTR void _hwy_vector_mul(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        const apy_limb_t* HWY_RESTRICT src2,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1 = hn::LoadU(d, src1 + i);
            const auto v2 = hn::LoadU(d, src2 + i);
            const auto res = hn::Mul(v1, v2);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = src1[i] * src2[i];
        }
    }

    HWY_ATTR void _hwy_vector_mul_const(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        const apy_limb_t constant,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        const auto c1 = hn::Set(d, constant);
        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1 = hn::LoadU(d, src1 + i);
            const auto res = hn::Mul(v1, c1);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = src1[i] * constant;
        }
    }

    HWY_ATTR void _hwy_vector_add(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        const apy_limb_t* HWY_RESTRICT src2,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1 = hn::LoadU(d, src1 + i);
            const auto v2 = hn::LoadU(d, src2 + i);
            const auto res = hn::Add(v1, v2);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = src1[i] + src2[i];
        }
    }

    HWY_ATTR void _hwy_vector_sub(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        const apy_limb_t* HWY_RESTRICT src2,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1 = hn::LoadU(d, src1 + i);
            const auto v2 = hn::LoadU(d, src2 + i);
            const auto res = hn::Sub(v1, v2);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = src1[i] - src2[i];
        }
    }

    HWY_ATTR void _hwy_vector_add_const(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        apy_limb_t constant,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        const auto c1 = hn::Set(d, constant);
        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1 = hn::LoadU(d, src1 + i);
            const auto res = hn::Add(v1, c1);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = src1[i] + constant;
        }
    }

    HWY_ATTR void _hwy_vector_add_const_even_odd(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        apy_limb_t even_constant,
        apy_limb_t odd_constant,
        const std::size_t size
    )
    {
        assert(size & 1U == 0U && "size must be even for vector_add_const_even_odd");
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t lanes = hn::Lanes(d);
        const std::size_t size_simd = size - size % lanes;

        const auto even = hn::Set(d, even_constant);
        const auto odd = hn::Set(d, odd_constant);
        const auto constants_even_start = hn::OddEven(odd, even);
        std::size_t i = 0;
        if ((lanes & 1U) == 0U) {
            for (; i < size_simd; i += lanes) {
                const auto v1 = hn::LoadU(d, src1 + i);
                const auto res = hn::Add(v1, constants_even_start);
                hn::StoreU(res, d, dst + i);
            }
            for (; i < size; i += 2) {
                dst[i] = src1[i] + even_constant;
                dst[i + 1] = src1[i + 1] + odd_constant;
            }
        } else {
            const auto constants_odd_start = hn::OddEven(even, odd);
            for (; i < size_simd; i += lanes) {
                const auto constants
                    = (i & 1) ? constants_odd_start : constants_even_start;
                const auto v1 = hn::LoadU(d, src1 + i);
                const auto res = hn::Add(v1, constants);
                hn::StoreU(res, d, dst + i);
            }
            for (; i < size; i++) {
                dst[i] = src1[i] + ((i & 1) ? odd_constant : even_constant);
            }
        }
    }

    HWY_ATTR void _hwy_vector_sub_const(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        apy_limb_t constant,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        const auto c1 = hn::Set(d, constant);
        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1 = hn::LoadU(d, src1 + i);
            const auto res = hn::Sub(v1, c1);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = src1[i] - constant;
        }
    }

    HWY_ATTR void _hwy_vector_rsub_const(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        apy_limb_t constant,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        const auto c1 = hn::Set(d, constant);
        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1 = hn::LoadU(d, src1 + i);
            const auto res = hn::Sub(c1, v1);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = constant - src1[i];
        }
    }

    HWY_ATTR void _hwy_vector_rdiv_const_signed(
        apy_limb_signed_t* HWY_RESTRICT dst,
        const apy_limb_signed_t* HWY_RESTRICT src1,
        apy_limb_signed_t constant,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_signed_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        const auto c1 = hn::Set(d, constant);
        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1 = hn::LoadU(d, src1 + i);
            const auto res = hn::Div(c1, v1);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = constant / src1[i];
        }
    }

    HWY_ATTR apy_limb_t _hwy_vector_multiply_accumulate(
        const apy_limb_t* HWY_RESTRICT src1,
        const apy_limb_t* HWY_RESTRICT src2,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        auto simd_sum = hn::Zero(d);
        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v1 = hn::LoadU(d, src1 + i);
            const auto v2 = hn::LoadU(d, src2 + i);
            simd_sum = hn::MulAdd(v1, v2, simd_sum);
        }

        apy_limb_t sum = hn::ReduceSum(d, simd_sum);
        for (; i < size; i++) {
            sum += apy_limb_signed_t(src1[i]) * apy_limb_signed_t(src2[i]);
        }
        return sum;
    }

    HWY_ATTR void _hwy_vector_sub_const_even_odd(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        apy_limb_t even_constant,
        apy_limb_t odd_constant,
        const std::size_t size
    )
    {
        assert(size & 1U == 0U && "size must be even for vector_sub_const_even_odd");
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t lanes = hn::Lanes(d);
        const std::size_t size_simd = size - size % lanes;

        const auto even = hn::Set(d, even_constant);
        const auto odd = hn::Set(d, odd_constant);
        const auto constants_even_start = hn::OddEven(odd, even);
        std::size_t i = 0;
        if ((lanes & 1U) == 0U) {
            for (; i < size_simd; i += lanes) {
                const auto v1 = hn::LoadU(d, src1 + i);
                const auto res = hn::Sub(v1, constants_even_start);
                hn::StoreU(res, d, dst + i);
            }
            for (; i < size; i += 2) {
                dst[i] = src1[i] - even_constant;
                dst[i + 1] = src1[i + 1] - odd_constant;
            }
        } else {
            const auto constants_odd_start = hn::OddEven(even, odd);
            for (; i < size_simd; i += lanes) {
                const auto constants
                    = (i & 1) ? constants_odd_start : constants_even_start;
                const auto v1 = hn::LoadU(d, src1 + i);
                const auto res = hn::Sub(v1, constants);
                hn::StoreU(res, d, dst + i);
            }
            for (; i < size; i++) {
                dst[i] = src1[i] - ((i & 1) ? odd_constant : even_constant);
            }
        }
    }

    HWY_ATTR bool
    _hwy_vector_any_zero(const apy_limb_t* HWY_RESTRICT src, const std::size_t size)
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t size_simd = size - size % hn::Lanes(d);

        const auto zeros = hn::Zero(d);
        const auto ones = hn::Set(d, apy_limb_t(1));
        std::size_t i = 0;
        for (; i < size_simd; i += hn::Lanes(d)) {
            const auto v = hn::LoadU(d, src + i);
            const auto is_zero_mask = hn::Eq(v, zeros);
            const auto flagged = hn::IfThenElseZero(is_zero_mask, ones);
            if (hn::ReduceSum(d, flagged) != 0) {
                return true;
            }
        }

        for (; i < size; i++) {
            if (src[i] == 0) {
                return true;
            }
        }
        return false;
    }

    HWY_ATTR void _hwy_vector_rsub_const_even_odd(
        apy_limb_t* HWY_RESTRICT dst,
        const apy_limb_t* HWY_RESTRICT src1,
        apy_limb_t even_constant,
        apy_limb_t odd_constant,
        const std::size_t size
    )
    {
        assert(size & 1U == 0U && "size must be even for vector_rsub_const_even_odd");
        constexpr const hn::ScalableTag<apy_limb_t> d;
        const std::size_t lanes = hn::Lanes(d);
        const std::size_t size_simd = size - size % lanes;

        const auto even = hn::Set(d, even_constant);
        const auto odd = hn::Set(d, odd_constant);
        const auto constants_even_start = hn::OddEven(odd, even);
        std::size_t i = 0;
        if ((lanes & 1U) == 0U) {
            for (; i < size_simd; i += lanes) {
                const auto v1 = hn::LoadU(d, src1 + i);
                const auto res = hn::Sub(constants_even_start, v1);
                hn::StoreU(res, d, dst + i);
            }
            for (; i < size; i += 2) {
                dst[i] = even_constant - src1[i];
                dst[i + 1] = odd_constant - src1[i + 1];
            }
        } else {
            const auto constants_odd_start = hn::OddEven(even, odd);
            for (; i < size_simd; i += lanes) {
                const auto constants
                    = (i & 1) ? constants_odd_start : constants_even_start;
                const auto v1 = hn::LoadU(d, src1 + i);
                const auto res = hn::Sub(constants, v1);
                hn::StoreU(res, d, dst + i);
            }
            for (; i < size; i++) {
                dst[i] = ((i & 1) ? odd_constant : even_constant) - src1[i];
            }
        }
    }

    HWY_ATTR std::string _hwy_simd_version_str()
    {
        constexpr const hn::ScalableTag<apy_limb_t> d;
        return fmt::format(
            "APyTypes SIMD:\n"
            "    * SIMD runtime target: {}\n"
            "    * Runtime parallel-limb support: {} x {}-bit",
            hwy::TargetName(HWY_TARGET),
            hn::Lanes(d),
            APY_LIMB_SIZE_BITS
        );
    }

} // namespace HWY_NAMESPACE
} // namespace simd

// The table of pointers to the various implementations in HWY_NAMESPACE must
// be compiled only once (foreach_target #includes this file multiple times).
// HWY_ONCE is true for only one of these 'compilation passes'.
#if HWY_ONCE

namespace simd {

HWY_EXPORT(_hwy_simd_version_str);

HWY_EXPORT(_hwy_vector_shift_add);
HWY_EXPORT(_hwy_vector_shift_add_const);
HWY_EXPORT(_hwy_vector_shift_add_const_even_odd);
HWY_EXPORT(_hwy_vector_shift_sub);
HWY_EXPORT(_hwy_vector_shift_sub_const);
HWY_EXPORT(_hwy_vector_shift_sub_const_even_odd);
HWY_EXPORT(_hwy_vector_shift_div_signed);
HWY_EXPORT(_hwy_vector_shift_div_const_signed);
HWY_EXPORT(_hwy_vector_mul);
HWY_EXPORT(_hwy_vector_mul_const);
HWY_EXPORT(_hwy_vector_add);
HWY_EXPORT(_hwy_vector_sub);
HWY_EXPORT(_hwy_vector_neg);
HWY_EXPORT(_hwy_vector_conj);
HWY_EXPORT(_hwy_vector_abs);
HWY_EXPORT(_hwy_vector_not);
HWY_EXPORT(_hwy_vector_add_const);
HWY_EXPORT(_hwy_vector_add_const_even_odd);
HWY_EXPORT(_hwy_vector_sub_const);
HWY_EXPORT(_hwy_vector_sub_const_even_odd);
HWY_EXPORT(_hwy_vector_rsub_const);
HWY_EXPORT(_hwy_vector_rsub_const_even_odd);
HWY_EXPORT(_hwy_vector_rdiv_const_signed);
HWY_EXPORT(_hwy_vector_multiply_accumulate);
HWY_EXPORT(_hwy_vector_any_zero);

std::string get_simd_version_str()
{
    return HWY_DYNAMIC_DISPATCH(_hwy_simd_version_str)();
}

void vector_shift_add(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    APyBuffer<apy_limb_t>::vector_type::const_iterator src2_begin,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    unsigned src1_shift_amount,
    unsigned src2_shift_amount,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_shift_add)(
        &*dst_begin,
        &*src1_begin,
        &*src2_begin,
        src1_shift_amount,
        src2_shift_amount,
        size
    );
}

void vector_shift_add_const(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    apy_limb_t constant,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    unsigned src1_shift_amount,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_shift_add_const)(
        &*dst_begin, &*src1_begin, constant, src1_shift_amount, size
    );
}

void vector_shift_add_const_even_odd(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    apy_limb_t even_constant,
    apy_limb_t odd_constant,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    unsigned src1_shift_amount,
    unsigned src2_shift_amount,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_shift_add_const_even_odd)(
        &*dst_begin,
        &*src1_begin,
        even_constant,
        odd_constant,
        src1_shift_amount,
        src2_shift_amount,
        size
    );
}

void vector_shift_sub(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    APyBuffer<apy_limb_t>::vector_type::const_iterator src2_begin,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    unsigned src1_shift_amount,
    unsigned src2_shift_amount,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_shift_sub)(
        &*dst_begin,
        &*src1_begin,
        &*src2_begin,
        src1_shift_amount,
        src2_shift_amount,
        size
    );
}

void vector_shift_sub_const(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    apy_limb_t constant,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    unsigned src1_shift_amount,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_shift_sub_const)(
        &*dst_begin, &*src1_begin, constant, src1_shift_amount, size
    );
}

void vector_shift_sub_const_even_odd(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    apy_limb_t even_constant,
    apy_limb_t odd_constant,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    unsigned src1_shift_amount,
    unsigned src2_shift_amount,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_shift_sub_const_even_odd)(
        &*dst_begin,
        &*src1_begin,
        even_constant,
        odd_constant,
        src1_shift_amount,
        src2_shift_amount,
        size
    );
}

void vector_neg(
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    APyBuffer<apy_limb_t>::vector_type::const_iterator src_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_neg)(
        reinterpret_cast<apy_limb_signed_t*>(&*dst_begin),
        reinterpret_cast<const apy_limb_signed_t*>(&*src_begin),
        size
    );
}

void vector_conj(
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    APyBuffer<apy_limb_t>::vector_type::const_iterator src_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_conj)(
        reinterpret_cast<apy_limb_signed_t*>(&*dst_begin),
        reinterpret_cast<const apy_limb_signed_t*>(&*src_begin),
        size
    );
}

void vector_not(
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    APyBuffer<apy_limb_t>::vector_type::const_iterator src_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_not)(
        reinterpret_cast<apy_limb_signed_t*>(&*dst_begin),
        reinterpret_cast<const apy_limb_signed_t*>(&*src_begin),
        size
    );
}

void vector_abs(
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    APyBuffer<apy_limb_t>::vector_type::const_iterator src_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_abs)(
        reinterpret_cast<apy_limb_signed_t*>(&*dst_begin),
        reinterpret_cast<const apy_limb_signed_t*>(&*src_begin),
        size
    );
}

void vector_shift_div_signed(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    APyBuffer<apy_limb_t>::vector_type::const_iterator src2_begin,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    unsigned src1_shift_amount,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_shift_div_signed)(
        reinterpret_cast<apy_limb_signed_t*>(&*dst_begin),
        reinterpret_cast<const apy_limb_signed_t*>(&*src1_begin),
        reinterpret_cast<const apy_limb_signed_t*>(&*src2_begin),
        src1_shift_amount,
        size
    );
}

void vector_shift_div_const_signed(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    apy_limb_t constant,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    unsigned src1_shift_amount,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_shift_div_const_signed)(
        reinterpret_cast<apy_limb_signed_t*>(&*dst_begin),
        reinterpret_cast<const apy_limb_signed_t*>(&*src1_begin),
        constant,
        src1_shift_amount,
        size
    );
}

void vector_mul(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    APyBuffer<apy_limb_t>::vector_type::const_iterator src2_begin,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_mul)(
        &*dst_begin, &*src1_begin, &*src2_begin, size
    );
}

void vector_mul_const(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    apy_limb_t constant,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_mul_const)(
        &*dst_begin, &*src1_begin, constant, size
    );
}

void vector_add(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    APyBuffer<apy_limb_t>::vector_type::const_iterator src2_begin,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_add)(
        &*dst_begin, &*src1_begin, &*src2_begin, size
    );
}

void vector_sub(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    APyBuffer<apy_limb_t>::vector_type::const_iterator src2_begin,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_sub)(
        &*dst_begin, &*src1_begin, &*src2_begin, size
    );
}

void vector_add_const(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    apy_limb_t constant,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_add_const)(
        &*dst_begin, &*src1_begin, constant, size
    );
}

void vector_add_const_even_odd(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    apy_limb_t even_constant,
    apy_limb_t odd_constant,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_add_const_even_odd)(
        &*dst_begin, &*src1_begin, even_constant, odd_constant, size
    );
}

void vector_sub_const(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    apy_limb_t constant,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_sub_const)(
        &*dst_begin, &*src1_begin, constant, size
    );
}

void vector_sub_const_even_odd(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    apy_limb_t even_constant,
    apy_limb_t odd_constant,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_sub_const_even_odd)(
        &*dst_begin, &*src1_begin, even_constant, odd_constant, size
    );
}

void vector_rsub_const(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    apy_limb_t constant,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_rsub_const)(
        &*dst_begin, &*src1_begin, constant, size
    );
}

void vector_rsub_const_even_odd(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    apy_limb_t even_constant,
    apy_limb_t odd_constant,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_rsub_const_even_odd)(
        &*dst_begin, &*src1_begin, even_constant, odd_constant, size
    );
}

void vector_rdiv_const_signed(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    apy_limb_t constant,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_rdiv_const_signed)(
        reinterpret_cast<apy_limb_signed_t*>(&*dst_begin),
        reinterpret_cast<const apy_limb_signed_t*>(&*src1_begin),
        constant,
        size
    );
}

apy_limb_t vector_multiply_accumulate(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    APyBuffer<apy_limb_t>::vector_type::const_iterator src2_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_multiply_accumulate)(
        &*src1_begin, &*src2_begin, size
    );
}

bool vector_any_zero(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src_begin, std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_any_zero)(&*src_begin, size);
}

} // namespace simd
#endif // HWY_ONCE
