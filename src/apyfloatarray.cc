// Python object access through Pybind
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
namespace nb = nanobind;

#include "apyfloat.h"
#include "apyfloatarray.h"
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

APyFloatArray APyFloatArray::perform_basic_arithmetic(
    const APyFloatArray& rhs, ArithmeticOperation op
) const
{
    // Make sure `_shape` of `*this` and `rhs` are the same
    if (shape != rhs.shape) {
        std::string op_str;
        if (op == ArithmeticOperation::ADDITION)
            op_str = "add";
        else if (op == ArithmeticOperation::SUBTRACTION)
            op_str = "sub";
        else if (op == ArithmeticOperation::MULTIPLICATION)
            op_str = "mul";
        else if (op == ArithmeticOperation::DIVISION)
            op_str = "truediv";
        throw std::length_error(fmt::format(
            "APyFloatArray.__{}__: shape missmatch, lhs.shape={}, rhs.shape={}",
            op_str,
            string_from_vec(shape),
            string_from_vec(rhs.shape)
        ));
    }

    // Calculate new format
    APyFloatArray res(
        shape, std::max(exp_bits, rhs.exp_bits), std::max(man_bits, rhs.man_bits)
    );
    res.bias = APyFloat::ieee_bias(res.exp_bits);

    APyFloat lhs_scalar(exp_bits, man_bits, bias);
    APyFloat rhs_scalar(rhs.exp_bits, rhs.man_bits, rhs.bias);
    // Perform operation
    for (std::size_t i = 0; i < data.size(); i++) {
        lhs_scalar.set_data(data[i]);
        rhs_scalar.set_data(rhs.data[i]);

        if (op == ArithmeticOperation::ADDITION)
            res.data[i] = (lhs_scalar + rhs_scalar).get_data();
        else if (op == ArithmeticOperation::SUBTRACTION)
            res.data[i] = (lhs_scalar - rhs_scalar).get_data();
        else if (op == ArithmeticOperation::MULTIPLICATION)
            res.data[i] = (lhs_scalar * rhs_scalar).get_data();
        else if (op == ArithmeticOperation::DIVISION)
            res.data[i] = (lhs_scalar / rhs_scalar).get_data();
        else
            throw NotImplementedException("Arithmetic operation not implemented yet");
    }

    return res;
}

APyFloatArray APyFloatArray::perform_basic_arithmetic(
    const APyFloat& rhs, ArithmeticOperation op
) const
{
    // Calculate new format
    APyFloatArray res(
        shape,
        std::max(exp_bits, rhs.get_exp_bits()),
        std::max(man_bits, rhs.get_man_bits())
    );
    res.bias = APyFloat::ieee_bias(res.exp_bits);

    // Perform operations
    for (std::size_t i = 0; i < data.size(); i++) {
        APyFloat lhs_scalar(data[i], exp_bits, man_bits, bias);

        if (op == ArithmeticOperation::ADDITION)
            res.data[i] = (lhs_scalar + rhs).get_data();
        else if (op == ArithmeticOperation::SUBTRACTION)
            res.data[i] = (lhs_scalar - rhs).get_data();
        else if (op == ArithmeticOperation::MULTIPLICATION)
            res.data[i] = (lhs_scalar * rhs).get_data();
        else if (op == ArithmeticOperation::DIVISION)
            res.data[i] = (lhs_scalar / rhs).get_data();
        else
            throw NotImplementedException("Arithmetic operation not implemented yet");
    }

    return res;
}

APyFloatArray APyFloatArray::operator+(const APyFloatArray& rhs) const
{
    return perform_basic_arithmetic(rhs, ArithmeticOperation::ADDITION);
}

APyFloatArray APyFloatArray::operator+(const APyFloat& rhs) const
{
    return perform_basic_arithmetic(rhs, ArithmeticOperation::ADDITION);
}

APyFloatArray APyFloatArray::operator-(const APyFloatArray& rhs) const
{
    return perform_basic_arithmetic(rhs, ArithmeticOperation::SUBTRACTION);
}

APyFloatArray APyFloatArray::operator-(const APyFloat& rhs) const
{
    return perform_basic_arithmetic(rhs, ArithmeticOperation::SUBTRACTION);
}

APyFloatArray APyFloatArray::operator*(const APyFloatArray& rhs) const
{
    return perform_basic_arithmetic(rhs, ArithmeticOperation::MULTIPLICATION);
}

