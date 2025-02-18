/* For std::size_t */
#include <cassert>
#include <cstddef>

#include "apytypes_mp.h"
#include "apytypes_util.h"

// Addition

apy_limb_t apy_inplace_addition(
    apy_limb_t* dest,
    const std::size_t dest_limbs,
    const apy_limb_t* src,
    const std::size_t src_limbs
)
{
    assert(dest_limbs >= src_limbs);
    assert(src_limbs > 0);

    apy_limb_t carry = apy_inplace_addition_same_length(dest, src, src_limbs);
    if (dest_limbs > src_limbs && carry != 0) {
        carry = apy_inplace_addition_single_limb(
            dest + src_limbs, dest_limbs - src_limbs, carry
        );
    }
    return carry;
}

// Shift

apy_limb_t apy_inplace_left_shift(
    apy_limb_t* dest, const std::size_t limbs, const unsigned int shift_amount
)
{
    assert(limbs > 0);
    assert(shift_amount > 0);
    assert(shift_amount < APY_LIMB_SIZE_BITS);

    dest += limbs;

    const apy_limb_t* src = dest;
    const unsigned int overlap = APY_LIMB_SIZE_BITS - shift_amount;
    apy_limb_t low_limb = *--src;
    const apy_limb_t retval = low_limb >> overlap;
    apy_limb_t high_limb = (low_limb << shift_amount);
    std::size_t n = limbs;
    // TODO: Rewrite as for-loop and/or possibly avoid using two pointers
    while (--n != 0) {
        low_limb = *--src;
        *--dest = high_limb | (low_limb >> overlap);
        high_limb = (low_limb << shift_amount);
    }
    *--dest = high_limb;

    return retval;
}

apy_limb_t apy_inplace_right_shift(
    apy_limb_t* dest, const std::size_t limbs, const unsigned int shift_amount
)
{
    assert(limbs > 0);
    assert(shift_amount > 0);
    assert(shift_amount < APY_LIMB_SIZE_BITS);

    const apy_limb_t* src = dest;
    const unsigned int overlap = APY_LIMB_SIZE_BITS - shift_amount;
    apy_limb_t high_limb = *src++;
    const apy_limb_t retval = (high_limb << overlap);
    apy_limb_t low_limb = high_limb >> shift_amount;
    std::size_t n = limbs;
    // TODO: Rewrite as for-loop and/or possibly avoid using two pointers
    while (--n != 0) {
        high_limb = *src++;
        *dest++ = low_limb | (high_limb << overlap);
        low_limb = high_limb >> shift_amount;
    }
    *dest = low_limb;

    return retval;
}

apy_limb_t apy_left_shift(
    apy_limb_t* dest,
    const apy_limb_t* src,
    const std::size_t limbs,
    const unsigned int shift_amount
)
{
    assert(limbs > 0);
    assert(shift_amount > 0);
    assert(shift_amount < APY_LIMB_SIZE_BITS);

    src += limbs;
    dest += limbs;

    const unsigned int overlap = APY_LIMB_SIZE_BITS - shift_amount;
    apy_limb_t low_limb = *--src;
    const apy_limb_t retval = low_limb >> overlap;
    apy_limb_t high_limb = (low_limb << shift_amount);
    std::size_t n = limbs;

    while (--n != 0) {
        low_limb = *--src;
        *--dest = high_limb | (low_limb >> overlap);
        high_limb = (low_limb << shift_amount);
    }
    *--dest = high_limb;

    return retval;
}

/* Currently unused. Remove?
apy_limb_t apy_rshift(
    apy_limb_t* dest,
    const apy_limb_t* src,
    const std::size_t limbs,
    const unsigned int shift_amount
)
{
    assert(limbs > 0);
    assert(shift_amount > 0);
    assert(shift_amount < APY_LIMB_SIZE_BITS);

    const unsigned int overlap = APY_LIMB_SIZE_BITS - shift_amount;
    apy_limb_t high_limb = *src++;
    const apy_limb_t retval = (high_limb << overlap);
    apy_limb_t low_limb = high_limb >> shift_amount;
    std::size_t n = limbs;

    while (--n != 0) {
        high_limb = *src++;
        *dest++ = low_limb | (high_limb << overlap);
        low_limb = high_limb >> shift_amount;
    }
    *dest = low_limb;

    return retval;
}
*/

