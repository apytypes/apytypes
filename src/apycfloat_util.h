/*
 * Arithmetic and utility functions for APyTypes complex-valued floating-point types
 */

#include "apyfloat_util.h"
#include "apytypes_fwd.h"

#include <cstddef>    // std::size_t
#include <functional> // std::invoke

/* ********************************************************************************** *
 * *              Floating-point iterator-based arithmetic functors                 * *
 * ********************************************************************************** */

template <std::size_t SRC1_INC = 1, std::size_t SRC2_INC = 1, std::size_t DST_INC = 1>
struct FloatingPointComplexMultiplier {
    /*
     * GENERAL COMPLEX-VALUED MULTIPLICATION FORMULA:
     *
     *  (a + bi) * (c + di) = (ac - bd) + (ad + bc)i
     *
     */
    explicit FloatingPointComplexMultiplier(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
        : mul(src1_spec, src2_spec, dst_spec, qntz)
        , add(dst_spec, dst_spec, dst_spec, qntz)
        , sub(dst_spec, dst_spec, dst_spec, qntz)
    {
    }

    // Perform floating-point complex multiplications
    void operator()(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems = 1
    ) const
    {
        APyFloatData ac, ad, bc, bd;
        for (std::size_t i = 0; i < nitems; i++) {
            const APyFloatData* a = src1 + 2 * i * SRC1_INC + 0;
            const APyFloatData* b = src1 + 2 * i * SRC1_INC + 1;
            const APyFloatData* c = src2 + 2 * i * SRC2_INC + 0;
            const APyFloatData* d = src2 + 2 * i * SRC2_INC + 1;

            // Perform partial products: a*c, a*d, b*c, b*d
            mul(a, c, &ac);
            mul(a, d, &ad);
            mul(b, c, &bc);
            mul(b, d, &bd);

            // Perform additions: ac - bd, ad + bc
            sub(&ac, &bd, dst + 2 * i * DST_INC + 0);
            add(&ad, &bc, dst + 2 * i * DST_INC + 1);
        }
    }

private:
    // Set first during functor initialization
    FloatingPointMultiplier<> mul;
    FloatingPointAdder<> add;
    FloatingPointSubtractor<> sub;
};

template <std::size_t SRC1_INC = 1, std::size_t SRC2_INC = 1, std::size_t DST_INC = 1>
struct FloatingPointComplexDivider {
    /*
     * For valuable information on floating-point complex-valued division arithmetic,
     * read ``Annex G, IEC 60559-compatibale complex arithmetic'' of the C99 standard:
     * https://open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf
     *
     * The used complex-valued division formula (algorithm) can be found here:
     * https://dl.acm.org/doi/pdf/10.1145/368637.368661
     *
     * Normalization is especially important to achieve equivalence between real-valued
     * floating-point division and complex-valued floating-point division of real-valued
     * operands. Logic for restoring proper NaNs and infs is further needed to have
     * equivalent behaviour between real-valued and complex-valued divisions.
     *
     * GENERAL COMPLEX-VALUED DIVISION FORMULA:
     *
     *   a + bi        ac + bd       bc - ad
     *  --------  =  ----------- + ----------- i
     *   c + di       c^2 + d^2     c^2 + d^2
     *
     */
    explicit FloatingPointComplexDivider(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
        : src1_spec(src1_spec)
        , src2_spec(src2_spec)
        , dst_spec(dst_spec)
        , qntz(qntz)
        , qntz_f(get_qntz_func(qntz))
        , add(dst_spec, dst_spec, dst_spec, qntz)
        , sub(dst_spec, dst_spec, dst_spec, qntz)
        , mul(dst_spec, dst_spec, dst_spec, qntz)
        , div(dst_spec, dst_spec, dst_spec, qntz)
    {
        using F = FloatingPointComplexDivider<SRC1_INC, SRC2_INC, DST_INC>;
        if (src1_spec == dst_spec && src2_spec == dst_spec) {
            f = &F::template complex_div</* PROMOTE_RHS = */ false>;
        } else { /* SUM_MAN_BITS > _MAN_LIMIT_BITS */
            f = &F::template complex_div</* PROMOTE_RHS = */ true>;
        }
    }

    // Perform floating-point complex divisions
    void operator()(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems = 1
    ) const
    {
        std::invoke(f, this, src1, src2, dst, nitems);
    }

private:
    // Set first during functor initialization
    const APyFloatSpec src1_spec, src2_spec, dst_spec;
    const QuantizationMode qntz;

    // Pointer to quantization function
    decltype(get_qntz_func(qntz)) qntz_f;

    FloatingPointAdder<> add;
    FloatingPointSubtractor<> sub;
    FloatingPointMultiplier<> mul;
    FloatingPointDivider<> div;

    // Pointer `f` to the correct function based on the floating-point specs
    void (FloatingPointComplexDivider::*f)(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems
    ) const {};

    template <bool PROMOTE_RHS>
    void complex_div(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems
    ) const
    {

        APyFloatData r;
        APyFloatData den;
        APyFloatData ar, br, cr, dr;
        APyFloatData num_real, num_imag;

        for (std::size_t i = 0; i < nitems; i++) {
            auto caster = floating_point_cast<decltype(get_qntz_func(qntz))>;

            // Operands: a, b, c, d; (a + bi) / (c + di)
            const APyFloatData& a = PROMOTE_RHS
                ? caster(
                      *(src1 + 2 * i * SRC1_INC + 0), src1_spec, dst_spec, qntz, qntz_f
                  )
                : *(src1 + 2 * i * SRC1_INC + 0);
            const APyFloatData& b = PROMOTE_RHS
                ? caster(
                      *(src1 + 2 * i * SRC1_INC + 1), src1_spec, dst_spec, qntz, qntz_f
                  )
                : *(src1 + 2 * i * SRC1_INC + 1);
            const APyFloatData& c = PROMOTE_RHS
                ? caster(
                      *(src2 + 2 * i * SRC2_INC + 0), src2_spec, dst_spec, qntz, qntz_f
                  )
                : *(src2 + 2 * i * SRC2_INC + 0);
            const APyFloatData& d = PROMOTE_RHS
                ? caster(
                      *(src2 + 2 * i * SRC2_INC + 1), src2_spec, dst_spec, qntz, qntz_f
                  )
                : *(src2 + 2 * i * SRC2_INC + 1);

            // Destination
            APyFloatData& dst_real = *(dst + 2 * i * DST_INC + 0);
            APyFloatData& dst_imag = *(dst + 2 * i * DST_INC + 1);

            if (is_zero(c) && is_zero(d)
                && (!is_nan(a, src1_spec) || !is_nan(b, src1_spec))) {
                // This branch should be removed once the zeros and infs recovery logic
                // is added to floating-point complex division.
                const exp_t DST_MAX_EXP = exp_t((1ULL << dst_spec.exp_bits) - 1);
                bool re = a.sign ^ c.sign;
                bool im = b.sign ^ c.sign;
                dst_real = { re, DST_MAX_EXP, is_zero(a) || is_nan(a, src1_spec) };
                dst_imag = { im, DST_MAX_EXP, is_zero(b) || is_nan(b, src1_spec) };
                continue;
#if PY_VERSION_HEX >= 0x030E0000
                /*
                 * Recovering-zeros-and-infs logic was added to Python in Python 3.14:
                 * See: ``gh-119372: Recover inf's and zeros in _Py_c_quot (GH-119457)''
                 * (*) https://github.com/python/cpython/pull/119457
                 */
            } else if (is_nan(c, dst_spec) && is_nan(d, dst_spec)) {
#else
            } else if (is_nan(c, dst_spec) || is_nan(d, dst_spec)) {
#endif
                const exp_t DST_MAX_EXP = exp_t((1ULL << dst_spec.exp_bits) - 1);
                dst_real = { 0, DST_MAX_EXP, 1 }; // NaN
                dst_imag = { 0, DST_MAX_EXP, 1 }; // NaN
                continue;
            } else if (!floating_point_less_than_abs_same_wl(c, d)) { /* |c| >= |d| */
                div(d, c, r);
                mul(a, r, ar);
                mul(b, r, br);
                mul(d, r, dr);
                add(c, dr, den);
                add(a, br, num_real);
                sub(b, ar, num_imag);
                div(num_real, den, dst_real);
                div(num_imag, den, dst_imag);
            } else { /* floating_point_less_than_abs_same_wl(c, d) */
                // |c| < |d|
                div(c, d, r);
                mul(a, r, ar);
                mul(b, r, br);
                mul(c, r, cr);
                add(cr, d, den);
                add(ar, b, num_real);
                sub(br, a, num_imag);
                div(num_real, den, dst_real);
                div(num_imag, den, dst_imag);
            }

#if PY_VERSION_HEX >= 0x030E0000
            /*
             * Recovering-zeros-and-infs logic was added to Python in Python 3.14:
             * See: ``gh-119372: Recover inf's and zeros in _Py_c_quot (GH-119457)''
             * (*) https://github.com/python/cpython/pull/119457
             */
            if (is_nan(dst_real, dst_spec) && is_nan(dst_imag, dst_spec)) {
                if ((is_inf(a, dst_spec) || is_inf(b, dst_spec))
                    && is_finite(c, dst_spec) && is_finite(d, dst_spec)) {
                    APyFloatData INF = { 0, exp_t((1ULL << dst_spec.exp_bits)) - 1, 0 };
                    APyFloatData xc, xd, yc, yd, xc_yd, yc_xd;
                    exp_t x_exp = is_inf(a, dst_spec) ? dst_spec.bias : exp_t(0);
                    exp_t y_exp = is_inf(b, dst_spec) ? dst_spec.bias : exp_t(0);
                    APyFloatData x { a.sign, x_exp, 0 };
                    APyFloatData y { b.sign, y_exp, 0 };
                    mul(x, c, xc);
                    mul(y, d, yd);
                    mul(y, c, yc);
                    mul(x, d, xd);
                    add(xc, yd, xc_yd);
                    sub(yc, xd, yc_xd);
                    mul(INF, xc_yd, dst_real);
                    mul(INF, yc_xd, dst_imag);
                } else if ((is_inf(c, dst_spec) || is_inf(d, dst_spec))
                           && is_finite(a, dst_spec) && is_finite(b, dst_spec)) {
                    APyFloatData ZERO = { 0, 0, 0 };
                    APyFloatData ax, ay, bx, by, ax_by, bx_ay;
                    exp_t x_exp = is_inf(c, dst_spec) ? dst_spec.bias : exp_t(0);
                    exp_t y_exp = is_inf(d, dst_spec) ? dst_spec.bias : exp_t(0);
                    APyFloatData x { c.sign, x_exp, 0 };
                    APyFloatData y { d.sign, y_exp, 0 };
                    mul(a, x, ax);
                    mul(a, y, ay);
                    mul(b, x, bx);
                    mul(b, y, by);
                    add(ax, by, ax_by);
                    sub(bx, ay, bx_ay);
                    mul(ZERO, ax_by, dst_real);
                    mul(ZERO, bx_ay, dst_imag);
                }
            }
#endif
        }
    }
};
