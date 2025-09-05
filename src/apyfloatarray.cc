#include "apyfloatarray.h"
#include "apyarray.h"
#include "apyfloat.h"
#include "apyfloat_util.h"
#include "apytypes_common.h"
#include "apytypes_util.h"
#include "array_utils.h"
#include "ieee754.h"
#include "python_util.h"
#include "src/apytypes_intrinsics.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
namespace nb = nanobind;

// Standard header includes
#include <algorithm>
#include <cstddef>
#include <fmt/format.h>
#include <functional>
#include <set>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <variant>

#include <iostream>

void APyFloatArray::create_in_place(
    APyFloatArray* apyfloatarray,
    const nb::typed<nb::iterable, nb::any>& sign_seq,
    const nb::typed<nb::iterable, nb::any>& exp_seq,
    const nb::typed<nb::iterable, nb::any>& man_seq,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits, "APyFloatArray.__init__");
    check_mantissa_format(man_bits, "APyFloatArray.__init__");

    new (apyfloatarray)
        APyFloatArray(sign_seq, exp_seq, man_seq, exp_bits, man_bits, bias);
}

APyFloatArray::APyFloatArray(
    const nb::iterable& sign_seq,
    const nb::iterable& exp_seq,
    const nb::iterable& man_seq,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
    : APyArray(python_iterable_extract_shape(sign_seq, "APyFloatArray.__init__"))
    , exp_bits(exp_bits)
    , man_bits(man_bits)
{
    constexpr std::string_view caller_name = "APyFloatArray.__init__";

    const auto& signs_shape = _shape;
    const auto exps_shape = python_iterable_extract_shape(exp_seq, caller_name);
    const auto mans_shape = python_iterable_extract_shape(man_seq, caller_name);
    if (!((signs_shape == exps_shape) && (signs_shape == mans_shape))) {
        throw std::domain_error(
            fmt::format(
                "APyFloatArray.__init__: shape mismatch, sign: {}, exp: {}, man: {}",
                tuple_string_from_vec(signs_shape),
                tuple_string_from_vec(exps_shape),
                tuple_string_from_vec(mans_shape)
            )
        );
    }

    auto signs = python_iterable_walk<nb::int_, nb::bool_>(sign_seq, caller_name);
    auto exps = python_iterable_walk<nb::int_>(exp_seq, caller_name);
    auto mans = python_iterable_walk<nb::int_>(man_seq, caller_name);

    this->bias = bias.value_or(APyFloat::ieee_bias(exp_bits));
    for (std::size_t i = 0; i < signs.size(); ++i) {
        bool sign;
        if (nb::isinstance<nb::bool_>(signs[i])) {
            sign = static_cast<bool>(nb::cast<nb::bool_>(signs[i]));
        } else if (nb::isinstance<nb::int_>(signs[i])) {
            sign = static_cast<int>(nb::cast<nb::int_>(signs[i]));
        } else {
            APYTYPES_UNREACHABLE();
        }
        exp_t exp = static_cast<exp_t>(nb::cast<nb::int_>(exps[i]));
        man_t man = static_cast<man_t>(nb::cast<nb::int_>(mans[i]));

        _data[i] = { sign, exp, man };
    }
}

APyFloatArray::APyFloatArray(
    const std::vector<std::size_t>& shape,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
    : APyArray(shape)
    , exp_bits(exp_bits)
    , man_bits(man_bits)
    , bias(bias.value_or(APyFloat::ieee_bias(exp_bits)))
{
}

/* ********************************************************************************** *
 * *                            Binary arithmetic operators                         * *
 * ********************************************************************************** */

APyFloatArray APyFloatArray::operator+(const APyFloatArray& rhs) const
{
    if (_shape != rhs._shape) {
        return try_broadcast_and_then<std::plus<>>(rhs, "__add__");
    }

    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform addition
    auto add = FloatingPointAdder<>(spec(), rhs.spec(), res.spec(), qntz);
    add(&_data[0], &rhs._data[0], &res._data[0], res._nitems);

    return res;
}

APyFloatArray APyFloatArray::operator+(const APyFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform addition
    const APyFloatData& rhs_data = rhs.get_data();
    auto add = FloatingPointAdder<1, 0, 1>(spec(), rhs.spec(), res.spec(), qntz);
    add(&_data[0], &rhs_data, &res._data[0], res._nitems);

    return res;
}

APyFloatArray APyFloatArray::operator-(const APyFloatArray& rhs) const
{
    if (_shape != rhs._shape) {
        return try_broadcast_and_then<std::minus<>>(rhs, "__sub__");
    }

    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform subtraction
    auto sub = FloatingPointSubtractor<>(spec(), rhs.spec(), res.spec(), qntz);
    sub(&_data[0], &rhs._data[0], &res._data[0], res._nitems);

    return res;
}

APyFloatArray APyFloatArray::operator-(const APyFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform the subtraction
    const APyFloatData& rhs_data = rhs.get_data();
    auto sub = FloatingPointSubtractor<1, 0, 1>(spec(), rhs.spec(), res.spec(), qntz);
    sub(&_data[0], &rhs_data, &res._data[0], res._nitems);

    return res;
}

APyFloatArray APyFloatArray::operator-() const
{
    auto res = *this;
    for (std::size_t i = 0; i < res._data.size(); i++) {
        res._data[i].sign = !res._data[i].sign;
    }
    return res;
}

APyFloatArray APyFloatArray::abs() const
{
    auto res = *this;
    for (std::size_t i = 0; i < res._data.size(); i++) {
        res._data[i].sign = false;
    }
    return res;
}

APyFloatArray APyFloatArray::operator*(const APyFloatArray& rhs) const
{
    if (_shape != rhs._shape) {
        return try_broadcast_and_then<std::multiplies<>>(rhs, "__mul__");
    }

    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform multiplication
    auto mul = FloatingPointMultiplier<>(spec(), rhs.spec(), res.spec(), qntz);
    mul(&_data[0], &rhs._data[0], &res._data[0], _nitems);

    return res;
}

APyFloatArray APyFloatArray::operator*(const APyFloat& rhs) const
{
    // Calculate new format
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform multiplication
    const APyFloatData& rhs_data = rhs.get_data();
    auto mul = FloatingPointMultiplier<1, 0, 1>(spec(), rhs.spec(), res.spec(), qntz);
    mul(&_data[0], &rhs_data, &res._data[0], _nitems);

    return res;
}

APyFloatArray APyFloatArray::operator/(const APyFloatArray& rhs) const
{
    if (_shape != rhs._shape) {
        return try_broadcast_and_then<std::divides<>>(rhs, "__truediv__");
    }

    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform division
    FloatingPointDivider div(spec(), rhs.spec(), res.spec(), qntz);
    div(&_data[0], &rhs._data[0], &res._data[0], _nitems);

    return res;
}

APyFloatArray APyFloatArray::operator/(const APyFloat& rhs) const
{
    // Calculate new format
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform division
    const APyFloatData& rhs_data = rhs.get_data();
    FloatingPointDivider<1, 0, 1> div(spec(), rhs.spec(), res.spec(), qntz);
    div(&_data[0], &rhs_data, &res._data[0], _nitems);

    return res;
}

APyFloatArray APyFloatArray::rdiv(const APyFloat& lhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, lhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, lhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), lhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform division
    const APyFloatData& lhs_data = lhs.get_data();
    FloatingPointDivider<0, 1, 1> div(lhs.spec(), spec(), res.spec(), qntz);
    div(&lhs_data, &_data[0], &res._data[0], _nitems);

    return res;
}

APyFloatArray APyFloatArray::rsub(const APyFloat& lhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, lhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, lhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), lhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    // Perform subtraction
    const APyFloatData& lhs_data = lhs.get_data();
    FloatingPointSubtractor<0, 1, 1> sub(lhs.spec(), spec(), res.spec(), qntz);
    sub(&lhs_data, &_data[0], &res._data[0], _nitems);

    return res;
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::matmul(const APyFloatArray& rhs) const
{
    if (ndim() == 1 && rhs.ndim() == 1) {
        if (_shape[0] == rhs._shape[0]) {
            // Dimensionality for a standard scalar inner product checks out.
            // Perform the checked inner product.
            return checked_inner_product(rhs);
        }
    }
    if (ndim() == 2 && (rhs.ndim() == 2 || rhs.ndim() == 1)) {
        if (_shape[1] == rhs._shape[0]) {
            // Dimensionality for a standard 2D matrix multiplication checks out.
            // Perform the checked 2D matrix
            return checked_2d_matmul(rhs);
        }
    }

    // Unsupported `__matmul__` dimensionality, raise exception
    throw std::length_error(
        fmt::format(
            "APyFloatArray.__matmul__: input shape mismatch, lhs: {}, rhs: {}",
            tuple_string_from_vec(_shape),
            tuple_string_from_vec(rhs._shape)
        )
    );
}

APyFloatArray APyFloatArray::operator~() const
{
    auto res = *this;
    auto exp_mask = ((1ULL << exp_bits) - 1);
    auto man_mask = ((1ULL << man_bits) - 1);
    for (std::size_t i = 0; i < res._data.size(); i++) {
        res._data[i].sign = !res._data[i].sign;
        res._data[i].exp = (~res._data[i].exp) & exp_mask;
        res._data[i].man = (~res._data[i].man) & man_mask;
    }
    return res;
}

/* ****************************************************************************** *
 *                     Static methods for array initialization                    *
 * ****************************************************************************** */

APyFloatArray APyFloatArray::zeros(
    const PyShapeParam_t& shape,
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
    const PyShapeParam_t& shape,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    return full(shape, APyFloat::one(exp_bits, man_bits, bias));
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
    PyShapeTuple_t shape = PyShapeTuple_t(nb::make_tuple(N, M.value_or(N)));
    return diagonal(shape, APyFloat::one(exp_bits, man_bits, bias));
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

APyFloatArray APyFloatArray::arange(
    const nb::object& start,
    const nb::object& stop,
    const nb::object& step,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits, "APyFloatArray.arange");
    check_mantissa_format(man_bits, "APyFloatArray.arange");

    const std::vector<APyFixed> apy_vals = ::arange(start, stop, step);
    APyFloatArray result({ apy_vals.size() }, exp_bits, man_bits, bias);

    for (std::size_t i = 0; i < apy_vals.size(); i++) {
        result._data[i]
            = APyFloat::from_fixed(apy_vals[i], exp_bits, man_bits, bias).get_data();
    }

    return result;
}

APyFloatArray APyFloatArray::fullrange(
    const nb::object& start,
    const nb::object& stop,
    std::uint8_t exp_bits,
    std::uint8_t man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits, "APyFloatArray.fullrange");
    check_mantissa_format(man_bits, "APyFloatArray.fullrange");

    const APyFloat apy_start = APyFloat::from_number(start, exp_bits, man_bits, bias);
    const APyFloat apy_stop = APyFloat::from_number(stop, exp_bits, man_bits, bias);

    if (apy_start.is_nan() || apy_stop.is_nan()) {
        throw nanobind::value_error(
            "APyFloatArray.fullrange: start or stop is NaN, cannot compute range"
        );
    }

    if ((apy_start > apy_stop)) {
        return APyFloatArray({ 0 }, exp_bits, man_bits, bias);
    }

    // Calculate size of resulting array
    std::size_t num_elements;
    const man_t max_man = apy_start.man_mask();
    const man_t leading_one = apy_start.leading_one();
    const std::int64_t exp_diff = static_cast<std::int64_t>(apy_stop.get_exp())
        - static_cast<std::int64_t>(apy_start.get_exp());

    man_t tmp_man = apy_start.get_man();
    exp_t tmp_exp = apy_start.get_exp();

    if (apy_start.get_sign() && apy_stop.get_sign()) {
        // Both numbers are negative
        num_elements
            = -exp_diff * (max_man + 1) + apy_start.get_man() - apy_stop.get_man();
        APyFloatArray result({ num_elements }, exp_bits, man_bits, bias);

        result._data[0] = { true, tmp_exp, tmp_man };
        for (std::size_t i = 1; i < num_elements; i++) {
            --tmp_man;
            if (tmp_man >= leading_one) {
                --tmp_exp;
                tmp_man = max_man;
            }
            result._data[i] = { true, tmp_exp, tmp_man };
        }
        return result;
    } else if (apy_start.get_sign() && !apy_stop.get_sign()) {
        // The start is negative, the stop is positive
        const std::size_t neg_numbers
            = 1 + apy_start.get_exp() * (max_man + 1) + apy_start.get_man();
        num_elements
            = neg_numbers + apy_stop.get_exp() * (max_man + 1) + apy_stop.get_man();

        APyFloatArray result({ num_elements }, exp_bits, man_bits, bias);

        result._data[0] = { true, tmp_exp, tmp_man };
        for (std::size_t i = 1; i < neg_numbers; i++) {
            --tmp_man;
            if (tmp_man >= leading_one) {
                --tmp_exp;
                tmp_man = max_man;
            }
            result._data[i] = { true, tmp_exp, tmp_man };
        }
        if (neg_numbers < num_elements) {
            result._data[neg_numbers] = { false, 0, 0 };
            for (std::size_t i = neg_numbers + 1; i < num_elements; i++) {
                ++tmp_man;
                if (tmp_man >= leading_one) {
                    ++tmp_exp;
                    tmp_man = 0;
                }
                result._data[i] = { false, tmp_exp, tmp_man };
            }
        }
        return result;
    } else { // Both numbers are positive
        num_elements
            = exp_diff * (max_man + 1) - apy_start.get_man() + apy_stop.get_man();
        APyFloatArray result({ num_elements }, exp_bits, man_bits, bias);

        result._data[0] = { 0, tmp_exp, tmp_man };
        for (std::size_t i = 1; i < num_elements; i++) {
            ++tmp_man;
            if (tmp_man >= leading_one) {
                ++tmp_exp;
                tmp_man = 0;
            }
            result._data[i] = { false, tmp_exp, tmp_man };
        }
        return result;
    }
}

