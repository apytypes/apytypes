#include "apytypes_mp.h"

#if defined(__GNUC__)
static APY_INLINE void complex_multiplication_1_1_2_64(
    apy_limb_t* res, const apy_limb_t* src0, const apy_limb_t* src1
)
{
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
}
#endif

#if COMPILER_LIMB_SIZE == 32
static APY_INLINE void complex_multiplication_1_1_2_32(
    apy_limb_t* res, const apy_limb_t* src0, const apy_limb_t* src1
)
{
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
}
#endif
