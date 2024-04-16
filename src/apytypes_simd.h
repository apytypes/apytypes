/*!
 * This file contains declarations of vectorized arithmetic function that make a
 * best-effort attempt in parallelizing operations using SIMD intrinsics.
 */

#ifndef _APYTYPES_SIMD_H
#define _APYTYPES_SIMD_H

#include <string>
#include <vector>

/*
 * GMP should be included after all other includes
 */
#include "../extern/mini-gmp/mini-gmp.h"

namespace simd {

//! Retrieve a string for
std::string get_simd_version_str();

/*!
 * For each element in the iterator regions [ `*_begin`, `*_begin + size` ):
 * * Shift element in `src1_begin` left by `src1_shift_amount`
 * * Shift element in `src2_begin` left by `src2_shift_amount`
 * * Add shifted values and store in `dst_begin`
 */
void vector_shift_add(
    std::vector<mp_limb_t>::const_iterator src1_begin,
    std::vector<mp_limb_t>::const_iterator src2_begin,
    std::vector<mp_limb_t>::iterator dst_begin,
    unsigned src1_shift_amount,
    unsigned src2_shift_amount,
    std::size_t size
);

/*!
 * For each element in the iterator regions [ `*_begin`, `*_begin + size` ):
 * * Shift element in `src1_begin` left by `src1_shift_amount`
 * * Shift element in `src2_begin` left by `src2_shift_amount`
 * * Subtract shifted values and store in `dst_begin`
 */
void vector_shift_sub(
    std::vector<mp_limb_t>::const_iterator src1_begin,
    std::vector<mp_limb_t>::const_iterator src2_begin,
    std::vector<mp_limb_t>::iterator dst_begin,
    unsigned src1_shift_amount,
    unsigned src2_shift_amount,
    std::size_t size
);

/*
 * For each element in the iterator regions [ `*_begin`, `*_begin + size` ):
 * * Shift element in `src1_begin` left by `src1_shift_amount`
 * * Divide shifted element (numerator) by `src2_begin` (denominator) and store the
 *   result in `dst_begin`
 */
void vector_shift_div_signed(
    std::vector<mp_limb_t>::const_iterator src1_begin,
    std::vector<mp_limb_t>::const_iterator src2_begin,
    std::vector<mp_limb_t>::iterator dst_begin,
    unsigned src1_shift_amount,
    std::size_t size
);

/*
 * For each element in the iterator regions [ `*_begin`, `*_begin + size` ):
 * * Shift element in `src1_begin` left by `src1_shift_amount`
 * * Divide shifted element (numerator) by `constant` (denominator) and store the
 *   result in `dst_begin`
 */
void vector_shift_div_const_signed(
    std::vector<mp_limb_t>::const_iterator src1_begin,
    mp_limb_t constant,
    std::vector<mp_limb_t>::iterator dst_begin,
    unsigned src1_shift_amount,
    std::size_t size
);

/*!
 * Perform signed multiplication of the the elements in `src1_begin` with `src2_begin`
 * and store the result in `dst_begin`, for `size` number of elements.
 */
void vector_mul(
    std::vector<mp_limb_t>::const_iterator src1_begin,
    std::vector<mp_limb_t>::const_iterator src2_begin,
    std::vector<mp_limb_t>::iterator dst_begin,
    std::size_t size
);

/*!
 * Perform addition of the the elements in `src1_begin` with `src2_begin`
 * and store the result in `dst_begin`, for `size` number of elements.
 */
void vector_add(
    std::vector<mp_limb_t>::const_iterator src1_begin,
    std::vector<mp_limb_t>::const_iterator src2_begin,
    std::vector<mp_limb_t>::iterator dst_begin,
    std::size_t size
);

/*!
 * Perform subtraction of the the elements in `src1_begin` with `src2_begin`
 * and store the result in `dst_begin`, for `size` number of elements.
 */
void vector_sub(
    std::vector<mp_limb_t>::const_iterator src1_begin,
    std::vector<mp_limb_t>::const_iterator src2_begin,
    std::vector<mp_limb_t>::iterator dst_begin,
    std::size_t size
);

/*!
 * Perform addition of the the elements in `src1_begin` with a constant `constant`
 * and store the result in `dst_begin`, for `size` number of elements.
 */
void vector_add_const(
    std::vector<mp_limb_t>::const_iterator src1_begin,
    mp_limb_t constant,
    std::vector<mp_limb_t>::iterator dst_begin,
    std::size_t size
);

/*!
 * Perform subtraction of the the elements in `src1_begin` with a constant `constant`
 * and store the result in `dst_begin`, for `size` number of elements.
 */
void vector_sub_const(
    std::vector<mp_limb_t>::const_iterator src1_begin,
    mp_limb_t constant,
    std::vector<mp_limb_t>::iterator dst_begin,
    std::size_t size
);

/*!
 * Perform signed multiplication of the the elements in `src1_begin` with a constant
 * `constant` and store the result in `dst_begin`, for `size` number of elements.
 */
void vector_mul_const(
    std::vector<mp_limb_t>::const_iterator src1_begin,
    mp_limb_t constant,
    std::vector<mp_limb_t>::iterator dst_begin,
    std::size_t size
);

/*!
 * Perform subtraction of the the constant in `constant` with the elements in
 * `src1_begin` and store the result in `dst_begin`, for `size` number of elements.
 */
void vector_rsub_const(
    std::vector<mp_limb_t>::const_iterator src1_begin,
    mp_limb_t constant,
    std::vector<mp_limb_t>::iterator dst_begin,
    std::size_t size
);

/*!
 * Multiply (signed) and accumulate all elements from `src1_begin` with `src2_begin`
 * for `size` number of elements. Returns accumulated value.
 */
mp_limb_t vector_multiply_accumulate(
    std::vector<mp_limb_t>::const_iterator src1_begin,
    std::vector<mp_limb_t>::const_iterator src2_begin,
    std::size_t size
);

}; // namespace simd

#endif
