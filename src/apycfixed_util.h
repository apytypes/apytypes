#include "apyfixed_util.h"
#include "apytypes_mp.h"
#include "src/apytypes_fwd.h"
#include "src/apytypes_util.h"

#include <algorithm>
#include <cstdint>

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

/* ********************************************************************************** *
 * *     Fixed-point iterator based arithmetic functions with multi-limb support    * *
 * ********************************************************************************** */

//! Iterator-based multi-limb two's complement complex-valued fixed-point
//! multiplication. The scratch vector `prod_imm` must have space for
//! at least `2 + 2 * src1_limbs + 2 * src2_limbs` limbs. The scratch vectors `op1_abs`
//! and `op2_abs` must have space for at least `src1_limbs` and `src2_limbs`
//! limbs, respectively. No overlap between `prod_imm` and `op[12]_abs` allowed. No
//! overlap between `prod_imm` and `dst` allowed.
template <
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_OUT,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void complex_fixed_point_product(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_OUT dst,
    std::size_t src1_limbs,
    std::size_t src2_limbs,
    std::size_t dst_limbs,
    RANDOM_ACCESS_ITERATOR_INOUT op1_abs,
    RANDOM_ACCESS_ITERATOR_INOUT op2_abs,
    RANDOM_ACCESS_ITERATOR_INOUT prod_imm
)
{
    /*
     * (a + bi)(c + di) = ac - bd + (bc + ad)i
     * Compute the imaginary part of the result first.
     */

    // b*c
    fixed_point_product(
        src1 + src1_limbs,           // src1 (b)
        src2,                        // src2 (c)
        prod_imm,                    // dst
        src1_limbs,                  // src1_limbs
        src2_limbs,                  // src2_limbs
        src1_limbs + src2_limbs + 1, // dst_limbs
        op1_abs,                     // op1_abs
        op2_abs,                     // op2_abs
        prod_imm                     // prod_abs
    );

    // a*d
    fixed_point_product(
        src1,                                   // src1 (a)
        src2 + src2_limbs,                      // src2 (d)
        prod_imm + src1_limbs + src2_limbs + 1, // dst
        src1_limbs,                             // src1_limbs
        src2_limbs,                             // src2_limbs
        src1_limbs + src2_limbs + 1,            // dst_limbs
        op1_abs,                                // op1_abs
        op2_abs,                                // op2_abs
        prod_imm + src1_limbs + src2_limbs + 1  // prod_abs
    );

    // Copy `src1 (imag)` to `op1_abs`, so that `src1` can be used as both a source and
    // destination. This is important for performing folding of complex-valued arrays.
    std::copy_n(src1 + src1_limbs, src1_limbs, op1_abs);

    // bc + ad
    apy_addition_same_length(
        &*(dst + dst_limbs),                             // dst (imag part)
        &*(prod_imm),                                    // src1 (b*c)
        &*(prod_imm + src1_limbs + src2_limbs + 1),      // src2 (a*d)
        std::min(src1_limbs + src2_limbs + 1, dst_limbs) // limbs
    );

    // b*d
    fixed_point_product(
        op1_abs,                                // src1 (b)
        src2 + src2_limbs,                      // src2 (d)
        prod_imm + src1_limbs + src2_limbs + 1, // dst
        src1_limbs,                             // src1_limbs
        src2_limbs,                             // src2_limbs
        src1_limbs + src2_limbs + 1,            // dst_limbs
        op1_abs,                                // op1_abs
        op2_abs,                                // op2_abs
        prod_imm + src1_limbs + src2_limbs + 1  // prod_abs
    );

    // a*c
    fixed_point_product(
        src1,                        // src1 (a)
        src2,                        // src2 (c)
        prod_imm,                    // dst
        src1_limbs,                  // src1_limbs
        src2_limbs,                  // src2_limbs
        src1_limbs + src2_limbs + 1, // dst limbs
        op1_abs,                     // op1_abs
        op2_abs,                     // op2_abs
        prod_imm                     // prod_abs
    );

    // ac - bd
    apy_subtraction_same_length(
        &*(dst),                                         // dst (real part)
        &*(prod_imm),                                    // src1 (a*c)
        &*(prod_imm + src1_limbs + src2_limbs + 1),      // src2 (b*d)
        std::min(src1_limbs + src2_limbs + 1, dst_limbs) // limbs
    );
}

