/*
 * Arithmetic and utility functions for APyTypes complex-valued floating-point types
 */

#include "apyfloat_util.h"
#include "src/apytypes_fwd.h"

#include <cstddef> // std::size_t

//! Iterator-based complex-valued floating-point products
template <
    const std::size_t SRC1_INC = 1,
    const std::size_t SRC2_INC = 1,
    const std::size_t DST_INC = 1,
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
[[maybe_unused]] static APY_INLINE void floating_point_complex_products(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_INOUT dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const APyFloatSpec& dst_spec,
    const std::size_t n_elements,
    const QuantizationMode& qntz
)
{
    // Retrieve the specialized quantization method to use
    auto qntz_func = get_qntz_func(qntz);

    // Pre-compute some useful values
    const unsigned SUM_MAN_BITS = src1_spec.man_bits + src2_spec.man_bits;
    const exp_t SRC1_MAX_EXP = ((1ULL << src1_spec.exp_bits) - 1);
    const exp_t SRC2_MAX_EXP = ((1ULL << src2_spec.exp_bits) - 1);
    const exp_t RES_MAX_EXP = ((1ULL << dst_spec.exp_bits) - 1);

    const unsigned NEW_MAN_BITS = SUM_MAN_BITS + 2;
    const man_t TWO = 1ULL << (NEW_MAN_BITS);
    const man_t TWO_BEFORE = 1ULL << (NEW_MAN_BITS - 1);
    const man_t ONE_BEFORE = 1ULL << (NEW_MAN_BITS - 2);
    const man_t TWO_RES = (1ULL << dst_spec.man_bits);
    const int MAN_DELTA = NEW_MAN_BITS - dst_spec.man_bits;
    const man_t STICKY = (1ULL << (MAN_DELTA - 1)) - 1;

    // Reusable constants
    const man_t FINAL_RES_LO = (1ULL << dst_spec.man_bits);
    const man_t RES_LO = FINAL_RES_LO << 3;
    const man_t CARRY_RES_LO = RES_LO << 1;
    const man_t MAN_MASK = CARRY_RES_LO - 1;
    const unsigned NORMALIZATION_CONST = _MAN_T_SIZE_BITS - dst_spec.man_bits - 4;

    /*
     * Lambdas for calling the actual floating-point multiplication/addition functions
     */
    const auto MUL_SHORT = [&](auto lambda_src1, auto lambda_src2, auto lambda_dst) {
        _floating_point_mul_short(
            lambda_src1,
            lambda_src2,
            lambda_dst,
            src1_spec,
            src2_spec,
            dst_spec,
            qntz,
            qntz_func,
            SUM_MAN_BITS,
            SRC1_MAX_EXP,
            SRC2_MAX_EXP,
            RES_MAX_EXP,
            TWO,
            TWO_BEFORE,
            ONE_BEFORE,
            TWO_RES,
            MAN_DELTA,
            STICKY
        );
    };
    const auto MUL_LONG = [&](auto lambda_src1, auto lambda_src2, auto lambda_dst) {
        _floating_point_mul_general(
            lambda_src1,
            lambda_src2,
            lambda_dst,
            src1_spec,
            src2_spec,
            dst_spec,
            qntz,
            SRC1_MAX_EXP,
            SRC2_MAX_EXP,
            RES_MAX_EXP
        );
    };
    const auto ADD_SHORT = [&](auto lambda_src1, auto lambda_src2, auto lambda_dst) {
        _floating_point_add_same_wl</* SUB = */ false>(
            lambda_src1,
            lambda_src2,
            lambda_dst,
            dst_spec,
            qntz,
            qntz_func,
            RES_MAX_EXP,
            FINAL_RES_LO,
            RES_LO,
            CARRY_RES_LO,
            MAN_MASK,
            NORMALIZATION_CONST
        );
    };
    const auto SUB_SHORT = [&](auto lambda_src1, auto lambda_src2, auto lambda_dst) {
        _floating_point_add_same_wl</* SUB = */ true>(
            lambda_src1,
            lambda_src2,
            lambda_dst,
            dst_spec,
            qntz,
            qntz_func,
            RES_MAX_EXP,
            FINAL_RES_LO,
            RES_LO,
            CARRY_RES_LO,
            MAN_MASK,
            NORMALIZATION_CONST
        );
    };
    const auto ADD_LONG = [&](auto lambda_src1, auto lambda_src2, auto lambda_dst) {
        _floating_point_add_general</* SUB = */ false>(
            lambda_src1,
            lambda_src2,
            lambda_dst,
            dst_spec,
            dst_spec,
            dst_spec,
            qntz,
            RES_MAX_EXP
        );
    };
    const auto SUB_LONG = [&](auto lambda_src1, auto lambda_src2, auto lambda_dst) {
        _floating_point_add_general</* SUB = */ true>(
            lambda_src1,
            lambda_src2,
            lambda_dst,
            dst_spec,
            dst_spec,
            dst_spec,
            qntz,
            RES_MAX_EXP
        );
    };

    const bool CAN_MUL_SHORT = SUM_MAN_BITS < _MAN_LIMIT_BITS;
    const bool CAN_ADD_SHORT = std::size_t(dst_spec.man_bits) + 5 <= _MAN_T_SIZE_BITS
        && qntz != QuantizationMode::STOCH_WEIGHTED;

    // Partial products
    APyFloatData ac, ad, bc, bd;

    if (CAN_MUL_SHORT && CAN_ADD_SHORT) {
        /*
         * Specialization #1: both the short floating-point multiplication and addition
         * can be used. This is the fastest path
         */
        for (std::size_t i = 0; i < n_elements; i++) {
            // Perform partial products: a*c, a*d, b*c, b*d
            for (APyFloatData* const spec : { &ac, &ad, &bc, &bd }) {
                auto a = src1 + 2 * i * SRC1_INC + (spec == &bc || spec == &bd ? 1 : 0);
                auto b = src2 + 2 * i * SRC2_INC + (spec == &ad || spec == &bd ? 1 : 0);
                MUL_SHORT(a, b, spec);
            }

            // Perform additions: ac - bd, ad + bc
            SUB_SHORT(ac, bd, dst + 2 * i * DST_INC + 0);
            ADD_SHORT(ad, bc, dst + 2 * i * DST_INC + 1);
        }
    } else if (CAN_ADD_SHORT) {
        /*
         * Specialization #2: It is not possible to perform the short multiplication,
         * but short addition is still possible.
         */
        for (std::size_t i = 0; i < n_elements; i++) {
            // Perform partial products: a*c, a*d, b*c, b*d
            for (APyFloatData* const spec : { &ac, &ad, &bc, &bd }) {
                auto a = src1 + 2 * i * SRC1_INC + (spec == &bc || spec == &bd ? 1 : 0);
                auto b = src2 + 2 * i * SRC2_INC + (spec == &ad || spec == &bd ? 1 : 0);
                MUL_LONG(a, b, spec);
            }

            // Perform additions: ac - bd, ad + bc
            SUB_SHORT(ac, bd, dst + 2 * i * DST_INC + 0);
            ADD_SHORT(ad, bc, dst + 2 * i * DST_INC + 1);
        }
    } else if (CAN_MUL_SHORT) {
        /*
         * Specialization #3: It is not possible to perform the short addition, but
         * short multiplication is still possible.
         */
        for (std::size_t i = 0; i < n_elements; i++) {
            //
            // Perform partial products: a*c, a*d, b*c, b*d
            //
            for (APyFloatData* const spec : { &ac, &ad, &bc, &bd }) {
                auto a = src1 + 2 * i * SRC1_INC + (spec == &bc || spec == &bd ? 1 : 0);
                auto b = src2 + 2 * i * SRC2_INC + (spec == &ad || spec == &bd ? 1 : 0);
                MUL_LONG(a, b, spec);
            }

            // Perform additions: ac - bd, ad + bc
            SUB_SHORT(ac, bd, dst + 2 * i * DST_INC + 0);
            ADD_SHORT(ad, bc, dst + 2 * i * DST_INC + 1);
        }
    } else {
        /*
         * General case: always works, but is the slowest. It performs the most general
         * floating-point multiplication and addition/subtraction.
         */
        for (std::size_t i = 0; i < n_elements; i++) {
            //
            // Perform partial products: a*c, a*d, b*c, b*d
            //
            for (APyFloatData* const spec : { &ac, &ad, &bc, &bd }) {
                auto a = src1 + 2 * i * SRC1_INC + (spec == &bc || spec == &bd ? 1 : 0);
                auto b = src2 + 2 * i * SRC2_INC + (spec == &ad || spec == &bd ? 1 : 0);
                MUL_LONG(a, b, spec);
            }

            // Perform additions: ac - bd, ad + bc
            SUB_LONG(ac, bd, dst + 2 * i * DST_INC + 0);
            ADD_LONG(ad, bc, dst + 2 * i * DST_INC + 1);
        }
    }
}