// Multiplication
apy_limb_t apy_multiplication_with_single_limb(
    apy_limb_t* dest,
    const apy_limb_t* src0,
    const std::size_t limbs,
    const apy_limb_t src1
)
{
    assert(limbs > 0);

    apy_limb_t carry = 0;
    for (std::size_t i = 0; i < limbs; i++) {
        auto [prod_high, prod_low] = long_mult(src0[i], src1);

        prod_low += carry;
        carry = (prod_low < carry) + prod_high;

        dest[i] = prod_low;
    }

    return carry;
}

apy_limb_t apy_addmul_single_limb(
    apy_limb_t* dest,
    const apy_limb_t* src0,
    const std::size_t limbs,
    const apy_limb_t src1
)
{
    assert(limbs > 0);

    apy_limb_t carry = 0;
    for (std::size_t i = 0; i < limbs; i++) {
        auto [prod_high, prod_low] = long_mult(src0[i], src1);

        prod_low += carry;
        carry = (prod_low < carry) + prod_high;

        prod_low += dest[i];
        carry += prod_low < dest[i];
        dest[i] = prod_low;
    }

    return carry;
}

apy_limb_t apy_submul_single_limb(
    apy_limb_t* dest, const apy_limb_t* src0, std::size_t limbs, apy_limb_t src1
)
{
    assert(limbs > 0);

    apy_limb_t carry = 0;
    for (std::size_t i = 0; i < limbs; i++) {
        auto [prod_high, prod_low] = long_mult(src0[i], src1);

        prod_low += carry;
        carry = (prod_low < carry) + prod_high;

        apy_limb_t tmp_dest = dest[i];
        prod_low = tmp_dest - prod_low;
        carry += prod_low > tmp_dest;
        dest[i] = prod_low;
    }

    return carry;
}

apy_limb_t apy_unsigned_multiplication(
    apy_limb_t* dest,
    const apy_limb_t* src0,
    const std::size_t src0_limbs,
    const apy_limb_t* src1,
    const std::size_t src1_limbs
)
{
    assert(src0_limbs >= src1_limbs);
    assert(src1_limbs > 0);

    // Multiply src0 with the least significant limb of src1
    dest[src0_limbs]
        = apy_multiplication_with_single_limb(dest, src0, src0_limbs, src1[0]);

    // Multiply src0 with the remaining limbs of src1, adding the previous partial
    // results
    for (std::size_t i = 1; i < src1_limbs; i++) {
        dest[src0_limbs + i]
            = apy_addmul_single_limb(&dest[i], src0, src0_limbs, src1[i]);
    }
    return dest[src0_limbs + src1_limbs - 1];
}

//! Utility function to avoid automatic promotion to signed int in case apy_limb_t
//! is smaller than int.
//! Should rarely happen, but better safe than sorry...
inline apy_limb_t apy_limb_multiplication(apy_limb_t src0, apy_limb_t src1)
{
    static_assert(
        APY_LIMB_SIZE_BYTES >= sizeof(int),
        "You are trying to build APyTypes on a platform which has default types that "
        "are not currently supported. Please open an issue at "
        "https://github.com/apytypes/apytypes/issues with information about the "
        "platform and we will be happy to add support for it."
    );
    return src0 * src1;
}

inline void apy_inplace_two_limb_addition(
    apy_limb_t* dest_high, apy_limb_t* dest_low, apy_limb_t src_high, apy_limb_t src_low
)
{
    apy_limb_t tmp_low = *dest_low + src_low;
    *dest_high += src_high + (tmp_low < *dest_low);
    *dest_low = tmp_low;
}

inline void apy_inplace_two_limb_subtraction(
    apy_limb_t* dest_high, apy_limb_t* dest_low, apy_limb_t src_high, apy_limb_t src_low
)
{
    apy_limb_t tmp_low = *dest_low - src_low;
    *dest_high -= (src_high + (*dest_low < src_low));
    *dest_low = tmp_low;
}

