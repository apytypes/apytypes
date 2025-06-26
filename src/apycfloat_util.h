/*
 * Arithmetic and utility functions for APyTypes complex-valued floating-point types
 */

#include "apyfloat_util.h"
#include "apytypes_fwd.h"

#include <cmath>      // std::isnan, std::signbit
#include <cstddef>    // std::size_t
#include <functional> // std::invoke

/* ********************************************************************************** *
 * *              Complex-valued floating-point helper functions                    * *
 * ********************************************************************************** */

/*!
 * Convert a APyTypes complex-valued floating-point to a string:
 * Returns, e.g., "1.25-2.5j" or "-6+nanj"
 */
[[maybe_unused, nodiscard]] static APY_INLINE std::string
complex_floating_point_to_str_dec(
    const APyFloatData& re_data, const APyFloatData& im_data, const APyFloatSpec& spec
)
{
    // NOTE: Python, unlike C++, unconditionally encodes the string of a floating-point
    //       NaN without a minus sign.
    const double re = floating_point_to_double(re_data, spec);
    const double im = floating_point_to_double(im_data, spec);

    auto&& re_str = std::isnan(re) ? "nan" : fmt::format("{}", re);
    if (std::isnan(im)) {
        return fmt::format("{}+nanj", re_str);
    } else {
        return fmt::format("{}{}{}j", re_str, std::signbit(im) ? "" : "+", im);
    }
}

/* ********************************************************************************** *
 * *              Floating-point iterator-based arithmetic functors                 * *
 * ********************************************************************************** */

template <
    bool IS_SUBTRACT,
    std::size_t SRC1_INC,
    std::size_t SRC2_INC,
    std::size_t DST_INC>