//! Iterator-based complex-valued floating-point quotients
template <
    const std::size_t SRC1_INC = 1,
    const std::size_t SRC2_INC = 1,
    const std::size_t DST_INC = 1,
    typename RANDOM_ACCESS_ITERATOR_IN1,
    typename RANDOM_ACCESS_ITERATOR_IN2,
    typename RANDOM_ACCESS_ITERATOR_INOUT>
[[maybe_unused]] static APY_INLINE void floating_point_complex_quotients(
    RANDOM_ACCESS_ITERATOR_IN1 src1,
    RANDOM_ACCESS_ITERATOR_IN2 src2,
    RANDOM_ACCESS_ITERATOR_INOUT dst,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const APyFloatSpec& dst_spec,
    const std::size_t n_elements,
    const QuantizationMode& qntz
)
{
    /*
     * For valuable information on floating-point complex-valued division arithmetic,
     * read ``Annex G, IEC 60559-compatibale complex arithmetic'' of the C99 standard:
     * https://open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf
     *
     * Normalization is especially important to achieve equivilance between real-valued
     * floating-point division and complex-valued floating-point division of real-valued
     * operands. It is further needed to have equivalent behaviour for NaNs and infs
     * between real-valued and complex-valued divisions.
     *
     * COMPLEX-VALUED DIVISION FORMULA:
     *
     *   a + bi        ac + bd       bc - ad
     *  --------  =  ----------- + ----------- i
     *   c + di       c^2 + d^2     c^2 + d^2
     *
     */

    // Partial results
    APyFloatData c_scaled, d_scaled;
    APyFloatData ac, ad, bc, bd;
    APyFloatData c_sqr, d_sqr;
    APyFloatData real_num, imag_num, den;

    // Denominator normalization value
    int ilogbw;

    // Retrieve the specialized quantization method to use
    auto qntz_func = get_qntz_func(qntz);

    // Logic for recovering zeros and infs
    const auto RECOVER_INFS_AND_ZEROS = [&](APyFloatData& dst_real,
                                            APyFloatData& dst_imag,
                                            const APyFloatData& a,
                                            const APyFloatData& b,
                                            const APyFloatData& c,
                                            const APyFloatData& d) {
        if (is_nan(dst_real, dst_spec) && is_nan(dst_imag, dst_spec)) {
            const exp_t DST_MAX_EXP = (1ULL << dst_spec.exp_bits) - 1;
            if (is_zero(den) && (!is_nan(a, src1_spec) || !is_nan(b, src1_spec))) {
                dst_real = { ac.sign, DST_MAX_EXP, is_zero(a) || is_nan(a, src1_spec) };
                dst_imag = { bc.sign, DST_MAX_EXP, is_zero(b) || is_nan(b, src1_spec) };
            } else if ((is_inf(a, src1_spec) || is_inf(b, src1_spec))
                       && is_finite(c, src2_spec) && is_finite(d, src2_spec)) {
                dst_real = { real_num.sign, DST_MAX_EXP, 0 }; // inf
                dst_imag = { imag_num.sign, DST_MAX_EXP, 0 }; // inf
            } else if ((is_inf(c, src2_spec) || is_inf(d, src2_spec))
                       && is_finite(a, src1_spec) && is_finite(b, src1_spec)) {
                dst_real = { real_num.sign, 0, 0 }; // zero
                dst_imag = { imag_num.sign, 0, 0 }; // zero
            }
        }
    };

    /*
     * NOTE:
     *
     * A significant optimization opportunity is to here specialize the different
     * floating-point arithmetic routines (`short`, `long`, `same_wl`, etc). There are
     * alot of combinations to consider however. It should give somewhat of a speed up
     * to specialize as all floating-point constants used in the arithmetic routines
     * won't have to be re-evaluated for each element.
     */
    if (src1_spec == dst_spec && src2_spec == dst_spec) {
        /*
         * Sources and destination have the exact same floating-point format (spec)
         */
        const APyFloatSpec& spec = dst_spec;
        for (std::size_t i = 0; i < n_elements; i++) {

            const APyFloatData& a = *(src1 + 2 * i * SRC1_INC + 0);
            const APyFloatData& b = *(src1 + 2 * i * SRC1_INC + 1);
            const APyFloatData& c = *(src2 + 2 * i * SRC2_INC + 0);
            const APyFloatData& d = *(src2 + 2 * i * SRC2_INC + 1);

            // `ilogbw` = `log2(max(abs(c, d)))`
            ilogbw = is_zero(c) && is_zero(d)
                ? 0
                : std::max(
                      std::get<0>(pure_exp(c, spec)), std::get<0>(pure_exp(d, spec))
                  );

            // Compute squared magnitued denominator: c^2 + d^2
            c_scaled = floating_point_scalbn(c, spec, -ilogbw, qntz, qntz_func);
            d_scaled = floating_point_scalbn(d, spec, -ilogbw, qntz, qntz_func);
            floating_point_product(*c_scaled, *c_scaled, c_sqr, spec, spec, spec, qntz);
            floating_point_product(*d_scaled, *d_scaled, d_sqr, spec, spec, spec, qntz);
            floating_point_sum(c_sqr, d_sqr, den, spec, spec, spec, qntz);

            // Perform partial products: a*c, a*d, b*c, b*d
            floating_point_product(a, c_scaled, ac, spec, spec, spec, qntz);
            floating_point_product(a, d_scaled, ad, spec, spec, spec, qntz);
            floating_point_product(b, c_scaled, bc, spec, spec, spec, qntz);
            floating_point_product(b, d_scaled, bd, spec, spec, spec, qntz);

            // Perform additions: (ac + bd) and (bc - ad)
            constexpr bool IS_ADD = false, IS_SUB = true;
            floating_point_sum<IS_ADD>(ac, bd, real_num, spec, spec, spec, qntz);
            floating_point_sum<IS_SUB>(bc, ad, imag_num, spec, spec, spec, qntz);

            APyFloatData& dst_real = *(dst + 2 * i * DST_INC + 0);
            APyFloatData& dst_imag = *(dst + 2 * i * DST_INC + 1);
            floating_point_quotient(real_num, den, dst_real, spec, spec, spec, qntz);
            floating_point_quotient(imag_num, den, dst_imag, spec, spec, spec, qntz);
            dst_real = floating_point_scalbn(dst_real, spec, -ilogbw, qntz, qntz_func);
            dst_imag = floating_point_scalbn(dst_imag, spec, -ilogbw, qntz, qntz_func);
            RECOVER_INFS_AND_ZEROS(dst_real, dst_imag, a, b, c, d);
        }
    } else {
        /*
         * The two sources have different floating-point formats (spec)
         */
        for (std::size_t i = 0; i < n_elements; i++) {
            const APyFloatData& a = *(src1 + 2 * i * SRC1_INC + 0);
            const APyFloatData& b = *(src1 + 2 * i * SRC1_INC + 1);
            const APyFloatData& c = floating_point_cast_no_quant(
                *(src2 + 2 * i * SRC2_INC + 0), src2_spec, dst_spec
            );
            const APyFloatData& d = floating_point_cast_no_quant(
                *(src2 + 2 * i * SRC2_INC + 1), src2_spec, dst_spec
            );

            // `ilogbw` = `log2(max(abs(c, d)))`
            if (is_zero(c) && is_zero(d)) {
                ilogbw = 0;
            } else {
                ilogbw = std::max(
                    std::get<0>(pure_exp(c, dst_spec)),
                    std::get<0>(pure_exp(d, dst_spec))
                );
            }

            // Compute squared magnitued denominator: c^2 + d^2
            c_scaled = floating_point_scalbn(c, dst_spec, -ilogbw, qntz, qntz_func);
            d_scaled = floating_point_scalbn(d, dst_spec, -ilogbw, qntz, qntz_func);
            floating_point_product(
                *c_scaled, *c_scaled, c_sqr, dst_spec, dst_spec, dst_spec, qntz
            );
            floating_point_product(
                *d_scaled, *d_scaled, d_sqr, dst_spec, dst_spec, dst_spec, qntz
            );
            floating_point_sum(c_sqr, d_sqr, den, dst_spec, dst_spec, dst_spec, qntz);

            // Perform partial products: a*c, a*d, b*c, b*d
            floating_point_product(
                a, c_scaled, ac, src1_spec, dst_spec, dst_spec, qntz
            );
            floating_point_product(
                a, d_scaled, ad, src1_spec, dst_spec, dst_spec, qntz
            );
            floating_point_product(
                b, c_scaled, bc, src1_spec, dst_spec, dst_spec, qntz
            );
            floating_point_product(
                b, d_scaled, bd, src1_spec, dst_spec, dst_spec, qntz
            );

            // Perform additions: (ac + bd) and (bc - ad)
            constexpr bool ADD = false, SUB = true;
            floating_point_sum<ADD>(
                ac, bd, real_num, dst_spec, dst_spec, dst_spec, qntz
            );
            floating_point_sum<SUB>(
                bc, ad, imag_num, dst_spec, dst_spec, dst_spec, qntz
            );

            APyFloatData& dst_real = *(dst + 2 * i * DST_INC + 0);
            APyFloatData& dst_imag = *(dst + 2 * i * DST_INC + 1);
            floating_point_quotient(
                real_num, den, dst_real, dst_spec, dst_spec, dst_spec, qntz
            );
            floating_point_quotient(
                imag_num, den, dst_imag, dst_spec, dst_spec, dst_spec, qntz
            );
            dst_real
                = floating_point_scalbn(dst_real, dst_spec, -ilogbw, qntz, qntz_func);
            dst_imag
                = floating_point_scalbn(dst_imag, dst_spec, -ilogbw, qntz, qntz_func);
            RECOVER_INFS_AND_ZEROS(dst_real, dst_imag, a, b, c, d);
        }
    }
}