//! Iterator-based multi-limb two's complement complex-valued fixed-point
//! division. The scratch vector `prod_imm` must have space for
//! at least `2 + 2 * src1_limbs + 2 * src2_limbs` limbs. The scratch vectors `op1_abs`
//! and `op2_abs` must have space for at least `src1_limbs` and `src2_limbs`
//! limbs, respectively. No overlap between `prod_imm` and `op[12]_abs` allowed. No
//! overlap between `prod_imm` and `dst` allowed.
template <
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_OUT,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
static APY_INLINE void complex_fixed_point_division(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_OUT dst,
    std::size_t src1_limbs,
    std::size_t src2_limbs,
    std::size_t dst_limbs,
    std::size_t src2_bits,
    std::size_t div_limbs,
    RANDOM_ACCESS_ITERATOR_INOUT op1_abs,
    RANDOM_ACCESS_ITERATOR_INOUT op2_abs,
    RANDOM_ACCESS_ITERATOR_INOUT prod_imm,
    RANDOM_ACCESS_ITERATOR_INOUT den_imm,
    RANDOM_ACCESS_ITERATOR_INOUT num_imm,
    RANDOM_ACCESS_ITERATOR_INOUT qte_imm
)
{
    //
    //  a + bi        ac + bd       bc - ad
    // --------  =  ----------- + ----------- i
    //  c + di       c^2 + d^2     c^2 + d^2
    //

    /*
     * Pre-compute the denominator: c^2 + d^2
     */
    fixed_point_square(
        src2,           // src (c)
        den_imm,        // dst
        src2_limbs,     // src_limbs
        2 * src2_limbs, // dst_limbs
        op2_abs,        // op_abs
        prod_imm        // prod_abs
    );
    fixed_point_square(
        src2 + src2_limbs, // src (c)
        prod_imm,          // dst
        src2_limbs,        // src_limbs
        2 * src2_limbs,    // dst_limbs
        op2_abs,           // op_abs
        prod_imm           // prod_abs
    );
    apy_inplace_addition_same_length(&den_imm[0], &prod_imm[0], 2 * src2_limbs);
    auto den_significant_limbs = significant_limbs(den_imm, den_imm + 2 * src2_limbs);

    /*                                          ac + bd
     * Compute the real part of the quotient: -----------
     *                                         c^2 + d^2
     */
    std::size_t prod_len = src1_limbs + src2_limbs + 1;
    fixed_point_product(
        src1,       // src1 (a)
        src2,       // src2 (c)
        prod_imm,   // dst
        src1_limbs, // src1_limbs
        src2_limbs, // src2_limbs
        prod_len,   // dst_limbs
        op1_abs,    // op1_abs
        op2_abs,    // op1_abs
        prod_imm    // prod_abs
    );
    fixed_point_product(
        src1 + src1_limbs,   // src1 (b)
        src2 + src2_limbs,   // src2 (d)
        prod_imm + prod_len, // dst
        src1_limbs,          // src1_limbs
        src2_limbs,          // src2_limbs
        prod_len,            // dst_limbs
        op1_abs,             // op1_abs
        op2_abs,             // op1_abs
        prod_imm + prod_len  // prod_abs
    );

    apy_inplace_addition_same_length(&prod_imm[0], &prod_imm[0] + prod_len, prod_len);
    bool real_negative = limb_vector_is_negative(prod_imm, prod_imm + prod_len);
    if (real_negative) {
        limb_vector_negate(prod_imm, prod_imm + div_limbs, num_imm);
    } else {
        std::copy(prod_imm, prod_imm + div_limbs, num_imm);
    }
    limb_vector_lsl(num_imm, num_imm + div_limbs, src2_bits);

    apy_unsigned_division(
        &qte_imm[0],          // Quotient
        &num_imm[0],          // Numerator
        div_limbs,            // Numerator limbs
        &den_imm[0],          // Denominator
        den_significant_limbs // Denominator significant limbs
    );

    if (real_negative) {
        limb_vector_negate(qte_imm, qte_imm + dst_limbs, dst);
    } else {
        std::copy_n(qte_imm, dst_limbs, dst);
    }

    /*                                               bc - ad
     * Compute the imaginary part of the quotient: -----------
     *                                              c^2 + d^2
     */
    fixed_point_product(
        src1 + src1_limbs, // src1 (b)
        src2,              // src2 (c)
        prod_imm,          // dst
        src1_limbs,        // src1_limbs
        src2_limbs,        // src2_limbs
        prod_len,          // dst_limbs
        op1_abs,           // op1_abs
        op2_abs,           // op1_abs
        prod_imm           // prod_abs
    );
    fixed_point_product(
        src1,                // src1 (a)
        src2 + src2_limbs,   // src2 (d)
        prod_imm + prod_len, // dst
        src1_limbs,          // src1_limbs
        src2_limbs,          // src2_limbs
        prod_len,            // dst_limbs
        op1_abs,             // op1_abs
        op2_abs,             // op1_abs
        prod_imm + prod_len  // prod_abs
    );

    apy_inplace_subtraction_same_length(
        &prod_imm[0], &prod_imm[0] + prod_len, prod_len
    );
    bool imag_negative = limb_vector_is_negative(prod_imm, prod_imm + prod_len);
    if (imag_negative) {
        limb_vector_negate(prod_imm, prod_imm + div_limbs, num_imm);
    } else {
        std::copy(prod_imm, prod_imm + div_limbs, num_imm);
    }
    limb_vector_lsl(num_imm, num_imm + div_limbs, src2_bits);

    apy_unsigned_division(
        &qte_imm[0],          // Quotient
        &num_imm[0],          // Numerator
        div_limbs,            // Numerator limbs
        &den_imm[0],          // Denominator
        den_significant_limbs // Denominator significant limbs
    );

    if (imag_negative) {
        limb_vector_negate(qte_imm, qte_imm + dst_limbs, dst + dst_limbs);
    } else {
        std::copy_n(qte_imm, dst_limbs, dst + dst_limbs);
    }
}

