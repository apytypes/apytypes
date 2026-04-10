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

    HWY_ATTR void _hwy_vector_multiply_accumulate_1_1_2(
        const apy_limb_t* HWY_RESTRICT src1,
        const apy_limb_t* HWY_RESTRICT src2,
        apy_limb_t* HWY_RESTRICT dst,
        const std::size_t size
    )
    {
#if COMPILER_LIMB_SIZE == 32
        constexpr const hn::ScalableTag<apy_limb_signed_t> ds32;
        const hn::RepartitionToWide<decltype(ds32)> ds64;
        const std::size_t lanes32 = hn::Lanes(ds32);
        const std::size_t size_simd = size - size % lanes32;

        auto sum64_lo = hn::Zero(ds64);
        auto sum64_hi = hn::Zero(ds64);

        std::size_t i = 0;
        for (; i < size_simd; i += lanes32) {
            const auto va
                = hn::LoadU(ds32, reinterpret_cast<const apy_limb_signed_t*>(src1 + i));
            const auto vb
                = hn::LoadU(ds32, reinterpret_cast<const apy_limb_signed_t*>(src2 + i));

            sum64_lo = hn::Add(sum64_lo, hn::MulEven(va, vb));
            sum64_hi = hn::Add(sum64_hi, hn::MulOdd(va, vb));
        }

        const auto sum64 = hn::Add(sum64_lo, sum64_hi);
        const hn::RebindToUnsigned<decltype(ds64)> du64;
        std::uint64_t acc64 = hn::ReduceSum(du64, hn::BitCast(du64, sum64));

        for (; i < size; i++) {
            const auto a = static_cast<std::int64_t>(apy_limb_signed_t(src1[i]));
            const auto b = static_cast<std::int64_t>(apy_limb_signed_t(src2[i]));
            acc64 += static_cast<std::uint64_t>(a * b);
        }

        dst[0] = static_cast<apy_limb_t>(acc64);
        dst[1] = static_cast<apy_limb_t>(acc64 >> 32);

#elif COMPILER_LIMB_SIZE == 64
        constexpr const hn::ScalableTag<apy_limb_t> du;
        const std::size_t lanes = hn::Lanes(du);
        std::size_t i = 0;

        const std::size_t size_simd = size - size % lanes;
        auto sum_lo = hn::Zero(du);
        auto sum_hi = hn::Zero(du);
        for (; i < size_simd; i += lanes) {
            const auto va_u = hn::LoadU(du, src1 + i);
            const auto vb_u = hn::LoadU(du, src2 + i);
            const auto prod_lo = hn::Mul(va_u, vb_u);
            const auto prod_hi_unsigned = hn::MulHigh(va_u, vb_u);
            const auto sign_a = hn::ShiftRightSame(va_u, 63);
            const auto sign_b = hn::ShiftRightSame(vb_u, 63);
            const auto prod_hi = hn::Sub(
                hn::Sub(prod_hi_unsigned, hn::Mul(sign_a, vb_u)), hn::Mul(sign_b, va_u)
            );
            const auto new_lo = hn::Add(sum_lo, prod_lo);
            const auto carry = hn::VecFromMask(du, hn::Lt(new_lo, sum_lo));
            sum_lo = new_lo;
            sum_hi = hn::Sub(hn::Add(sum_hi, prod_hi), carry);
        }

        // Fold lane-wise accumulators with scalar carry propagation.
        // Reuse thread-local buffers to avoid repeated heap allocations.
        thread_local std::vector<apy_limb_t> lane_lo;
        thread_local std::vector<apy_limb_t> lane_hi;
        lane_lo.resize(lanes);
        lane_hi.resize(lanes);
        hn::StoreU(sum_lo, du, lane_lo.data());
        hn::StoreU(sum_hi, du, lane_hi.data());

        apy_limb_t acc_lo = 0;
        apy_limb_t acc_hi = 0;
        for (std::size_t lane = 0; lane < lanes; lane++) {
            const apy_limb_t new_lo = acc_lo + lane_lo[lane];
            const apy_limb_t carry = new_lo < acc_lo;
            acc_lo = new_lo;
            acc_hi = acc_hi + lane_hi[lane] + carry;
        }

        for (; i < size; i++) {
            auto [prod_hi, prod_lo] = long_signed_mult(src1[i], src2[i]);
            const apy_limb_t new_lo = acc_lo + prod_lo;
            const apy_limb_t carry = new_lo < acc_lo;
            acc_lo = new_lo;
            acc_hi = acc_hi + prod_hi + carry;
        }

        dst[0] = acc_lo;
        dst[1] = acc_hi;

#else
        static_assert(false, "COMPILER_LIMB_SIZE must be 32 or 64");
#endif
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
HWY_EXPORT(_hwy_vector_shift_sub);
HWY_EXPORT(_hwy_vector_shift_sub_const);
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
HWY_EXPORT(_hwy_vector_sub_const);
HWY_EXPORT(_hwy_vector_rsub_const);
HWY_EXPORT(_hwy_vector_rdiv_const_signed);
HWY_EXPORT(_hwy_vector_multiply_accumulate);
HWY_EXPORT(_hwy_vector_multiply_accumulate_1_1_2);

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

void vector_multiply_accumulate_1_1_2(
    APyBuffer<apy_limb_t>::vector_type::const_iterator src1_begin,
    APyBuffer<apy_limb_t>::vector_type::const_iterator src2_begin,
    APyBuffer<apy_limb_t>::vector_type::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_multiply_accumulate_1_1_2)(
        &*src1_begin, &*src2_begin, &*dst_begin, size
    );
}

} // namespace simd
#endif // HWY_ONCE
