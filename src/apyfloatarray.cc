// Python object access through Pybind
#include "apyfloatarray_iterator.h"
#include "apytypes_common.h"
#include "apytypes_util.h"
#include <cstddef>
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
#include <variant>
namespace nb = nanobind;
#include "apyfloat.h"
#include "apyfloat_util.h"
#include "apyfloatarray.h"
#include "array_utils.h"
#include "broadcast.h"
#include "ieee754.h"
#include "python_util.h"
#include <algorithm>
#include <fmt/format.h>
#include <iostream>
#include <set>
#include <stdexcept>
#include <stdlib.h>
#include <string>

void APyFloatArray::create_in_place(
    APyFloatArray* apyfloatarray,
    const nanobind::sequence& sign_seq,
    const nanobind::sequence& exp_seq,
    const nanobind::sequence& man_seq,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits);
    check_mantissa_format(man_bits);

    new (apyfloatarray)
        APyFloatArray(sign_seq, exp_seq, man_seq, exp_bits, man_bits, bias);
}

APyFloatArray::APyFloatArray(
    const nanobind::sequence& sign_seq,
    const nanobind::sequence& exp_seq,
    const nanobind::sequence& man_seq,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
    : exp_bits(exp_bits)
    , man_bits(man_bits)
{
    this->bias = bias.value_or(APyFloat::ieee_bias(exp_bits));

    const auto signs_shape = python_sequence_extract_shape(sign_seq);
    const auto exps_shape = python_sequence_extract_shape(exp_seq);
    const auto mans_shape = python_sequence_extract_shape(man_seq);

    if (!((signs_shape == exps_shape) && (signs_shape == mans_shape))) {
        throw std::domain_error("Shape mismatch during construction");
    }

    shape = signs_shape;

    auto signs = python_sequence_walk<nb::int_, nb::bool_>(sign_seq);
    auto exps = python_sequence_walk<nb::int_>(exp_seq);
    auto mans = python_sequence_walk<nb::int_>(man_seq);

    for (std::size_t i = 0; i < signs.size(); ++i) {
        bool sign;
        if (nb::isinstance<nb::bool_>(signs[i])) {
            sign = static_cast<bool>(nb::cast<nb::bool_>(signs[i]));
        } else if (nb::isinstance<nb::int_>(signs[i])) {
            sign = static_cast<int>(nb::cast<nb::int_>(signs[i])
            ); // Must cast to int here
        } else {
            throw std::domain_error("Invalid objects in sign");
        }
        exp_t exp = static_cast<exp_t>(nb::cast<nb::int_>(exps[i]));
        man_t man = static_cast<man_t>(nb::cast<nb::int_>(mans[i]));

        data.push_back({ sign, exp, man });
    }
}

APyFloatArray::APyFloatArray(
    const std::vector<std::size_t>& shape,
    exp_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
    : exp_bits(exp_bits)
    , man_bits(man_bits)
    , bias(bias.value_or(APyFloat::ieee_bias(exp_bits)))
    , shape(shape)
{
    data = std::vector<APyFloatData>(fold_shape(shape), { 0, 0, 0 });
}

/* ********************************************************************************** *
 * *                            Binary arithmetic operators                         * *
 * ********************************************************************************* */

APyFloatArray APyFloatArray::operator+(const APyFloatArray& rhs) const
{
    if (shape != rhs.shape) {
        const auto broadcast_shape = smallest_broadcastable_shape(shape, rhs.shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyFloatArray.__add__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(shape),
                tuple_string_from_vec(rhs.shape)
            ));
        }
        return broadcast_to(broadcast_shape) + rhs.broadcast_to(broadcast_shape);
    }

    const auto quantization = get_float_quantization_mode();
    const bool is_to_neg = quantization == QuantizationMode::TRN;
    // +5 to give room for leading one, carry, and 3 guard bits
    const unsigned int max_man_bits = man_bits + 5;
    if (same_type_as(rhs) && (max_man_bits <= _MAN_T_SIZE_BITS)
        && (quantization != QuantizationMode::STOCH_WEIGHTED)) {
        // Result array
        APyFloatArray res(shape, exp_bits, man_bits, bias);

        APyFloatData x, y;
        const exp_t res_max_exponent = ((1ULL << exp_bits) - 1);
        const man_t final_res_leading_one = (1ULL << man_bits);
        const man_t res_leading_one = final_res_leading_one << 3;
        const man_t carry_res_leading_one = res_leading_one << 1;
        const std::uint8_t tmp_man_bits
            = man_bits + 4; // 4 for leading 1 and 3 guard bits
        const auto shift_normalization_const = _MAN_T_SIZE_BITS - tmp_man_bits;
        const auto man_mask = carry_res_leading_one - 1;
        // Perform operation
        for (std::size_t i = 0; i < data.size(); i++) {
            x = data[i];
            y = rhs.data[i];
            bool x_is_zero_exponent = (x.exp == 0);
            bool y_is_zero_exponent = (y.exp == 0);
            // Handle zero cases
            if (x_is_zero_exponent && x.man == 0) {
                if (y_is_zero_exponent && y.man == 0) {
                    y.sign = (x.sign == y.sign) ? x.sign : is_to_neg;
                }
                res.data[i] = y;
                continue;
            }
            if (y_is_zero_exponent && y.man == 0) {
                res.data[i] = x;
                continue;
            }
            bool x_is_max_exponent = x.exp == res_max_exponent;
            bool y_is_max_exponent = y.exp == res_max_exponent;

            // Handle the NaN and inf cases
            if (x_is_max_exponent || y_is_max_exponent) {
                if ((x_is_max_exponent && x.man != 0)
                    || (y_is_max_exponent && y.man != 0)
                    || (x_is_max_exponent && y_is_max_exponent && x.sign != y.sign)) {
                    // Set to NaN
                    res.data[i] = { x.sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(1) };
                    continue;
                }

                // Handle inf cases
                if (x_is_max_exponent && x.man == 0) {
                    // Set to inf
                    res.data[i] = { x.sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(0) };
                    continue;
                }

                if (y_is_max_exponent && y.man == 0) {
                    // Set to inf
                    res.data[i] = { y.sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(0) };
                    continue;
                }
            }
            // Compute sign and swap operands if need to make sure |x| >= |y|
            if (x.exp < y.exp || (x.exp == y.exp && x.man < y.man)) {
                std::swap(x, y);
                std::swap(x_is_zero_exponent, y_is_zero_exponent);
            } else if (x.sign != y.sign && x.exp == y.exp && x.man == y.man) {
                // Set to zero
                res.data[i]
                    = { is_to_neg, static_cast<exp_t>(0), static_cast<man_t>(0) };
                continue;
            }

            // Tentative exponent
            exp_t new_exp = x.exp + x_is_zero_exponent;

            // Conditionally add leading one's, also add room for guard bits
            // Note that exp can never be res_max_exponent here
            man_t mx = (x_is_zero_exponent ? 0 : res_leading_one) | (x.man << 3);
            man_t my = (y_is_zero_exponent ? 0 : res_leading_one) | (y.man << 3);

            // Align mantissas based on exponent difference
            const unsigned exp_delta = new_exp - y.exp - y_is_zero_exponent;

            // Align mantissa based on difference in exponent
            man_t m_aligned;
            if (exp_delta <= 3) {
                m_aligned = my >> exp_delta;
            } else if (exp_delta >= max_man_bits) {
                m_aligned = (my >> max_man_bits) | 1;
            } else {
                m_aligned
                    = (my >> exp_delta) | ((my << (_MAN_T_SIZE_BITS - exp_delta)) != 0);
            }

            // Perform addition / subtraction
            man_t new_man = (x.sign == y.sign) ? mx + m_aligned : mx - m_aligned;

            // Check for carry and cancellation
            if (new_man & carry_res_leading_one) {
                // Carry
                new_exp++;
            } else if (new_man & res_leading_one) {
                // Align mantissa to carry case
                new_man <<= 1;
            } else {
                // Cancellation or addition with subnormals
                // Mantissa should be shifted until 1.xx is obtained or new_exp equals 0
                const unsigned int man_leading_zeros = leading_zeros(new_man);
                const unsigned int normalizing_shift
                    = man_leading_zeros - shift_normalization_const;

                if (new_exp > normalizing_shift) {
                    new_man <<= normalizing_shift + 1;
                    new_exp -= normalizing_shift;
                } else {
                    // The result will be a subnormal
                    new_man <<= new_exp;
                    new_exp = 0;
                }
            }

            new_man &= man_mask;

            quantize_mantissa(
                new_man,
                new_exp,
                res_max_exponent,
                4,
                x.sign,
                final_res_leading_one,
                3,
                7,
                quantization
            );

            res.data[i]
                = { x.sign, static_cast<exp_t>(new_exp), static_cast<man_t>(new_man) };
        }
        return res;
    }
    // Calculate new format
    const auto res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const auto res_man_bits = std::max(man_bits, rhs.man_bits);
    const auto res_bias
        = calc_bias(res_exp_bits, exp_bits, bias, rhs.exp_bits, rhs.bias);
    APyFloatArray res(shape, res_exp_bits, res_man_bits, res_bias);

    APyFloat lhs_scalar(exp_bits, man_bits, bias);
    APyFloat rhs_scalar(rhs.exp_bits, rhs.man_bits, rhs.bias);
    // Perform operation
    for (std::size_t i = 0; i < data.size(); i++) {
        lhs_scalar.set_data(data[i]);
        rhs_scalar.set_data(rhs.data[i]);

        res.data[i] = (lhs_scalar + rhs_scalar).get_data();
    }

    return res;
}