//! Perform a single float complex product
[[maybe_unused]] static APY_INLINE void floating_point_complex_product(
    const APyFloatData& src1_real,
    const APyFloatData& src1_imag,
    const APyFloatData& src2_real,
    const APyFloatData& src2_imag,
    APyFloatData& dst_real,
    APyFloatData& dst_imag,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const APyFloatSpec& dst_spec,
    const QuantizationMode& qntz
)
{
    const auto src1 = { std::cref(src1_real), std::cref(src1_imag) };
    const auto src2 = { std::cref(src2_real), std::cref(src2_imag) };
    const auto dst = { std::ref(dst_real), std::ref(dst_imag) };
    floating_point_complex_products(
        std::begin(src1),
        std::begin(src2),
        std::begin(dst),
        src1_spec,
        src2_spec,
        dst_spec,
        1 /* n */,
        qntz
    );
}

//! Perform a single float complex division
[[maybe_unused]] static APY_INLINE void floating_point_complex_quotient(
    const APyFloatData& src1_real,
    const APyFloatData& src1_imag,
    const APyFloatData& src2_real,
    const APyFloatData& src2_imag,
    APyFloatData& dst_real,
    APyFloatData& dst_imag,
    const APyFloatSpec& src1_spec,
    const APyFloatSpec& src2_spec,
    const APyFloatSpec& dst_spec,
    const QuantizationMode& qntz
)
{
    const auto src1 = { std::cref(src1_real), std::cref(src1_imag) };
    const auto src2 = { std::cref(src2_real), std::cref(src2_imag) };
    const auto dst = { std::ref(dst_real), std::ref(dst_imag) };
    floating_point_complex_quotients(
        std::begin(src1),
        std::begin(src2),
        std::begin(dst),
        src1_spec,
        src2_spec,
        dst_spec,
        1 /* n */,
        qntz
    );
}
