/* ********************************************************************************** *
 * *               APyTypes limb types and multi-precision operations               * *
 * ********************************************************************************** */

#ifndef __APYTYPES_MP_H__
#define __APYTYPES_MP_H__

#include <cassert> // assert
#include <cstddef> // std::size_t
#include <cstdint> // std::int64_t, std::uint64_t, std::int32_t, std::uint32_t

#if !defined(COMPILER_LIMB_SIZE)
#error "C Macro `COMPILER_LIMB_SIZE` not specified. Must be set during compilation."
#else /* defined(COMPILER_LIMB_SIZE) */
#if COMPILER_LIMB_SIZE == 32
typedef std::uint32_t apy_limb_t;
typedef std::int32_t apy_limb_signed_t;
#elif COMPILER_LIMB_SIZE == 64
typedef std::uint64_t apy_limb_t;
typedef std::int64_t apy_limb_signed_t;
#elif COMPILER_LIMB_SIZE == NATIVE
#if SIZE_MAX == 4294967295ull /* 32-bit system detected */
typedef std::uint32_t apy_limb_t;
typedef std::int32_t apy_limb_signed_t;
#undef COMPILER_LIMB_SIZE
#define COMPILER_LIMB_SIZE 32
#elif SIZE_MAX == 18446744073709551615ull /* 64-bit system detected */
typedef std::uint64_t apy_limb_t;
typedef std::int64_t apy_limb_signed_t;
#undef COMPILER_LIMB_SIZE
#define COMPILER_LIMB_SIZE 64
#else
#error "Cannot detect native target architecture word size."
#endif
#else
#error "C Macro `COMPILER_LIMB_SIZE` must be one of `32`, `64` or `NATIVE`"
#endif

#endif

/*
 * Conditional inlining of utility functions if profiling `_APY_PROFILING`
 */
#ifdef _APY_PROFILING
#define APY_INLINE
#else
#define APY_INLINE inline
#endif

/*
 * Sizes of APy limbs (underlying words)
 */
//! Number of bits in a char, as defined by POSIX
#define POSIX_CHAR_BITS 8
//! Number of bytes in a limb
constexpr std::size_t APY_LIMB_SIZE_BYTES = sizeof(apy_limb_t);
constexpr apy_limb_t APY_NUMBER_MASK = ~((apy_limb_t)0);
//! Number of bits in a limb
constexpr std::size_t APY_LIMB_SIZE_BITS = POSIX_CHAR_BITS * APY_LIMB_SIZE_BYTES;
//! Number of bits in a half limb
constexpr std::size_t APY_HALF_LIMB_SIZE_BITS = APY_LIMB_SIZE_BITS / 2;
//! Mask for lower half of a limb
constexpr apy_limb_t APY_LOWER_LIMB_MASK
    = ((apy_limb_t)1 << APY_HALF_LIMB_SIZE_BITS) - 1;
constexpr apy_limb_t endian_test = (((apy_limb_t)1) << (APY_LIMB_SIZE_BITS - 7)) - 1;
#define HOST_ENDIAN (*(signed char*)&endian_test)
constexpr std::size_t NIBBLES_PER_LIMB = 2 * APY_LIMB_SIZE_BYTES;
constexpr std::size_t BITS_PER_NIBBLE = 4;

// Only used for asserts
constexpr apy_limb_t APY_LIMB_MSBWEIGHT = ((apy_limb_t)1 << (APY_LIMB_SIZE_BITS - 1));

/* ********************************************************************************** *
 * *                             Other APy MP types                                 * *
 * ********************************************************************************** */

typedef long apy_size_t;

// TODO: document the functions

// Addition

//! Add two limb vectors in place: dest += src, where len(dest) >= len(src)
apy_limb_t apy_inplace_addition(
    apy_limb_t*, const std::size_t, const apy_limb_t*, const std::size_t
);