APyFloatArray APyFloatArray::operator+(const APyFloat& rhs) const
{
    const auto quantization = get_float_quantization_mode();
    // +5 to give room for leading one, carry, and 3 guard bits
    const unsigned int max_man_bits = man_bits + 5;
    const bool same_types = same_type_as(rhs);
    const bool is_to_neg = quantization == QuantizationMode::TRN;
    if (same_types && (max_man_bits <= _MAN_T_SIZE_BITS)
        && (quantization != QuantizationMode::STOCH_WEIGHTED)) {

        const exp_t res_max_exponent = ((1ULL << exp_bits) - 1);
        APyFloatData x;
        // Result array
        APyFloatArray res(shape, exp_bits, man_bits, bias);
        APyFloatData y = rhs.get_data();
        bool y_is_max_exponent = y.exp == res_max_exponent;

        if (y_is_max_exponent) {
            for (std::size_t i = 0; i < data.size(); i++) {
                x = data[i];
                bool x_is_max_exponent = x.exp == res_max_exponent;
                if (y.man != 0 || (x_is_max_exponent && x.man != 0)
                    || (x.sign != y.sign && x_is_max_exponent)) {
                    // Set to NaN
                    res.data[i] = { x.sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(1) };
                } else {
                    // Set to inf
                    res.data[i] = { y.sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(0) };
                }
            }
            return res;
        }
        const man_t final_res_leading_one = (1ULL << man_bits);
        const man_t res_leading_one = final_res_leading_one << 3;
        const man_t carry_res_leading_one = res_leading_one << 1;
        const std::uint8_t tmp_man_bits
            = man_bits + 4; // 4 for leading 1 and 3 guard bits
        const auto shift_normalization_const = _MAN_T_SIZE_BITS - tmp_man_bits;
        const auto man_mask = carry_res_leading_one - 1;

        const bool y_is_zero_exponent = (y.exp == 0);
        const std::int64_t true_y_exp = y.exp + y_is_zero_exponent;
        const man_t my = (y_is_zero_exponent ? 0 : res_leading_one) | (y.man << 3);
        bool res_sign;
        // Perform operation
        for (std::size_t i = 0; i < data.size(); i++) {
            x = data[i];
            // Handle zero case
            if (x.exp == 0 && x.man == 0) {
                if (y_is_zero_exponent && y.man == 0) {
                    res.data[i].sign = (x.sign == y.sign) ? x.sign : is_to_neg;
                } else {
                    res.data[i] = y;
                }
                continue;
            }
            bool x_is_max_exponent = x.exp == res_max_exponent;

            // Handle the NaN and inf cases
            if (x_is_max_exponent) {
                if (x.man != 0) {
                    // Set to NaN
                    res.data[i] = { x.sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(1) };
                } else {
                    // Set to inf
                    res.data[i] = { x.sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(0) };
                }
                continue;
            }
            // Check if sum is zero
            if (x.sign != y.sign && x.exp == y.exp && x.man == y.man) {
                // Set to zero
                res.data[i]
                    = { is_to_neg, static_cast<exp_t>(0), static_cast<man_t>(0) };
                continue;
            }

            const bool x_is_zero_exponent = (x.exp == 0);
            const exp_t true_x_exp = x.exp + x_is_zero_exponent;

            // Conditionally add leading one's, also add room for guard bits
            // Note that exp can never be res_max_exponent here
            man_t mx = (x_is_zero_exponent ? 0 : res_leading_one) | (x.man << 3);

            // Tentative exponent
            exp_t new_exp;
            // Tentative mantissa
            man_t new_man;
            // Compute sign and swap operands if need to make sure |x| >= |y|
            if (x.exp < y.exp || (x.exp == y.exp && x.man < y.man)) {
                // |y| >= |x|
                res_sign = y.sign;
                new_exp = true_y_exp;
                const unsigned exp_delta = true_y_exp - true_x_exp;
                if (exp_delta <= 3) {
                    mx >>= exp_delta;
                } else if (exp_delta >= max_man_bits) {
                    mx = (mx >> max_man_bits) | 1;
                } else {
                    mx = (mx >> exp_delta)
                        | ((mx << (_MAN_T_SIZE_BITS - exp_delta)) != 0);
                }
                // Perform addition / subtraction
                new_man = (x.sign == y.sign) ? my + mx : my - mx;
            } else {
                // |x| >= |y|
                res_sign = x.sign;
                new_exp = true_x_exp;
                const unsigned exp_delta = true_x_exp - true_y_exp;
                // Aligned mantissa
                man_t m_aligned;
                if (exp_delta <= 3) {
                    m_aligned = my >> exp_delta;
                } else if (exp_delta >= max_man_bits) {
                    m_aligned = (my >> max_man_bits) | 1;
                } else {
                    m_aligned = (my >> exp_delta)
                        | ((my << (_MAN_T_SIZE_BITS - exp_delta)) != 0);
                }
                // Perform addition / subtraction
                new_man = (x.sign == y.sign) ? mx + m_aligned : mx - m_aligned;
            }

            // Check for carry and cancellation
            if (new_man & carry_res_leading_one) {
                // Carry
                new_exp++;
            } else if (new_man & res_leading_one) {
                // Align mantissa to carry case
                new_man <<= 1;
            } else {
                // Cancellation or addition with subnormals
                // Mantissa should be shifted until 1.xx is obtained or new_exp equals 0
                const unsigned int man_leading_zeros = leading_zeros(new_man);
                const unsigned int normalizing_shift
                    = man_leading_zeros - shift_normalization_const;

                if (new_exp > normalizing_shift) {
                    new_man <<= normalizing_shift + 1;
                    new_exp -= normalizing_shift;
                } else {
                    // The result will be a subnormal
                    new_man <<= new_exp;
                    new_exp = 0;
                }
            }

            new_man &= man_mask;

            quantize_mantissa(
                new_man,
                new_exp,
                res_max_exponent,
                4,
                res_sign,
                final_res_leading_one,
                3,
                7,
                quantization
            );

            res.data[i] = { res_sign,
                            static_cast<exp_t>(new_exp),
                            static_cast<man_t>(new_man) };
        }
        return res;
    }
    auto new_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    auto new_man_bits = std::max(man_bits, rhs.get_man_bits());
    const auto res_bias
        = calc_bias(new_exp_bits, exp_bits, bias, rhs.get_exp_bits(), rhs.get_bias());

    // Calculate new format
    APyFloatArray res(shape, new_exp_bits, new_man_bits, res_bias);

    APyFloat lhs_scalar(exp_bits, man_bits, bias);
    // Perform operations
    for (std::size_t i = 0; i < data.size(); i++) {
        lhs_scalar.set_data(data[i]);
        res.data[i] = (lhs_scalar + rhs).get_data();
    }

    return res;
}

APyFloatArray APyFloatArray::operator-(const APyFloatArray& rhs) const
{
    if (shape != rhs.shape) {
        auto broadcast_shape = smallest_broadcastable_shape(shape, rhs.shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyFloatArray.__sub__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(shape),
                tuple_string_from_vec(rhs.shape)
            ));
        }
        return broadcast_to(broadcast_shape) - rhs.broadcast_to(broadcast_shape);
    }

    return operator+(-rhs);
}

APyFloatArray APyFloatArray::operator-(const APyFloat& rhs) const
{
    return operator+(-rhs);
}

APyFloatArray APyFloatArray::operator-() const
{
    auto res = *this;
    for (std::size_t i = 0; i < res.data.size(); i++) {
        res.data[i].sign = !res.data[i].sign;
    }
    return res;
}

APyFloatArray APyFloatArray::abs() const
{
    auto res = *this;
    for (std::size_t i = 0; i < res.data.size(); i++) {
        res.data[i].sign = false;
    }
    return res;
}

APyFloatArray APyFloatArray::operator*(const APyFloatArray& rhs) const
{
    if (shape != rhs.shape) {
        auto broadcast_shape = smallest_broadcastable_shape(shape, rhs.shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyFloatArray.__mul__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(shape),
                tuple_string_from_vec(rhs.shape)
            ));
        }
        return broadcast_to(broadcast_shape) * rhs.broadcast_to(broadcast_shape);
    }

    // Calculate new format
    const uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const auto res_bias
        = calc_bias(res_exp_bits, exp_bits, bias, rhs.exp_bits, rhs.bias);
    APyFloatArray res(shape, res_exp_bits, res_man_bits, res_bias);
    const auto quantization = get_float_quantization_mode();
    hadamard_multiplication(
        &rhs.data[0], rhs.exp_bits, rhs.man_bits, rhs.bias, res, quantization
    );
    return res;
}

void APyFloatArray::hadamard_multiplication(
    const APyFloatData* rhs,
    const uint8_t rhs_exp_bits,
    const uint8_t rhs_man_bits,
    const exp_t rhs_bias,
    APyFloatArray& res,
    const QuantizationMode quantization
) const
{
    const int sum_man_bits = man_bits + rhs_man_bits;

    if (unsigned(sum_man_bits) + 3 <= _MAN_T_SIZE_BITS) {
        // Compute constants for reuse
        const exp_t x_max_exponent = ((1ULL << exp_bits) - 1);
        const exp_t y_max_exponent = ((1ULL << rhs_exp_bits) - 1);
        const exp_t res_max_exponent = ((1ULL << res.exp_bits) - 1);
        const uint8_t new_man_bits = sum_man_bits + 2;
        const man_t two = 1ULL << new_man_bits;
        const man_t two_before = two >> 1;
        const man_t one_before = 1ULL << sum_man_bits;
        const man_t two_res = 1 << res.man_bits;
        const man_t mask_two = two - 1;
        const uint8_t man_bits_delta = new_man_bits - res.man_bits;
        const uint8_t man_bits_delta_dec = man_bits_delta - 1;
        const man_t sticky_constant = (1ULL << man_bits_delta_dec) - 1;
        const std::int64_t bias_sum = bias + rhs_bias - res.bias;

        // Perform operation
        for (std::size_t i = 0; i < data.size(); i++) {
            const auto x = data[i];
            const auto y = rhs[i];

            // Calculate sign
            const bool res_sign = x.sign ^ y.sign;

            const bool x_is_subnormal = (x.exp == 0);
            const bool x_is_maxexp = (x.exp == x_max_exponent);
            const bool y_is_subnormal = (y.exp == 0);
            const bool y_is_maxexp = (y.exp == y_max_exponent);

            // Handle special operands
            if (x_is_maxexp || y_is_maxexp || x_is_subnormal || y_is_subnormal) {
                const bool x_is_nan = (x_is_maxexp && x.man != 0);
                const bool x_is_inf = (x_is_maxexp && x.man == 0);
                const bool y_is_nan = (y_is_maxexp && y.man != 0);
                const bool y_is_inf = (y_is_maxexp && y.man == 0);
                const bool x_is_zero = (x_is_subnormal && x.man == 0);
                const bool y_is_zero = (y_is_subnormal && y.man == 0);
                if (x_is_nan || y_is_nan || (x_is_inf && y_is_zero)
                    || (y_is_inf && x_is_zero)) {
                    // Set to nan
                    res.data[i] = { res_sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(1) };
                    continue;
                }

                if (x_is_inf || y_is_inf) {
                    // Set to inf
                    res.data[i] = { res_sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(0) };
                    continue;
                }

                // x is zero or y is zero (and the other is not inf)
                if (x_is_zero || y_is_zero) {
                    // Set to zero
                    res.data[i]
                        = { res_sign, static_cast<exp_t>(0), static_cast<man_t>(0) };
                    continue;
                }
            }

            // Tentative exponent
            std::int64_t tmp_exp = (std::int64_t)x.exp + x_is_subnormal
                + (std::int64_t)y.exp + y_is_subnormal - bias_sum;
            const man_t mx = (static_cast<man_t>(!x_is_subnormal) << man_bits) | x.man;
            const man_t my
                = (static_cast<man_t>(!y_is_subnormal) << rhs_man_bits) | y.man;

            man_t new_man = mx * my;

            // Check result from multiplication larger than/equal two
            if (new_man & two_before) {
                tmp_exp++;
                new_man <<= 1;
            } else if (new_man & one_before) {
                // Align with longer result
                new_man <<= 2;
            } else {
                // One or two of the operands were subnormal.
                // If the exponent is positive, the result is normalized by
                // left-shifting until the exponent is zero or the mantissa is 1.xx
                const int leading_zeros = 1 + sum_man_bits - bit_width(new_man);
                const int shift = std::max(
                    std::min(tmp_exp, (std::int64_t)leading_zeros), (std::int64_t)0
                );
                tmp_exp -= shift;
                // + 2 to align with longer result
                new_man <<= shift + 2;
            }

            if (tmp_exp <= 0) {
                if (tmp_exp < -static_cast<std::int64_t>(res.man_bits)) {
                    // Exponent too small after rounding
                    man_t res_man
                        = quantize_close_to_zero(res_sign, new_man, quantization);
                    res.data[i] = { res_sign, 0, res_man };
                    continue;
                }
                // Shift and add sticky bit
                new_man = (new_man >> (-tmp_exp + 1))
                    | ((new_man & ((1 << (-tmp_exp + 1)) - 1)) != 0);
                tmp_exp = 0;
            }

            new_man &= mask_two;
            exp_t new_exp = static_cast<exp_t>(tmp_exp);
            quantize_mantissa(
                new_man,
                new_exp,
                res_max_exponent,
                man_bits_delta,
                res_sign,
                two_res,
                man_bits_delta_dec,
                sticky_constant,
                quantization
            );

            res.data[i] = { res_sign,
                            static_cast<exp_t>(new_exp),
                            static_cast<man_t>(new_man) };
        }
    } else {
        APyFloat lhs_scalar(exp_bits, man_bits, bias);
        APyFloat rhs_scalar(rhs_exp_bits, rhs_man_bits, rhs_bias);
        // Perform operation
        for (std::size_t i = 0; i < data.size(); i++) {
            lhs_scalar.set_data(data[i]);
            rhs_scalar.set_data(rhs[i]);

            res.data[i] = (lhs_scalar * rhs_scalar).get_data();
        }
    }
}

