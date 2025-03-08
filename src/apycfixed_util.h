#include "apytypes_mp.h"

static APY_INLINE void complex_multiplication_1_1_2(
    apy_limb_t* res, const apy_limb_t* src0, const apy_limb_t* src1
)
{
#if COMPILER_LIMB_SIZE == 64
#if defined(__GNUC__)
    // GCC and Clang
    __int128 re0 = (__int128)apy_limb_signed_t(src0[0]);
    __int128 im0 = (__int128)apy_limb_signed_t(src0[1]);
    __int128 re1 = (__int128)apy_limb_signed_t(src1[0]);
    __int128 im1 = (__int128)apy_limb_signed_t(src1[1]);
    auto re_res = re0 * re1 - im0 * im1;
    auto im_res = re0 * im1 + im0 * re1;
    res[0] = apy_limb_t(re_res);
    res[1] = apy_limb_t(re_res >> COMPILER_LIMB_SIZE);
    res[2] = apy_limb_t(im_res);
    res[3] = apy_limb_t(im_res >> COMPILER_LIMB_SIZE);
#elif defined(_MSC_VER)
    // Microsoft Visual C/C++ compiler
    auto [p0_high, p0_low] = long_signed_mult(src0[0], src1[0]);
    auto [p1_high, p1_low] = long_signed_mult(src0[1], src1[1]);
    auto [p2_high, p2_low] = long_signed_mult(src0[0], src1[1]);
    auto [p3_high, p3_low] = long_signed_mult(src0[1], src1[0]);
    apy_limb_t p0_low_tmp = p0_low - p1_low;
    p0_high -= (p0_low < p0_low_tmp) + p1_high;
    p2_low += p3_low;
    p2_high += (p2_low < p3_low) + p3_high;
    res[0] = p0_low_tmp;
    res[1] = p0_high;
    res[2] = p2_low;
    res[3] = p2_high;
#else
    // No 128-bit multiplication intrinsic found. We could implement this function,
    // but fail for now so we can clearly see which systems are missing out on these
    // intrinsics.
    static_assert(
        false,
        "complex_multiplication_1_1_2(): No intrinsic available on your compiler. "
        "Please "
        "open an issue at https://github.com/apytypes/apytypes/issues with "
        "information about the compiler and platform and we will be happy to add "
        "support for it."
    );
#endif
#else
    // COMPILER_LIMB_SIZE = 32
    std::int64_t re0 = (std::int64_t)apy_limb_signed_t(src0[0]);
    std::int64_t im0 = (std::int64_t)apy_limb_signed_t(src0[1]);
    std::int64_t re1 = (std::int64_t)apy_limb_signed_t(src1[0]);
    std::int64_t im1 = (std::int64_t)apy_limb_signed_t(src1[1]);
    auto re_res = re0 * re1 - im0 * im1;
    auto im_res = re0 * im1 + im0 * re1;
    res[0] = apy_limb_t(re_res);
    res[1] = apy_limb_t(re_res >> COMPILER_LIMB_SIZE);
    res[2] = apy_limb_t(im_res);
    res[3] = apy_limb_t(im_res >> COMPILER_LIMB_SIZE);
#endif
}