inline void apy_partial_inplace_two_limb_subtraction(
    apy_limb_t* dest_high,
    apy_limb_t* dest_low,
    apy_limb_t partial_low,
    apy_limb_t src_high,
    apy_limb_t src_low
)
{
    // Although *dest_low can be assigned here directly without using a temporary
    // variable, compilers seems to prefer this version. For gcc x86-64, one less
    // instruction is used.
    apy_limb_t tmp_low = partial_low - src_low;
    *dest_high -= (src_high + (partial_low < src_low));
    *dest_low = tmp_low;
}

/* The 3/2 inverse is defined as

    m = floor( (B^3-1) / (B u1 + u0)) - B
*/
apy_limb_t APyDivInverse::compute_3by2_inverse(apy_limb_t u1, apy_limb_t u0)
{
    static_assert(
        sizeof(unsigned) * 2 >= APY_LIMB_SIZE_BYTES,
        "You are trying to build APyTypes on a platform which has default types "
        "that are not currently supported. Please open an issue at "
        "https://github.com/apytypes/apytypes/issues with information about the "
        "platform and we will be happy to add support for it."
    );
    /* For notation, let b denote the half-limb base, so that B = b^2.
        Split u1 = b u1_high + u1_low. */
    unsigned u1_low = u1 & APY_LOWER_LIMB_MASK;
    unsigned u1_high = u1 >> APY_HALF_LIMB_SIZE_BITS;

    /* Approximation of the high half of quotient. Differs from the 2/1
        inverse of the half limb u1_high, since we have already subtracted
        u0. */
    unsigned quotient_high_approx = (u1 ^ APY_NUMBER_MASK) / u1_high;

    /* Adjust to get a half-limb 3/2 inverse, i.e., we want

        quotient_high_approx' = floor( (b^3 - 1) / u) - b
                              = floor ((b^3 - b u - 1) / u
                              = floor( (b (~u) + b-1) / u),

        and the remainder

        remainder = b (~u) + b-1 - quotient_high_approx (b u1_high + u1_low)
                  = b (~u - quotient_high_approx u1_high) +
                    b-1 - quotient_high_approx u1_low

        Subtraction of quotient_high_approx u1_low may underflow, which implies
       adjustments. But by normalization, 2 u >= B > quotient_high_approx u1_low, so we
       need to adjust by at most 2.
    */

    apy_limb_t remainder = ((~u1 - (apy_limb_t)quotient_high_approx * u1_high)
                            << APY_HALF_LIMB_SIZE_BITS)
        | APY_LOWER_LIMB_MASK;

    apy_limb_t p = (apy_limb_t)quotient_high_approx * u1_low;
    /* Adjustment steps taken from udiv_qrnnd_c */
    if (remainder < p) {
        quotient_high_approx--;
        remainder += u1;
        if (remainder >= u1) /* i.e. we didn't get carry when adding to remainder */
            if (remainder < p) {
                quotient_high_approx--;
                remainder += u1;
            }
    }
    remainder -= p;

    /* Low half of the quotient is

        quotient_low = floor ( (b remainder + b-1) / u1).

        This is a 3/2 division (on half-limbs), for which quotient_high_approx is a
        suitable inverse. */

    p = (remainder >> APY_HALF_LIMB_SIZE_BITS) * quotient_high_approx + remainder;
    /* Unlike full-limb 3/2, we can add 1 without overflow. For this to
        work, it is essential that quotient_low is a full apy_limb_t. */
    apy_limb_t quotient_low = (p >> APY_HALF_LIMB_SIZE_BITS) + 1;

    /* By the 3/2 trick, we don't need the high half limb. */
    remainder = (remainder << APY_HALF_LIMB_SIZE_BITS) + APY_LOWER_LIMB_MASK
        - quotient_low * u1;

    if (remainder >= (APY_NUMBER_MASK & (p << APY_HALF_LIMB_SIZE_BITS))) {
        quotient_low--;
        remainder += u1;
    }
    apy_limb_t m
        = ((apy_limb_t)quotient_high_approx << APY_HALF_LIMB_SIZE_BITS) + quotient_low;
    if (remainder >= u1) {
        m++;
        remainder -= u1;
    }

    /* Now m is the 2/1 inverse of u1. If u0 > 0, adjust it to become a
       3/2 inverse. */
    if (u0 > 0) {
        remainder = ~remainder;
        remainder += u0;
        if (remainder < u0) {
            m--;
            if (remainder >= u1) {
                m--;
                remainder -= u1;
            }
            remainder -= u1;
        }
        auto [prod_high, prod_low] = long_mult(u0, m);
        remainder += prod_high;
        if (remainder < prod_high) {
            m--;
            m -= ((remainder > u1) | ((remainder == u1) & (prod_low > u0)));
        }
    }

    return m;
}