struct ComplexFixedPointInnerProduct {
    explicit ComplexFixedPointInnerProduct(
        const APyFixedSpec& src1_spec,
        const APyFixedSpec& src2_spec,
        const APyFixedSpec& dst_spec,
        const std::optional<APyFixedAccumulatorOption> acc_mode
    )
        : src1_limbs { bits_to_limbs(src1_spec.bits) }
        , src2_limbs { bits_to_limbs(src2_spec.bits) }
        , dst_limbs { bits_to_limbs(dst_spec.bits) }
        , acc_mode { acc_mode }
    {
        using F = ComplexFixedPointInnerProduct;

        if (!acc_mode.has_value()) {
            if (dst_limbs == 1) {
                // Specialization #1: the resulting number of limbs is exactly one and
                // no accumulator mode has been set.
                f = &F::inner_product_one_limb_src_one_limb_dst;
                return;
            } else if (dst_limbs == 2 && src1_limbs == 1 && src2_limbs == 1) {
                // Special case #2: resulting elements fit into two limbs, and each
                // argument element fits into a single limb.
                f = &F::inner_product_one_limb_src_two_limb_dst;
                return;
            }
        }

        // Initialize scratch vectors
        product_limbs = 1 + src1_limbs + src2_limbs;
        prod_imm = ScratchVector<apy_limb_t, 16>(2 * product_limbs);
        product = ScratchVector<apy_limb_t, 16>(2 * product_limbs);
        op1_abs = ScratchVector<apy_limb_t, 8>(src1_limbs);
        op2_abs = ScratchVector<apy_limb_t, 8>(src2_limbs);

        if (acc_mode.has_value()) {
            // Accumulator mode set, use the accumulator inner product
            assert(acc_mode->bits == dst_spec.bits);
            assert(acc_mode->int_bits == dst_spec.int_bits);
            product_bits = src1_spec.bits + src2_spec.bits;
            product_int_bits = src1_spec.int_bits + src2_spec.int_bits;
            f = &F::inner_product</*is_acc_context=*/true>;
        } else {
            f = &F::inner_product</*is_acc_context=*/false>;
        }
    }

    using It = APyBuffer<apy_limb_t>::vector_type::iterator;
    using CIt = APyBuffer<apy_limb_t>::vector_type::const_iterator;

    void operator()(
        CIt src1,
        CIt src2,
        It dst,
        std::size_t N,
        std::size_t M = 1,
        std::size_t DST_STEP = 1
    ) const
    {
        // Matrix-vector multiplication $`A \times b`$, where
        // * A (src1): [ `M` x `N` ]
        // * b (src2): [ `N` x `1` ]
        std::invoke(f, this, src1, src2, dst, N, M, DST_STEP);
    }

private:
    void inner_product_one_limb_src_one_limb_dst(
        CIt src1, CIt src2, It dst, std::size_t N, std::size_t M, std::size_t DST_STEP
    ) const
    {
        assert(src1_limbs == 1);
        assert(src2_limbs == 1);
        assert(dst_limbs == 1);
        for (std::size_t m = 0; m < M; m++) {
            auto A_it = src1 + 2 * N * m;
            auto acc = dst + m * 2 * DST_STEP;
            std::fill_n(acc, 2, 0);
            for (std::size_t n = 0; n < N; n++) {
                std::complex<apy_limb_signed_t> a(A_it[2 * n + 0], A_it[2 * n + 1]);
                std::complex<apy_limb_signed_t> b(src2[2 * n + 0], src2[2 * n + 1]);
                auto&& prod = a * b;
                acc[0] += prod.real();
                acc[1] += prod.imag();
            }
        }
    }

