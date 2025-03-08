#include "apyfloatarray.h"
#include "apyarray.h"
#include "apyfloat.h"
#include "apyfloat_util.h"
#include "apytypes_common.h"
#include "apytypes_util.h"
#include "array_utils.h"
#include "broadcast.h"
#include "ieee754.h"
#include "python_util.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
namespace nb = nanobind;

// Standard header includes
#include <algorithm>
#include <cstddef>
#include <fmt/format.h>
#include <set>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <variant>

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
    : APyArray(python_sequence_extract_shape(sign_seq))
    , exp_bits(exp_bits)
    , man_bits(man_bits)
{
    this->bias = bias.value_or(APyFloat::ieee_bias(exp_bits));

    const auto& signs_shape = _shape;
    const auto exps_shape = python_sequence_extract_shape(exp_seq);
    const auto mans_shape = python_sequence_extract_shape(man_seq);

    if (!((signs_shape == exps_shape) && (signs_shape == mans_shape))) {
        throw std::domain_error("Shape mismatch during construction");
    }

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

        _data[i] = { sign, exp, man };
    }
}

APyFloatArray::APyFloatArray(
    const std::vector<std::size_t>& shape,
    exp_t exp_bits,
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
 * ********************************************************************************* */

APyFloatArray APyFloatArray::operator+(const APyFloatArray& rhs) const
{
    if (_shape != rhs._shape) {
        const auto broadcast_shape = smallest_broadcastable_shape(_shape, rhs._shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyFloatArray.__add__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(rhs._shape)
            ));
        }
        return broadcast_to(broadcast_shape) + rhs.broadcast_to(broadcast_shape);
    }

    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);
    const QuantizationMode& qntz = get_float_quantization_mode();

    // Perform the addition
    floating_point_sums(
        std::begin(_data),     // src1
        std::begin(rhs._data), // src2
        std::begin(res._data), // dst
        spec(),                // src1_spec
        rhs.spec(),            // src2_spec
        res.spec(),            // dst_spec
        res._nitems,           // n_items
        qntz                   // qntz
    );

    return res;
}

APyFloatArray APyFloatArray::operator+(const APyFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();

    // Perform the addition
    const APyFloatData& rhs_data = rhs.get_data();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);
    floating_point_sums<false, 1, 0, 1>(
        std::begin(_data),     // src1
        &rhs_data,             // src2
        std::begin(res._data), // dst
        spec(),                // src1_spec
        rhs.spec(),            // src2_spec
        res.spec(),            // dst_spec
        res._nitems,           // n_items
        qntz                   // qntz
    );

    return res;
}

APyFloatArray APyFloatArray::operator-(const APyFloatArray& rhs) const
{
    if (_shape != rhs._shape) {
        auto broadcast_shape = smallest_broadcastable_shape(_shape, rhs._shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyFloatArray.__sub__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(rhs._shape)
            ));
        }
        return broadcast_to(broadcast_shape) - rhs.broadcast_to(broadcast_shape);
    }

    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);
    const QuantizationMode& qntz = get_float_quantization_mode();

    // Perform the addition
    floating_point_sums<true>(
        std::begin(_data),     // src1
        std::begin(rhs._data), // src2
        std::begin(res._data), // dst
        spec(),                // src1_spec
        rhs.spec(),            // src2_spec
        res.spec(),            // dst_spec
        res._nitems,           // n_items
        qntz                   // qntz
    );

    return res;
}

APyFloatArray APyFloatArray::operator-(const APyFloat& rhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();

    // Perform the addition
    const APyFloatData& rhs_data = rhs.get_data();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);
    floating_point_sums<true, 1, 0, 1>(
        std::begin(_data),     // src1
        &rhs_data,             // src2
        std::begin(res._data), // dst
        spec(),                // src1_spec
        rhs.spec(),            // src2_spec
        res.spec(),            // dst_spec
        res._nitems,           // n_items
        qntz                   // qntz
    );

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
        auto broadcast_shape = smallest_broadcastable_shape(_shape, rhs._shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyFloatArray.__mul__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(rhs._shape)
            ));
        }
        return broadcast_to(broadcast_shape) * rhs.broadcast_to(broadcast_shape);
    }

    // Calculate new format
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.exp_bits);
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.man_bits);
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();

    APyFloatArray result(_shape, res_exp_bits, res_man_bits, res_bias);
    floating_point_products(
        std::cbegin(_data),
        std::cbegin(rhs._data),
        std::begin(result._data),
        spec(),
        rhs.spec(),
        result.spec(),
        _nitems,
        qntz
    );
    return result;
}