//! Add two limb vectors: dest = src0 + src1, where len(dest) == len(src0) ==
//! len(src1)
[[maybe_unused]] static APY_INLINE apy_limb_t apy_addition_same_length(
    apy_limb_t* dest,
    const apy_limb_t* src0,
    const apy_limb_t* src1,
    const std::size_t limbs
)
{
    assert(limbs > 0);

    // Specialized first iteration
    dest[0] = src0[0] + src1[0];
    apy_limb_t carry = (dest[0] < src1[0]);

    for (std::size_t i = 1; i < limbs; i++) {
        apy_limb_t tmp_res = src0[i] + carry;
        carry = (tmp_res < carry);
        tmp_res += src1[i];
        carry += (tmp_res < src1[i]);
        dest[i] = tmp_res;
    }
    return carry;
}

//! Add a single limb to a limb vector in place: dest += src
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_addition_single_limb(
    apy_limb_t* dest, const std::size_t limbs, const apy_limb_t src
)
{
    assert(limbs > 0);

    /* src is initial "carry" */
    apy_limb_t carry = src;
    for (std::size_t i = 0; i < limbs; i++) {
        dest[i] += carry;
        carry = (dest[i] < carry);
    }

    return carry;
}

//! Add a single one to a limb vector in place: dest++
[[maybe_unused]] static APY_INLINE apy_limb_t
apy_inplace_add_one_lsb(apy_limb_t* dest, const std::size_t limbs)
{
    assert(limbs > 0);

    apy_limb_t carry = 1;
    for (std::size_t i = 0; i < limbs; i++) {
        dest[i] += carry;
        carry = (dest[i] < carry);
    }

    return carry;
}

//! Add two limb vectors of the same length in place: dest += src, where len(dest)
//! == len(src)
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_addition_same_length(
    apy_limb_t* dest, const apy_limb_t* src, const std::size_t limbs
)
{
    assert(limbs > 0);

    // Specialized first iteration
    dest[0] += src[0];
    apy_limb_t carry = (dest[0] < src[0]);

    for (std::size_t i = 1; i < limbs; i++) {
        dest[i] += carry;
        carry = (dest[i] < carry);
        dest[i] += src[i];
        carry += (dest[i] < src[i]);
    }
    return carry;
}

// Subtraction

//! Negate a limb vector in place: dest = -dest
[[maybe_unused]] static APY_INLINE apy_limb_t
apy_inplace_negate(apy_limb_t* dest, const std::size_t limbs)
{
    assert(limbs > 0);

    apy_limb_t carry = 1;
    for (std::size_t i = 0; i < limbs; i++) {
        dest[i] = ~dest[i] + carry;
        carry = (dest[i] < carry);
    }

    return carry;
}

//! Negate a limb vector: dest = -src
[[maybe_unused]] static APY_INLINE apy_limb_t
apy_negate(apy_limb_t* dest, const apy_limb_t* src, const std::size_t limbs)
{
    assert(limbs > 0);

    apy_limb_t carry = 1;
    for (std::size_t i = 0; i < limbs; i++) {
        dest[i] = ~src[i] + carry;
        carry = (dest[i] < carry);
    }

    return carry;
}

//! Subtract a single limb from a limb vectors in place: dest -= src
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_subtraction_single_limb(
    apy_limb_t* dest, const std::size_t limbs, const apy_limb_t src
)
{
    assert(limbs > 0);

    /* src is initial "carry" */
    apy_limb_t carry = src;

    for (std::size_t i = 0; i < limbs; i++) {
        /* Determine carry to next limb */
        apy_limb_t carry_tmp = dest[i] < carry;
        dest[i] -= carry;
        carry = carry_tmp;
    }

    return carry;
}

//! Subtract two limb vectors in place: dest -= src, where len(dest) == len(src)
[[maybe_unused]] static APY_INLINE apy_limb_t apy_inplace_subtraction_same_length(
    apy_limb_t* dest, const apy_limb_t* src, const std::size_t limbs
)
{
    assert(limbs > 0);

    // Specialized first iteration
    apy_limb_t carry = (dest[0] < src[0]);
    dest[0] -= src[0];

    for (std::size_t i = 1; i < limbs; i++) {
        apy_limb_t tmp_src = src[i] + carry;
        carry = (tmp_src < carry) + (dest[i] < tmp_src);
        dest[i] -= tmp_src;
    }
    return carry;
}