APyDivInverse::APyDivInverse(
    const apy_limb_t* denominator, const std::size_t denominator_limbs
)
{
    assert(denominator_limbs > 0);
    if (denominator_limbs == 1) {
        norm_denominator_1 = denominator[0];
        assert(norm_denominator_1 > 0);
        norm_shift = leading_zeros(norm_denominator_1);
        norm_denominator_1 <<= norm_shift;
        norm_denominator_0 = 0;
    } else if (denominator_limbs == 2) {
        norm_denominator_1 = denominator[1];
        norm_denominator_0 = denominator[0];
        assert(norm_denominator_1 > 0);
        norm_shift = leading_zeros(norm_denominator_1);
        if (norm_shift > 0) {
            norm_denominator_1 = (norm_denominator_1 << norm_shift)
                | (norm_denominator_0 >> (APY_LIMB_SIZE_BITS - norm_shift));
            norm_denominator_0 <<= norm_shift;
        }
    } else {
        norm_denominator_1 = denominator[denominator_limbs - 1];
        norm_denominator_0 = denominator[denominator_limbs - 2];
        assert(norm_denominator_1 > 0);
        norm_shift = leading_zeros(norm_denominator_1);
        if (norm_shift > 0) {
            norm_denominator_1 = (norm_denominator_1 << norm_shift)
                | (norm_denominator_0 >> (APY_LIMB_SIZE_BITS - norm_shift));
            norm_denominator_0 = (norm_denominator_0 << norm_shift)
                | (denominator[denominator_limbs - 3]
                   >> (APY_LIMB_SIZE_BITS - norm_shift));
        }
    }
    inverse = compute_3by2_inverse(norm_denominator_1, norm_denominator_0);
}

apy_limb_t apy_division_single_limb_preinverted(
    apy_limb_t* quotient,
    const apy_limb_t* numerator,
    const std::size_t numerator_limbs,
    const APyDivInverse* inv
)
{
    assert(quotient != NULL);

    apy_limb_t remainder;

    if (inv->norm_shift > 0) {
        /* Shift, reusing quotient area. */
        remainder
            = apy_left_shift(quotient, numerator, numerator_limbs, inv->norm_shift);
        numerator = quotient;
    } else {
        remainder = 0;
    }

    for (apy_size_t limbs = numerator_limbs - 1; limbs >= 0; limbs--) {
        auto [quotient_high, quotient_low] = long_mult(remainder, inv->inverse);
        apy_inplace_two_limb_addition(
            &quotient_high, &quotient_low, remainder + 1, numerator[limbs]
        );
        remainder = numerator[limbs]
            - apy_limb_multiplication(quotient_high, inv->norm_denominator_1);
        apy_limb_t _mask
            = -(apy_limb_t)(remainder > quotient_low); /* both > and >= are OK */
        quotient_high += _mask;
        remainder += _mask & inv->norm_denominator_1;
        if (remainder >= inv->norm_denominator_1) {
            remainder -= inv->norm_denominator_1;
            quotient_high++;
        }

        quotient[limbs] = quotient_high;
    }

    return remainder >> inv->norm_shift;
}