APyFloatArray APyFloatArray::operator*(const APyFloat& rhs) const
{
    // Calculate new format
    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    const APyFloatData& rhs_data = rhs.get_data();

    APyFloatArray result(_shape, res_exp_bits, res_man_bits, res_bias);
    floating_point_products<1, 0, 1>(
        std::begin(_data),
        &rhs_data,
        std::begin(result._data),
        spec(),
        rhs.spec(),
        result.spec(),
        _nitems,
        qntz
    );

    return result;
}

APyFloatArray APyFloatArray::operator/(const APyFloatArray& rhs) const
{
    if (_shape != rhs._shape) {
        auto broadcast_shape = smallest_broadcastable_shape(_shape, rhs._shape);
        if (broadcast_shape.size() == 0) {
            throw std::length_error(fmt::format(
                "APyFloatArray.__truediv__: shape mismatch, lhs.shape={}, rhs.shape={}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(rhs._shape)
            ));
        }
        return broadcast_to(broadcast_shape) / rhs.broadcast_to(broadcast_shape);
    }

    const std::uint8_t res_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, rhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), rhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    floating_point_quotients(
        std::cbegin(_data),
        std::cbegin(rhs._data),
        std::begin(res._data),
        spec(),
        rhs.spec(),
        res.spec(),
        _nitems,
        qntz
    );

    return res;
}

APyFloatArray APyFloatArray::operator/(const APyFloat& rhs) const
{
    // Calculate new format
    const auto res_exp_bits = std::max(exp_bits, rhs.get_exp_bits());
    const auto res_man_bits = std::max(man_bits, rhs.get_man_bits());
    const auto res_bias
        = calc_bias(res_exp_bits, exp_bits, bias, rhs.get_exp_bits(), rhs.get_bias());
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    APyFloat lhs_scalar(exp_bits, man_bits, bias);
    // Perform operations
    for (std::size_t i = 0; i < _data.size(); i++) {
        lhs_scalar.set_data(_data[i]);
        res._data[i] = (lhs_scalar / rhs).get_data();
    }

    return res;
}

APyFloatArray APyFloatArray::rdiv(const APyFloat& lhs) const
{
    const std::uint8_t res_exp_bits = std::max(exp_bits, lhs.get_exp_bits());
    const std::uint8_t res_man_bits = std::max(man_bits, lhs.get_man_bits());
    const exp_t res_bias = calc_bias(res_exp_bits, spec(), lhs.spec());
    const QuantizationMode& qntz = get_float_quantization_mode();
    APyFloatArray res(_shape, res_exp_bits, res_man_bits, res_bias);

    const APyFloatData& lhs_data = lhs.get_data();
    floating_point_quotients<0, 1, 1>(
        &lhs_data,
        std::cbegin(_data),
        std::begin(res._data),
        lhs.spec(),
        spec(),
        res.spec(),
        _nitems,
        qntz
    );

    return res;
}

APyFloatArray APyFloatArray::rsub(const APyFloat& lhs) const { return (-*this) + lhs; }

