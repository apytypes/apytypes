#ifndef _APYTYPES_SIMD_H
#define _APYTYPES_SIMD_H

#include <vector>

#include "apybuffer.h"

/*
 * GMP should be included after all other includes
 */
#include "../extern/mini-gmp/mini-gmp.h"

namespace simd {

//! Retrieve a string for
std::string get_simd_version_str();

/*!
 * * Shift the limbs in `src1_begin` left by `src1_shift_amount`
 * * Shift the limbs in `src2_begin` left by `src2_shift_amount`
 * * Add the shifted values (`src1` + `src2`) and store them in `dst_begin`
 */
void vector_shift_add(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src2_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t src1_shift_amount,
    std::size_t src2_shift_amount,
    std::size_t size
);

/*!
 * * Shift the limbs in `src1_begin` left by `src1_shift_amount`
 * * Shift the limbs in `src2_begin` left by `src2_shift_amount`
 * * Subtract the shifted values (`src1` - `src2`) and store them in `dst_begin`
 */
void vector_shift_sub(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src2_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t src1_shift_amount,
    std::size_t src2_shift_amount,
    std::size_t size
);

/*!
 * Perform signed multiplication of the the elements in `src1_begin` with `src2_begin`
 * and store the result in `dst_begin`, for `size` number of elements.
 */
void vector_mul(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src2_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t size
);

/*!
 * Perform addition of the the elements in `src1_begin` with `src2_begin`
 * and store the result in `dst_begin`, for `size` number of elements.
 */
void vector_add(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src2_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t size
);

/*!
 * Perform subtraction of the the elements in `src1_begin` with `src2_begin`
 * and store the result in `dst_begin`, for `size` number of elements.
 */
void vector_sub(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src2_begin,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t size
);

/*!
 * Perform addition of the the elements in `src1_begin` with a constant `constant`
 * and store the result in `dst_begin`, for `size` number of elements.
 */
void vector_add_const(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    mp_limb_t constant,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t size
);

/*!
 * Perform subtraction of the the elements in `src1_begin` with a constant `constant`
 * and store the result in `dst_begin`, for `size` number of elements.
 */
void vector_sub_const(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    mp_limb_t constant,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t size
);

/*!
 * Perform subtraction of the the constant in `constant` with the elements in
 * `src1_begin` and store the result in `dst_begin`, for `size` number of elements.
 */
void vector_rsub_const(
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::const_iterator src1_begin,
    mp_limb_t constant,
    std::vector<mp_limb_t, AlignedAllocator<mp_limb_t>>::iterator dst_begin,
    std::size_t size
);

};

#endif