struct _ComplexFloatingPointAddSub {
public:
    explicit _ComplexFloatingPointAddSub(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
        : functor(src1_spec, src2_spec, dst_spec, qntz)
    {
    }

    // Perform `nitems` complex-valued floating-point additions/subtractions
    void operator()(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems = 1
    ) const
    {
        functor(src1 + 0, src2 + 0, dst + 0, nitems); // real
        functor(src1 + 1, src2 + 1, dst + 1, nitems); // imag
    }

private:
    _FloatingPointAddSub<IS_SUBTRACT, 2 * SRC1_INC, 2 * SRC2_INC, 2 * DST_INC> functor;
};

template <std::size_t SRC1_INC = 1, std::size_t SRC2_INC = 1, std::size_t DST_INC = 1>
using ComplexFloatingPointAdder = _ComplexFloatingPointAddSub<
    /*is_subtract = */ false,
    SRC1_INC,
    SRC2_INC,
    DST_INC>;

template <std::size_t SRC1_INC = 1, std::size_t SRC2_INC = 1, std::size_t DST_INC = 1>
using ComplexFloatingPointSubtractor = _ComplexFloatingPointAddSub<
    /* is_subtract = */ true,
    SRC1_INC,
    SRC2_INC,
    DST_INC>;

template <std::size_t SRC1_INC = 1, std::size_t SRC2_INC = 1, std::size_t DST_INC = 1>
struct ComplexFloatingPointMultiplier {
    /*
     * GENERAL COMPLEX-VALUED MULTIPLICATION FORMULA:
     *
     *  (a + bi) * (c + di) = (ac - bd) + (ad + bc)i
     *
     */
    explicit ComplexFloatingPointMultiplier(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
    {
        using F = ComplexFloatingPointMultiplier;
        using MUL_F_SHORT = _FloatingPointMultiplierShort;
        using MUL_F_GENERAL = _FloatingPointMultiplierGeneral;
        using ADD_F_GENERAL = _FloatingPointAddSubGeneral</*IS_SUBTRACT=*/false>;
        using ADD_F_SHORT = _FloatingPointAddSubSameWl</*IS_SUBTRACT=*/false>;
        using SUB_F_GENERAL = _FloatingPointAddSubGeneral</*IS_SUBTRACT=*/true>;
        using SUB_F_SHORT = _FloatingPointAddSubSameWl</*IS_SUBTRACT=*/true>;

        bool is_short_mul = src1_spec.man_bits + src2_spec.man_bits <= _MAN_LIMIT_BITS;
        bool is_short_add = qntz != QuantizationMode::STOCH_WEIGHTED
            && std::size_t(dst_spec.man_bits + 5) <= _MAN_T_SIZE_BITS;

        if (is_short_mul) {
            mul_short = MUL_F_SHORT(src1_spec, src2_spec, dst_spec, qntz);
            if (is_short_add) {
                add_same_wl = ADD_F_SHORT(dst_spec, dst_spec, dst_spec, qntz);
                sub_same_wl = SUB_F_SHORT(dst_spec, dst_spec, dst_spec, qntz);
                f = &F::product</*SHORT_MUL=*/true, /*SHORT_ADD=*/true>;
            } else { /* !is_short_add */
                add_general = ADD_F_GENERAL(dst_spec, dst_spec, dst_spec, qntz);
                sub_general = SUB_F_GENERAL(dst_spec, dst_spec, dst_spec, qntz);
                f = &F::product</*SHORT_MUL=*/true, /*SHORT_ADD=*/false>;
            }
        } else { /* !is_short_mul */
            mul_general = MUL_F_GENERAL(src1_spec, src2_spec, dst_spec, qntz);
            if (is_short_add) {
                add_same_wl = ADD_F_SHORT(dst_spec, dst_spec, dst_spec, qntz);
                sub_same_wl = SUB_F_SHORT(dst_spec, dst_spec, dst_spec, qntz);
                f = &F::product</*SHORT_MUL=*/false, /*SHORT_ADD=*/true>;
            } else { /* !is_short_add */
                add_general = ADD_F_GENERAL(dst_spec, dst_spec, dst_spec, qntz);
                sub_general = SUB_F_GENERAL(dst_spec, dst_spec, dst_spec, qntz);
                f = &F::product</*SHORT_MUL=*/false, /*SHORT_ADD=*/false>;
            }
        }
    }

    // Perform floating-point complex multiplications
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
    template <bool SHORT_MUL, bool SHORT_ADD>
    void product(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems
    ) const
    {
        APyFloatData ac, ad, bc, bd;
        for (std::size_t i = 0; i < nitems; i++) {
            const APyFloatData* a = src1 + 2 * i * SRC1_INC + 0;
            const APyFloatData* b = src1 + 2 * i * SRC1_INC + 1;
            const APyFloatData* c = src2 + 2 * i * SRC2_INC + 0;
            const APyFloatData* d = src2 + 2 * i * SRC2_INC + 1;

            // Perform partial products: a*c, a*d, b*c, b*d
            if constexpr (SHORT_MUL) {
                mul_short(a, c, &ac);
                mul_short(a, d, &ad);
                mul_short(b, c, &bc);
                mul_short(b, d, &bd);
            } else {
                mul_general(a, c, &ac);
                mul_general(a, d, &ad);
                mul_general(b, c, &bc);
                mul_general(b, d, &bd);
            }

            // Perform additions: ac - bd, ad + bc
            if constexpr (SHORT_ADD) {
                sub_same_wl(&ac, &bd, dst + 2 * i * DST_INC + 0);
                add_same_wl(&ad, &bc, dst + 2 * i * DST_INC + 1);
            } else {
                sub_general(&ac, &bd, dst + 2 * i * DST_INC + 0);
                add_general(&ad, &bc, dst + 2 * i * DST_INC + 1);
            }
        }
    }

    // Pointer `f` to the correct function based on the floating-point specs
    void (ComplexFloatingPointMultiplier::*f)(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t nitems
    ) const;

    _FloatingPointAddSubSameWl<false> add_same_wl;
    _FloatingPointAddSubGeneral<false> add_general;
    _FloatingPointAddSubSameWl<true> sub_same_wl;
    _FloatingPointAddSubGeneral<true> sub_general;
    _FloatingPointMultiplierShort mul_short;
    _FloatingPointMultiplierGeneral mul_general;
};

template <std::size_t SRC1_INC = 1, std::size_t SRC2_INC = 1, std::size_t DST_INC = 1>
struct ComplexFloatingPointDivider {
    /*
     * For valuable information on floating-point complex-valued division arithmetic,
     * read ``Annex G, IEC 60559-compatible complex arithmetic'' of the C99 standard:
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
    explicit ComplexFloatingPointDivider(
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
        using F = ComplexFloatingPointDivider<SRC1_INC, SRC2_INC, DST_INC>;
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
    void (ComplexFloatingPointDivider::*f)(
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

        auto caster = floating_point_cast<decltype(get_qntz_func(qntz))>;
        for (std::size_t i = 0; i < nitems; i++) {

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

struct ComplexFloatingPointInnerProduct {
    explicit ComplexFloatingPointInnerProduct(
        const APyFloatSpec& src1_spec,
        const APyFloatSpec& src2_spec,
        const APyFloatSpec& dst_spec,
        const QuantizationMode& qntz
    )
    {
        using F = ComplexFloatingPointInnerProduct;
        using MUL_F_SHORT = _FloatingPointMultiplierShort;
        using MUL_F_GENERAL = _FloatingPointMultiplierGeneral;
        using ADD_F_GENERAL = _FloatingPointAddSubGeneral</*IS_SUBTRACT=*/false>;
        using ADD_F_SHORT = _FloatingPointAddSubSameWl</*IS_SUBTRACT=*/false>;
        using SUB_F_GENERAL = _FloatingPointAddSubGeneral</*IS_SUBTRACT=*/true>;
        using SUB_F_SHORT = _FloatingPointAddSubSameWl</*IS_SUBTRACT=*/true>;