    void inner_product_one_limb_src_two_limb_dst(
        CIt src1, CIt src2, It dst, std::size_t N, std::size_t M, std::size_t DST_STEP
    ) const
    {
        assert(src1_limbs == 1);
        assert(src2_limbs == 1);
        assert(dst_limbs == 2);

        for (std::size_t m = 0; m < M; m++) {
            auto A_it = src1 + 2 * N * m;
#if (COMPILER_LIMB_SIZE == 64)
#if defined(__GNUC__)
            /*
             * GNU C-compatible compiler, including Clang, MacOS Xcode, and Intel C++
             * compiler (ICC).
             */
            __int128_t acc_re = 0;
            __int128_t acc_im = 0;
            for (std::size_t n = 0; n < N; n++) {
                __int128 z1_re = apy_limb_signed_t(A_it[2 * n + 0]);
                __int128 z1_im = apy_limb_signed_t(A_it[2 * n + 1]);
                __int128 z2_re = apy_limb_signed_t(src2[2 * n + 0]);
                __int128 z2_im = apy_limb_signed_t(src2[2 * n + 1]);
                __int128 ac = z1_re * z2_re;
                __int128 bd = z1_im * z2_im;
                __int128 bc = z1_im * z2_re;
                __int128 ad = z1_re * z2_im;
                acc_re += ac - bd;
                acc_im += bc + ad;
            }
            dst[4 * m * DST_STEP + 0] = apy_limb_t(acc_re >> 0);
            dst[4 * m * DST_STEP + 1] = apy_limb_t(acc_re >> 64);
            dst[4 * m * DST_STEP + 2] = apy_limb_t(acc_im >> 0);
            dst[4 * m * DST_STEP + 3] = apy_limb_t(acc_im >> 64);
#else
            /*
             * Microsoft Visual C/C++ compiler (or other unknown compiler)
             */
            constexpr auto tuple_to_array = [](auto&& t) {
                return std::array<apy_limb_t, 2> { std::get<1>(t), std::get<0>(t) };
            };
            std::array<apy_limb_t, 2> ac_bd;
            std::array<apy_limb_t, 2> bc_ad;
            auto acc = dst + m * 4 * DST_STEP;
            std::fill_n(acc, 4, 0);
            for (std::size_t n = 0; n < N; n++) {
                const std::complex<apy_limb_signed_t> z1(
                    A_it[2 * n + 0], A_it[2 * n + 1]
                );
                const std::complex<apy_limb_signed_t> z2(
                    src2[2 * n + 0], src2[2 * n + 1]
                );

                // Perform complex-valued multiplication:
                // * Re = ac - bd
                // * Im = bc + ad
                auto&& ac = tuple_to_array(long_signed_mult(z1.real(), z2.real()));
                auto&& bd = tuple_to_array(long_signed_mult(z1.imag(), z2.imag()));
                auto&& bc = tuple_to_array(long_signed_mult(z1.imag(), z2.real()));
                auto&& ad = tuple_to_array(long_signed_mult(z1.real(), z2.imag()));
                apy_subtraction_same_length(&ac_bd[0], &ac[0], &bd[0], 2);
                apy_addition_same_length(&bc_ad[0], &bc[0], &ad[0], 2);

                // Perform accumulation
                apy_inplace_addition_same_length(&acc[0], &ac_bd[0], 2);
                apy_inplace_addition_same_length(&acc[2], &bc_ad[0], 2);
            }

#endif
#elif (COMPILER_LIMB_SIZE == 32)
            std::int64_t acc_re = 0;
            std::int64_t acc_im = 0;
            for (std::size_t n = 0; n < N; n++) {
                std::int64_t z1_re = apy_limb_signed_t(A_it[2 * n + 0]);
                std::int64_t z1_im = apy_limb_signed_t(A_it[2 * n + 1]);
                std::int64_t z2_re = apy_limb_signed_t(src2[2 * n + 0]);
                std::int64_t z2_im = apy_limb_signed_t(src2[2 * n + 1]);
                std::int64_t ac = z1_re * z2_re;
                std::int64_t bd = z1_im * z2_im;
                std::int64_t bc = z1_im * z2_re;
                std::int64_t ad = z1_re * z2_im;
                acc_re += ac - bd;
                acc_im += bc + ad;
            }
            dst[4 * m * DST_STEP + 0] = apy_limb_t(acc_re >> 0);
            dst[4 * m * DST_STEP + 1] = apy_limb_t(acc_re >> 32);
            dst[4 * m * DST_STEP + 2] = apy_limb_t(acc_im >> 0);
            dst[4 * m * DST_STEP + 3] = apy_limb_t(acc_im >> 32);

#endif
        }
    }