APyFloatArray APyFloatArray::operator*(const APyFloat& rhs) const
{
    return perform_basic_arithmetic(rhs, ArithmeticOperation::MULTIPLICATION);
}

APyFloatArray APyFloatArray::operator/(const APyFloatArray& rhs) const
{
    return perform_basic_arithmetic(rhs, ArithmeticOperation::DIVISION);
}

APyFloatArray APyFloatArray::operator/(const APyFloat& rhs) const
{
    return perform_basic_arithmetic(rhs, ArithmeticOperation::DIVISION);
}

APyFloatArray APyFloatArray::matmul(const APyFloatArray& rhs) const
{
    if (get_ndim() == 1 && rhs.get_ndim() == 1) {
        if (shape[0] == rhs.shape[0]) {
            // Dimensionality for a standard scalar inner product checks out. Perform
            // the checked inner product.
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
            "APyFloatArray.__getitem__: index {} is out of bounds for axis 0 with size "
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
    double* result_data = new double[fold_shape()];
    for (std::size_t i = 0; i < fold_shape(); i++) {
        const auto apy_f = APyFloat(data.at(i), exp_bits, man_bits, bias);
        result_data[i] = double(apy_f);
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
    std::optional<exp_t> bias,
    std::optional<QuantizationMode> quantization
)
{
    if (bias.has_value() && bias.value() != APyFloat::ieee_bias(exp_bits)) {
        throw NotImplementedException(
            "Not implemented: APyFloatArray with non IEEE-like bias"
        );
    }

    APyFloatArray result(
        python_sequence_extract_shape(double_seq), exp_bits, man_bits, bias
    );

    auto py_obj = python_sequence_walk<nb::float_, nb::int_>(double_seq);

    for (std::size_t i = 0; i < result.data.size(); i++) {
        double d;
        if (nb::isinstance<nb::float_>(py_obj[i])) {
            d = static_cast<double>(nb::cast<nb::float_>(py_obj[i]));
        } else if (nb::isinstance<nb::int_>(py_obj[i])) {
            d = static_cast<int>(nb::cast<nb::int_>(py_obj[i]));
        } else {
            throw std::domain_error("Invalid Python objects in sequence");
        }
        const auto fp = APyFloat::from_double(
            d,
            exp_bits,
            man_bits,
            result.bias,
            quantization.value_or(get_quantization_mode())
        );
        result.data[i] = { fp.get_sign(), fp.get_exp(), fp.get_man() };
    }
    return result;
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
    if ((new_exp_bits == exp_bits) && (new_man_bits == man_bits)
        && (new_bias == bias)) {
        return *this;
    }

    APyFloatArray result(shape, new_exp_bits, new_man_bits, new_bias);

    for (std::size_t i = 0; i < data.size(); i++) {
        result.data[i] = APyFloat(data[i], exp_bits, man_bits, bias)
                             .cast(new_exp_bits, new_man_bits, new_bias, quantization)
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

    // If an accumulator is used, the operands must be resized before the
    // multiplication. This is because the products would otherwise get quantized too
    // early.

    const auto orig_quant_mode = get_quantization_mode();

    if (get_accumulator_mode().has_value()) {
        const auto acc_option = get_accumulator_mode().value();
        tmp_exp_bits = acc_option.exp_bits;
        tmp_man_bits = acc_option.man_bits;
        set_quantization_mode(acc_option.quantization);
    }

    // Hadamard product of `*this` and `rhs`
    APyFloatArray hadamard;
    if (get_accumulator_mode().has_value()) {
        hadamard = this->cast(tmp_exp_bits, tmp_man_bits)
            * rhs.cast(tmp_exp_bits, tmp_man_bits);
    } else {
        hadamard = *this * rhs;
    }

    APyFloat sum = APyFloat(0, 0, 0, tmp_exp_bits, max_man_bits);
    for (std::size_t i = 0; i < hadamard.fold_shape(); i++) {
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
    return cast(11, 52, std::nullopt, quantization);
}

APyFloatArray APyFloatArray::cast_to_single(std::optional<QuantizationMode> quantization
) const
{
    return cast(8, 23, std::nullopt, quantization);
}

APyFloatArray APyFloatArray::cast_to_half(std::optional<QuantizationMode> quantization
) const
{
    return cast(5, 10, std::nullopt, quantization);
}

APyFloatArray
APyFloatArray::cast_to_bfloat16(std::optional<QuantizationMode> quantization) const
{
    return cast(8, 7, std::nullopt, quantization);
}