void apy_division_3by2(
    apy_limb_t* quotient_tmp,
    apy_limb_t* numerator_1,
    apy_limb_t* numerator_0,
    apy_limb_t numerator_tmp,
    const APyDivInverse* inv
)
{
    auto [quotient_high, quotient_low] = long_mult(*numerator_1, inv->inverse);
    *quotient_tmp = quotient_high;
    apy_inplace_two_limb_addition(
        quotient_tmp, &quotient_low, *numerator_1, *numerator_0
    );

    /* Compute the two most significant limbs of limbs - quotient_tmp'd */
    *numerator_1 = *numerator_0
        - apy_limb_multiplication((inv->norm_denominator_1), *quotient_tmp);
    apy_partial_inplace_two_limb_subtraction(
        numerator_1,
        numerator_0,
        numerator_tmp,
        inv->norm_denominator_1,
        inv->norm_denominator_0
    );
    auto [t_high, t_low] = long_mult(inv->norm_denominator_0, *quotient_tmp);
    apy_inplace_two_limb_subtraction(numerator_1, numerator_0, t_high, t_low);
    (*quotient_tmp)++;

    /* Conditionally adjust quotient_tmp and the remainders */
    apy_limb_t _mask = -(apy_limb_t)(*numerator_1 >= quotient_low);
    *quotient_tmp += _mask;
    apy_inplace_two_limb_addition(
        numerator_1,
        numerator_0,
        _mask & (inv->norm_denominator_1),
        _mask & (inv->norm_denominator_0)
    );
    if (*numerator_1 >= inv->norm_denominator_1) {
        if (*numerator_1 > inv->norm_denominator_1
            || *numerator_0 >= inv->norm_denominator_0) {
            (*quotient_tmp)++;
            apy_inplace_two_limb_subtraction(
                numerator_1,
                numerator_0,
                (inv->norm_denominator_1),
                (inv->norm_denominator_0)
            );
        }
    }
}

void apy_division_double_limbs_preinverted(
    apy_limb_t* quotient,
    apy_limb_t* numerator,
    const std::size_t numerator_limbs,
    const APyDivInverse* inv
)
{
    assert(numerator_limbs >= 2);
    assert(quotient != NULL);

    apy_limb_t numerator_1
        = (inv->norm_shift > 0
               ? apy_inplace_left_shift(numerator, numerator_limbs, inv->norm_shift)
               : 0);

    apy_limb_t numerator_0 = numerator[numerator_limbs - 1];

    for (apy_size_t i = numerator_limbs - 2; i >= 0; i--) {
        apy_limb_t quotient_tmp;
        apy_division_3by2(&quotient_tmp, &numerator_1, &numerator_0, numerator[i], inv);

        quotient[i] = quotient_tmp;
    };

    if (inv->norm_shift > 0) {
        assert(
            (numerator_0 & (APY_NUMBER_MASK >> (APY_LIMB_SIZE_BITS - inv->norm_shift)))
            == 0
        );
        numerator_0 = (numerator_0 >> inv->norm_shift)
            | (numerator_1 << (APY_LIMB_SIZE_BITS - inv->norm_shift));
        numerator_1 >>= inv->norm_shift;
    }

    numerator[1] = numerator_1;
    numerator[0] = numerator_0;
}