std::variant<APyFloatArray, APyFloat> APyFloatArray::matmul(const APyFloatArray& rhs
) const
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
    throw std::length_error(fmt::format(
        "APyFloatArray.__matmul__: input shape mismatch, lhs: {}, rhs: {}",
        tuple_string_from_vec(_shape),
        tuple_string_from_vec(rhs._shape)
    ));
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
    nb::tuple shape = nb::make_tuple(N, M.value_or(N));
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
    check_exponent_format(exp_bits);
    check_mantissa_format(man_bits);

    const std::vector<APyFixed> apy_vals = ::arange(start, stop, step);
    APyFloatArray result({ apy_vals.size() }, exp_bits, man_bits, bias);

    for (std::size_t i = 0; i < apy_vals.size(); i++) {
        result._data[i]
            = APyFloat::from_fixed(apy_vals[i], exp_bits, man_bits, bias).get_data();
    }

    return result;
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

    using SRC_T = vector_type::const_iterator;
    using DST_T = vector_type::iterator;
    auto linear_inner_product_f = FloatInnerProdFunctor<SRC_T, SRC_T, DST_T>(
        swap ? rhs.spec() : spec(), swap ? spec() : rhs.spec(), result.spec(), qntz
    );

    // `b` limits length of the inner product length
    for (std::size_t i = 0; i < n_left; i++) {
        linear_inner_product_f(src1, src2, dst, n);
        src2--;
        dst++;
        n++;
    }

    // full inner product length
    for (std::size_t i = 0; i < a->_shape[0] - b->_shape[0] + 1; i++) {
        linear_inner_product_f(src1, src2, dst, n);
        src1++;
        dst++;
    }

    // `a` limits length of the inner product length
    for (std::size_t i = 0; i < n_right; i++) {
        n--;
        linear_inner_product_f(src1, src2, dst, n);
        src1++;
        dst++;
    }

    return result;
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::sum(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Accumulation function
    APyFloat acc(get_exp_bits(), get_man_bits(), get_bias());
    APyFloat src(get_exp_bits(), get_man_bits(), get_bias());
    auto accumulate = [&](auto acc_it, auto src_it) {
        acc.set_data(*acc_it);
        src.set_data(*src_it);
        *acc_it = (acc + src).get_data();
    };

    return array_fold(
        axes, accumulate, std::nullopt /* no init */, exp_bits, man_bits, bias
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
    APyFloat acc(get_exp_bits(), get_man_bits(), get_bias());
    APyFloat src(get_exp_bits(), get_man_bits(), get_bias());
    auto accumulate = [&](auto acc_it, auto src_it) {
        acc.set_data(*acc_it);
        src.set_data(*src_it);
        *acc_it = (acc + src).get_data();
    };

    auto post_proc = [](auto, auto) { /* no post processing */ };
    return array_fold_cumulative(
        axis, accumulate, post_proc, std::nullopt, exp_bits, man_bits, bias
    );
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::nansum(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
{
    // Extract axes to sum over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Accumulation function
    APyFloat acc(get_exp_bits(), get_man_bits(), get_bias());
    APyFloat src(get_exp_bits(), get_man_bits(), get_bias());
    auto accumulate = [&](auto acc_it, auto src_it) {
        acc.set_data(*acc_it);
        src.set_data(*src_it);
        if (src.is_nan()) {
            src.set_data({ 0, 0, 0 });
        }
        *acc_it = (acc + src).get_data();
    };

    return array_fold(
        axes, accumulate, std::nullopt /* no init */, exp_bits, man_bits, bias
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
    APyFloat acc(get_exp_bits(), get_man_bits(), get_bias());
    APyFloat src(get_exp_bits(), get_man_bits(), get_bias());
    auto accumulate = [&](auto acc_it, auto src_it) {
        acc.set_data(*acc_it);
        src.set_data(*src_it);
        if (src.is_nan()) {
            src.set_data({ 0, 0, 0 });
        }
        *acc_it = (acc + src).get_data();
    };

    auto post_proc = [](auto, auto) { /* no post processing */ };
    return array_fold_cumulative(
        axis, accumulate, post_proc, std::nullopt, exp_bits, man_bits, bias
    );
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::prod(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
{
    // Extract axes to multiply over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    // Multiplicative fold function
    APyFloat acc(get_exp_bits(), get_man_bits(), get_bias());
    APyFloat src(get_exp_bits(), get_man_bits(), get_bias());
    auto accumulate = [&](auto acc_it, auto src_it) {
        acc.set_data(*acc_it);
        src.set_data(*src_it);
        *acc_it = (acc * src).get_data();
    };

    APyFloat init_one = APyFloat::one(get_exp_bits(), get_man_bits());
    return array_fold(
        axes, accumulate, init_one, get_exp_bits(), get_man_bits(), get_bias()
    );
}

APyFloatArray APyFloatArray::cumprod(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axis = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axis.has_value() && axis >= _ndim) {
        std::string msg = fmt::format(
            "APyFloatArray.cumprod: axis {} out of range (ndim = {})", *axis, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Accumulation function
    APyFloat acc(get_exp_bits(), get_man_bits(), get_bias());
    APyFloat src(get_exp_bits(), get_man_bits(), get_bias());
    auto accumulate = [&](auto acc_it, auto src_it) {
        acc.set_data(*acc_it);
        src.set_data(*src_it);
        *acc_it = (acc * src).get_data();
    };

    APyFloat init_one = APyFloat::one(get_exp_bits(), get_man_bits());
    auto post_proc = [](auto, auto) { /* no post processing */ };
    return array_fold_cumulative(
        axis, accumulate, post_proc, init_one, exp_bits, man_bits, bias
    );
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::nanprod(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
{
    // Extract axes to multiply over
    std::vector<std::size_t> axes = cpp_axes_from_python(py_axis, _ndim);

    APyFloat init_one = APyFloat::one(get_exp_bits(), get_man_bits());

    // Multiplicative fold function
    APyFloat acc(get_exp_bits(), get_man_bits(), get_bias());
    APyFloat src(get_exp_bits(), get_man_bits(), get_bias());
    auto accumulate = [&](auto acc_it, auto src_it) {
        acc.set_data(*acc_it);
        src.set_data(*src_it);
        if (src.is_nan()) {
            src = init_one;
        }
        *acc_it = (acc * src).get_data();
    };

    return array_fold(axes, accumulate, init_one, exp_bits, man_bits, bias);
}

APyFloatArray APyFloatArray::nancumprod(std::optional<nb::int_> py_axis) const
{
    std::optional<std::size_t> axis = py_axis.has_value()
        ? std::optional<std::size_t>(nb::cast<std::size_t>(*py_axis))
        : std::nullopt;

    if (axis.has_value() && axis >= _ndim) {
        std::string msg = fmt::format(
            "APyFloatArray.nancumprod: axis {} out of range (ndim = {})", *axis, _ndim
        );
        throw nb::index_error(msg.c_str());
    }

    // Accumulation function
    APyFloat init_one = APyFloat::one(get_exp_bits(), get_man_bits());
    APyFloat acc(get_exp_bits(), get_man_bits(), get_bias());
    APyFloat src(get_exp_bits(), get_man_bits(), get_bias());
    auto accumulate = [&](auto acc_it, auto src_it) {
        acc.set_data(*acc_it);
        src.set_data(*src_it);
        if (src.is_nan()) {
            src = init_one;
        }
        *acc_it = (acc * src).get_data();
    };

    auto post_proc = [](auto, auto) { /* no post processing */ };
    return array_fold_cumulative(
        axis, accumulate, post_proc, init_one, exp_bits, man_bits, bias
    );
}

std::variant<APyFloatArray, APyFloat>
APyFloatArray::max(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
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
APyFloatArray::min(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
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
APyFloatArray::nanmax(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
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
APyFloatArray::nanmin(std::optional<std::variant<nb::tuple, nb::int_>> py_axis) const
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
    std::stringstream ss {};
    ss << "APyFloatArray(";
    if (_shape[0]) {
        std::stringstream sign_str {}, exp_str {}, man_str {};
        sign_str << "[";
        exp_str << "[";
        man_str << "[";
        for (std::size_t i = 0; i < _data.size(); ++i) {
            const APyFloatData d = _data[i];
            const bool is_last = i == (_data.size() - 1);
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
    ss << tuple_string_from_vec(_shape);
    ss << ", " << "exp_bits=" << static_cast<unsigned>(exp_bits) << ", "
       << "man_bits=" << static_cast<unsigned>(man_bits) << ", " << "bias=" << bias
       << ")";
    return ss.str();
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

nb::ndarray<nb::numpy, double> APyFloatArray::to_numpy() const
{
    // Dynamically allocate data to be passed to Python
    double* result_data = new double[_data.size()];
    auto apy_f = APyFloat(exp_bits, man_bits, bias);
    for (std::size_t i = 0; i < _data.size(); i++) {
        apy_f.set_data(_data[i]);
        result_data[i] = apy_f.to_double();
    }

    // Delete 'data' when the 'owner' capsule expires
    nb::capsule owner(result_data, [](void* p) noexcept { delete[] (double*)p; });

    return nb::ndarray<nb::numpy, double>(result_data, ndim(), &_shape[0], owner);
}

APyFloatArray APyFloatArray::from_numbers(
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

    const auto py_obj
        = python_sequence_walk<nb::float_, nb::int_, APyFixed, APyFloat>(number_seq);

    for (std::size_t i = 0; i < result._data.size(); i++) {
        if (nb::isinstance<nb::float_>(py_obj[i])) {
            result._data[i] = APyFloat::from_double(
                                  static_cast<double>(nb::cast<nb::float_>(py_obj[i])),
                                  exp_bits,
                                  man_bits,
                                  result.bias
            )
                                  .get_data();
        } else if (nb::isinstance<nb::int_>(py_obj[i])) {
            result._data[i]
                = APyFloat::from_integer(
                      nb::cast<nb::int_>(py_obj[i]), exp_bits, man_bits, result.bias
                )
                      .get_data();
        } else if (nb::isinstance<APyFixed>(py_obj[i])) {
            const auto d = static_cast<APyFixed>(nb::cast<APyFixed>(py_obj[i]));
            result._data[i]
                = APyFloat::from_fixed(d, exp_bits, man_bits, bias).get_data();
        } else if (nb::isinstance<APyFloat>(py_obj[i])) {
            const auto d = static_cast<APyFloat>(nb::cast<APyFloat>(py_obj[i]));
            result._data[i]
                = d.cast(exp_bits, man_bits, bias, QuantizationMode::RND_CONV)
                      .get_data();
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
                double_caster.set_data({ sign_of_double(value),                        \
                                         exp_t(exp_of_double(value)),                  \
                                         man_of_double(value) });                      \
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
    const nb::sequence& python_bit_patterns,
    int exp_bits,
    int man_bits,
    std::optional<exp_t> bias
)
{
    check_exponent_format(exp_bits);
    check_mantissa_format(man_bits);

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
        python_sequence_extract_shape(python_bit_patterns), exp_bits, man_bits, bias
    );

    const auto py_obj = python_sequence_walk<nb::float_, nb::int_>(python_bit_patterns);

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

    APyFloatArray result(_shape, new_exp_bits, new_man_bits, new_bias);

    APyFloat caster(exp_bits, man_bits, bias);
    for (std::size_t i = 0; i < _data.size(); i++) {
        caster.set_data(_data[i]);
        result._data[i]
            = caster._checked_cast(new_exp_bits, new_man_bits, new_bias, quantization)
                  .get_data();
    }

    return result;
}

APyFloatArray APyFloatArray::cast_no_quant(
    std::uint8_t new_exp_bits, std::uint8_t new_man_bits, std::optional<exp_t> new_bias
) const
{
    APyFloatArray result(_shape, new_exp_bits, new_man_bits, new_bias);

    APyFloat caster(exp_bits, man_bits, bias);
    for (std::size_t i = 0; i < _data.size(); i++) {
        caster.set_data(_data[i]);
        result._data[i]
            = caster.cast_no_quant(new_exp_bits, new_man_bits, new_bias).get_data();
    }

    return result;
}

// Evaluate the inner between two vectors. This method assumes that the the shape of
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

    using SRC_T = vector_type::const_iterator;
    using DST_T = APyFloatData*;
    auto linear_inner_product_f = FloatInnerProdFunctor<SRC_T, SRC_T, DST_T>(
        spec(), rhs.spec(), result.spec(), qntz
    );

    // dst = A x b
    APyFloatData sum {};
    linear_inner_product_f(
        std::cbegin(_data),     // src1, a: [1 x N]
        std::cbegin(rhs._data), // src2, b: [N x 1]
        &sum,                   // dst
        _shape[0]               // N
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

    using SRC_T = vector_type::const_iterator;
    using DST_T = vector_type::iterator;
    auto multi_linear_inner_product_f = FloatInnerProdFunctor<SRC_T, SRC_T, DST_T>(
        spec(), rhs.spec(), result.spec(), qntz
    );

    for (std::size_t x = 0; x < res_cols; x++) {
        // Copy column from `rhs` and use as the current working column. As reading
        // columns from `rhs` is cache-inefficient, we like to do this only once for
        // each element in the resulting matrix.
        for (std::size_t col = 0; col < rhs._shape[0]; col++) {
            current_column._data[col] = rhs._data[x + col * res_cols];
        }

        // dst = A x b
        multi_linear_inner_product_f(
            std::cbegin(_data),                // src1, A: [M x N]
            std::cbegin(current_column._data), // src2, b: [N x 1]
            std::begin(result._data) + x,      // dst
            _shape[1],                         // N
            res_shape[0],                      // M
            res_cols                           // DST_STEP
        );
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