        bool is_short_mul = src1_spec.man_bits + src2_spec.man_bits <= _MAN_LIMIT_BITS;
        bool is_short_add = qntz != QuantizationMode::STOCH_WEIGHTED
            && std::size_t(dst_spec.man_bits + 5) <= _MAN_T_SIZE_BITS;

        if (is_short_mul) {
            mul_short = MUL_F_SHORT(src1_spec, src2_spec, dst_spec, qntz);
            if (is_short_add) {
                add_same_wl = ADD_F_SHORT(dst_spec, dst_spec, dst_spec, qntz);
                sub_same_wl = SUB_F_SHORT(dst_spec, dst_spec, dst_spec, qntz);
                f = &F::inner_product</*SHORT_MUL=*/true, /*SHORT_ADD=*/true>;
            } else { /* !is_short_add */
                add_general = ADD_F_GENERAL(dst_spec, dst_spec, dst_spec, qntz);
                sub_general = SUB_F_GENERAL(dst_spec, dst_spec, dst_spec, qntz);
                f = &F::inner_product</*SHORT_MUL=*/true, /*SHORT_ADD=*/false>;
            }
        } else { /* !is_short_mul */
            mul_general = MUL_F_GENERAL(src1_spec, src2_spec, dst_spec, qntz);
            if (is_short_add) {
                add_same_wl = ADD_F_SHORT(dst_spec, dst_spec, dst_spec, qntz);
                sub_same_wl = SUB_F_SHORT(dst_spec, dst_spec, dst_spec, qntz);
                f = &F::inner_product</*SHORT_MUL=*/false, /*SHORT_ADD=*/true>;
            } else { /* !is_short_add */
                add_general = ADD_F_GENERAL(dst_spec, dst_spec, dst_spec, qntz);
                sub_general = SUB_F_GENERAL(dst_spec, dst_spec, dst_spec, qntz);
                f = &F::inner_product</*SHORT_MUL=*/false, /*SHORT_ADD=*/false>;
            }
        }
    }