APyFloatArray APyFloatArray::operator*(const APyFloat& rhs) const
{
    // Calculate new format
    const auto res_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    const auto res_man_bits = std::max(man_bits, rhs.get_man_bits());
    const auto res_bias
        = calc_bias(res_exp_bits, exp_bits, bias, rhs.get_exp_bits(), rhs.get_bias());
    APyFloatArray res(shape, res_exp_bits, res_man_bits, res_bias);

    const int sum_man_bits = man_bits + rhs.get_man_bits();
    const auto quantization = get_float_quantization_mode();

    if (unsigned(sum_man_bits) + 3 <= _MAN_T_SIZE_BITS) {
        // Compute constants for reuse
        const auto x_max_exponent = ((1ULL << exp_bits) - 1);
        const auto y_max_exponent = ((1ULL << rhs.get_exp_bits()) - 1);
        const exp_t res_max_exponent = ((1ULL << res.exp_bits) - 1);
        const auto y = rhs.get_data();
        const bool y_is_maxexp = (y.exp == y_max_exponent);

        if (y_is_maxexp) {
            // y is nan
            if (y.man != 0) {
                for (std::size_t i = 0; i < data.size(); i++) {
                    // Calculate sign
                    const bool res_sign = data[i].sign ^ y.sign;
                    // Set to nan
                    res.data[i] = { res_sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(1) };
                }
                return res;
            }
            // Y is inf
            for (std::size_t i = 0; i < data.size(); i++) {
                const auto x = data[i];
                // Calculate sign
                const bool res_sign = x.sign ^ y.sign;
                // X is zero or nan
                if ((x.exp == 0 && x.man == 0)
                    || (x.exp == x_max_exponent && x.man != 0)) {
                    // Set to nan
                    res.data[i] = { res_sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(1) };
                } else {
                    // Set to inf
                    res.data[i] = { res_sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(0) };
                }
            }
            return res;
        }
        const bool y_is_subnormal = (y.exp == 0);
        // y is zero
        if (y_is_subnormal && y.man == 0) {
            for (std::size_t i = 0; i < data.size(); i++) {
                const auto x = data[i];
                // Calculate sign
                const bool res_sign = x.sign ^ y.sign;
                // X is inf or nan
                if (x.exp == x_max_exponent) {
                    // Set to nan
                    res.data[i] = { res_sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(1) };
                } else {
                    // Set to zero
                    res.data[i]
                        = { res_sign, static_cast<exp_t>(0), static_cast<man_t>(0) };
                }
            }
            return res;
        }

        // Compute more constants to be reused
        const std::int64_t bias_sum = bias + rhs.get_bias();
        const std::int64_t exp_offset
            = (std::int64_t)y.exp + y_is_subnormal - bias_sum + res.bias;
        const man_t my
            = (static_cast<man_t>(!y_is_subnormal) << rhs.get_man_bits()) | y.man;
        const auto new_man_bits = sum_man_bits + 2;
        const man_t two = 1ULL << (new_man_bits);
        const man_t two_before = two >> 1;
        const man_t one_before = 1ULL << sum_man_bits;
        const man_t two_res = 1 << res_man_bits;
        const auto mask_two = two - 1;
        const auto man_bits_delta = new_man_bits - res_man_bits;
        const auto man_bits_delta_dec = man_bits_delta - 1;
        const man_t sticky_constant = (1ULL << man_bits_delta_dec) - 1;
        exp_t new_exp;
        // Perform operation
        for (std::size_t i = 0; i < data.size(); i++) {
            const auto x = data[i];

            // Calculate sign
            const bool res_sign = x.sign ^ y.sign;

            const bool x_is_subnormal = (x.exp == 0);
            const bool x_is_maxexp = (x.exp == x_max_exponent);

            // Handle special operands
            // All cases where y is special is handled outside of this loop
            if (x_is_maxexp || x_is_subnormal) {
                // x is nan
                if (x_is_maxexp && x.man != 0) {
                    res.data[i] = { res_sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(1) };
                    continue;
                }

                // x is inf
                if (x_is_maxexp && x.man == 0) {
                    res.data[i] = { res_sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(0) };
                    continue;
                }

                // x is zero (x is subnormal here)
                if (x.man == 0) {
                    res.data[i]
                        = { res_sign, static_cast<exp_t>(0), static_cast<man_t>(0) };
                    continue;
                }
            }

            // Tentative exponent
            std::int64_t tmp_exp = (std::int64_t)x.exp + x_is_subnormal + exp_offset;
            const man_t mx = (static_cast<man_t>(!x_is_subnormal) << man_bits) | x.man;

            man_t new_man = mx * my;

            // Check result from multiplication larger than/equal two
            if (new_man & two_before) {
                tmp_exp++;
                new_man <<= 1;
            } else if (new_man & one_before) {
                // Align with longer result
                new_man <<= 2;
            } else {
                // One or two of the operands were subnormal.
                // If the exponent is positive, the result is normalized by
                // left-shifting until the exponent is zero or the mantissa is 1.xx
                const int leading_zeros = 1 + sum_man_bits - bit_width(new_man);
                const int shift = std::max(
                    std::min(tmp_exp, (std::int64_t)leading_zeros), (std::int64_t)0
                );
                tmp_exp -= shift;
                // + 2 to align with longer result
                new_man <<= shift + 2;
            }

            if (tmp_exp <= 0) {
                if (tmp_exp < -static_cast<std::int64_t>(res.man_bits)) {
                    // Exponent too small after rounding
                    man_t res_man
                        = quantize_close_to_zero(res_sign, new_man, quantization);
                    res.data[i] = { res_sign, 0, res_man };
                    continue;
                }
                // Shift and add sticky bit
                new_man = (new_man >> (-tmp_exp + 1))
                    | ((new_man & ((1 << (-tmp_exp + 1)) - 1)) != 0);
                tmp_exp = 0;
            }

            new_man &= mask_two;
            new_exp = static_cast<exp_t>(tmp_exp);
            quantize_mantissa(
                new_man,
                new_exp,
                res_max_exponent,
                man_bits_delta,
                res_sign,
                two_res,
                man_bits_delta_dec,
                sticky_constant,
                quantization
            );

            res.data[i] = { res_sign,
                            static_cast<exp_t>(new_exp),
                            static_cast<man_t>(new_man) };
        }
        return res;
    }

    APyFloat lhs_scalar(exp_bits, man_bits, bias);
    // Perform operations
    for (std::size_t i = 0; i < data.size(); i++) {
        lhs_scalar.set_data(data[i]);
        res.data[i] = (lhs_scalar * rhs).get_data();
    }

    return res;
}

APyFloatArray APyFloatArray::operator/(const APyFloatArray& rhs) const
{
    if (shape != rhs.shape) {
        auto broadcast_shape = smallest_broadcastable_shape(shape, rhs.shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyFloatArray.__truediv__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(shape),
                tuple_string_from_vec(rhs.shape)
            ));
        }
        return broadcast_to(broadcast_shape) / rhs.broadcast_to(broadcast_shape);
    }

    // Calculate new format
    const auto res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const auto res_man_bits = std::max(man_bits, rhs.man_bits);
    const auto res_bias
        = calc_bias(res_exp_bits, exp_bits, bias, rhs.exp_bits, rhs.bias);
    APyFloatArray res(shape, res_exp_bits, res_man_bits, res_bias);

    APyFloat lhs_scalar(exp_bits, man_bits, bias);
    APyFloat rhs_scalar(rhs.exp_bits, rhs.man_bits, rhs.bias);
    // Perform operation
    for (std::size_t i = 0; i < data.size(); i++) {
        lhs_scalar.set_data(data[i]);
        rhs_scalar.set_data(rhs.data[i]);

        res.data[i] = (lhs_scalar / rhs_scalar).get_data();
    }

    return res;
}

APyFloatArray APyFloatArray::operator/(const APyFloat& rhs) const
{
    // Calculate new format
    const auto res_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    const auto res_man_bits = std::max(man_bits, rhs.get_man_bits());
    const auto res_bias
        = calc_bias(res_exp_bits, exp_bits, bias, rhs.get_exp_bits(), rhs.get_bias());
    APyFloatArray res(shape, res_exp_bits, res_man_bits, res_bias);

    APyFloat lhs_scalar(exp_bits, man_bits, bias);
    // Perform operations
    for (std::size_t i = 0; i < data.size(); i++) {
        lhs_scalar.set_data(data[i]);
        res.data[i] = (lhs_scalar / rhs).get_data();
    }

    return res;
}

APyFloatArray APyFloatArray::rtruediv(const APyFloat& lhs) const
{
    // Calculate new format
    APyFloatArray res(
        shape,
        std::max(exp_bits, lhs.get_exp_bits()),
        std::max(man_bits, lhs.get_man_bits())
    );

    APyFloat rhs_scalar(exp_bits, man_bits, bias);
    // Perform operations
    for (std::size_t i = 0; i < data.size(); i++) {
        rhs_scalar.set_data(data[i]);
        res.data[i] = (lhs / rhs_scalar).get_data();
    }

    return res;
}

std::variant<APyFloatArray, APyFloat> APyFloatArray::matmul(const APyFloatArray& rhs
) const
{
    if (get_ndim() == 1 && rhs.get_ndim() == 1) {
        if (shape[0] == rhs.shape[0]) {
            // Dimensionality for a standard scalar inner product checks out.
            // Perform the checked inner product.
            return checked_inner_product(
                rhs,
                get_accumulator_mode_float(),
                std::max(exp_bits, rhs.exp_bits),
                std::max(man_bits, rhs.man_bits)
            );
        }
    }
    if (get_ndim() == 2 && (rhs.get_ndim() == 2 || rhs.get_ndim() == 1)) {
        if (shape[1] == rhs.shape[0]) {
            // Dimensionality for a standard 2D matrix multiplication checks out.
            // Perform the checked 2D matrix
            return checked_2d_matmul(rhs);
        }
    }

    // Unsupported `__matmul__` dimensionality, raise exception
    throw std::length_error(fmt::format(
        "APyFloatArray.__matmul__: input shape mismatch, lhs: {}, rhs: {}",
        tuple_string_from_vec(shape),
        tuple_string_from_vec(rhs.shape)
    ));
}

/* ****************************************************************************** *
 *                     Static methods for array initialization                    *
 * ****************************************************************************** */

APyFloatArray APyFloatArray::zeros(
    const nb::tuple& shape,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    std::vector<std::size_t> new_shape = cpp_shape_from_python_shape_like(shape);
    APyFloatArray result(new_shape, exp_bits, man_bits, bias);
    return result;
}

APyFloatArray APyFloatArray::ones(
    const nb::tuple& shape,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    return full(shape, one(exp_bits, man_bits, bias));
}

APyFloatArray APyFloatArray::eye(
    const nb::int_& N,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<nb::int_> M,
    std::optional<exp_t> bias
)
{
    // Use N for both dimensions if M is not provided
    nb::tuple shape
        = M.has_value() ? nb::make_tuple(N, M.value()) : nb::make_tuple(N, N);
    return diagonal(shape, one(exp_bits, man_bits, bias));
}

APyFloatArray APyFloatArray::identity(
    const nb::int_& N,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    return eye(N, exp_bits, man_bits, std::nullopt, bias);
}

APyFloatArray APyFloatArray::full(const nb::tuple& shape, const APyFloat& fill_value)
{
    std::vector<std::size_t> cpp_shape = cpp_shape_from_python_shape_like(shape);
    APyFloatArray result(
        cpp_shape,
        fill_value.get_exp_bits(),
        fill_value.get_man_bits(),
        fill_value.get_bias()
    );

    std::size_t num_elem = ::fold_shape(cpp_shape);
    result.data = std::vector<APyFloatData>(num_elem, fill_value.get_data());
    return result;
}

APyFloatArray
APyFloatArray::diagonal(const nb::tuple& shape, const APyFloat& fill_value)
{
    std::vector<std::size_t> new_shape = cpp_shape_from_python_shape_like(shape);
    if (new_shape.size() > 2) {
        throw nb::value_error(
            "Creating higher dimensional diagonal arrays are not yet defined"
        );
    }
    APyFloatArray result(
        new_shape,
        fill_value.get_exp_bits(),
        fill_value.get_man_bits(),
        fill_value.get_bias()
    );

    std::size_t min_dim = *std::min_element(new_shape.begin(), new_shape.end());
    std::vector<std::size_t> strides = ::strides_from_shape(new_shape);
    std::size_t multiplier = std::accumulate(strides.begin(), strides.end(), 0);
    for (std::size_t i = 0; i < min_dim; ++i) {
        std::size_t index = i * multiplier;
        result.data[index] = fill_value.get_data();
    }
    return result;
}

