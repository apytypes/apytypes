// Python object access through Pybind
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
namespace nb = nanobind;

#include "apyfloat.h"
#include "apyfloat_util.h"
#include "apyfloatarray.h"
#include "ieee754.h"
#include "python_util.h"
#include <algorithm>
#include <fmt/format.h>
#include <stdexcept>
#include <string>

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
    this->bias = APyFloat::ieee_bias(exp_bits);
    if (bias.value_or(this->bias) != this->bias) {
        throw NotImplementedException(
            "Not implemented: APyFloatArray with non IEEE-like bias"
        );
    }

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
    data = std::vector<APyFloatData>(fold_shape(), { 0, 0, 0 });
}

/* ********************************************************************************** *
 * *                            Binary arithmetic operators                         * *
 * ********************************************************************************* */

APyFloatArray APyFloatArray::operator+(const APyFloatArray& rhs) const
{
    // Make sure `_shape` of `*this` and `rhs` are the same
    if (shape != rhs.shape) {
        throw std::length_error(fmt::format(
            "APyFloatArray.__add__: shape missmatch, lhs.shape={}, rhs.shape={}",
            string_from_vec(shape),
            string_from_vec(rhs.shape)
        ));
    }

    const auto quantization = get_quantization_mode();
    // +5 to give room for leading one, carry, and 3 guard bits
    const unsigned int max_man_bits = man_bits + 5;
    if (same_type_as(rhs) && (max_man_bits <= _MAN_T_SIZE_BITS)
        && (quantization != QuantizationMode::STOCH_WEIGHTED)) {
        // Result array
        APyFloatArray res(shape, exp_bits, man_bits);

        APyFloatData x, y;
        const exp_t res_max_exponent = ((1ULL << exp_bits) - 1);
        bool res_sign;
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
            // Handle zero cases
            if (x.exp == 0 && x.man == 0) {
                res.data[i] = y;
                continue;
            }
            if (y.exp == 0 && y.man == 0) {
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
                res_sign = y.sign;
                std::swap(x, y);
            } else {
                if (x.sign != y.sign && x.exp == y.exp && x.man == y.man) {
                    // Set to zero
                    res.data[i]
                        = { true, static_cast<exp_t>(0), static_cast<man_t>(0) };
                    continue;
                }
                res_sign = x.sign;
            }

            // Tentative exponent
            std::int64_t new_exp = x.exp;

            // Conditionally add leading one's, also add room for guard bits
            // Note that exp can never be res_max_exponent here
            man_t mx = ((x.exp != 0) ? res_leading_one : 0) | (x.man << 3);
            man_t my = ((y.exp != 0) ? res_leading_one : 0) | (y.man << 3);

            // Align mantissas based on exponent difference
            const unsigned exp_delta = x.exp - y.exp;

            // Align mantissa based on difference in exponent
            man_t highY;
            if (exp_delta <= 3) {
                highY = my >> exp_delta;
            } else if (exp_delta >= max_man_bits) {
                highY = (my >> max_man_bits) | 1;
            } else {
                highY
                    = (my >> exp_delta) | ((my << (_MAN_T_SIZE_BITS - exp_delta)) != 0);
            }

            // Perform addition / subtraction
            man_t new_man = (x.sign == y.sign) ? mx + highY : mx - highY;

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
                    new_man <<= new_exp + 1;
                    new_exp = 0;
                }
            }

            new_man &= man_mask;

            man_t res_man = quantize_mantissa(new_man, 4, res_sign, quantization);
            if (res_man & final_res_leading_one) {
                ++new_exp;
                res_man = 0;
            }

            // Check for overflow
            if (new_exp >= res_max_exponent) {
                // Inf
                res.data[i] = { x.sign,
                                static_cast<exp_t>(res_max_exponent),
                                static_cast<man_t>(0) };
            } else {
                res.data[i] = { res_sign,
                                static_cast<exp_t>(new_exp),
                                static_cast<man_t>(res_man) };
            }
        }
        return res;
    }
    // Calculate new format
    APyFloatArray res(
        shape, std::max(exp_bits, rhs.exp_bits), std::max(man_bits, rhs.man_bits)
    );

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
    const auto quantization = get_quantization_mode();
    // +5 to give room for leading one, carry, and 3 guard bits
    const unsigned int max_man_bits = man_bits + 5;
    if (same_type_as(rhs) && (max_man_bits <= _MAN_T_SIZE_BITS)
        && (quantization != QuantizationMode::STOCH_WEIGHTED)) {

        // Handle zero
        if (rhs.is_zero()) {
            return *this;
        }
        const exp_t res_max_exponent = ((1ULL << exp_bits) - 1);
        APyFloatData x, y;
        // Result array
        APyFloatArray res(shape, exp_bits, man_bits);
        APyFloatData yref = rhs.get_data();
        bool y_is_max_exponent = yref.exp == res_max_exponent;

        if (y_is_max_exponent) {
            for (std::size_t i = 0; i < data.size(); i++) {
                x = data[i];
                bool x_is_max_exponent = x.exp == res_max_exponent;
                if ((yref.man != 0) || (x_is_max_exponent && x.man != 0)
                    || (x.sign != yref.sign && x_is_max_exponent)) {
                    // Set to NaN
                    res.data[i] = { x.sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(1) };
                } else {
                    // Set to inf
                    res.data[i] = { yref.sign,
                                    static_cast<exp_t>(res_max_exponent),
                                    static_cast<man_t>(0) };
                    continue;
                }
            }
            return res;
        }
        bool res_sign;
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
            // Handle zero case
            if (x.exp == 0 && x.man == 0) {
                res.data[i] = yref;
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
            y = yref;
            // Compute sign and swap operands if need to make sure |x| >= |y|
            if (x.exp < y.exp || (x.exp == y.exp && x.man < y.man)) {
                res_sign = y.sign;
                std::swap(x, y);
            } else {
                if (x.sign != y.sign && x.exp == y.exp && x.man == y.man) {
                    // Set to zero
                    res.data[i]
                        = { false, static_cast<exp_t>(0), static_cast<man_t>(0) };
                    continue;
                }
                res_sign = x.sign;
            }

            // Tentative exponent
            std::int64_t new_exp = x.exp;

            // Conditionally add leading one's, also add room for guard bits
            // Note that exp can never be res_max_exponent here
            man_t mx = ((x.exp != 0) ? res_leading_one : 0) | (x.man << 3);
            man_t my = ((y.exp != 0) ? res_leading_one : 0) | (y.man << 3);

            // Align mantissas based on exponent difference
            const unsigned exp_delta = x.exp - y.exp;

            // Align mantissa based on difference in exponent
            man_t highY;
            if (exp_delta <= 3) {
                highY = my >> exp_delta;
            } else if (exp_delta >= max_man_bits) {
                highY = (my >> max_man_bits) | 1;
            } else {
                highY
                    = (my >> exp_delta) | ((my << (_MAN_T_SIZE_BITS - exp_delta)) != 0);
            }

            // Perform addition / subtraction
            man_t new_man = (x.sign == y.sign) ? mx + highY : mx - highY;

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
                    new_man <<= new_exp + 1;
                    new_exp = 0;
                }
            }

            new_man &= man_mask;

            man_t res_man = quantize_mantissa(new_man, 4, res_sign, quantization);
            if (res_man & final_res_leading_one) {
                ++new_exp;
                res_man = 0;
            }

            // Check for overflow
            if (new_exp >= res_max_exponent) {
                // Inf
                res.data[i] = { x.sign,
                                static_cast<exp_t>(res_max_exponent),
                                static_cast<man_t>(0) };
            } else {
                res.data[i] = { res_sign,
                                static_cast<exp_t>(new_exp),
                                static_cast<man_t>(res_man) };
            }
        }
        return res;
    }
    auto new_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    auto new_man_bits = std::max(man_bits, rhs.get_man_bits());
    if (rhs.is_zero()) {
        return cast_no_quant(new_exp_bits, new_man_bits);
    }

    // Calculate new format
    APyFloatArray res(shape, new_exp_bits, new_man_bits);

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
    // Make sure `_shape` of `*this` and `rhs` are the same
    if (shape != rhs.shape) {
        throw std::length_error(fmt::format(
            "APyFloatArray.__sub__: shape missmatch, lhs.shape={}, rhs.shape={}",
            string_from_vec(shape),
            string_from_vec(rhs.shape)
        ));
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
    // Make sure `_shape` of `*this` and `rhs` are the same
    if (shape != rhs.shape) {
        throw std::length_error(fmt::format(
            "APyFloatArray.__mul__: shape missmatch, lhs.shape={}, rhs.shape={}",
            string_from_vec(shape),
            string_from_vec(rhs.shape)
        ));
    }

    // Calculate new format
    const auto res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const auto res_man_bits = std::max(man_bits, rhs.man_bits);
    const auto res_bias = APyFloat::ieee_bias(res_exp_bits);
    APyFloatArray res(shape, res_exp_bits, res_man_bits, res_bias);

    const int sum_man_bits = man_bits + rhs.man_bits;
    const auto quantization = get_quantization_mode();

    if (unsigned(sum_man_bits) + 2 <= _MAN_T_SIZE_BITS) {
        // Compute constants for reuse
        const auto x_max_exponent = ((1ULL << exp_bits) - 1);
        const auto y_max_exponent = ((1ULL << rhs.exp_bits) - 1);
        const auto res_max_exponent = ((1ULL << res_exp_bits) - 1);
        const man_t one = 1ULL << sum_man_bits;
        const man_t two = one << 1;
        const auto mask_two = two - 1;
        const std::int64_t bias_sum = bias + rhs.bias;
        const auto new_man_bits = sum_man_bits + 1;
        const auto new_exp_bits = bit_width(bias_sum + new_man_bits) + 1;
        const auto extended_bias = APyFloat::ieee_bias(new_exp_bits);
        // Use longer format for intermediate result
        APyFloat larger_float(new_exp_bits, new_man_bits, extended_bias);

        // Perform operation
        for (std::size_t i = 0; i < data.size(); i++) {
            const auto x = data[i];
            const auto y = rhs.data[i];

            // Calculate sign
            const bool res_sign = x.sign ^ y.sign;

            const bool x_is_subnormal = (x.exp == 0);
            const bool x_is_maxexp = (x.exp == x_max_exponent);
            const bool y_is_subnormal = (y.exp == 0);
            const bool y_is_maxexp = (y.exp == y_max_exponent);

            // Handle special operands
            if (x_is_maxexp || y_is_maxexp) {
                const bool x_is_nan = (x_is_maxexp && x.man != 0);
                const bool x_is_inf = (x_is_maxexp && x.man == 0);
                const bool y_is_nan = (y_is_maxexp && y.man != 0);
                const bool y_is_inf = (y_is_maxexp && y.man == 0);
                if (x_is_nan || y_is_nan || (x_is_inf && y_is_subnormal && y.man == 0)
                    || (y_is_inf && x_is_subnormal && x.man == 0)) {
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
            }

            // x is zero or y is zero (and the other is not inf)
            if ((x_is_subnormal && x.man == 0) || (y_is_subnormal && y.man == 0)) {
                // Set to zero
                res.data[i]
                    = { res_sign, static_cast<exp_t>(0), static_cast<man_t>(0) };
                continue;
            }

            // Tentative exponent
            std::int64_t tmp_exp = (std::int64_t)x.exp + x_is_subnormal
                + (std::int64_t)y.exp + y_is_subnormal - bias_sum;
            const man_t mx = (static_cast<man_t>(!x_is_subnormal) << man_bits) | x.man;
            const man_t my
                = (static_cast<man_t>(!y_is_subnormal) << rhs.man_bits) | y.man;

            man_t new_man = mx * my;

            // In case of denormalized data
            if (new_man < one) {
                int cnt = new_man_bits - bit_width(new_man);
                new_man <<= cnt + 1;
                tmp_exp -= cnt;
            } else {
                // Result may be larger than two
                if (new_man >= two) {
                    tmp_exp++;
                } else {
                    // Align with longer result
                    new_man <<= 1;
                }
            }
            // Remove leading one
            new_man &= mask_two;

            auto new_exp = tmp_exp + extended_bias;
            // Use longer format for intermediate result
            larger_float.set_data({ res_sign, (exp_t)new_exp, new_man });
            res.data[i]
                = larger_float._cast(res_exp_bits, res_man_bits, res_bias, quantization)
                      .get_data();
        }
        return res;
    }
    APyFloat lhs_scalar(exp_bits, man_bits, bias);
    APyFloat rhs_scalar(rhs.exp_bits, rhs.man_bits, rhs.bias);
    // Perform operation
    for (std::size_t i = 0; i < data.size(); i++) {
        lhs_scalar.set_data(data[i]);
        rhs_scalar.set_data(rhs.data[i]);

        res.data[i] = (lhs_scalar * rhs_scalar).get_data();
    }

    return res;
}

APyFloatArray APyFloatArray::operator*(const APyFloat& rhs) const
{
    // Calculate new format
    const auto res_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    const auto res_man_bits = std::max(man_bits, rhs.get_man_bits());
    const auto res_bias = APyFloat::ieee_bias(res_exp_bits);
    APyFloatArray res(shape, res_exp_bits, res_man_bits, res_bias);

    const int sum_man_bits = man_bits + rhs.get_man_bits();
    const auto quantization = get_quantization_mode();

    if (unsigned(sum_man_bits) + 2 <= _MAN_T_SIZE_BITS) {
        // Compute constants for reuse
        const auto x_max_exponent = ((1ULL << exp_bits) - 1);
        const auto y_max_exponent = ((1ULL << rhs.get_exp_bits()) - 1);
        const auto res_max_exponent = ((1ULL << res_exp_bits) - 1);
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
        const man_t one = 1ULL << sum_man_bits;
        const man_t two = one << 1;
        const auto mask_two = two - 1;
        const std::int64_t bias_sum = bias + rhs.get_bias();
        const std::int64_t exp_offset = (std::int64_t)y.exp + y_is_subnormal - bias_sum;
        const man_t my
            = (static_cast<man_t>(!y_is_subnormal) << rhs.get_man_bits()) | y.man;
        const auto new_man_bits = sum_man_bits + 1;
        const auto new_exp_bits = bit_width(bias_sum + new_man_bits) + 1;
        const auto extended_bias = APyFloat::ieee_bias(new_exp_bits);
        // Use longer format for intermediate result
        APyFloat larger_float(new_exp_bits, new_man_bits, extended_bias);
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

            // In case of denormalized data
            if (new_man < one) {
                int cnt = new_man_bits - bit_width(new_man);
                new_man <<= cnt + 1;
                tmp_exp -= cnt;
            } else {
                // Result may be larger than two
                if (new_man >= two) {
                    tmp_exp++;
                } else {
                    // Align with longer result
                    new_man <<= 1;
                }
            }
            // Remove leading one
            new_man &= mask_two;

            auto new_exp = tmp_exp + extended_bias;
            // Use longer format for intermediate result
            larger_float.set_data({ res_sign, (exp_t)new_exp, new_man });
            res.data[i]
                = larger_float._cast(res_exp_bits, res_man_bits, res_bias, quantization)
                      .get_data();
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
    // Make sure `_shape` of `*this` and `rhs` are the same
    if (shape != rhs.shape) {
        throw std::length_error(fmt::format(
            "APyFloatArray.__truediv__: shape missmatch, lhs.shape={}, "
            "rhs.shape={}",
            string_from_vec(shape),
            string_from_vec(rhs.shape)
        ));
    }

    // Calculate new format
    APyFloatArray res(
        shape, std::max(exp_bits, rhs.exp_bits), std::max(man_bits, rhs.man_bits)
    );

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
    APyFloatArray res(
        shape,
        std::max(exp_bits, rhs.get_exp_bits()),
        std::max(man_bits, rhs.get_man_bits())
    );

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

APyFloatArray APyFloatArray::matmul(const APyFloatArray& rhs) const
{
    if (get_ndim() == 1 && rhs.get_ndim() == 1) {
        if (shape[0] == rhs.shape[0]) {
            // Dimensionality for a standard scalar inner product checks out.
            // Perform the checked inner product.
            return checked_inner_product(rhs);
        }
    }
    if (get_ndim() == 2 && (rhs.get_ndim() == 2 || rhs.get_ndim() == 1)) {
        if (shape[1] == rhs.shape[0]) {
            // Dimensionality for a standard 2D matrix mutliplication checks out.
            // Perform the checked 2D matrix
            return checked_2d_matmul(rhs);
        }
    }

    // Unsupported `__matmul__` dimensionality, raise exception
    throw std::length_error(fmt::format(
        "APyFloatArray.__matmul__: input shape missmatch, lhs: ({}), rhs: ({})",
        string_from_vec(shape),
        string_from_vec(rhs.shape)
    ));
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
    ss << "shape=(";
    ss << string_from_vec(shape);
    ss << "), "
       << "exp_bits=" << static_cast<unsigned>(exp_bits) << ", "
       << "man_bits=" << static_cast<unsigned>(man_bits) << ", "
       << "bias=" << bias << ")";
    return ss.str();
}

// The shape of the array
nanobind::tuple APyFloatArray::get_shape() const
{
    nb::list result_list;
    for (std::size_t i = 0; i < shape.size(); i++) {
        result_list.append(shape[i]);
    }
    return nb::tuple(result_list);
}

size_t APyFloatArray::get_ndim() const { return shape.size(); }

size_t APyFloatArray::get_size() const { return shape[0]; }

std::variant<APyFloatArray, APyFloat> APyFloatArray::get_item(std::size_t idx) const
{
    if (idx >= shape[0]) {
        throw std::out_of_range(fmt::format(
            "APyFloatArray.__getitem__: index {} is out of bounds for axis 0 with "
            "size "
            "{}",
            idx,
            shape[0]
        ));
    }

    if (get_ndim() == 1) {
        // One dimension, return APyFloat
        APyFloat result(data[idx], exp_bits, man_bits, bias);
        return result;
    } else {
        // New shape contains all dimensions except the very first one
        auto new_shape = std::vector<std::size_t>(shape.begin() + 1, shape.end());

        // Element stride is the new shape folded over multiplication
        std::size_t element_stride = std::accumulate(
            new_shape.begin(), new_shape.end(), 1, std::multiplies {}
        );

        APyFloatArray result(new_shape, exp_bits, man_bits, bias);
        std::copy_n(
            data.begin() + idx * element_stride, element_stride, result.data.begin()
        );
        return result;
    }
}

nb::ndarray<nb::numpy, double> APyFloatArray::to_numpy() const
{
    auto n_elements = data.size();
    // Dynamically allocate data to be passed to python
    double* result_data = new double[n_elements];
    auto apy_f = APyFloat(exp_bits, man_bits, bias);
    for (std::size_t i = 0; i < n_elements; i++) {
        apy_f.set_data(data[i]);
        result_data[i] = apy_f.to_double();
    }

    // Delete 'data' when the 'owner' capsule expires
    nb::capsule owner(result_data, [](void* p) noexcept { delete[] (double*)p; });

    std::size_t ndim = shape.size();
    return nb::ndarray<nb::numpy, double>(result_data, ndim, &shape[0], owner);
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
    const nanobind::sequence& double_seq,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    if (bias.has_value() && bias.value() != APyFloat::ieee_bias(exp_bits)) {
        throw NotImplementedException(
            "Not implemented: APyFloatArray with non IEEE-like bias"
        );
    }

    if (nb::isinstance<nb::ndarray<nb::numpy>>(double_seq)) {
        // Sequence is NumPy NDArray. Initialize using
        // `_set_values_from_numpy_ndarray`
        auto ndarray = nb::cast<nb::ndarray<nb::numpy>>(double_seq);
        std::size_t ndim = ndarray.ndim();
        if (ndim == 0) {
            throw nb::type_error(
                "APyFixedArray.from_float(): NDArray with ndim == 0 not supported"
            );
        }
        std::vector<std::size_t> shape(ndim, 0);
        for (std::size_t i = 0; i < ndim; i++) {
            shape[i] = ndarray.shape(i);
        }

        APyFloatArray result(shape, exp_bits, man_bits, bias);
        result._set_values_from_numpy_ndarray(ndarray);
        return result;
    }

    APyFloatArray result(
        python_sequence_extract_shape(double_seq), exp_bits, man_bits, bias
    );

    auto py_obj = python_sequence_walk<nb::float_, nb::int_>(double_seq);

    APyFloat apytypes_double(11, 52, 1023);
    auto actual_bias = bias.value_or(APyFloat::ieee_bias(exp_bits));
    for (std::size_t i = 0; i < result.data.size(); i++) {
        double d;
        if (nb::isinstance<nb::float_>(py_obj[i])) {
            d = static_cast<double>(nb::cast<nb::float_>(py_obj[i]));
        } else if (nb::isinstance<nb::int_>(py_obj[i])) {
            d = static_cast<int>(nb::cast<nb::int_>(py_obj[i]));
        } else {
            throw std::domain_error("Invalid Python objects in sequence");
        }
        apytypes_double.set_data(
            { sign_of_double(d), exp_t(exp_of_double(d)), man_of_double(d) }
        );
        APyFloat fp = apytypes_double.cast_from_double(exp_bits, man_bits, actual_bias);
        result.data[i] = { fp.get_sign(), fp.get_exp(), fp.get_man() };
    }
    return result;
}

void APyFloatArray::_set_values_from_numpy_ndarray(const nb::ndarray<nb::numpy>& ndarray
)
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
    // `CHECK_AND_SET_VALUES_FROM_NPTYPES` returns. Otherwise, the next attemted
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
        "APyFixedArray::_set_values_from_numpy_ndarray(): "
        "unsupported `dtype` expecting integer/float"
    );
}

APyFloatArray APyFloatArray::transpose() const
{
    if (get_ndim() > 2) {
        throw NotImplementedException(fmt::format(
            "Not implemented: high-dimensional (ndim={} > 2) tensor transposition",
            get_ndim()
        ));
    } else if (get_ndim() <= 1) {
        // Behave like `NumPy`, simply return `*this` if single-dimensional
        return *this;
    }

    // Resulting array with shape dimensions
    APyFloatArray result(shape, exp_bits, man_bits, bias);
    std::reverse(result.shape.begin(), result.shape.end());

    // Copy data
    for (std::size_t y = 0; y < shape[0]; y++) {
        for (std::size_t x = 0; x < shape[1]; x++) {
            result.data[x * shape[0] + y] = data[y * shape[1] + x];
        }
    }

    return result;
}

APyFloatArray APyFloatArray::cast(
    std::uint8_t new_exp_bits,
    std::uint8_t new_man_bits,
    std::optional<exp_t> new_bias,
    std::optional<QuantizationMode> quantization
) const
{
    return _cast(
        new_exp_bits,
        new_man_bits,
        new_bias.value_or(APyFloat::ieee_bias(new_exp_bits)),
        quantization.value_or(get_quantization_mode())
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
        quantization.value_or(get_quantization_mode())
    );
}

APyFloatArray APyFloatArray::_cast(
    std::uint8_t new_exp_bits,
    std::uint8_t new_man_bits,
    exp_t new_bias,
    QuantizationMode quantization
) const
{
    if ((new_exp_bits == exp_bits) && (new_man_bits == man_bits)
        && (new_bias == bias)) {
        return *this;
    }

    APyFloatArray result(shape, new_exp_bits, new_man_bits, new_bias);

    for (std::size_t i = 0; i < data.size(); i++) {
        result.data[i] = APyFloat(data[i], exp_bits, man_bits, bias)
                             ._cast(new_exp_bits, new_man_bits, new_bias, quantization)
                             .get_data();
    }

    return result;
}

APyFloatArray APyFloatArray::cast_no_quant(
    std::uint8_t new_exp_bits, std::uint8_t new_man_bits, std::optional<exp_t> new_bias
) const
{
    if ((new_exp_bits == exp_bits) && (new_man_bits == man_bits)
        && (new_bias == bias)) {
        return *this;
    }

    APyFloatArray result(shape, new_exp_bits, new_man_bits, new_bias);

    for (std::size_t i = 0; i < data.size(); i++) {
        result.data[i] = APyFloat(data[i], exp_bits, man_bits, bias)
                             .cast_no_quant(new_exp_bits, new_man_bits, new_bias)
                             .get_data();
    }

    return result;
}

APyFloatArray APyFloatArray::resize(
    std::uint8_t new_exp_bits,
    std::uint8_t new_man_bits,
    std::optional<exp_t> new_bias,
    std::optional<QuantizationMode> quantization
) const
{
    PyErr_WarnEx(
        PyExc_DeprecationWarning, "resize() is deprecated, use cast() instead.", 1
    );
    APyFloatArray result(shape, new_exp_bits, new_man_bits, new_bias);

    for (std::size_t i = 0; i < data.size(); i++) {
        result.data[i] = APyFloat(data[i], exp_bits, man_bits, bias)
                             .cast(new_exp_bits, new_man_bits, new_bias, quantization)
                             .get_data();
    }

    return result;
}

std::size_t APyFloatArray::fold_shape() const
{
    // Fold the shape over multiplication
    return std::accumulate(shape.begin(), shape.end(), 1, std::multiplies {});
}

// Evaluate the inner between two vectors. This method assumes that the the shape of
// both `*this` and `rhs` are equally long. Anything else is undefined behaviour.
APyFloatArray APyFloatArray::checked_inner_product(const APyFloatArray& rhs) const
{
    const std::uint8_t max_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t max_man_bits = std::max(man_bits, rhs.man_bits);

    if (shape[0] == 0) {
        return APyFloatArray({ 0 }, max_exp_bits, max_man_bits);
    }

    auto tmp_exp_bits = max_exp_bits;
    auto tmp_man_bits = max_man_bits;

    // Hadamard product of `*this` and `rhs`
    APyFloatArray hadamard;

    // If an accumulator is used, the operands must be resized before the
    // multiplication. This is because the products would otherwise get quantized
    // too early.

    const auto orig_quant_mode = get_quantization_mode();

    if (get_accumulator_mode().has_value()) {
        const auto acc_option = get_accumulator_mode().value();
        tmp_exp_bits = acc_option.exp_bits;
        tmp_man_bits = acc_option.man_bits;
        set_quantization_mode(acc_option.quantization);
        hadamard = this->cast(tmp_exp_bits, tmp_man_bits)
            * rhs.cast(tmp_exp_bits, tmp_man_bits);
    } else {
        hadamard = *this * rhs;
    }

    APyFloat sum = APyFloat(0, 0, 0, tmp_exp_bits, max_man_bits);
    for (std::size_t i = 0; i < hadamard.data.size(); i++) {
        const APyFloatData scalar = hadamard.data.at(i);
        sum = sum + APyFloat(scalar, tmp_exp_bits, tmp_man_bits);
    }

    APyFloatArray result({ 1 }, max_exp_bits, max_man_bits);

    // The result must be quantized back if an accumulator was used.
    if (get_accumulator_mode().has_value()) {
        result.data[0] = sum.cast(max_exp_bits, max_man_bits).get_data();
    } else {
        result.data[0] = sum.get_data();
    }

    // Change the quantization mode back, even if it wasn't changed
    set_quantization_mode(orig_quant_mode);
    return result;
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
    const auto res_cols = rhs.shape.size() > 1 ? rhs.shape[1] : 1;

    // Resulting `APyFloatArray`
    APyFloatArray result(res_shape, max_exp_bits, max_man_bits);

    for (std::size_t x = 0; x < res_cols; x++) {

        // Copy column from `rhs` and use as the current working column. As reading
        // columns from `rhs` is cache-inefficient, we like to do this only once for
        // each element in the resulting matrix.
        APyFloatArray current_column({ rhs.shape[0] }, rhs.exp_bits, rhs.man_bits);
        for (std::size_t col = 0; col < rhs.shape[0]; col++) {
            current_column.data[col] = rhs.data[x + col * res_cols];
        }

        for (std::size_t y = 0; y < res_shape[0]; y++) {

            // Current row from lhs (`*this`)
            APyFloatArray current_row({ shape[1] }, exp_bits, man_bits);
            for (std::size_t i = 0; i < shape[1]; i++) {
                current_row.data[i] = data[i + y * shape[1]];
            }

            // Perform the inner product
            APyFloatArray current_res
                = current_column.checked_inner_product(current_row);
            assert(current_res.shape == std::vector<std::size_t> { 1 });
            assert(current_res.exp_bits == result.exp_bits);
            assert(current_res.man_bits == result.man_bits);

            // Copy into the resulting vector
            // result.data.insert(result.data.begin(), current_res.data.begin(),
            // current_res.data.end());
            std::copy_n(
                current_res.data.begin(), // src
                current_res.data.size(),  // limbs to copy
                result.data.begin() + x + y * res_cols
            );
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
