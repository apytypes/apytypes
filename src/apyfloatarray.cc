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
            "APyFloatArray.__add__: shape mismatch, lhs.shape={}, rhs.shape={}",
            string_from_vec(shape),
            string_from_vec(rhs.shape)
        ));
    }

    const auto quantization = get_float_quantization_mode();
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
            bool x_is_zero_exponent = (x.exp == 0);
            // Handle zero cases
            if (x_is_zero_exponent && x.man == 0) {
                res.data[i] = y;
                continue;
            }
            bool y_is_zero_exponent = (y.exp == 0);
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
                res_sign = y.sign;
                std::swap(x, y);
                std::swap(x_is_zero_exponent, y_is_zero_exponent);
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
            std::int64_t new_exp = x.exp + x_is_zero_exponent;

            // Conditionally add leading one's, also add room for guard bits
            // Note that exp can never be res_max_exponent here
            man_t mx = (x_is_zero_exponent ? 0 : res_leading_one) | (x.man << 3);
            man_t my = (y_is_zero_exponent ? 0 : res_leading_one) | (y.man << 3);

            // Align mantissas based on exponent difference
            const unsigned exp_delta = new_exp - y.exp - y_is_zero_exponent;

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
                    new_man <<= new_exp;
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
    const auto quantization = get_float_quantization_mode();
    // +5 to give room for leading one, carry, and 3 guard bits
    const unsigned int max_man_bits = man_bits + 5;
    const bool same_types = same_type_as(rhs);
    if (same_types && (max_man_bits <= _MAN_T_SIZE_BITS)
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

            bool x_is_zero_exponent = (x.exp == 0);
            bool y_is_zero_exponent = (y.exp == 0);
            // Tentative exponent
            std::int64_t new_exp = x.exp + x_is_zero_exponent;

            // Conditionally add leading one's, also add room for guard bits
            // Note that exp can never be res_max_exponent here
            man_t mx = (x_is_zero_exponent ? 0 : res_leading_one) | (x.man << 3);
            man_t my = (y_is_zero_exponent ? 0 : res_leading_one) | (y.man << 3);

            // Align mantissas based on exponent difference
            const unsigned exp_delta = new_exp - y.exp - y_is_zero_exponent;

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
                    new_man <<= new_exp;
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
        if (same_types) {
            return *this;
        }
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
            "APyFloatArray.__sub__: shape mismatch, lhs.shape={}, rhs.shape={}",
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
            "APyFloatArray.__mul__: shape mismatch, lhs.shape={}, rhs.shape={}",
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
    const auto quantization = get_float_quantization_mode();

    if (unsigned(sum_man_bits) + 3 <= _MAN_T_SIZE_BITS) {
        // Compute constants for reuse
        const auto x_max_exponent = ((1ULL << exp_bits) - 1);
        const auto y_max_exponent = ((1ULL << rhs.exp_bits) - 1);
        const exp_t res_max_exponent = ((1ULL << res_exp_bits) - 1);
        const auto new_man_bits = sum_man_bits + 2;
        const man_t two = 1ULL << new_man_bits;
        const man_t two_before = two >> 1;
        const man_t one_before = 1ULL << sum_man_bits;
        const man_t two_res = 1 << res_man_bits;
        const auto mask_two = two - 1;
        const auto man_bits_delta = new_man_bits - res_man_bits;
        const std::int64_t bias_sum = bias + rhs.bias - res_bias;
        exp_t new_exp;

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
                // Shift and add sticky bit
                new_man = (new_man >> (-tmp_exp + 1))
                    | ((new_man & ((1 << (-tmp_exp + 1)) - 1)) != 0);
                tmp_exp = 0;
            }

            new_man = quantize_mantissa(
                new_man & mask_two, man_bits_delta, res_sign, quantization
            );

            // Carry in quantization
            if (new_man >= two_res) {
                ++tmp_exp;
                new_man = 0;
            }

            // Check for overflow
            if (tmp_exp >= res_max_exponent) {
                new_exp = res_max_exponent;
                new_man = 0;
            } else {
                new_exp = tmp_exp;
            }
            res.data[i] = { res_sign,
                            static_cast<exp_t>(new_exp),
                            static_cast<man_t>(new_man) };
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
    const auto quantization = get_float_quantization_mode();

    if (unsigned(sum_man_bits) + 3 <= _MAN_T_SIZE_BITS) {
        // Compute constants for reuse
        const auto x_max_exponent = ((1ULL << exp_bits) - 1);
        const auto y_max_exponent = ((1ULL << rhs.get_exp_bits()) - 1);
        const exp_t res_max_exponent = ((1ULL << res_exp_bits) - 1);
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
                // Shift and add sticky bit
                new_man = (new_man >> (-tmp_exp + 1))
                    | ((new_man & ((1 << (-tmp_exp + 1)) - 1)) != 0);
                tmp_exp = 0;
            }

            new_man = quantize_mantissa(
                new_man & mask_two, man_bits_delta, res_sign, quantization
            );

            // Carry in quantization
            if (new_man >= two_res) {
                ++tmp_exp;
                new_man = 0;
            }

            // Check for overflow
            if (tmp_exp >= res_max_exponent) {
                new_exp = res_max_exponent;
                new_man = 0;
            } else {
                new_exp = tmp_exp;
            }
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
    // Make sure `_shape` of `*this` and `rhs` are the same
    if (shape != rhs.shape) {
        throw std::length_error(fmt::format(
            "APyFloatArray.__truediv__: shape mismatch, lhs.shape={}, "
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
        "APyFloatArray.__matmul__: input shape mismatch, lhs: ({}), rhs: ({})",
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
    ss << "), " << "exp_bits=" << static_cast<unsigned>(exp_bits) << ", "
       << "man_bits=" << static_cast<unsigned>(man_bits) << ", " << "bias=" << bias
       << ")";
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
    // Dynamically allocate data to be passed to python
    double* result_data = new double[data.size()];
    auto apy_f = APyFloat(exp_bits, man_bits, bias);
    for (std::size_t i = 0; i < data.size(); i++) {
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
                "APyFloatArray.from_float(): NDArray with ndim == 0 not supported"
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
        "APyFloatArray::_set_values_from_numpy_ndarray(): "
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
    std::optional<uint8_t> new_exp_bits,
    std::optional<uint8_t> new_man_bits,
    std::optional<exp_t> new_bias,
    std::optional<QuantizationMode> quantization
) const
{
    auto actual_exp_bits = new_exp_bits.value_or(exp_bits);
    return _cast(
        actual_exp_bits,
        new_man_bits.value_or(man_bits),
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
    if ((new_exp_bits == exp_bits) && (new_man_bits == man_bits)
        && (new_bias == bias)) {
        return *this;
    }

    APyFloatArray result(shape, new_exp_bits, new_man_bits, new_bias);

    APyFloat caster(exp_bits, man_bits, bias);
    for (std::size_t i = 0; i < data.size(); i++) {
        caster.set_data(data[i]);
        result.data[i]
            = caster._cast(new_exp_bits, new_man_bits, new_bias, quantization)
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

std::size_t APyFloatArray::fold_shape() const
{
    // Fold the shape over multiplication
    return std::accumulate(shape.begin(), shape.end(), 1, std::multiplies {});
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
        const auto orig_quant_mode = get_float_quantization_mode();
        const auto acc_option = accumulator_mode.value();
        const auto tmp_exp_bits = acc_option.exp_bits;
        const auto tmp_man_bits = acc_option.man_bits;
        const auto tmp_bias = APyFloat::ieee_bias(tmp_exp_bits);
        set_float_quantization_mode(acc_option.quantization);
        // If an accumulator is used, the operands must be resized before the
        // multiplication. This is because the products would otherwise get quantized
        // too early.

        // Hadamard product of `*this` and `rhs`
        APyFloatArray hadamard
            = this->_cast(tmp_exp_bits, tmp_man_bits, tmp_bias, acc_option.quantization)
            * rhs._cast(tmp_exp_bits, tmp_man_bits, tmp_bias, acc_option.quantization);
        APyFloat sum = hadamard.vector_sum(acc_option.quantization);
        // The result must be quantized back if an accumulator was used.
        sum = sum.cast(max_exp_bits, max_man_bits);
        // Change the quantization mode back, even if it wasn't changed
        set_float_quantization_mode(orig_quant_mode);
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
    APyFloat ret(0, 0, 0, exp_bits, man_bits);
    if ((max_man_bits <= _MAN_T_SIZE_BITS)
        && (quantization != QuantizationMode::STOCH_WEIGHTED)) {
        APyFloatData x, y;
        APyFloatData sum = { false, static_cast<exp_t>(0), static_cast<man_t>(0) };
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
            y = sum;
            bool x_is_zero_exponent = (x.exp == 0);
            // Handle zero cases
            if (x_is_zero_exponent && x.man == 0) {
                continue;
            }
            bool y_is_zero_exponent = (y.exp == 0);
            if (y_is_zero_exponent && y.man == 0) {
                sum = x;
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
                    ret.set_data({ false,
                                   static_cast<exp_t>(res_max_exponent),
                                   static_cast<man_t>(1) });
                    return ret;
                }

                // Handle inf cases
                if (x_is_max_exponent && x.man == 0) {
                    // Set to inf
                    // No early return as later values may be NaN
                    sum = { x.sign,
                            static_cast<exp_t>(res_max_exponent),
                            static_cast<man_t>(0) };
                    continue;
                }

                if (y_is_max_exponent && y.man == 0) {
                    // Keep at inf
                    // No early return as later values may be NaN
                    continue;
                }
            }
            // Compute sign and swap operands if need to make sure |x| >= |y|
            if (x.exp < y.exp || (x.exp == y.exp && x.man < y.man)) {
                res_sign = y.sign;
                std::swap(x, y);
                std::swap(x_is_zero_exponent, y_is_zero_exponent);
            } else {
                if (x.sign != y.sign && x.exp == y.exp && x.man == y.man) {
                    // Set to zero
                    sum = { false, static_cast<exp_t>(0), static_cast<man_t>(0) };
                    continue;
                }
                res_sign = x.sign;
            }

            // Tentative exponent
            std::int64_t new_exp = x.exp + x_is_zero_exponent;

            // Conditionally add leading one's, also add room for guard bits
            // Note that exp can never be res_max_exponent here
            man_t mx = (x_is_zero_exponent ? 0 : res_leading_one) | (x.man << 3);
            man_t my = (y_is_zero_exponent ? 0 : res_leading_one) | (y.man << 3);

            // Align mantissas based on exponent difference
            const unsigned exp_delta = new_exp - y.exp - y_is_zero_exponent;

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
                    new_man <<= new_exp;
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
                sum = { x.sign,
                        static_cast<exp_t>(res_max_exponent),
                        static_cast<man_t>(0) };
            } else {
                sum = { res_sign,
                        static_cast<exp_t>(new_exp),
                        static_cast<man_t>(res_man) };
            }
        }
        ret.set_data(sum);
        return ret;
    }
    APyFloat tmp(0, 0, 0, exp_bits, man_bits);

    for (std::size_t i = 0; i < data.size(); i++) {
        tmp.set_data(data[i]);
        ret = ret + tmp;
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
    const auto res_bias = APyFloat::ieee_bias(max_exp_bits);
    const auto res_cols = rhs.shape.size() > 1 ? rhs.shape[1] : 1;

    auto accumulator_mode = get_accumulator_mode_float();

    // Resulting `APyFloatArray`
    APyFloatArray result(res_shape, max_exp_bits, max_man_bits, res_bias);

    // Current row from lhs (`*this`)
    APyFloatArray current_row({ shape[1] }, exp_bits, man_bits, bias);

    // Current column from rhs
    APyFloatArray current_column(
        { rhs.shape[0] }, rhs.exp_bits, rhs.man_bits, rhs.bias
    );

    const auto orig_quant_mode = get_float_quantization_mode();

    // Accumulator mode set
    if (accumulator_mode.has_value()) {
        const auto acc_option = accumulator_mode.value();
        const auto tmp_exp_bits = acc_option.exp_bits;
        const auto tmp_man_bits = acc_option.man_bits;
        const auto tmp_bias = APyFloat::ieee_bias(tmp_exp_bits);
        set_float_quantization_mode(acc_option.quantization);

        for (std::size_t x = 0; x < res_cols; x++) {

            // Copy column from `rhs` and use as the current working column. As reading
            // columns from `rhs` is cache-inefficient, we like to do this only once for
            // each element in the resulting matrix.
            for (std::size_t col = 0; col < rhs.shape[0]; col++) {
                current_column.data[col] = rhs.data[x + col * res_cols];
            }

            APyFloatArray casted_current_column = current_column._cast(
                tmp_exp_bits, tmp_man_bits, tmp_bias, acc_option.quantization
            );
            for (std::size_t y = 0; y < res_shape[0]; y++) {

                // Copy row from lhs (*this)
                std::copy_n(&data[y * shape[1]], shape[1], current_row.data.begin());
                // If an accumulator is used, the operands must be resized before the
                // multiplication. This is because the products would otherwise get
                // quantized too early.

                // Hadamard product of `*this` and `rhs`
                APyFloatArray hadamard = casted_current_column
                    * current_row._cast(
                        tmp_exp_bits, tmp_man_bits, tmp_bias, acc_option.quantization
                    );
                APyFloat sum = hadamard.vector_sum(acc_option.quantization);
                // The result must be quantized back if an accumulator was used.
                sum = sum.cast(
                    max_exp_bits, max_man_bits, res_bias, acc_option.quantization
                );

                assert(sum.get_exp_bits() == result.get_exp_bits());
                assert(sum.get_man_bits() == result.get_man_bits());

                // Copy into the resulting vector
                result.data[x + y * res_cols] = sum.get_data();
            }
        }
        // Change the quantization mode back, even if it wasn't changed
        set_float_quantization_mode(orig_quant_mode);
        return result;
    }

    // No accumulator mode

    for (std::size_t x = 0; x < res_cols; x++) {

        // Copy column from `rhs` and use as the current working column. As reading
        // columns from `rhs` is cache-inefficient, we like to do this only once for
        // each element in the resulting matrix.
        for (std::size_t col = 0; col < rhs.shape[0]; col++) {
            current_column.data[col] = rhs.data[x + col * res_cols];
        }

        for (std::size_t y = 0; y < res_shape[0]; y++) {

            // Copy row from lhs (*this)
            std::copy_n(&data[y * shape[1]], shape[1], current_row.data.begin());

            // Perform the inner product
            // Hadamard product of `current_column` and `current_row`
            APyFloatArray hadamard = current_column * current_row;
            APyFloat sum = hadamard.vector_sum(orig_quant_mode);
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