APyFloatArray APyFloatArray::arange(
    const nb::object& start,
    const nb::object& stop,
    const nb::object& step,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits);
    check_mantissa_format(man_bits);

    const std::vector<APyFixed> apy_vals = ::arange(start, stop, step);
    APyFloatArray result({ apy_vals.size() }, exp_bits, man_bits, bias);

    for (size_t i = 0; i < apy_vals.size(); i++) {
        result.data[i]
            = APyFloat::from_fixed(apy_vals[i], exp_bits, man_bits, bias).get_data();
    }

    return result;
}

/* ****************************************************************************** *
 *                            Methods for array modification                      *
 * ****************************************************************************** */

APyFloatArray
APyFloatArray::squeeze(std::optional<std::variant<nb::int_, nb::tuple>> axis) const
{
    std::vector<std::size_t> _shape = shape;
    std::set<int> axis_set;
    if (axis.has_value()) {
        auto ax = axis.value();
        nb::tuple axis_tuple;

        if (std::holds_alternative<nb::tuple>(ax)) {
            axis_tuple = std::get<nb::tuple>(ax);
        } else if (std::holds_alternative<nb::int_>(ax)) {
            int axis = int(std::get<nb::int_>(ax));
            axis_tuple = nb::make_tuple(axis);
        }
        for (auto ptr = axis_tuple.begin(); ptr != axis_tuple.end(); ptr++) {
            int axis_n = int(nanobind::cast<nb::int_>(*ptr));
            if (axis_n >= int(_shape.size())) {
                throw nb::index_error(
                    "Specified axis with larger than number of dimensions in the "
                    "APyFloatArray"
                );
            }
            axis_set.insert(axis_n);
        }
        // precidate to squeeze without specified axes
        int cnt = 0;
        auto predicate = [&](std::size_t dim) {
            if (axis_set.find(cnt) != axis_set.end() && dim != 1) {
                throw nb::value_error(
                    "Cannot select an axis to squeeze that has size other than one"
                );
            }
            return axis_set.find(cnt++) != axis_set.end() && dim == 1;
        };
        _shape.erase(
            std::remove_if(_shape.begin(), _shape.end(), predicate), _shape.end()
        );
    } else {
        // precidate to squeeze without specified axes
        auto predicate = [](std::size_t dim) { return dim == 1; };
        _shape.erase(
            std::remove_if(_shape.begin(), _shape.end(), predicate), _shape.end()
        );
    }

    if (_shape.size() == 0) {
        _shape = { 1 };
    }

    // create the resulting Array and copy the data
    APyFloatArray result(_shape, exp_bits, man_bits, bias);
    std::copy_n(data.begin(), data.size(), result.data.begin());
    return result;
}

//! Perform a linear convolution with `other` using `mode`
APyFloatArray
APyFloatArray::convolve(const APyFloatArray& other, const std::string& mode) const
{
    if (get_ndim() != 1 || other.get_ndim() != 1) {
        auto msg = fmt::format(
            "can only convolve 1D arrays (lhs.ndim = {}, rhs.ndim = {})",
            get_ndim(),
            other.get_ndim()
        );
        throw nanobind::value_error(msg.c_str());
    }

    // TODO: Add convolution support for accumulator contexts
    if (get_accumulator_mode_float().has_value()) {
        throw NotImplementedException(
            "APyFloatArray convolve with accumulator context not implemented"
        );
    }

    // Find the shorter array of `*this` and `other` based on length.
    bool swap = shape[0] < other.shape[0];

    // Make a reverse copy of the shorter array
    APyFloatArray b_cpy = swap ? *this : other;
    std::reverse(std::begin(b_cpy.data), std::end(b_cpy.data));

    // Let `a` be a pointer to the longer array, and let `b` be a pointer to the reverse
    // copy of the shorter array.
    const APyFloatArray* a = swap ? &other : this;
    const APyFloatArray* b = &b_cpy;

    // Extract convolution properties
    auto [len, n_left, n_right] = get_conv_lengths(mode, a, b);

    auto acc_mode = get_accumulator_mode_float();

    // Result vector
    const int res_exp_bits
        = acc_mode ? acc_mode->exp_bits : std::max(a->exp_bits, b->exp_bits);
    const int res_man_bits
        = acc_mode ? acc_mode->man_bits : std::max(a->man_bits, b->man_bits);
    const int res_bias = acc_mode.has_value() && acc_mode->bias.has_value()
        ? *acc_mode->bias
        : calc_bias(res_exp_bits, a->exp_bits, a->bias, b->exp_bits, b->bias);
    APyFloatArray result({ len }, res_exp_bits, res_man_bits, res_bias);

    // Loop working variables
    std::size_t n = b->shape[0] - n_left;
    auto dst = std::begin(result.data);
    auto src1 = std::cbegin(a->data);
    auto src2 = std::cbegin(b->data) + n_left;

    // `b` limits length of the inner product length
    for (std::size_t i = 0; i < n_left; i++) {
        float_inner_product(src1, src2, dst, *a, *b, n);
        src2--;
        dst++;
        n++;
    }

    // full inner product length
    for (std::size_t i = 0; i < a->shape[0] - b->shape[0] + 1; i++) {
        float_inner_product(src1, src2, dst, *a, *b, n);
        src1++;
        dst++;
    }

    // `a` limits length of the inner product length
    for (std::size_t i = 0; i < n_right; i++) {
        n--;
        float_inner_product(src1, src2, dst, *a, *b, n);
        src1++;
        dst++;
    }

    return result;
}

std::variant<APyFloatArray, APyFloat> APyFloatArray::prod_sum_function(
    void (*pos_func)(std::size_t, std::size_t, std::size_t, APyFloatArray&, APyFloatArray&, APyFloat&, APyFloat&),
    std::optional<std::variant<nb::tuple, nb::int_>> axis
) const
{
    std::set<std::size_t> axes_set;
    if (axis.has_value()) {
        std::vector<std::size_t> axes_vector
            = cpp_shape_from_python_shape_like(axis.value());
        for (auto i : axes_vector) {
            if (i >= shape.size()) {
                throw nb::index_error(
                    "specified axis outside number of dimensions in the APyFixedArray"
                );
            }
            axes_set.insert(i);
        }
    } else {
        axes_set.insert(shape.size());
    }

    // Resulting vector
    APyFloatArray result(shape, exp_bits, man_bits);
    APyFloatArray source(shape, exp_bits, man_bits);
    std::copy_n(data.begin(), data.size(), source.data.begin());

    APyFloat lhs_scalar(exp_bits, man_bits, bias);
    APyFloat rhs_scalar(exp_bits, man_bits, bias);

    std::vector<std::size_t> shape_;
    std::size_t res_elements = source.data.size();
    std::size_t sec_length = source.data.size();
    for (std::size_t x = 0; x <= shape.size(); x++) {
        std::size_t elements = res_elements;
        if (axes_set.find(x) == axes_set.end()) {
            if (x < shape.size()) {
                sec_length /= source.shape.at(x);
                shape_.push_back(shape.at(x));
            }
            continue;
        }

        if (x < shape.size()) {
            elements = source.shape.at(x);
            sec_length /= source.shape.at(x);
        } else {
            shape_ = {};
        }
        for (std::size_t i = 0; i < res_elements; i++) {
            pos_func(i, sec_length, elements, source, result, lhs_scalar, rhs_scalar);
        }

        res_elements /= elements;
        source = result;
        result.data.assign(result.data.size(), { 0, 0, 0 });
    }
    if (shape_.size() == 0) {
        APyFloat res(exp_bits, man_bits);
        res.set_data(source.data.at(0));
        return res;
    } else {
        APyFloatArray res(shape_, exp_bits, man_bits);
        std::copy_n(source.data.begin(), res_elements, res.data.begin());
        return res;
    }
}