    template <bool is_acc_context = false>
    void inner_product(
        CIt src1, CIt src2, It dst, std::size_t N, std::size_t M, std::size_t DST_STEP
    ) const
    {
        for (std::size_t m = 0; m < M; m++) {
            auto A_it = src1 + 2 * src1_limbs * N * m;
            auto acc = dst + m * 2 * dst_limbs * DST_STEP;
            std::fill_n(acc, 2 * dst_limbs, 0);
            for (std::size_t n = 0; n < N; n++) {
                complex_fixed_point_product(
                    A_it + n * 2 * src1_limbs, // src1
                    src2 + n * 2 * src2_limbs, // src2
                    std::begin(product),       // dst
                    src1_limbs,                // src1_limbs
                    src2_limbs,                // src2_limbs
                    product_limbs,             // dst_limbs
                    std::begin(op1_abs),       // op1_abs scratch vector
                    std::begin(op2_abs),       // op2_abs scratch vector
                    std::begin(prod_imm)       // prod_imm scratch_vector
                );

                // NOTE: No sign extension needed for the complex-valued fixed-point
                // inner product as `complex_fixed_point_product` already increases the
                // limb length of the product by one. A one-dimensional array would need
                // to be larger than 32 GB (32-limb system) and 36 EB (64-limb system)
                // before additional sign extension is needed...

                // *Constexpr*: conditionally quantize and overflow the product if this
                // inner product takes place within an accumulator context.
                if constexpr (is_acc_context) {
                    // Real
                    quantize(
                        std::begin(product) + 0 * product_limbs,
                        std::begin(product) + 1 * product_limbs,
                        product_bits,
                        product_int_bits,
                        acc_mode->bits,
                        acc_mode->int_bits,
                        acc_mode->quantization
                    );
                    // Imag
                    quantize(
                        std::begin(product) + 1 * product_limbs,
                        std::begin(product) + 2 * product_limbs,
                        product_bits,
                        product_int_bits,
                        acc_mode->bits,
                        acc_mode->int_bits,
                        acc_mode->quantization
                    );
                    // Real
                    overflow(
                        std::begin(product) + 0 * product_limbs,
                        std::begin(product) + 1 * product_limbs,
                        acc_mode->bits,
                        acc_mode->int_bits,
                        acc_mode->overflow
                    );
                    // Imag
                    overflow(
                        std::begin(product) + 1 * product_limbs,
                        std::begin(product) + 2 * product_limbs,
                        acc_mode->bits,
                        acc_mode->int_bits,
                        acc_mode->overflow
                    );
                }

                // Accumulate data
                apy_inplace_addition_same_length(
                    &acc[0 * dst_limbs],
                    &product[0 * product_limbs],
                    std::min(dst_limbs, product_limbs)
                );
                apy_inplace_addition_same_length(
                    &acc[1 * dst_limbs],
                    &product[1 * product_limbs],
                    std::min(dst_limbs, product_limbs)
                );
            }
        }
    }

    // Pointer `f` to the correct function based on the limb lengths
    void (ComplexFixedPointInnerProduct::*f)(
        CIt src1, CIt src2, It dst, std::size_t N, std::size_t M, std::size_t DST_STEP
    ) const;

    std::size_t src1_limbs, src2_limbs, dst_limbs, product_limbs;
    std::optional<APyFixedAccumulatorOption> acc_mode;
    int product_bits, product_int_bits;
    mutable ScratchVector<apy_limb_t, 8> op1_abs;
    mutable ScratchVector<apy_limb_t, 8> op2_abs;
    mutable ScratchVector<apy_limb_t, 16> product;
    mutable ScratchVector<apy_limb_t, 16> prod_imm;
};