//! Subtract two limb vectors in place: dest = src - dest, where len(dest) ==
//! len(src)
[[maybe_unused]] static APY_INLINE apy_limb_t
apy_inplace_reversed_subtraction_same_length(
    apy_limb_t* dest, const apy_limb_t* src, const std::size_t limbs
)
{
    assert(limbs > 0);

    // Specialized first iteration
    apy_limb_t carry = (src[0] < dest[0]);
    dest[0] = src[0] - dest[0];

    for (std::size_t i = 1; i < limbs; i++) {
        dest[i] += carry;
        carry = (dest[i] < carry) + (src[i] < dest[i]);
        dest[i] = src[i] - dest[i];
    }
    return carry;
}

//! Subtract two limb vectors: dest = src0 - src1, where len(dest) == len(src0) ==
//! len(src1)
[[maybe_unused]] static APY_INLINE apy_limb_t apy_subtraction_same_length(
    apy_limb_t* dest,
    const apy_limb_t* src0,
    const apy_limb_t* src1,
    const std::size_t limbs
)
{
    assert(limbs > 0);

    // Specialized first iteration
    apy_limb_t carry = (src0[0] < src1[0]);
    dest[0] = src0[0] - src1[0];

    for (std::size_t i = 1; i < limbs; i++) {
        apy_limb_t tmp_src1 = src1[i] + carry;
        carry = (tmp_src1 < carry) + (src0[i] < tmp_src1);
        dest[i] = src0[i] - tmp_src1;
    }
    return carry;
}

// Shift

//! Left-shift limb vector in place
apy_limb_t apy_inplace_left_shift(apy_limb_t*, const std::size_t, unsigned int);
//! Left-shift limb vector: dest == src << shift_amount
apy_limb_t apy_left_shift(
    apy_limb_t* dest,
    const apy_limb_t* src,
    const std::size_t limbs,
    const unsigned shift_amount
);

//! Right-shift limb vector in place
apy_limb_t apy_inplace_right_shift(apy_limb_t*, const std::size_t, unsigned int);
/* apy_limb_t
apy_rshift(apy_limb_t*, const apy_limb_t*, const std::size_t, const unsigned int); */

// Multiplication

//! Multiply two unsigned limb vectors
apy_limb_t apy_unsigned_multiplication(
    apy_limb_t*,
    const apy_limb_t*,
    const std::size_t,
    const apy_limb_t*,
    const std::size_t
);

//! Square an unsigned limb vector
apy_limb_t apy_unsigned_square(apy_limb_t*, const apy_limb_t*, const std::size_t);

// Division

//! Class representing the inverse/reciprocal of the denominator
class APyDivInverse {
public:
    //! Normalization shift count.
    unsigned norm_shift;
    //! Normalized denominator, most significant limb
    apy_limb_t norm_denominator_1;
    //! Normalized denominator, least singificant limb (unused for single limb
    //! denominators)
    apy_limb_t norm_denominator_0;
    //! Inverse, for 2/1 or 3/2.
    apy_limb_t inverse;
    //! Constructor from limb vector.
    explicit APyDivInverse(const apy_limb_t*, std::size_t);

private:
    //! Compute the 3/2 (or 2/1) inverse.
    void compute_3by2_inverse();
};

//! Divide two unsigned limb vectors
void apy_unsigned_division(
    apy_limb_t*, apy_limb_t*, const std::size_t, const apy_limb_t*, const std::size_t
);

//! Divide two unsigned limb vectors where the APyDivInverse is pre-computed
void apy_unsigned_division_preinverted(
    apy_limb_t* quotient,
    apy_limb_t* numerator,
    const std::size_t numerator_limbs,
    const apy_limb_t* denominator,
    const std::size_t denominator_limbs,
    const APyDivInverse* inv
);

#endif