void apy_division_multiple_limbs_preinverted(
    apy_limb_t* quotient,
    apy_limb_t* numerator,
    const std::size_t numerator_limbs,
    const apy_limb_t* denominator,
    const std::size_t denominator_limbs,
    const APyDivInverse* inv
)
{
    assert(denominator_limbs > 2);
    assert(numerator_limbs >= denominator_limbs);
    assert(quotient != NULL);
    assert((inv->norm_denominator_1 & APY_LIMB_MSBWEIGHT) != 0);

    // Normalize numerator
    apy_limb_t numerator_1
        = (inv->norm_shift > 0
               ? apy_inplace_left_shift(numerator, numerator_limbs, inv->norm_shift)
               : 0);
    /* Iteration variable is the index of the quotient_tmp limb.
     *
     * We divide <numerator_1, numerator[denominator_limbs-1+i],
     * numerator[denominator_limbs-2+i], numerator[denominator_limbs-3+i],...,
     * numerator[i]> by <inv->norm_denominator_1, inv->norm_denominator_0,
     * denominator[denominator_limbs-3],
     * ..., denominator[0] >
     */

    apy_limb_t quotient_tmp;
    for (apy_size_t i = numerator_limbs - denominator_limbs; i >= 0; i--) {
        apy_limb_t numerator_0 = numerator[denominator_limbs - 1 + i];

        if (numerator_1 == inv->norm_denominator_1
            && numerator_0 == inv->norm_denominator_0) {
            quotient_tmp = APY_NUMBER_MASK;
            apy_submul_single_limb(
                numerator + i, denominator, denominator_limbs, quotient_tmp
            );
            numerator_1
                = numerator[denominator_limbs - 1 + i]; /* update numerator_1, last
                                                  loop's value will now be invalid */
        } else {
            apy_division_3by2(
                &quotient_tmp,
                &numerator_1,
                &numerator_0,
                numerator[denominator_limbs - 2 + i],
                inv
            );

            apy_limb_t carry = apy_submul_single_limb(
                numerator + i, denominator, denominator_limbs - 2, quotient_tmp
            );

            apy_limb_t carry1 = numerator_0 < carry;
            numerator_0 -= carry;
            carry = numerator_1 < carry1;
            numerator_1 -= carry1;
            numerator[denominator_limbs - 2 + i] = numerator_0;

            if (carry != 0) {
                numerator_1 += inv->norm_denominator_1
                    + apy_inplace_addition_same_length(
                                   numerator + i,
                                   denominator,
                                   (std::size_t)(denominator_limbs - 1)
                    );
                quotient_tmp--;
            }
        }

        quotient[i] = quotient_tmp;
    };

    numerator[denominator_limbs - 1] = numerator_1;

    // Denormalize numerator back
    if (inv->norm_shift > 0) {
        apy_limb_t carry
            = apy_inplace_right_shift(numerator, denominator_limbs, inv->norm_shift);
        assert(carry == 0);
        (void)carry; // Avoid unused-warning
    }
}

void apy_division_preinverted(
    apy_limb_t* quotient,
    apy_limb_t* numerator,
    const std::size_t numerator_limbs,
    const apy_limb_t* denominator,
    const std::size_t denominator_limbs,
    const APyDivInverse* inv
)
{
    assert(denominator_limbs > 0);
    assert(numerator_limbs >= denominator_limbs);

    if (denominator_limbs == 1)
        numerator[0] = apy_division_single_limb_preinverted(
            quotient, numerator, numerator_limbs, inv
        );
    else if (denominator_limbs == 2)
        apy_division_double_limbs_preinverted(
            quotient, numerator, numerator_limbs, inv
        );
    else {
        apy_division_multiple_limbs_preinverted(
            quotient, numerator, numerator_limbs, denominator, denominator_limbs, inv
        );
    }
}

void apy_unsigned_division(
    apy_limb_t* quotient,
    apy_limb_t* numerator,
    const std::size_t numerator_limbs,
    const apy_limb_t* denominator,
    const std::size_t denominator_limbs
)
{
    assert(denominator_limbs > 0);
    assert(numerator_limbs >= denominator_limbs);
    assert(quotient != NULL);

    auto inv = APyDivInverse(denominator, denominator_limbs);
    if (denominator_limbs > 2 && inv.norm_shift > 0) {
        auto norm_denominator = std::vector<apy_limb_t>(denominator_limbs);
        apy_limb_t carry = apy_left_shift(
            &norm_denominator[0], denominator, denominator_limbs, inv.norm_shift
        );
        assert(carry == 0);
        (void)carry; // Avoid unused-warning
        apy_division_preinverted(
            quotient,
            numerator,
            numerator_limbs,
            &norm_denominator[0],
            denominator_limbs,
            &inv
        );
    } else {
        apy_division_preinverted(
            quotient, numerator, numerator_limbs, denominator, denominator_limbs, &inv
        );
    }
}