//! Perform a linear convolution with `other` using `mode`
APyFloatArray
APyFloatArray::convolve(const APyFloatArray& rhs, const std::string& conv_mode) const
{
    if (ndim() != 1 || rhs.ndim() != 1) {
        auto msg = fmt::format(
            "can only convolve 1D arrays (lhs.ndim = {}, rhs.ndim = {})",
            ndim(),
            rhs.ndim()
        );
        throw nanobind::value_error(msg.c_str());
    }

    // Find the shorter array of `*this` and `other` based on length.
    bool swap = _shape[0] < rhs._shape[0];

    // Make a reverse copy of the shorter array
    APyFloatArray b_cpy = swap ? *this : rhs;
    std::reverse(std::begin(b_cpy._data), std::end(b_cpy._data));

    // Let `a` be a pointer to the longer array, and let `b` be a pointer to the reverse
    // copy of the shorter array.
    const APyFloatArray* a = swap ? &rhs : this;
    const APyFloatArray* b = &b_cpy;

    // Extract convolution properties
    auto [len, n_left, n_right] = get_conv_lengths(conv_mode, a, b);

    // Create result vector
    const std::optional<APyFloatAccumulatorOption> mode = get_accumulator_mode_float();
    const QuantizationMode& qntz
        = mode.has_value() ? mode->quantization : get_float_quantization_mode();
    std::uint8_t res_exp_bits;
    std::uint8_t res_man_bits;
    exp_t res_bias;
    if (mode.has_value()) {
        res_exp_bits = mode->exp_bits;
        res_man_bits = mode->man_bits;
        res_bias = mode->bias.has_value() ? *mode->bias : ieee_bias(res_exp_bits);
    } else {
        res_exp_bits = std::max(exp_bits, rhs.exp_bits);
        res_man_bits = std::max(man_bits, rhs.man_bits);
        res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    }
    APyFloatArray result({ len }, res_exp_bits, res_man_bits, res_bias);

    // Loop working variables
    std::size_t n = b->_shape[0] - n_left;
    auto dst = std::begin(result._data);
    auto src1 = std::cbegin(a->_data);
    auto src2 = std::cbegin(b->_data) + n_left;

    const APyFloatSpec& lhs_spec = swap ? rhs.spec() : spec();
    const APyFloatSpec& rhs_spec = swap ? spec() : rhs.spec();
    const APyFloatSpec& res_spec = result.spec();
    auto inner_product = FloatingPointInnerProduct(lhs_spec, rhs_spec, res_spec, qntz);

    // `b` limits length of the inner product length
    for (std::size_t i = 0; i < n_left; i++) {
        inner_product(&*src1, &*src2, &*dst, n);
        src2--;
        dst++;
        n++;
    }

    // full inner product length
    for (std::size_t i = 0; i < a->_shape[0] - b->_shape[0] + 1; i++) {
        inner_product(&*src1, &*src2, &*dst, n);
        src1++;
        dst++;
    }

    // `a` limits length of the inner product length
    for (std::size_t i = 0; i < n_right; i++) {
        n--;
        inner_product(&*src1, &*src2, &*dst, n);
        src1++;
        dst++;
    }

    return result;
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::sum(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Accumulation function
    auto&& qntz = get_float_quantization_mode();
    FloatingPointAdder<> add(spec(), spec(), spec(), qntz);
    auto accumulate
        = [&](auto acc_it, auto src_it) { add(&*acc_it, &*src_it, &*acc_it); };

    return array_fold(
        axes,         // axes
        accumulate,   // fold function
        std::nullopt, // initial fold data

        /*  bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

APyFloatArray APyFloatArray::cumsum(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axis = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axis.has_value() && axis >= _ndim) {
        std::string msg = fmt::format(
            "APyFloatArray.cumsum: axis {} out of range (ndim = {})", *axis, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Accumulation function
    auto&& qntz = get_float_quantization_mode();
    FloatingPointAdder<> add(spec(), spec(), spec(), qntz);
    auto accumulate
        = [&](auto acc_it, auto src_it) { add(&*acc_it, &*src_it, &*acc_it); };

    return array_fold_cumulative(
        axis,               // axes
        accumulate,         // fold function
        [](auto, auto) { }, // post-processing function
        std::nullopt,       // initial fold data

        /*  bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::nansum(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Accumulation function
    auto&& qntz = get_float_quantization_mode();
    FloatingPointAdder<> add(spec(), spec(), spec(), qntz);
    auto accumulate = [&](auto acc_it, auto src_it) {
        if (!is_nan(*src_it, spec())) {
            add(&*acc_it, &*src_it, &*acc_it);
        }
    };

    return array_fold(
        axes,         // axes
        accumulate,   // fold function
        std::nullopt, // initial fold data

        /*  bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

APyFloatArray APyFloatArray::nancumsum(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axis = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axis.has_value() && axis >= _ndim) {
        std::string msg = fmt::format(
            "APyFloatArray.nancumsum: axis {} out of range (ndim = {})", *axis, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Accumulation function
    auto&& qntz = get_float_quantization_mode();
    FloatingPointAdder<> add(spec(), spec(), spec(), qntz);
    auto accumulate = [&](auto acc_it, auto src_it) {
        if (!is_nan(*src_it, spec())) {
            add(&*acc_it, &*src_it, &*acc_it);
        }
    };

    return array_fold_cumulative(
        axis,               // axes
        accumulate,         // fold function
        [](auto, auto) { }, // post-processing function
        std::nullopt,       // initial fold data

        /*  bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::prod(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to multiply over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Multiplicative fold function
    auto&& qntz = get_float_quantization_mode();
    FloatingPointMultiplier<> prod(spec(), spec(), spec(), qntz);
    auto fold_func
        = [&](auto acc_it, auto src_it) { prod(&*acc_it, &*src_it, &*acc_it); };

    APyFloat init_one = APyFloat::one(get_exp_bits(), get_man_bits());
    return array_fold(
        axes,      // axes
        fold_func, // fold function
        init_one,  // initial fold data

        /* bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

APyFloatArray APyFloatArray::cumprod(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axes = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axes.has_value() && axes >= _ndim) {
        std::string msg = fmt::format(
            "APyFloatArray.cumprod: axis {} out of range (ndim = {})", *axes, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Multiplicative fold function
    auto&& qntz = get_float_quantization_mode();
    FloatingPointMultiplier<> prod(spec(), spec(), spec(), qntz);
    auto fold_func
        = [&](auto acc_it, auto src_it) { prod(&*acc_it, &*src_it, &*acc_it); };

    APyFloat init_one = APyFloat::one(get_exp_bits(), get_man_bits());
    return array_fold_cumulative(
        axes,               // axes
        fold_func,          // fuld function
        [](auto, auto) { }, // post-processing function
        init_one,           // initial data

        /* bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::nanprod(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to multiply over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Multiplicative fold function
    auto&& qntz = get_float_quantization_mode();
    FloatingPointMultiplier<> prod(spec(), spec(), spec(), qntz);
    auto fold_func = [&](auto acc_it, auto src_it) {
        if (!is_nan(*src_it, spec())) {
            prod(&*acc_it, &*src_it, &*acc_it);
        }
    };

    APyFloat init_one = APyFloat::one(get_exp_bits(), get_man_bits());
    return array_fold(
        axes,      // axes
        fold_func, // fold function
        init_one,  // initial fold data

        /* bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

APyFloatArray APyFloatArray::nancumprod(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axes = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axes.has_value() && axes >= _ndim) {
        std::string msg = fmt::format(
            "APyFloatArray.nancumprod: axis {} out of range (ndim = {})", *axes, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Multiplicative fold function
    auto&& qntz = get_float_quantization_mode();
    FloatingPointMultiplier<> prod(spec(), spec(), spec(), qntz);
    auto fold_func = [&](auto acc_it, auto src_it) {
        if (!is_nan(*src_it, spec())) {
            prod(&*acc_it, &*src_it, &*acc_it);
        }
    };

    APyFloat init_one = APyFloat::one(get_exp_bits(), get_man_bits());
    return array_fold_cumulative(
        axes,               // axes
        fold_func,          // fuld function
        [](auto, auto) { }, // post-processing function
        init_one,           // initial data

        /* bit-specifier args: */
        exp_bits,
        man_bits,
        bias
    );
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::max(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    APyFloat dst(get_exp_bits(), get_man_bits(), get_bias());
    APyFloat src(get_exp_bits(), get_man_bits(), get_bias());
    auto max_fold = [&](auto dst_it, auto src_it) {
        dst.set_data(*dst_it);
        src.set_data(*src_it);
        if (dst.is_nan()) {
            *dst_it = *dst_it;
        } else if (src.is_nan()) {
            *dst_it = *src_it;
        } else {
            if (dst < src) {
                *dst_it = *src_it;
            }
        }
    };

    exp_t max_exp = (exp_t(1) << exp_bits) - 1;
    auto init_min = APyFloat(true, max_exp, 0, exp_bits, man_bits, bias);
    return array_fold(axes, max_fold, init_min, exp_bits, man_bits, bias);
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::min(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    APyFloat dst(get_exp_bits(), get_man_bits(), get_bias());
    APyFloat src(get_exp_bits(), get_man_bits(), get_bias());
    auto max_fold = [&](auto dst_it, auto src_it) {
        dst.set_data(*dst_it);
        src.set_data(*src_it);
        if (dst.is_nan()) {
            *dst_it = *dst_it;
        } else if (src.is_nan()) {
            *dst_it = *src_it;
        } else {
            if (src < dst) {
                *dst_it = *src_it;
            }
        }
    };

    exp_t max_exp = (exp_t(1) << exp_bits) - 1;
    auto init_max = APyFloat(false, max_exp, 0, exp_bits, man_bits, bias);
    return array_fold(axes, max_fold, init_max, exp_bits, man_bits, bias);
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::nanmax(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    APyFloat dst(get_exp_bits(), get_man_bits(), get_bias());
    APyFloat src(get_exp_bits(), get_man_bits(), get_bias());
    auto max_fold = [&](auto dst_it, auto src_it) {
        dst.set_data(*dst_it);
        src.set_data(*src_it);
        if (dst.is_nan()) {
            *dst_it = *src_it;
        } else if (src.is_nan()) {
            *dst_it = *dst_it;
        } else {
            if (dst < src) {
                *dst_it = *src_it;
            }
        }
    };

    exp_t max_exp = (exp_t(1) << exp_bits) - 1;
    auto init_nan = APyFloat(false, max_exp, 1, exp_bits, man_bits, bias);
    return array_fold(axes, max_fold, init_nan, exp_bits, man_bits, bias);
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::nanmin(const std::optional<PyShapeParam_t>& py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    APyFloat dst(get_exp_bits(), get_man_bits(), get_bias());
    APyFloat src(get_exp_bits(), get_man_bits(), get_bias());
    auto max_fold = [&](auto dst_it, auto src_it) {
        dst.set_data(*dst_it);
        src.set_data(*src_it);
        if (dst.is_nan()) {
            *dst_it = *src_it;
        } else if (src.is_nan()) {
            *dst_it = *dst_it;
        } else {
            if (src < dst) {
                *dst_it = *src_it;
            }
        }
    };

    exp_t max_exp = (exp_t(1) << exp_bits) - 1;
    auto init_nan = APyFloat(false, max_exp, 1, exp_bits, man_bits, bias);
    return array_fold(axes, max_fold, init_nan, exp_bits, man_bits, bias);
}

std::string APyFloatArray::repr() const
{
    const auto sign_fmt = [](auto cbegin_it, auto) -> std::string {
        return fmt::format("{}", int(cbegin_it->sign));
    };
    const auto exp_fmt = [](auto cbegin_it, auto) -> std::string {
        return fmt::format("{}", cbegin_it->exp);
    };
    const auto man_fmt = [](auto cbegin_it, auto) -> std::string {
        return fmt::format("{}", cbegin_it->man);
    };

    std::initializer_list<formatter_t> formatters { sign_fmt, exp_fmt, man_fmt };
    std::vector<std::string> kw_args = { fmt::format("exp_bits={}", exp_bits),
                                         fmt::format("man_bits={}", man_bits) };
    if (bias != ieee_bias(exp_bits)) {
        kw_args.push_back(fmt::format("bias={}", bias));
    }

    return array_repr(formatters, kw_args);
}

std::variant<
    nb::list,
    nb::ndarray<nb::numpy, std::uint64_t>,
    nb::ndarray<nb::numpy, std::uint32_t>,
    nb::ndarray<nb::numpy, std::uint16_t>,
    nb::ndarray<nb::numpy, std::uint8_t>>
APyFloatArray::to_bits(bool numpy) const
{
    if (numpy) {
        if (get_bits() <= 8) {
            return to_bits_ndarray<nb::numpy, std::uint8_t>();
        } else if (get_bits() <= 16) {
            return to_bits_ndarray<nb::numpy, std::uint16_t>();
        } else if (get_bits() <= 32) {
            return to_bits_ndarray<nb::numpy, std::uint32_t>();
        } else {
            return to_bits_ndarray<nb::numpy, std::uint64_t>();
        }
    } else {
        auto it = std::cbegin(_data);
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

    INT_TYPE* result_data = new INT_TYPE[_data.size()];
    for (std::size_t i = 0; i < _data.size(); i++) {
        result_data[i] = to_bits_uint64(_data[i], exp_bits, man_bits);
    }

    // Delete `result_data` when the `owner` capsule expires
    nb::capsule owner(result_data, [](void* p) noexcept { delete[] (INT_TYPE*)p; });

    return nb::ndarray<NB_ARRAY_TYPE, INT_TYPE>(result_data, ndim(), &_shape[0], owner);
}

nb::list APyFloatArray::to_bits_python_recursive_descent(
    std::size_t dim, std::vector<APyFloatData>::const_iterator& it
) const
{
    nb::list result;
    if (dim == ndim() - 1) {
        // Most inner dimension: append data
        for (std::size_t i = 0; i < _shape[dim]; i++) {
            result.append(apyfloat_to_bits(*it, exp_bits, man_bits));
            it++;
        }
    } else {
        // We need to go deeper...
        for (std::size_t i = 0; i < _shape[dim]; i++) {
            result.append(to_bits_python_recursive_descent(dim + 1, it));
        }
    }
    return result;
}

nb::ndarray<nb::numpy, double>
APyFloatArray::to_numpy(std::optional<nb::object> dtype, std::optional<bool> copy) const
{

    // If someone in the future wants to check the dtype, this is how it can be done:
    // const auto np = nb::module_::import_("numpy");
    // const auto t = dtype.value_or(np.attr("float64"));
    // if (!t.equal(np.attr("float64"))) {
    //     throw nb::value_error("APyFloatArray::to_numpy: only supports float64
    //     dtype");
    // }
    (void)dtype;

    if (!copy.value_or(true)) {
        throw nb::value_error("APyFloatArray.to_numpy: copy must be True");
    }

    // Dynamically allocate data to be passed to Python
    double* result_data = new double[_nitems];
    auto fp = APyFloat(exp_bits, man_bits, bias);
    for (std::size_t i = 0; i < _nitems; i++) {
        fp.set_data(_data[i]);
        result_data[i] = fp.to_double();
    }

    // Delete 'data' when the 'owner' capsule expires
    nb::capsule owner(result_data, [](void* p) noexcept { delete[] (double*)p; });

    return nb::ndarray<nb::numpy, double>(result_data, ndim(), &_shape[0], owner);
}

bool APyFloatArray::is_identical(const nb::object& other, bool ignore_zero_sign) const
{
    if (!nb::isinstance<APyFloatArray>(other)) {
        return false;
    } else {
        auto&& other_arr = nb::cast<APyFloatArray>(other);
        if (!is_same_spec(other_arr) || _shape != other_arr._shape) {
            return false;
        }

        for (std::size_t i = 0; i < _data.size(); i++) {
            // If `ignore_zero_sign` is set, ignore the sign bit if results equals zero
            if (ignore_zero_sign && is_zero(_data[i]) && is_zero(other_arr._data[i])) {
                continue;
            }

            if (_data[i] != other_arr._data[i]) {
                return false;
            }
        }
    }

    return true;
}

APyFloatArray APyFloatArray::from_numbers(
    const nb::typed<nb::iterable, nb::any>& number_seq,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
{
    if (nb::isinstance<nb::ndarray<>>(number_seq)) {
        // Sequence is NDArray. Initialize using `from_array`.
        auto ndarray = nb::cast<nb::ndarray<nb::c_contig>>(number_seq);
        return from_array(ndarray, exp_bits, man_bits, bias);
    }

    check_exponent_format(exp_bits, "APyFloatArray.from_float");
    check_mantissa_format(man_bits, "APyFloatArray.from_float");

    APyFloatArray result(
        python_iterable_extract_shape(number_seq, "APyFloatArray.from_float"),
        exp_bits,
        man_bits,
        bias
    );

    const auto py_objs = python_iterable_walk<nb::float_, nb::int_, APyFixed, APyFloat>(
        number_seq, "APyFloatArray.from_float"
    );

    for (std::size_t i = 0; i < result._data.size(); i++) {
        if (nb::isinstance<nb::float_>(py_objs[i])) {
            double val = static_cast<double>(nb::cast<nb::float_>(py_objs[i]));
            auto&& fp = APyFloat::from_double(val, exp_bits, man_bits, result.bias);
            result._data[i] = fp.get_data();
        } else if (nb::isinstance<nb::int_>(py_objs[i])) {
            auto&& obj = nb::cast<nb::int_>(py_objs[i]);
            auto&& fp = APyFloat::from_integer(obj, exp_bits, man_bits, result.bias);
            result._data[i] = fp.get_data();
        } else if (nb::isinstance<APyFixed>(py_objs[i])) {
            auto&& fx = nb::cast<APyFixed>(py_objs[i]);
            auto&& fp = APyFloat::from_fixed(fx, exp_bits, man_bits, bias);
            result._data[i] = fp.get_data();
        } else if (nb::isinstance<APyFloat>(py_objs[i])) {
            auto&& obj = nb::cast<APyFloat>(py_objs[i]);
            auto&& fp = obj.cast(exp_bits, man_bits, bias, QuantizationMode::RND_CONV);
            result._data[i] = fp.get_data();
        } else {
            APYTYPES_UNREACHABLE();
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
    check_exponent_format(exp_bits, "APyFloatArray.from_array");
    check_mantissa_format(man_bits, "APyFloatArray.from_array");

    assert(ndarray.ndim() > 0);
    std::vector<std::size_t> shape(ndarray.ndim(), 0);
    for (std::size_t i = 0; i < ndarray.ndim(); i++) {
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
                double_caster.set_data(                                                \
                    { sign_of_double(value),                                           \
                      exp_t(exp_of_double(value)),                                     \
                      man_of_double(value) }                                           \
                );                                                                     \
                APyFloat fp                                                            \
                    = double_caster.cast_from_double(exp_bits, man_bits, bias);        \
                _data[i] = fp.get_data();                                              \
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

APyFloatArray APyFloatArray::from_bits(
    const nb::iterable& python_bit_patterns,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits, "APyFloatArray.from_bits");
    check_mantissa_format(man_bits, "APyFloatArray.from_bits");

    if (nb::isinstance<nb::ndarray<>>(python_bit_patterns)) { // ndarray
        const auto ndarray = nb::cast<nb::ndarray<nb::c_contig>>(python_bit_patterns);

        assert(ndarray.ndim() > 0);
        std::vector<std::size_t> shape(ndarray.ndim(), 0);
        for (std::size_t i = 0; i < ndarray.ndim(); i++) {
            shape[i] = ndarray.shape(i);
        }

        APyFloatArray result(shape, exp_bits, man_bits, bias);
        result._set_bits_from_ndarray(ndarray);
        return result;
    }

    APyFloatArray result(
        python_iterable_extract_shape(python_bit_patterns, "APyFloatArray.from_bits"),
        exp_bits,
        man_bits,
        bias
    );

    const auto py_obj = python_iterable_walk<nb::float_, nb::int_>(
        python_bit_patterns, "APyFloatArray.from_bits"
    );

    APyFloat f(exp_bits, man_bits, result.bias);
    for (std::size_t i = 0; i < result._data.size(); i++) {
        if (nb::isinstance<nb::int_>(py_obj[i])) {
            result._data[i]
                = f.update_from_bits(nb::cast<nb::int_>(py_obj[i])).get_data();
        } else {
            throw std::domain_error("Invalid Python objects in sequence");
        }
    }

    return result;
}

void APyFloatArray::_set_bits_from_ndarray(const nb::ndarray<nb::c_contig>& ndarray)
{
    // Double value used for converting.
    APyFloat f(exp_bits, man_bits, bias);

#define CHECK_AND_SET_BITS_FROM_NPTYPE(__TYPE__)                                       \
    do {                                                                               \
        if (ndarray.dtype() == nb::dtype<__TYPE__>()) {                                \
            const auto ndarray_view = ndarray.view<__TYPE__, nb::ndim<1>>();           \
            for (std::size_t i = 0; i < ndarray.size(); i++) {                         \
                const auto bits = static_cast<std::uint64_t>(ndarray_view.data()[i]);  \
                f.update_from_bits(bits);                                              \
                _data[i] = f.get_data();                                               \
            }                                                                          \
            return; /* Conversion completed, exit function */                          \
        }                                                                              \
    } while (0)

    // Each `CHECK_AND_SET_BITS_FROM_NPTYPE` checks the dtype of `ndarray` and
    // converts all the data if it matches. If successful,
    // `CHECK_AND_SET_BITS_FROM_NPTYPES` returns. Otherwise, the next attempted
    // conversion will take place
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::int64_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::int32_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::int16_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::int8_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::uint64_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::uint32_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::uint16_t);
    CHECK_AND_SET_BITS_FROM_NPTYPE(std::uint8_t);

    // None of the `CHECK_AND_VALUES_FROM_NPTYPE` succeeded. Unsupported type, throw
    // an error. If possible, it would be nice to show a string representation of
    // the `dtype`. Seems hard to achieve with nanobind, but please fix this if you
    // find out how this can be achieved.
    throw nb::type_error(
        "APyFloatArray::_set_bits_from_ndarray(): "
        "unsupported `dtype` expecting integer"
    );
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

    check_exponent_format(actual_exp_bits, "APyFloatArray.cast");
    check_mantissa_format(actual_man_bits, "APyFloatArray.cast");

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
    // If same bit specifiers, return a copy of this.
    if (new_exp_bits == exp_bits && new_man_bits == man_bits && new_bias == bias) {
        return *this;
    }

    APyFloatArray result(_shape, new_exp_bits, new_man_bits, new_bias);

    const exp_t SRC_MAX_EXP = (1ULL << exp_bits) - 1;
    const exp_t DST_MAX_EXP = (1ULL << new_exp_bits) - 1;
    const int SPEC_MAN_BITS_DELTA = new_man_bits - man_bits;
    const std::int64_t BIAS_DELTA = std::int64_t(bias) - std::int64_t(new_bias);

    // If longer word lengths, use simpler/faster method
    if (new_exp_bits >= exp_bits && new_man_bits >= man_bits) {
        for (std::size_t i = 0; i < _data.size(); i++) {
            result._data[i] = array_floating_point_cast_no_quant(
                _data[i],
                spec(),
                SRC_MAX_EXP,
                DST_MAX_EXP,
                SPEC_MAN_BITS_DELTA,
                BIAS_DELTA
            );
        }
        return result;
    }

    const auto quantization_func = get_qntz_func(quantization);
    const man_t SRC_LEADING_ONE = (1ULL << man_bits);
    const man_t DST_LEADING_ONE = (1ULL << new_man_bits);
    const man_t SRC_HIDDEN_ONE = (1ULL << man_bits);

    if (SPEC_MAN_BITS_DELTA >= 0) {
        for (std::size_t i = 0; i < _data.size(); i++) {
            result._data[i] = array_floating_point_cast_pos_man_delta(
                _data[i],
                spec(),
                result.spec(),
                quantization,
                quantization_func,
                SRC_MAX_EXP,
                DST_MAX_EXP,
                SRC_LEADING_ONE,
                DST_LEADING_ONE,
                SPEC_MAN_BITS_DELTA,
                SRC_HIDDEN_ONE,
                BIAS_DELTA
            );
        }
    } else {
        const int SPEC_MAN_BITS_DELTA_REV = -SPEC_MAN_BITS_DELTA;
        const man_t FINAL_STICKY = (1ULL << (SPEC_MAN_BITS_DELTA_REV - 1)) - 1;
        for (std::size_t i = 0; i < _data.size(); i++) {
            result._data[i] = array_floating_point_cast_neg_man_delta(
                _data[i],
                spec(),
                result.spec(),
                quantization,
                quantization_func,
                SRC_MAX_EXP,
                DST_MAX_EXP,
                SRC_LEADING_ONE,
                DST_LEADING_ONE,
                SPEC_MAN_BITS_DELTA_REV,
                SRC_HIDDEN_ONE,
                FINAL_STICKY,
                BIAS_DELTA
            );
        }
    }

    return result;
}

// Evaluate the inner between two vectors. This method assumes that the shape of
// both `*this` and `rhs` are equally long. Anything else is undefined behaviour.
APyFloat APyFloatArray::checked_inner_product(const APyFloatArray& rhs) const
{
    const std::optional<APyFloatAccumulatorOption> mode = get_accumulator_mode_float();
    const QuantizationMode& qntz
        = mode.has_value() ? mode->quantization : get_float_quantization_mode();
    std::uint8_t res_exp_bits;
    std::uint8_t res_man_bits;
    exp_t res_bias;
    if (mode.has_value()) {
        res_exp_bits = mode->exp_bits;
        res_man_bits = mode->man_bits;
        res_bias = mode->bias.has_value() ? *mode->bias : ieee_bias(res_exp_bits);
    } else {
        res_exp_bits = std::max(exp_bits, rhs.exp_bits);
        res_man_bits = std::max(man_bits, rhs.man_bits);
        res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    }

    APyFloat result(res_exp_bits, res_man_bits, res_bias);

    auto inner_product
        = FloatingPointInnerProduct(spec(), rhs.spec(), result.spec(), qntz);

    // dst = A x b
    APyFloatData sum {};
    inner_product(
        &_data[0],     // src1, a: [1 x N]
        &rhs._data[0], // src2, b: [N x 1]
        &sum,          // dst
        _shape[0]      // N
    );
    result.set_data(sum);
    return result;
}

// Evaluate the matrix product between two 2D matrices. This method assumes that the
// shape of `*this` and `rhs` have been checked to match a 2d matrix multiplication.
APyFloatArray APyFloatArray::checked_2d_matmul(const APyFloatArray& rhs) const
{
    // Resulting parameters
    const std::vector<std::size_t> res_shape = rhs._shape.size() > 1
        ? std::vector<std::size_t> { _shape[0], rhs._shape[1] } // rhs is 2-D
        : std::vector<std::size_t> { _shape[0] };               // rhs is 1-D
    const std::optional<APyFloatAccumulatorOption> mode = get_accumulator_mode_float();
    const QuantizationMode& qntz
        = mode.has_value() ? mode->quantization : get_float_quantization_mode();
    std::uint8_t res_exp_bits;
    std::uint8_t res_man_bits;
    exp_t res_bias;
    if (mode.has_value()) {
        res_exp_bits = mode->exp_bits;
        res_man_bits = mode->man_bits;
        res_bias = mode->bias.has_value() ? *mode->bias : ieee_bias(res_exp_bits);
    } else {
        res_exp_bits = std::max(exp_bits, rhs.exp_bits);
        res_man_bits = std::max(man_bits, rhs.man_bits);
        res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    }

    // Result array
    APyFloatArray result(res_shape, res_exp_bits, res_man_bits, res_bias);

    // Current column from rhs, read once and cached for efficiency
    const auto res_cols = rhs._shape.size() > 1 ? rhs._shape[1] : 1;
    APyFloatArray current_column(
        { rhs._shape[0] }, rhs.exp_bits, rhs.man_bits, rhs.bias
    );

    auto inner_product
        = FloatingPointInnerProduct(spec(), rhs.spec(), result.spec(), qntz);

    for (std::size_t x = 0; x < res_cols; x++) {
        // Copy column from `rhs` and use as the current working column. As reading
        // columns from `rhs` is cache-inefficient, we like to do this only once for
        // each element in the resulting matrix.
        for (std::size_t col = 0; col < rhs._shape[0]; col++) {
            current_column._data[col] = rhs._data[x + col * res_cols];
        }

        // dst = A x b
        inner_product(
            &_data[0],                // src1, A: [M x N]
            &current_column._data[0], // src2, b: [N x 1]
            &result._data[x],         // dst
            _shape[1],                // N
            res_shape[0],             // M
            res_cols                  // DST_STEP
        );
    }

    return result;
}

std::string APyFloatArray::to_string_dec() const
{
    const auto formatter = [spec = spec()](auto cbegin_it, auto) -> std::string {
        // NOTE: Python, unlike C++, unconditionally encodes the string of a
        // floating-point NaN without a minus sign.
        if (is_nan(*cbegin_it, spec)) {
            return "nan";
        } else {
            return fmt::format("{:g}", floating_point_to_double(*cbegin_it, spec));
        }
    };

    return array_format(formatter, 88, false);
}

std::string APyFloatArray::to_string(int base) const
{
    switch (base) {
    case 10:
        return to_string_dec();
    default:
        auto msg = fmt::format("APyFloatArray.__str__: base={} is not supported", base);
        throw nb::value_error(msg.c_str());
    }
}

/*
 * Convenience methods
 */
APyFloatArray
APyFloatArray::cast_to_double(std::optional<QuantizationMode> quantization) const
{
    return _cast(11, 52, 1023, quantization);
}

APyFloatArray
APyFloatArray::cast_to_single(std::optional<QuantizationMode> quantization) const
{
    return _cast(8, 23, 127, quantization);
}

APyFloatArray
APyFloatArray::cast_to_half(std::optional<QuantizationMode> quantization) const
{
    return _cast(5, 10, 15, quantization);
}

APyFloatArray
APyFloatArray::cast_to_bfloat16(std::optional<QuantizationMode> quantization) const
{
    return _cast(8, 7, 127, quantization);
}