APyFloatArray APyFloatArray::cumulative_prod_sum_function(
    void (*pos_func)(std::size_t, std::size_t, std::size_t, APyFloatArray&, APyFloatArray&, APyFloat&, APyFloat&),
    std::optional<nb::int_> axis
) const
{
    // determine input value
    // if no input is received shape.size() will be used
    std::size_t _axis = shape.size();
    if (axis.has_value()) {
        _axis = std::size_t(axis.value());
        if (_axis >= shape.size()) {
            throw nb::index_error(
                "specified axis outside number of dimensions in the APyFloatArray"
            );
        }
    }

    // initialize variables used for the summation or multiplication
    // and determine the new shape
    std::size_t elements = data.size();
    std::size_t res_nitems = data.size();
    std::vector<std::size_t> shape_ = { data.size() };
    if (_axis < shape.size()) {
        elements = shape.at(_axis);
        shape_ = shape;
    }

    // temporary vector and source vector.
    // the source vector will also contain the result after the summation or
    // multiplication are performed
    APyFloatArray temp(shape, exp_bits, man_bits);
    APyFloatArray source(shape, exp_bits, man_bits);
    std::copy_n(data.begin(), data.size(), source.data.begin());

    // temporary APyFloat used for the arithmetic later on
    APyFloat lhs_scalar(exp_bits, man_bits, bias);
    APyFloat rhs_scalar(exp_bits, man_bits, bias);

    // get the stride
    std::size_t stride = 1;
    if (_axis < shape.size()) {
        stride = strides_from_shape(shape)[_axis];
    }
    // perform the summation or multiplication and place result in 'result'
    for (std::size_t i = 0; i < temp.data.size(); i++) {
        pos_func(i, stride, elements, source, temp, lhs_scalar, rhs_scalar);
    }
    // copy the elements into a new array to get the correct shape
    APyFloatArray result2(shape_, exp_bits, man_bits);
    std::copy_n(temp.data.begin(), res_nitems, result2.data.begin());
    return result2;
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::sum(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    auto pos_func = [](std::size_t i,
                       std::size_t sec_length,
                       std::size_t elements,
                       APyFloatArray& src,
                       APyFloatArray& dst,
                       APyFloat& lhs_scalar,
                       APyFloat& rhs_scalar) {
        // calculate new position
        std::size_t pos_in_sec = i % (sec_length);
        std::size_t sec_pos = (i - i % (elements * sec_length)) / (elements);
        auto pos = (pos_in_sec + sec_pos);

        // perform addition
        lhs_scalar.set_data(dst.data.at(pos));
        rhs_scalar.set_data(src.data.at(i));
        dst.data[pos] = (lhs_scalar + rhs_scalar).get_data();
    };

    return prod_sum_function(pos_func, axis);
}

APyFloatArray APyFloatArray::cumsum(std::optional<nb::int_> axis) const
{
    auto pos_func = [](std::size_t i,
                       std::size_t sec_length,
                       std::size_t elements,
                       APyFloatArray& src,
                       APyFloatArray& dst,
                       APyFloat& lhs_scalar,
                       APyFloat& rhs_scalar) {
        // calculate new position
        std::size_t pos = i - sec_length;
        if (i % (sec_length * elements) < sec_length) {
            pos = i;
        }

        // perform addition
        lhs_scalar.set_data(dst.data.at(pos));
        rhs_scalar.set_data(src.data.at(i));
        dst.data[i] = (lhs_scalar + rhs_scalar).get_data();
    };

    return cumulative_prod_sum_function(pos_func, axis);
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::nansum(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    auto pos_func = [](std::size_t i,
                       std::size_t sec_length,
                       std::size_t elements,
                       APyFloatArray& src,
                       APyFloatArray& dst,
                       APyFloat& lhs_scalar,
                       APyFloat& rhs_scalar) {
        // calculate new position
        std::size_t pos_in_sec = i % (sec_length);
        std::size_t sec_pos = (i - i % (elements * sec_length)) / (elements);
        auto pos = (pos_in_sec + sec_pos);

        // perform addition
        lhs_scalar.set_data(dst.data.at(pos));
        rhs_scalar.set_data(src.data.at(i));

        if (rhs_scalar.is_nan()) {
            rhs_scalar.set_data({ 0, 0, 0 });
        }
        dst.data[pos] = (lhs_scalar + rhs_scalar).get_data();
    };

    return prod_sum_function(pos_func, axis);
}

APyFloatArray APyFloatArray::nancumsum(std::optional<nb::int_> axis) const
{
    auto pos_func = [](std::size_t i,
                       std::size_t sec_length,
                       std::size_t elements,
                       APyFloatArray& src,
                       APyFloatArray& dst,
                       APyFloat& lhs_scalar,
                       APyFloat& rhs_scalar) {
        // calculate new position
        std::size_t pos = i - sec_length;
        if (i % (sec_length * elements) < sec_length) {
            pos = i;
        }

        // perform addition
        lhs_scalar.set_data(dst.data.at(pos));
        rhs_scalar.set_data(src.data.at(i));
        if (rhs_scalar.is_nan()) {
            rhs_scalar.set_data({ 0, 0, 0 });
        }
        dst.data[i] = (lhs_scalar + rhs_scalar).get_data();
    };

    return cumulative_prod_sum_function(pos_func, axis);
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::prod(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    auto pos_func = [](std::size_t i,
                       std::size_t sec_length,
                       std::size_t elements,
                       APyFloatArray& src,
                       APyFloatArray& dst,
                       APyFloat& lhs_scalar,
                       APyFloat& rhs_scalar) {
        // calculate new position
        std::size_t pos_in_sec = i % (sec_length);
        std::size_t sec_pos = (i - i % (elements * sec_length)) / (elements);
        auto pos = (pos_in_sec + sec_pos);
        // special case when first element in a multiplication chain
        if ((elements == src.data.size() && i == 0)
            || i % (sec_length * elements) < sec_length) {
            dst.data.at(pos) = src.data.at(i);
            return;
        }

        // perform multiplication
        lhs_scalar.set_data(dst.data.at(pos));
        rhs_scalar.set_data(src.data.at(i));
        auto res = lhs_scalar * rhs_scalar;
        dst.data[pos] = res.get_data();
    };

    return prod_sum_function(pos_func, axis);
}

APyFloatArray APyFloatArray::cumprod(std::optional<nb::int_> axis) const
{
    auto pos_func = [](std::size_t i,
                       std::size_t sec_length,
                       std::size_t elements,
                       APyFloatArray& src,
                       APyFloatArray& dst,
                       APyFloat& lhs_scalar,
                       APyFloat& rhs_scalar) {
        // calculate new position
        if (i % (sec_length * elements) < sec_length) {
            dst.data.at(i) = src.data.at(i);
            return;
        }
        std::size_t pos = i - sec_length;
        lhs_scalar.set_data(dst.data.at(pos));
        rhs_scalar.set_data(src.data.at(i));

        // perform multiplication
        dst.data[i] = (lhs_scalar * rhs_scalar).get_data();
    };

    return cumulative_prod_sum_function(pos_func, axis);
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::nanprod(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    auto pos_func = [](std::size_t i,
                       std::size_t sec_length,
                       std::size_t elements,
                       APyFloatArray& src,
                       APyFloatArray& dst,
                       APyFloat& lhs_scalar,
                       APyFloat& rhs_scalar) {
        // get source data for the multiplication

        // calculate new position
        std::size_t pos_in_sec = i % (sec_length);
        std::size_t sec_pos = (i - i % (elements * sec_length)) / (elements);
        auto pos = (pos_in_sec + sec_pos);
        // special case when first element in a multiplication chain
        if ((elements == src.data.size() && i == 0)
            || i % (sec_length * elements) < sec_length) {
            dst.data.at(pos) = src.data.at(i);
            return;
        }

        lhs_scalar.set_data(dst.data.at(pos));
        rhs_scalar.set_data(src.data.at(i));
        if (lhs_scalar.is_nan()) {
            lhs_scalar.set_data({ 0, lhs_scalar.get_bias(), 0 });
        }
        if (rhs_scalar.is_nan()) {
            rhs_scalar.set_data({ 0, rhs_scalar.get_bias(), 0 });
        }
        // perform multiplication
        dst.data[pos] = (lhs_scalar * rhs_scalar).get_data();
    };

    return prod_sum_function(pos_func, axis);
}

APyFloatArray APyFloatArray::nancumprod(std::optional<nb::int_> axis) const
{
    auto pos_func = [](std::size_t i,
                       std::size_t sec_length,
                       std::size_t elements,
                       APyFloatArray& src,
                       APyFloatArray& dst,
                       APyFloat& lhs_scalar,
                       APyFloat& rhs_scalar) {
        // get source data for the multiplication
        rhs_scalar.set_data(src.data.at(i));
        if (rhs_scalar.is_nan()) {
            rhs_scalar.set_data({ 0, rhs_scalar.get_bias(), 0 });
        }
        // if its the first in a multiplicatiojn chain we can assign it directly
        if (i % (sec_length * elements) < sec_length) {
            dst.data.at(i) = rhs_scalar.get_data();
            return;
        }
        // get temporary data for the multiplication
        // calculate new position
        std::size_t pos = i - sec_length;
        lhs_scalar.set_data(dst.data.at(pos));

        // perform multiplication
        dst.data[i] = (lhs_scalar * rhs_scalar).get_data();
    };

    return cumulative_prod_sum_function(pos_func, axis);
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::max(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    auto comp_func = [](APyFloat& lhs, APyFloat& rhs) {
        if (lhs.is_nan()) {
            return true;
        } else if (rhs.is_nan()) {
            return false;
        }
        return lhs > rhs;
    };
    return max_min_helper_function(comp_func, axis);
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::min(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    auto comp_func = [](APyFloat& lhs, APyFloat& rhs) {
        if (lhs.is_nan()) {
            return true;
        } else if (rhs.is_nan()) {
            return false;
        }
        return lhs < rhs;
    };
    return max_min_helper_function(comp_func, axis);
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::nanmax(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    auto comp_func = [](APyFloat& lhs, APyFloat& rhs) {
        if (lhs.is_nan()) {
            return false;
        } else if (rhs.is_nan()) {
            return true;
        }
        return lhs > rhs;
    };

    auto res = max_min_helper_function(comp_func, axis);

    if (std::holds_alternative<APyFloatArray>(res)) {
        APyFloatArray temp_arr = std::get<APyFloatArray>(res);
        APyFloat temp(exp_bits, man_bits, bias);
        for (auto elem : temp_arr.data) {
            temp.set_data(elem);
            if (temp.is_nan()) {
                std::cerr << "RuntimeWarning: All-NaN encountered" << std::endl;
            }
        }
    } else {
        APyFloat temp = std::get<APyFloat>(res);
        if (temp.is_nan()) {
            std::cerr << "RuntimeWarning: All-NaN encountered" << std::endl;
        }
    }
    return res;
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::nanmin(std::optional<std::variant<nb::tuple, nb::int_>> axis) const
{
    auto comp_func = [](APyFloat& lhs, APyFloat& rhs) {
        if (lhs.is_nan()) {
            return false;
        } else if (rhs.is_nan()) {
            return true;
        }
        return lhs < rhs;
    };

    auto res = max_min_helper_function(comp_func, axis);

    if (std::holds_alternative<APyFloatArray>(res)) {
        APyFloatArray temp_arr = std::get<APyFloatArray>(res);
        APyFloat temp(exp_bits, man_bits, bias);
        for (auto elem : temp_arr.data) {
            temp.set_data(elem);
            if (temp.is_nan()) {
                std::cerr << "RuntimeWarning: All-NaN encountered" << std::endl;
            }
        }
    } else {
        APyFloat temp = std::get<APyFloat>(res);
        if (temp.is_nan()) {
            std::cerr << "RuntimeWarning: All-NaN encountered" << std::endl;
        }
    }
    return res;
}

// Return the maximum of an array or the maximum along an axis.
std::variant<APyFloatArray, APyFloat> APyFloatArray::max_min_helper_function(
    bool (*comp_func)(APyFloat&, APyFloat&),
    std::optional<std::variant<nb::tuple, nb::int_>> axis
) const
{
    // determine the input axes
    std::set<std::size_t> axes_set;
    if (axis.has_value()) {
        std::vector<std::size_t> axes_vector
            = cpp_shape_from_python_shape_like(axis.value());
        for (auto i : axes_vector) {
            if (i >= shape.size()) {
                throw nb::index_error(
                    "specified axis outside number of dimensions in the APyFloatArray"
                );
            }
            axes_set.insert(i);
        }
    } else {
        axes_set.insert(shape.size());
    }

    // special case where the maximum or minimum from the whole array is wanted
    if (axes_set.size() == shape.size()
        || axes_set.find(shape.size()) != axes_set.end()) {
        APyFloat res(exp_bits, man_bits);
        res.set_data(data.at(0));
        APyFloat temp(exp_bits, man_bits);
        for (std::size_t i = 1; i < data.size(); i++) {
            temp.set_data(data.at(i));
            if (comp_func(temp, res)) {
                res = temp;
            }
        }
        return res;
    }

    std::size_t elements = data.size();
    std::vector<std::size_t> res_shape;
    std::vector<APyFloatData> source_data = data;
    std::vector<APyFloatData> temp_data(data.size(), { 0, 0, 0 });
    std::vector<std::size_t> strides = strides_from_shape(shape);
    APyFloat lhs_scalar(exp_bits, man_bits);
    APyFloat rhs_scalar(exp_bits, man_bits);

    // loop over the axes one at a time
    for (std::size_t x = 0; x < shape.size(); x++) {
        if (axes_set.find(x) == axes_set.end()) {
            res_shape.push_back(shape[x]);
            continue;
        }
        // loop over an axis and get the maximum or minimum along it
        for (std::size_t i = 0; i < elements; i++) {
            std::size_t new_pos
                = i % strides[x] + (i - i % (strides[x] * shape[x])) / shape[x];
            if (i % (strides[x] * shape[x]) < strides[x]) {
                temp_data.at(new_pos) = source_data.at(i);
                continue;
            }
            lhs_scalar.set_data(source_data.at(i));
            rhs_scalar.set_data(temp_data.at(new_pos));
            if (comp_func(lhs_scalar, rhs_scalar)) {
                temp_data.at(new_pos) = lhs_scalar.get_data();
            }
        }

        elements /= shape[x];
        source_data = temp_data;
        temp_data.assign(elements, { 0, 0, 0 });
    }
    APyFloatArray result(res_shape, exp_bits, man_bits);
    std::copy_n(source_data.begin(), elements, result.data.begin());
    return result;
}

std::string APyFloatArray::repr() const
{
    std::stringstream ss {};
    ss << "APyFloatArray(";
    if (shape[0]) {
        std::stringstream sign_str {}, exp_str {}, man_str {};
        sign_str << "[";
        exp_str << "[";
        man_str << "[";
        for (std::size_t i = 0; i < data.size(); ++i) {
            const APyFloatData d = data[i];
            const bool is_last = i == (data.size() - 1);
            sign_str << (d.sign ? "1" : "0") << (is_last ? "" : ", ");
            exp_str << d.exp << (is_last ? "" : ", ");
            man_str << d.man << (is_last ? "" : ", ");
        }
        ss << sign_str.str() << "], " << exp_str.str() << "], " << man_str.str()
           << "], ";
    } else {
        ss << "[], [], [], ";
    }
    ss << "shape=";
    ss << tuple_string_from_vec(shape);
    ss << ", "
       << "exp_bits=" << static_cast<unsigned>(exp_bits) << ", "
       << "man_bits=" << static_cast<unsigned>(man_bits) << ", "
       << "bias=" << bias << ")";
    return ss.str();
}

APyFloatArray APyFloatArray::reshape(nb::tuple new_shape) const
{
    // Argument checking and error handling
    std::size_t elem_count = ::fold_shape(this->shape);
    std::vector<std::size_t> new_shape_vec
        = ::reshape_from_tuple(new_shape, elem_count);

    std::size_t itemsize = 1;
    APyFloatArray result = APyFloatArray(new_shape_vec, exp_bits, man_bits, bias);
    std::copy_n(this->data.begin(), elem_count * itemsize, result.data.begin());

    return result;
}

APyFloatArray APyFloatArray::flatten() const
{
    // Reuse the reshape function to flatten the array
    return this->reshape(nb::make_tuple(-1));
}

APyFloatArray APyFloatArray::ravel() const
{
    // currently same as flatten
    return this->flatten();
}

// The shape of the array
nanobind::tuple APyFloatArray::python_get_shape() const
{
    nb::list result_list;
    for (std::size_t i = 0; i < shape.size(); i++) {
        result_list.append(shape[i]);
    }
    return nb::tuple(result_list);
}

size_t APyFloatArray::get_ndim() const { return shape.size(); }

size_t APyFloatArray::get_size() const { return shape[0]; }

std::variant<APyFloatArray, APyFloat>
APyFloatArray::get_item(std::variant<nb::int_, nb::slice, nb::ellipsis, nb::tuple> key
) const
{
    if (std::holds_alternative<nb::int_>(key)) {

        // Key is of integer type
        return get_item_integer(static_cast<std::ptrdiff_t>(std::get<nb::int_>(key)));

    } else if (std::holds_alternative<nb::slice>(key)) {

        // Key is of slice type
        return get_item_slice(std::get<nb::slice>(key));

    } else if (std::holds_alternative<nb::ellipsis>(key)) {

        // Key is a single ellipsis, return a copy of `*this`
        return *this;

    } else { /* std::holds_alternative<nb::tuple>(key) */

        // Key is a tuple of slicing instructions
        return get_item_tuple(get_item_to_cpp_tuple(std::get<nb::tuple>(key)));
    }
}

std::variant<APyFloatArray, APyFloat> APyFloatArray::get_item_integer(std::ptrdiff_t idx
) const
{
    if (idx >= std::ptrdiff_t(shape[0]) || idx < -std::ptrdiff_t(shape[0])) {
        throw std::out_of_range(fmt::format(
            "APyFloatArray.__getitem__: index {} is out of bounds for axis 0 with size "
            "{}",
            idx,
            shape[0]
        ));
    }

    // Adjust for negative index
    idx = idx < 0 ? idx + shape[0] : idx;

    if (get_ndim() == 1) {
        // One dimension, return APyFloat
        return APyFloat(data[idx], exp_bits, man_bits, bias);
    } else {
        // New shape contains all dimensions except the very first one
        auto new_shape = std::vector<std::size_t>(shape.begin() + 1, shape.end());

        // Element stride is the new shape folded over multiplication
        std::size_t element_stride = fold_shape(new_shape);

        APyFloatArray result(new_shape, exp_bits, man_bits, bias);
        std::copy_n(
            data.begin() + idx * element_stride, element_stride, result.data.begin()
        );
        return result;
    }
}

APyFloatArray APyFloatArray::get_item_slice(nb::slice slice) const
{
    auto [start, stop, step, len] = slice.compute(shape[0]);

    // New resulting shape
    std::vector<std::size_t> new_shape = shape;
    new_shape[0] = len;

    // Result floating-point array
    APyFloatArray result(new_shape, exp_bits, man_bits, bias);

    auto size = fold_shape(std::begin(shape) + 1, std::end(shape));
    std::ptrdiff_t src_i = start;
    std::ptrdiff_t dst_i = 0;
    if (step < 0) {
        // Copy data into result and return (negative src step size)
        for (; src_i > stop; src_i += step, dst_i++) {
            std::copy_n(
                std::begin(data) + src_i * size,       // src
                size,                                  // elements to copy
                std::begin(result.data) + dst_i * size // dst
            );
        }
    } else { /* step >= 0 */
        // Copy data into result and return (positive src step size)
        for (; src_i < stop; src_i += step, dst_i++) {
            std::copy_n(
                std::begin(data) + src_i * size,       // src
                size,                                  // elements to copy
                std::begin(result.data) + dst_i * size // dst
            );
        }
    }

    return result;
}

std::vector<APyFloatArray> APyFloatArray::get_item_slice_nested(nb::slice slice) const
{
    auto [start, stop, step, len] = slice.compute(shape[0]);

    // The shape of the sliced vectors
    std::vector<std::size_t> new_shape(std::begin(shape) + 1, std::end(shape));
    if (!new_shape.size()) {
        new_shape = { 1 };
    }

    // Result floating-point array
    std::vector<APyFloatArray> result;

    auto size = fold_shape(new_shape);
    APyFloatArray tmp(new_shape, exp_bits, man_bits, bias);
    if (step < 0) {
        // Copy data into result and return (negative src step size)
        for (std::ptrdiff_t src_i = start; src_i > stop; src_i += step) {
            std::copy_n(std::begin(data) + src_i * size, size, std::begin(tmp.data));
            result.push_back(tmp);
        }
    } else {
        // Copy data into result and return (positive src step size)
        for (std::ptrdiff_t src_i = start; src_i < stop; src_i += step) {
            std::copy_n(std::begin(data) + src_i * size, size, std::begin(tmp.data));
            result.push_back(tmp);
        }
    }
    return result;
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::get_item_tuple(std::vector<std::variant<nb::int_, nb::slice>> tuple
) const
{
    // Return everything on an empty tuple
    if (tuple.size() == 0) {
        return *this;
    }

    std::variant<nb::int_, nb::slice> current = tuple[0];
    std::vector<std::variant<nb::int_, nb::slice>> remaining(
        std::begin(tuple) + 1, std::end(tuple)
    );

    if (std::holds_alternative<nb::int_>(current)) {

        /*
         * Current tuple element is an integer
         */
        std::ptrdiff_t key = static_cast<std::ptrdiff_t>(std::get<nb::int_>(current));
        std::variant<APyFloatArray, APyFloat> result = get_item_integer(key);
        if (!remaining.size()) {
            return result; // is always APyFloat
        } else {
            return std::get<APyFloatArray>(result).get_item_tuple(remaining);
        }

    } else { /* std::holds_alternative<nb::slice>(current) */

        /*
         * Current tuple element is a slice
         */
        APyFloatArray result(
            get_item_tuple_shape(tuple, remaining), exp_bits, man_bits, bias
        );
        nb::slice slice = std::get<nb::slice>(current);
        auto arrays = get_item_slice_nested(slice);

        for (std::size_t i = 0; i < arrays.size(); i++) {
            auto v = arrays[i].get_item_tuple(remaining);
            if (std::holds_alternative<APyFloat>(v)) {
                result.data[i] = std::get<APyFloat>(v).get_data();
            } else {
                auto fparray = std::get<APyFloatArray>(v);
                std::copy_n(
                    std::begin(fparray.data),
                    fparray.data.size(),
                    std::begin(result.data) + i * fparray.data.size()
                );
            }
        }

        return result;
    }
}

std::vector<std::size_t> APyFloatArray::get_item_tuple_shape(
    const std::vector<std::variant<nb::int_, nb::slice>>& tuple,
    const std::vector<std::variant<nb::int_, nb::slice>>& remaining
) const
{
    // Compute the resulting array shape
    std::vector<std::size_t> result_shape;
    for (std::size_t i = 0; i < tuple.size(); i++) {
        auto&& element = tuple[i];
        if (std::holds_alternative<nb::int_>(element)) {
            continue;
        } else { /* std::holds_alternative<nb::slice>(v) */
            auto [_, __, ___, len] = std::get<nb::slice>(element).compute(shape[i]);
            result_shape.push_back(len);
        }
    }
    for (std::size_t i = remaining.size() + 1; i < shape.size(); i++) {
        result_shape.push_back(shape[i]);
    }
    return result_shape;
}

std::vector<std::variant<nb::int_, nb::slice>>
APyFloatArray::get_item_to_cpp_tuple(const nb::tuple& key) const
{
    if (key.size() > get_ndim()) {
        // The key tuple must have fewer elements than this array has number of
        // dimensions
        std::string msg = fmt::format(
            "APyFloatArray.__getitem__: key tuple size (={}) > ndim (={})",
            key.size(),
            get_ndim()
        );
        throw nb::value_error(msg.c_str());
    }

    nb::int_ integer;
    nb::slice slice;
    nb::ellipsis ellipsis;
    bool ellipsis_found = false;
    std::vector<std::variant<nb::int_, nb::slice>> cpp_tuple;

    for (std::size_t i = 0; i < key.size(); i++) {
        const auto& tuple_element = key[i];
        if (nb::try_cast<nb::slice>(tuple_element, slice)) {
            cpp_tuple.push_back(slice);
        } else if (nb::try_cast<nb::int_>(tuple_element, integer)) {
            cpp_tuple.push_back(integer);
        } else if (nb::try_cast<nb::ellipsis>(tuple_element, ellipsis)) {
            if (ellipsis_found) {
                // An ellipsis object has already been procecced. Only a single ellipsis
                // per tuple key is allowed
                throw nb::value_error(
                    "APyFloatArray.__getitem__: only one single ellipsis object allowed"
                );
            } else {
                // Found first ellipsis, fill missing dimesnsions with full slices
                ellipsis_found = true;
                std::size_t n_fill = get_ndim() - key.size() + 1;
                for (std::size_t j = 0; j < n_fill; j++) {
                    cpp_tuple.push_back(nb::slice(shape[i + j]));
                }
            }
        } else {
            throw nb::value_error(
                "APyFloatArray.__getitem__: supported keys are `int`, `slice`, "
                "`ellipsis`, or a single tuple thereof"
            );
        }
    }
    return cpp_tuple;
}

std::variant<
    nb::list,
    nb::ndarray<nb::numpy, uint64_t>,
    nb::ndarray<nb::numpy, uint32_t>,
    nb::ndarray<nb::numpy, uint16_t>,
    nb::ndarray<nb::numpy, uint8_t>>
APyFloatArray::to_bits(bool numpy) const
{
    if (numpy) {
        if (get_bits() <= 8) {
            return to_bits_ndarray<nb::numpy, uint8_t>();
        } else if (get_bits() <= 16) {
            return to_bits_ndarray<nb::numpy, uint16_t>();
        } else if (get_bits() <= 32) {
            return to_bits_ndarray<nb::numpy, uint32_t>();
        } else {
            return to_bits_ndarray<nb::numpy, uint64_t>();
        }
    } else {
        auto it = std::cbegin(data);
        return to_bits_python_recursive_descent(0, it);
    }
}

template <typename NB_ARRAY_TYPE, typename INT_TYPE>
nb::ndarray<NB_ARRAY_TYPE, INT_TYPE> APyFloatArray::to_bits_ndarray() const
{
    constexpr std::size_t INT_TYPE_SIZE_BITS = 8 * sizeof(INT_TYPE);
    if (get_bits() > int(INT_TYPE_SIZE_BITS)) {
        throw nb::value_error(
            fmt::format(
                "APyFloatArray::to_bits_ndarray(): only supports <= {}-bit elements",
                INT_TYPE_SIZE_BITS
            )
                .c_str()
        );
    }

    INT_TYPE* result_data = new INT_TYPE[data.size()];
    for (std::size_t i = 0; i < data.size(); i++) {
        result_data[i] = to_bits_uint64(data[i], exp_bits, man_bits);
    }

    // Delete `result_data` when the `owner` capsule expires
    nb::capsule owner(result_data, [](void* p) noexcept { delete[] (INT_TYPE*)p; });

    return nb::ndarray<NB_ARRAY_TYPE, INT_TYPE>(
        result_data, get_ndim(), &shape[0], owner
    );
}

nb::list APyFloatArray::to_bits_python_recursive_descent(
    std::size_t dim, std::vector<APyFloatData>::const_iterator& it
) const
{
    nb::list result;
    const auto dims = get_ndim();
    if (dim == dims - 1) {
        // Most inner dimension: append data
        for (std::size_t i = 0; i < shape[dim]; i++) {
            result.append(apyfloat_to_bits(*it, exp_bits, man_bits));
            it++;
        }
    } else {
        // We need to go deeper...
        for (std::size_t i = 0; i < shape[dim]; i++) {
            result.append(to_bits_python_recursive_descent(dim + 1, it));
        }
    }
    return result;
}

nb::ndarray<nb::numpy, double> APyFloatArray::to_numpy() const
{
    // Dynamically allocate data to be passed to python
    double* result_data = new double[data.size()];
    auto apy_f = APyFloat(exp_bits, man_bits, bias);
    for (std::size_t i = 0; i < data.size(); i++) {
        apy_f.set_data(data[i]);
        result_data[i] = apy_f.to_double();
    }

    // Delete 'data' when the 'owner' capsule expires
    nb::capsule owner(result_data, [](void* p) noexcept { delete[] (double*)p; });

    return nb::ndarray<nb::numpy, double>(result_data, get_ndim(), &shape[0], owner);
}

bool APyFloatArray::is_identical(const APyFloatArray& other) const
{
    const bool same_spec = (shape == other.shape) && (exp_bits == other.exp_bits)
        && (man_bits == other.man_bits) && (bias == other.bias)
        && (data.size() == other.data.size());

    return same_spec
        && std::equal(data.begin(), data.end(), other.data.begin(), other.data.end());
}

APyFloatArray APyFloatArray::from_double(
    const nb::sequence& number_seq,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits);
    check_mantissa_format(man_bits);

    if (nb::isinstance<nb::ndarray<>>(number_seq)) {
        // Sequence is NDArray. Initialize using `from_array`.
        auto ndarray = nb::cast<nb::ndarray<nb::c_contig>>(number_seq);
        return from_array(ndarray, exp_bits, man_bits, bias);
    }

    APyFloatArray result(
        python_sequence_extract_shape(number_seq), exp_bits, man_bits, bias
    );

    const auto py_obj = python_sequence_walk<nb::float_, nb::int_>(number_seq);
    const auto actual_bias = bias.value_or(APyFloat::ieee_bias(exp_bits));

    for (std::size_t i = 0; i < result.data.size(); i++) {
        if (nb::isinstance<nb::float_>(py_obj[i])) {
            result.data[i] = APyFloat::from_double(
                                 (double)nb::cast<nb::float_>(py_obj[i]),
                                 exp_bits,
                                 man_bits,
                                 actual_bias
            )
                                 .get_data();
        } else if (nb::isinstance<nb::int_>(py_obj[i])) {
            result.data[i]
                = APyFloat::from_integer(
                      nb::cast<nb::int_>(py_obj[i]), exp_bits, man_bits, actual_bias
                )
                      .get_data();
        } else {
            throw std::domain_error("Invalid Python objects in sequence");
        }
    }
    return result;
}

APyFloatArray APyFloatArray::from_array(
    const nb::ndarray<nb::c_contig>& ndarray,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits);
    check_mantissa_format(man_bits);

    const std::size_t ndim = ndarray.ndim();
    assert(ndim > 0);
    std::vector<std::size_t> shape(ndim, 0);
    for (std::size_t i = 0; i < ndim; i++) {
        shape[i] = ndarray.shape(i);
    }

    APyFloatArray result(shape, exp_bits, man_bits, bias);
    result._set_values_from_ndarray(ndarray);
    return result;
}

void APyFloatArray::_set_values_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray)
{
    // Double value used for converting.
    APyFloat double_caster(11, 52, 1023);

#define CHECK_AND_SET_VALUES_FROM_NPTYPE(__TYPE__)                                     \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            auto ndarray_view = ndarray.view<__TYPE__, nb::ndim<1>>();                 \
            for (std::size_t i = 0; i < ndarray.size(); i++) {                         \
                double value = static_cast<double>(ndarray_view.data()[i]);            \
                double_caster.set_data({ sign_of_double(value),                        \
                                         exp_t(exp_of_double(value)),                  \
                                         man_of_double(value) });                      \
                APyFloat fp                                                            \
                    = double_caster.cast_from_double(exp_bits, man_bits, bias);        \
                data[i] = { fp.get_sign(), fp.get_exp(), fp.get_man() };               \
            }                                                                          \
            return; /* Conversion completed, exit function */                          \
        }                                                                              \
    } while (0)

    // Each `CHECK_AND_SET_VALUES_FROM_NPTYPE` checks the dtype of `ndarray` and
    // converts all the data if it matches. If successful,
    // `CHECK_AND_SET_VALUES_FROM_NPTYPES` returns. Otherwise, the next attempted
    // conversion will take place
    CHECK_AND_SET_VALUES_FROM_NPTYPE(double);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(float);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::int64_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::int32_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::int16_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::int8_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::uint64_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::uint32_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::uint16_t);
    CHECK_AND_SET_VALUES_FROM_NPTYPE(std::uint8_t);

    // None of the `CHECK_AND_VALUES_FROM_NPTYPE` succeeded. Unsupported type, throw
    // an error. If possible, it would be nice to show a string representation of
    // the `dtype`. Seems hard to achieve with nanobind, but please fix this if you
    // find out how this can be achieved.
    throw nb::type_error(
        "APyFloatArray::_set_values_from_ndarray(): "
        "unsupported `dtype` expecting integer/float"
    );
}

APyFloatArray APyFloatArray::broadcast_to(const std::vector<std::size_t> shape) const
{
    if (!is_broadcastable(this->shape, shape)) {
        throw nb::value_error(
            fmt::format(
                "Operands could not be broadcast together with shapes: {}, {}",
                tuple_string_from_vec(this->shape),
                tuple_string_from_vec(shape)
            )
                .c_str()
        );
    }
    APyFloatArray result(shape, exp_bits, man_bits, bias);
    broadcast_data_copy(data.begin(), result.data.begin(), this->shape, shape);
    return result;
}

APyFloatArray
APyFloatArray::broadcast_to_python(const std::variant<nb::tuple, nb::int_> shape) const
{
    return broadcast_to(cpp_shape_from_python_shape_like(shape));
}

APyFloatArray APyFloatArray::swapaxes(nb::int_ axis1, nb::int_ axis2) const
{
    std::size_t ndim = get_ndim();
    size_t _axis1 = ::get_normalized_axes(axis1, ndim).front();
    size_t _axis2 = ::get_normalized_axes(axis2, ndim).front();

    std::vector<size_t> new_axis(ndim);
    std::iota(new_axis.begin(), new_axis.end(), 0);

    // Swap the specified axes
    std::swap(new_axis[_axis1], new_axis[_axis2]);

    std::vector<size_t> new_shape(ndim);
    for (size_t i = 0; i < ndim; ++i) {
        new_shape[i] = shape[new_axis[i]];
    }

    APyFloatArray result(new_shape, exp_bits, man_bits, bias);
    transpose_axes_and_copy_data(data.begin(), result.data.begin(), shape, new_axis);
    return result;
}

APyFloatArray APyFloatArray::transpose(std::optional<nb::tuple> axes) const
{
    std::size_t ndim = get_ndim();
    switch (ndim) {
    case 0:
    case 1:
        // Behave like `NumPy`, simply returns `*this` if single-dimensional
        return *this;
    case 2: {

        // Optimized code for dim == 2
        // Resulting array with shape dimensions
        std::vector<size_t> new_shape = { shape[1], shape[0] };
        APyFloatArray result(new_shape, exp_bits, man_bits, bias);

        // Transpose the data
        for (std::size_t y = 0; y < shape[0]; ++y) {
            for (std::size_t x = 0; x < shape[1]; ++x) {
                result.data[x * shape[0] + y] = data[y * shape[1] + x];
            }
        }
        return result;
    }

    default: {
        std::vector<size_t> new_axis(ndim);

        if (axes.has_value()) {
            std::variant<nb::tuple, nb::int_> axis_variant = axes.value();
            new_axis = get_normalized_axes(axis_variant, ndim);
        } else {
            // reverse the order of axes by default
            std::iota(new_axis.begin(), new_axis.end(), 0);
            std::reverse(new_axis.begin(), new_axis.end());
        }

        std::vector<size_t> new_shape(ndim);
        for (size_t i = 0; i < ndim; ++i) {
            new_shape[i] = shape[new_axis[i]];
        }

        APyFloatArray result(new_shape, exp_bits, man_bits, bias);
        transpose_axes_and_copy_data(
            data.begin(), result.data.begin(), shape, new_axis
        );
        return result;
    }
    }
}

APyFloatArray APyFloatArray::cast(
    std::optional<int> new_exp_bits,
    std::optional<int> new_man_bits,
    std::optional<exp_t> new_bias,
    std::optional<QuantizationMode> quantization
) const
{
    const auto actual_exp_bits = new_exp_bits.value_or(exp_bits);
    const auto actual_man_bits = new_man_bits.value_or(man_bits);

    check_exponent_format(actual_exp_bits);
    check_mantissa_format(actual_man_bits);

    return _cast(
        actual_exp_bits,
        actual_man_bits,
        new_bias.value_or(APyFloat::ieee_bias(actual_exp_bits)),
        quantization.value_or(get_float_quantization_mode())
    );
}
APyFloatArray APyFloatArray::_cast(
    std::uint8_t new_exp_bits,
    std::uint8_t new_man_bits,
    exp_t new_bias,
    std::optional<QuantizationMode> quantization
) const
{
    return _cast(
        new_exp_bits,
        new_man_bits,
        new_bias,
        quantization.value_or(get_float_quantization_mode())
    );
}

APyFloatArray APyFloatArray::_cast(
    std::uint8_t new_exp_bits,
    std::uint8_t new_man_bits,
    exp_t new_bias,
    QuantizationMode quantization
) const
{
    // If same parameters, do not quantize
    if (new_exp_bits == exp_bits && new_man_bits == man_bits && new_bias == bias) {
        return *this;
    }

    // If longer word lengths, use simpler/faster method
    if (new_exp_bits >= exp_bits && new_man_bits >= man_bits) {
        return cast_no_quant(new_exp_bits, new_man_bits, new_bias);
    }

    APyFloatArray result(shape, new_exp_bits, new_man_bits, new_bias);

    APyFloat caster(exp_bits, man_bits, bias);
    for (std::size_t i = 0; i < data.size(); i++) {
        caster.set_data(data[i]);
        result.data[i]
            = caster._checked_cast(new_exp_bits, new_man_bits, new_bias, quantization)
                  .get_data();
    }

    return result;
}

APyFloatArray APyFloatArray::cast_no_quant(
    std::uint8_t new_exp_bits, std::uint8_t new_man_bits, std::optional<exp_t> new_bias
) const
{
    APyFloatArray result(shape, new_exp_bits, new_man_bits, new_bias);

    APyFloat caster(exp_bits, man_bits, bias);
    for (std::size_t i = 0; i < data.size(); i++) {
        caster.set_data(data[i]);
        result.data[i]
            = caster.cast_no_quant(new_exp_bits, new_man_bits, new_bias).get_data();
    }

    return result;
}

// Evaluate the inner between two vectors. This method assumes that the the shape of
// both `*this` and `rhs` are equally long. Anything else is undefined behaviour.
APyFloat APyFloatArray::checked_inner_product(
    const APyFloatArray& rhs,
    std::optional<APyFloatAccumulatorOption> accumulator_mode,
    const std::uint8_t max_exp_bits,
    const std::uint8_t max_man_bits
) const
{
    // No accumulator context set
    if (accumulator_mode.has_value()) {
        const auto acc_option = accumulator_mode.value();
        const auto tmp_exp_bits = acc_option.exp_bits;
        const auto tmp_man_bits = acc_option.man_bits;
        const auto tmp_bias = acc_option.bias.value_or(
            calc_bias(tmp_exp_bits, exp_bits, bias, rhs.exp_bits, rhs.bias)
        );
        ;
        // If an accumulator is used, the operands must be resized before the
        // multiplication. This is because the products would otherwise get
        // quantized too early. NOTE: This assumes that the format of the
        // accumulator is larger

        APyFloatArray hadamard(shape, tmp_exp_bits, tmp_man_bits, tmp_bias);
        // Hadamard product of `*this` and `rhs`
        (this->_cast(tmp_exp_bits, tmp_man_bits, tmp_bias, acc_option.quantization))
            .hadamard_multiplication(
                &rhs._cast(
                        tmp_exp_bits, tmp_man_bits, tmp_bias, acc_option.quantization
                )
                     .data[0],
                tmp_exp_bits,
                tmp_man_bits,
                tmp_bias,
                hadamard,
                acc_option.quantization
            );
        APyFloat sum = hadamard.vector_sum(acc_option.quantization);
        // The result must be quantized back if an accumulator was used.
        sum = sum._cast(
            max_exp_bits,
            max_man_bits,
            calc_bias_general(max_exp_bits, exp_bits, bias, rhs.exp_bits, rhs.bias),
            acc_option.quantization
        );
        return sum;
    }
    // No accumulator context

    // Hadamard product of `*this` and `rhs`
    APyFloatArray hadamard = *this * rhs;
    APyFloat sum = hadamard.vector_sum(get_float_quantization_mode());
    return sum;
}

// Compute sum of all elements
APyFloat APyFloatArray::vector_sum(const QuantizationMode quantization) const
{
    // +5 to give room for leading one, carry, and 3 guard bits
    const unsigned int max_man_bits = man_bits + 5;
    APyFloat ret(0, 0, 0, exp_bits, man_bits, bias);
    if ((max_man_bits <= _MAN_T_SIZE_BITS)
        && (quantization != QuantizationMode::STOCH_WEIGHTED)) {
        APyFloatData x;
        bool sum_sign = false;
        exp_t sum_exp = 0;
        man_t sum_man = 0;
        const exp_t res_max_exponent = ((1ULL << exp_bits) - 1);
        const man_t final_res_leading_one = (1ULL << man_bits);
        const man_t res_leading_one = final_res_leading_one << 3;
        const man_t carry_res_leading_one = res_leading_one << 1;
        const std::uint8_t tmp_man_bits
            = man_bits + 4; // 4 for leading 1 and 3 guard bits
        const auto shift_normalization_const = _MAN_T_SIZE_BITS - tmp_man_bits;
        const auto man_mask = carry_res_leading_one - 1;
        bool sum_is_max_exponent = false;
        // Perform operation
        for (std::size_t i = 0; i < data.size(); i++) {
            x = data[i];
            const bool x_is_zero_exponent = (x.exp == 0);
            // Handle zero cases
            if (x_is_zero_exponent && x.man == 0) {
                continue;
            }
            const bool sum_is_zero_exponent = (sum_exp == 0);
            if (sum_is_zero_exponent && sum_man == 0) {
                sum_sign = x.sign;
                sum_exp = x.exp;
                sum_man = x.man;
                sum_is_max_exponent = (x.exp == res_max_exponent);
                continue;
            }
            const bool x_is_max_exponent = x.exp == res_max_exponent;
            const bool same_sign = (x.sign == sum_sign);
            // Handle the NaN and inf cases
            if (x_is_max_exponent || sum_is_max_exponent) {
                if ((x_is_max_exponent && x.man != 0)
                    || (sum_is_max_exponent && sum_man != 0)
                    || (x_is_max_exponent && sum_is_max_exponent && !same_sign)) {
                    // Set to NaN
                    ret.set_data({ false,
                                   static_cast<exp_t>(res_max_exponent),
                                   static_cast<man_t>(1) });
                    return ret;
                }

                // Handle inf cases
                if (x_is_max_exponent && x.man == 0) {
                    // Set to inf
                    // No early return as later values may be NaN
                    sum_sign = x.sign;
                    sum_exp = res_max_exponent;
                    sum_man = 0;
                    sum_is_max_exponent = true;
                    continue;
                }

                // Here: sum_is_max_exponent && sum_man == 0
                // Keep at inf
                // No early return as later values may be NaN
                continue;
            }

            if (!same_sign && x.exp == sum_exp && x.man == sum_man) {
                // Set to zero
                sum_sign = false;
                sum_exp = 0;
                sum_man = 0;
                continue;
            }

            const exp_t true_x_exp = x.exp + x_is_zero_exponent;
            const exp_t true_sum_exp = sum_exp + sum_is_zero_exponent;
            // Conditionally add leading one's, also add room for guard bits
            // Note that exp can never be res_max_exponent here
            man_t mx = (x_is_zero_exponent ? 0 : res_leading_one) | (x.man << 3);
            const man_t msum
                = (sum_is_zero_exponent ? 0 : res_leading_one) | (sum_man << 3);

            // Compute sign and swap operands if need to make sure |x| >= |y|
            if (x.exp < sum_exp || (x.exp == sum_exp && x.man < sum_man)) {
                const unsigned exp_delta = true_sum_exp - true_x_exp;
                // Align mantissa based on difference in exponent
                if (exp_delta <= 3) {
                    mx >>= exp_delta;
                } else if (exp_delta >= max_man_bits) {
                    mx = (mx >> max_man_bits) | 1;
                } else {
                    mx = (mx >> exp_delta)
                        | ((mx << (_MAN_T_SIZE_BITS - exp_delta)) != 0);
                }
                sum_exp = true_sum_exp;
                // Perform addition / subtraction
                sum_man = same_sign ? msum + mx : msum - mx;
            } else {
                // Align mantissas based on exponent difference
                const unsigned exp_delta = true_x_exp - true_sum_exp;
                // Align mantissa based on difference in exponent
                man_t m_aligned;
                if (exp_delta <= 3) {
                    m_aligned = msum >> exp_delta;
                } else if (exp_delta >= max_man_bits) {
                    m_aligned = (msum >> max_man_bits) | 1;
                } else {
                    m_aligned = (msum >> exp_delta)
                        | ((msum << (_MAN_T_SIZE_BITS - exp_delta)) != 0);
                }
                sum_exp = true_x_exp;
                // Perform addition / subtraction
                sum_man = same_sign ? mx + m_aligned : mx - m_aligned;
                sum_sign = x.sign;
            }

            // Check for carry and cancellation
            if (sum_man & carry_res_leading_one) {
                // Carry
                sum_exp++;
            } else if (sum_man & res_leading_one) {
                // Align mantissa to carry case
                sum_man <<= 1;
            } else {
                // Cancellation or addition with subnormals
                // Mantissa should be shifted until 1.xx is obtained or new_exp
                // equals 0
                const unsigned int man_leading_zeros = leading_zeros(sum_man);
                const unsigned int normalizing_shift
                    = man_leading_zeros - shift_normalization_const;

                if (sum_exp > normalizing_shift) {
                    sum_man <<= normalizing_shift + 1;
                    sum_exp -= normalizing_shift;
                } else {
                    // The result will be a subnormal
                    sum_man <<= sum_exp;
                    sum_exp = 0;
                }
            }

            sum_man &= man_mask;

            quantize_mantissa(
                sum_man,
                sum_exp,
                res_max_exponent,
                4,
                sum_sign,
                final_res_leading_one,
                3,
                7,
                quantization
            );

            // Check for overflow
            if (sum_exp == res_max_exponent) {
                sum_is_max_exponent = true;
            }
        }
        ret.set_data({ sum_sign, sum_exp, sum_man });
        return ret;
    }
    APyFloat tmp(0, 0, 0, exp_bits, man_bits, bias);

    for (std::size_t i = 0; i < data.size(); i++) {
        tmp.set_data(data[i]);
        ret += tmp;
    }
    return ret;
}

// Evaluate the matrix product between two 2D matrices. This method assumes that the
// shape of `*this` and `rhs` have been checked to match a 2d matrix multiplication.
APyFloatArray APyFloatArray::checked_2d_matmul(const APyFloatArray& rhs) const
{
    // Resulting parameters
    std::vector<std::size_t> res_shape = rhs.shape.size() > 1
        ? std::vector<std::size_t> { shape[0], rhs.shape[1] } // rhs is 2-D
        : std::vector<std::size_t> { shape[0] };              // rhs is 1-D
    const std::uint8_t max_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t max_man_bits = std::max(man_bits, rhs.man_bits);
    const auto res_bias
        = calc_bias(max_exp_bits, exp_bits, bias, rhs.exp_bits, rhs.bias);
    const auto res_cols = rhs.shape.size() > 1 ? rhs.shape[1] : 1;

    const auto accumulator_mode = get_accumulator_mode_float();

    // Resulting `APyFloatArray`
    APyFloatArray result(res_shape, max_exp_bits, max_man_bits, res_bias);

    // Current column from rhs
    APyFloatArray current_column(
        { rhs.shape[0] }, rhs.exp_bits, rhs.man_bits, rhs.bias
    );

    // Accumulator mode set
    if (accumulator_mode.has_value()) {
        const auto acc_option = accumulator_mode.value();
        const auto tmp_exp_bits = acc_option.exp_bits;
        const auto tmp_man_bits = acc_option.man_bits;

        const auto tmp_bias = acc_option.bias.value_or(
            calc_bias(tmp_exp_bits, exp_bits, bias, rhs.exp_bits, rhs.bias)
        );

        APyFloatArray hadamard({ shape[1] }, tmp_exp_bits, tmp_man_bits, tmp_bias);
        APyFloatArray casted_this
            = _cast(tmp_exp_bits, tmp_man_bits, tmp_bias, acc_option.quantization);
        for (std::size_t x = 0; x < res_cols; x++) {

            // Copy column from `rhs` and use as the current working column. As
            // reading columns from `rhs` is cache-inefficient, we like to do this
            // only once for each element in the resulting matrix.
            for (std::size_t col = 0; col < rhs.shape[0]; col++) {
                current_column.data[col] = rhs.data[x + col * res_cols];
            }

            APyFloatArray casted_current_column = current_column._cast(
                tmp_exp_bits, tmp_man_bits, tmp_bias, acc_option.quantization
            );
            for (std::size_t y = 0; y < res_shape[0]; y++) {
                // If an accumulator is used, the operands must be resized before
                // the multiplication. This is because the products would otherwise
                // get quantized too early.

                // Hadamard product of `*this` and `rhs`
                casted_current_column.hadamard_multiplication(
                    &casted_this.data[y * shape[1]],
                    tmp_exp_bits,
                    tmp_man_bits,
                    tmp_bias,
                    hadamard,
                    acc_option.quantization
                );
                APyFloat sum = hadamard.vector_sum(acc_option.quantization);
                // The result must be quantized back if an accumulator was used.
                sum = sum._cast(
                    max_exp_bits, max_man_bits, res_bias, acc_option.quantization
                );

                assert(sum.get_exp_bits() == result.get_exp_bits());
                assert(sum.get_man_bits() == result.get_man_bits());

                // Copy into the resulting vector
                result.data[x + y * res_cols] = sum.get_data();
            }
        }
        return result;
    }

    // No accumulator mode

    const auto quantization = get_float_quantization_mode();
    APyFloatArray hadamard({ shape[1] }, max_exp_bits, max_man_bits, res_bias);

    for (std::size_t x = 0; x < res_cols; x++) {

        // Copy column from `rhs` and use as the current working column. As reading
        // columns from `rhs` is cache-inefficient, we like to do this only once for
        // each element in the resulting matrix.
        for (std::size_t col = 0; col < rhs.shape[0]; col++) {
            current_column.data[col] = rhs.data[x + col * res_cols];
        }

        for (std::size_t y = 0; y < res_shape[0]; y++) {
            // Perform the inner product
            // Hadamard product of `current_column` and `current_row`
            current_column.hadamard_multiplication(
                &data[y * shape[1]], exp_bits, man_bits, bias, hadamard, quantization
            );
            APyFloat sum = hadamard.vector_sum(quantization);
            assert(sum.get_exp_bits() == result.get_exp_bits());
            assert(sum.get_man_bits() == result.get_man_bits());

            // Copy into the resulting vector
            result.data[x + y * res_cols] = sum.get_data();
        }
    }

    return result;
}

/*
 * Convenience methods
 */
APyFloatArray APyFloatArray::cast_to_double(std::optional<QuantizationMode> quantization
) const
{
    return _cast(11, 52, 1023, quantization);
}

APyFloatArray APyFloatArray::cast_to_single(std::optional<QuantizationMode> quantization
) const
{
    return _cast(8, 23, 127, quantization);
}

APyFloatArray APyFloatArray::cast_to_half(std::optional<QuantizationMode> quantization
) const
{
    return _cast(5, 10, 15, quantization);
}

APyFloatArray
APyFloatArray::cast_to_bfloat16(std::optional<QuantizationMode> quantization) const
{
    return _cast(8, 7, 127, quantization);
}
