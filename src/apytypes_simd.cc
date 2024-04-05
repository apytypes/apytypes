// Generates code for every target that this compiler can support.
#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "src/apytypes_simd.cc" // this file
#include <hwy/foreach_target.h>                   // must come before highway.h
#include <hwy/highway.h>

#include <string>
#include <vector>

#include <fmt/format.h>

#include "apybuffer.h"

#include "../extern/mini-gmp/mini-gmp.h"

namespace simd {
namespace HWY_NAMESPACE { // required: unique per target

    // Can skip hn:: prefixes if already inside hwy::HWY_NAMESPACE.
    namespace hn = hwy::HWY_NAMESPACE;

    HWY_ATTR void _hwy_vector_shift_add(
        mp_limb_t* HWY_RESTRICT dst,
        const mp_limb_t* HWY_RESTRICT src1,
        const mp_limb_t* HWY_RESTRICT src2,
        unsigned src1_shift_amount,
        unsigned src2_shift_amount,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<mp_limb_t> d;

        std::size_t i = 0;
        for (; i < (size - size % hn::Lanes(d)); i += hn::Lanes(d)) {
            const auto v1 = hn::ShiftLeftSame(hn::LoadU(d, src1 + i), src1_shift_amount);
            const auto v2 = hn::ShiftLeftSame(hn::LoadU(d, src2 + i), src2_shift_amount);
            const auto res = hn::Add(v1, v2);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = (src1[i] << src1_shift_amount) + (src2[i] << src2_shift_amount);
        }
    }

    HWY_ATTR void _hwy_vector_shift_sub(
        mp_limb_t* HWY_RESTRICT dst,
        const mp_limb_t* HWY_RESTRICT src1,
        const mp_limb_t* HWY_RESTRICT src2,
        unsigned src1_shift_amount,
        unsigned src2_shift_amount,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<mp_limb_t> d;

        std::size_t i = 0;
        for (; i < (size - size % hn::Lanes(d)); i += hn::Lanes(d)) {
            const auto v1 = hn::ShiftLeftSame(hn::LoadU(d, src1 + i), src1_shift_amount);
            const auto v2 = hn::ShiftLeftSame(hn::LoadU(d, src2 + i), src2_shift_amount);
            const auto res = hn::Sub(v1, v2);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = (src1[i] << src1_shift_amount) - (src2[i] << src2_shift_amount);
        }
    }

    HWY_ATTR void _hwy_vector_mul(
        mp_limb_t* HWY_RESTRICT dst,
        const mp_limb_t* HWY_RESTRICT src1,
        const mp_limb_t* HWY_RESTRICT src2,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<mp_limb_t> d;

        std::size_t i = 0;
        for (; i < (size - size % hn::Lanes(d)); i += hn::Lanes(d)) {
            const auto v1 = hn::LoadU(d, src1 + i);
            const auto v2 = hn::LoadU(d, src2 + i);
            const auto res = hn::Mul(v1, v2);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = src1[i] * src2[i];
        }
    }

    HWY_ATTR void _hwy_vector_add(
        mp_limb_t* HWY_RESTRICT dst,
        const mp_limb_t* HWY_RESTRICT src1,
        const mp_limb_t* HWY_RESTRICT src2,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<mp_limb_t> d;

        std::size_t i = 0;
        for (; i < (size - size % hn::Lanes(d)); i += hn::Lanes(d)) {
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
        mp_limb_t* HWY_RESTRICT dst,
        const mp_limb_t* HWY_RESTRICT src1,
        const mp_limb_t* HWY_RESTRICT src2,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<mp_limb_t> d;

        std::size_t i = 0;
        for (; i < (size - size % hn::Lanes(d)); i += hn::Lanes(d)) {
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
        mp_limb_t* HWY_RESTRICT dst,
        const mp_limb_t* HWY_RESTRICT src1,
        mp_limb_t constant,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<mp_limb_t> d;

        const auto c1 = hn::Set(d, constant);
        std::size_t i = 0;
        for (; i < (size - size % hn::Lanes(d)); i += hn::Lanes(d)) {
            const auto v1 = hn::LoadU(d, src1 + i);
            const auto res = hn::Add(v1, c1);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = src1[i] + constant;
        }
    }

    HWY_ATTR void _hwy_vector_sub_const(
        mp_limb_t* HWY_RESTRICT dst,
        const mp_limb_t* HWY_RESTRICT src1,
        mp_limb_t constant,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<mp_limb_t> d;

        const auto c1 = hn::Set(d, constant);
        std::size_t i = 0;
        for (; i < (size - size % hn::Lanes(d)); i += hn::Lanes(d)) {
            const auto v1 = hn::LoadU(d, src1 + i);
            const auto res = hn::Sub(v1, c1);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = src1[i] - constant;
        }
    }

    HWY_ATTR void _hwy_vector_rsub_const(
        mp_limb_t* HWY_RESTRICT dst,
        const mp_limb_t* HWY_RESTRICT src1,
        mp_limb_t constant,
        const std::size_t size
    )
    {
        constexpr const hn::ScalableTag<mp_limb_t> d;

        const auto c1 = hn::Set(d, constant);
        std::size_t i = 0;
        for (; i < (size - size % hn::Lanes(d)); i += hn::Lanes(d)) {
            const auto v1 = hn::LoadU(d, src1 + i);
            const auto res = hn::Sub(c1, v1);
            hn::StoreU(res, d, dst + i);
        }
        for (; i < size; i++) {
            dst[i] = constant - src1[i];
        }
    }

    HWY_ATTR std::string _hwy_simd_version_str()
    {
        constexpr const hn::ScalableTag<mp_limb_t> d;
        return fmt::format(
            "APyTypes SIMD: {{ "
            "'SIMD Runtime Target': '{}', "
            "'Runtime Parallel-Limb Support': '{} x {}-bit'"
            "}}",
            hwy::TargetName(HWY_TARGET),
            hn::Lanes(d),
            _LIMB_SIZE_BITS
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
HWY_EXPORT(_hwy_vector_shift_sub);
HWY_EXPORT(_hwy_vector_mul);
HWY_EXPORT(_hwy_vector_add);
HWY_EXPORT(_hwy_vector_sub);
HWY_EXPORT(_hwy_vector_add_const);
HWY_EXPORT(_hwy_vector_sub_const);
HWY_EXPORT(_hwy_vector_rsub_const);

std::string get_simd_version_str()
{
    return HWY_DYNAMIC_DISPATCH(_hwy_simd_version_str)();
}

void vector_shift_add(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src2_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t src1_shift_amount,
    std::size_t src2_shift_amount,
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

void vector_shift_sub(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src2_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t src1_shift_amount,
    std::size_t src2_shift_amount,
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

void vector_mul(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src2_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_mul)(
        &*dst_begin, &*src1_begin, &*src2_begin, size
    );
}

void vector_add(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src2_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_add)(
        &*dst_begin, &*src1_begin, &*src2_begin, size
    );
}

void vector_sub(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src2_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_sub)(
        &*dst_begin, &*src1_begin, &*src2_begin, size
    );
}

void vector_add_const(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    mp_limb_t constant,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_add_const)(
        &*dst_begin, &*src1_begin, constant, size
    );
}

void vector_sub_const(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    mp_limb_t constant,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_sub_const)(
        &*dst_begin, &*src1_begin, constant, size
    );
}

void vector_rsub_const(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    mp_limb_t constant,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t size
)
{
    return HWY_DYNAMIC_DISPATCH(_hwy_vector_rsub_const)(
        &*dst_begin, &*src1_begin, constant, size
    );
}

} // namespace simd
#endif // HWY_ONCE