    void operator()(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t N,
        std::size_t M = 1,
        std::size_t DST_STEP = 1
    ) const
    {
        std::invoke(f, this, src1, src2, dst, N, M, DST_STEP);
    }

private:
    template <bool SHORT_MUL, bool SHORT_ADD>
    void inner_product(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t N,
        std::size_t M,
        std::size_t DST_STEP
    ) const
    {
        // Matrix-vector multiplication $`A \times b`$, where
        // * A: [ `M` x `N` ]
        // * b: [ `N` x `1` ]
        APyFloatData prod_re { 0, 0, 0 };
        APyFloatData prod_im { 0, 0, 0 };
        for (std::size_t m = 0; m < M; m++) {
            auto A_it = src1 + 2 * N * m;
            APyFloatData sum_re { 0, 0, 0 };
            APyFloatData sum_im { 0, 0, 0 };
            for (std::size_t n = 0; n < N; n++) {
                auto lhs = A_it + 2 * n;
                auto rhs = src2 + 2 * n;
                complex_product<SHORT_MUL, SHORT_ADD>(
                    lhs + 0, lhs + 1, rhs + 0, rhs + 1, &prod_re, &prod_im
                );
                complex_sum<SHORT_ADD>(
                    &prod_re, &prod_im, &sum_re, &sum_im, &sum_re, &sum_im
                );
            }
            *(dst + 2 * DST_STEP * m + 0) = sum_re;
            *(dst + 2 * DST_STEP * m + 1) = sum_im;
        }
    }

    template <bool SHORT_MUL, bool SHORT_ADD>
    void complex_product(
        const APyFloatData* x_re,
        const APyFloatData* x_im,
        const APyFloatData* y_re,
        const APyFloatData* y_im,
        APyFloatData* z_re,
        APyFloatData* z_im
    ) const
    {
        // Perform partial products: a*c, a*d, b*c, b*d
        APyFloatData ac, ad, bc, bd;
        if constexpr (SHORT_MUL) {
            mul_short(x_re, y_re, &ac);
            mul_short(x_re, y_im, &ad);
            mul_short(x_im, y_re, &bc);
            mul_short(x_im, y_im, &bd);
        } else {
            mul_general(x_re, y_re, &ac);
            mul_general(x_re, y_im, &ad);
            mul_general(x_im, y_re, &bc);
            mul_general(x_im, y_im, &bd);
        }

        // Perform additions: ac - bd, ad + bc
        if constexpr (SHORT_ADD) {
            sub_same_wl(&ac, &bd, z_re);
            add_same_wl(&ad, &bc, z_im);
        } else {
            sub_general(&ac, &bd, z_re);
            add_general(&ad, &bc, z_im);
        }
    }

    template <bool SHORT_ADD>
    void complex_sum(
        const APyFloatData* x_re,
        const APyFloatData* x_im,
        const APyFloatData* y_re,
        const APyFloatData* y_im,
        APyFloatData* z_re,
        APyFloatData* z_im
    ) const
    {

        if constexpr (SHORT_ADD) {
            add_same_wl(x_re, y_re, z_re);
            add_same_wl(x_im, y_im, z_im);
        } else {
            add_general(x_re, y_re, z_re);
            add_general(x_im, y_im, z_im);
        }
    }

    // Pointer `f` to the correct function based on the floating-point specs
    void (ComplexFloatingPointInnerProduct::*f)(
        const APyFloatData* src1,
        const APyFloatData* src2,
        APyFloatData* dst,
        std::size_t N,
        std::size_t M,
        std::size_t DST_STEP
    ) const;

    _FloatingPointAddSubSameWl<false> add_same_wl;
    _FloatingPointAddSubGeneral<false> add_general;
    _FloatingPointAddSubSameWl<true> sub_same_wl;
    _FloatingPointAddSubGeneral<true> sub_general;
    _FloatingPointMultiplierShort mul_short;
    _FloatingPointMultiplierGeneral mul_general;
};
