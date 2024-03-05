// Python object access through Pybind
#include "apybuffer.h"
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h> // PYLONG_BITS_IN_DIGIT, PyLongObject

// Standard header includes
#include <algorithm> // std::copy, std::max, std::transform, etc...
#include <cstddef>   // std::size_t
#include <ios>       // std::dec, std::hex
#include <optional>  // std::optional
#include <sstream>   // std::stringstream
#include <stdexcept> // std::length_error
#include <string>    // std::string
#include <vector>    // std::vector, std::swap

#include <fmt/format.h>

#include "apyfixed.h"
#include "apyfixedarray.h"
#include "apyfixedarray_iterator.h"
#include "apytypes_common.h"
#include "apytypes_util.h"
#include "python_util.h"

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

/* ********************************************************************************** *
 * *                            Python constructors                                 * *
 * ********************************************************************************** */

APyFixedArray::APyFixedArray(
    const py::sequence& bit_pattern_sequence,
    std::optional<int> bits,
    std::optional<int> int_bits,
    std::optional<int> frac_bits
)
    : APyFixedArray(
        python_sequence_extract_shape(bit_pattern_sequence), bits, int_bits, frac_bits
    )
{
    auto python_ints = python_sequence_walk<py::int_>(bit_pattern_sequence);
    for (std::size_t i = 0; i < _data.size() / _itemsize; i++) {

        // Two's complements overflowing in `APyFixed` constructor
        auto limb_vec = python_long_to_limb_vec(python_ints[i], _itemsize);
        APyFixed fixed(_bits, _int_bits, limb_vec);
        std::copy_n(
            fixed._data.begin(),          // src
            _itemsize,                    // limbs to copy
            _data.begin() + i * _itemsize // dst
        );
    }
}

/* ********************************************************************************** *
 * *                       More C++ accessible constructors                         * *
 * ********************************************************************************** */

APyFixedArray::APyFixedArray(
    const std::vector<std::size_t>& shape,
    std::optional<int> bits,
    std::optional<int> int_bits,
    std::optional<int> frac_bits
)
    : APyBuffer(shape, bits_to_limbs(bits_from_optional(bits, int_bits, frac_bits)))
    , _bits { bits_from_optional(bits, int_bits, frac_bits) }
    , _int_bits { int_bits_from_optional(bits, int_bits, frac_bits) }
{
}

/* ********************************************************************************** *
 * *                            Binary arithmetic operators                         * *
 * ********************************************************************************* */

APyFixedArray APyFixedArray::operator+(const APyFixedArray& rhs) const
{
    // Make sure `_shape` of `*this` and `rhs` are the same
    if (_shape != rhs._shape) {
        throw std::length_error(fmt::format(
            "APyFixedArray.__add__: shape missmatch, lhs.shape={}, rhs.shape={}",
            string_from_vec(_shape),
            string_from_vec(rhs._shape)
        ));
    }

    // Increase word length of result by one
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());

    // Adjust binary point
    APyFixedArray result = _cast_correct_wl(res_int_bits + res_frac_bits, res_int_bits);
    APyFixedArray imm
        = rhs._cast_correct_wl(res_int_bits + res_frac_bits, res_int_bits);

    // Perform addition
    for (std::size_t i = 0; i < result._data.size(); i += result._itemsize) {
        mpn_add_n(
            &result._data[i], // dst
            &result._data[i], // src1
            &imm._data[i],    // src2
            result._itemsize  // limb vector length
        );
    }

    // Return result
    return result;
}

APyFixedArray APyFixedArray::operator+(const APyFixed& rhs) const
{
    // Increase word length of result by one
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());

    // Adjust binary point
    APyFixedArray result = _cast_correct_wl(res_int_bits + res_frac_bits, res_int_bits);
    APyFixed imm = rhs.cast(res_int_bits + res_frac_bits, res_int_bits);

    // Perform addition
    for (std::size_t i = 0; i < result._data.size(); i += result._itemsize) {
        mpn_add_n(
            &result._data[i], // dst
            &result._data[i], // src1
            &imm._data[0],    // src2
            result._itemsize  // limb vector length
        );
    }

    // Return result
    return result;
}

APyFixedArray APyFixedArray::operator-(const APyFixedArray& rhs) const
{
    // Make sure `_shape` of `*this` and `rhs` are the same
    if (_shape != rhs._shape) {
        throw std::length_error(fmt::format(
            "APyFixedArray.__sub__: shape missmatch, lhs.shape={}, rhs.shape={}",
            string_from_vec(_shape),
            string_from_vec(rhs._shape)
        ));
    }

    // Increase word length of result by one
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());

    // Adjust binary point
    APyFixedArray result = _cast_correct_wl(res_int_bits + res_frac_bits, res_int_bits);
    APyFixedArray imm
        = rhs._cast_correct_wl(res_int_bits + res_frac_bits, res_int_bits);

    // Perform addition
    for (std::size_t i = 0; i < result._data.size(); i += result._itemsize) {
        mpn_sub_n(
            &result._data[i], // dst
            &result._data[i], // src1
            &imm._data[i],    // src2
            result._itemsize  // limb vector length
        );
    }

    // Return result
    return result;
}

APyFixedArray APyFixedArray::operator-(const APyFixed& rhs) const
{
    // Increase word length of result by one
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());

    // Adjust binary point
    APyFixedArray result = cast(res_int_bits + res_frac_bits, res_int_bits);
    APyFixed imm = rhs.cast(res_int_bits + res_frac_bits, res_int_bits);

    // Perform addition
    for (std::size_t i = 0; i < result._data.size(); i += result._itemsize) {
        mpn_sub_n(
            &result._data[i], // dst
            &result._data[i], // src1
            &imm._data[0],    // src2
            result._itemsize  // limb vector length
        );
    }

    // Return result
    return result;
}

APyFixedArray APyFixedArray::operator*(const APyFixedArray& rhs) const
{
    // Make sure `_shape` of `*this` and `rhs` are the same
    if (_shape != rhs._shape) {
        throw std::length_error(fmt::format(
            "APyFixedArray.__mul__: shape missmatch, lhs.shape={}, rhs.shape={}",
            string_from_vec(_shape),
            string_from_vec(rhs._shape)
        ));
    }

    const int res_int_bits = int_bits() + rhs.int_bits();
    const int res_frac_bits = frac_bits() + rhs.frac_bits();

    // Resulting `APyFixedArray` fixed-point tensor
    APyFixedArray result(_shape, res_int_bits + res_frac_bits, res_int_bits);

    // Perform multiplication for each element in the tensor. `checked_hadamard_product`
    // requires:
    // "The destination has to have space for `s1n` + `s2n` limbs, even if the product’s
    //  most significant limb is zero."
    std::vector<mp_limb_t> prod_tmp(_itemsize + rhs._itemsize);
    std::vector<mp_limb_t> op1_abs(bits_to_limbs(bits()));
    std::vector<mp_limb_t> op2_abs(bits_to_limbs(rhs.bits()));
    _checked_hadamard_product(
        rhs,                  // rhs
        result._data.begin(), // dst
        prod_tmp,             // scratch memory: product
        op1_abs,              // scratch memory: operand 1 absolute value
        op2_abs               // scratch memroy: operand 2 absolute value
    );

    return result;
}

APyFixedArray APyFixedArray::operator*(const APyFixed& rhs) const
{
    const int res_int_bits = int_bits() + rhs.int_bits();
    const int res_frac_bits = frac_bits() + rhs.frac_bits();

    // Resulting `APyFixedArray` fixed-point tensor
    APyFixedArray result(_shape, res_int_bits + res_frac_bits, res_int_bits);

    auto op2_begin = rhs._data.begin() + (0) * rhs.vector_size();
    auto op2_end = rhs._data.begin() + (1) * rhs.vector_size();
    bool sign2 = mp_limb_signed_t(*(op2_end - 1)) < 0;
    std::vector<mp_limb_t> op2_abs = limb_vector_abs(op2_begin, op2_end);

    // Perform multiplication for each element in the tensor. `mpn_mul` requires:
    // "The destination has to have space for `s1n` + `s2n` limbs, even if the product’s
    //  most significant limb is zero."
    std::vector<mp_limb_t> res_tmp_vec(_itemsize + rhs.vector_size(), 0);
    std::vector<mp_limb_t> op1_abs(bits_to_limbs(bits()));
    for (std::size_t i = 0; i < fold_shape(_shape); i++) {
        // Current working operands
        auto op1_begin = _data.begin() + (i + 0) * _itemsize;
        auto op1_end = _data.begin() + (i + 1) * _itemsize;

        // Evaluate resulting sign
        bool sign1 = mp_limb_signed_t(*(op1_end - 1)) < 0;
        bool result_sign = sign1 ^ sign2;

        // Retrieve the absolute value of both operands, as required by GMP
        limb_vector_abs(op1_begin, op1_end, op1_abs.begin());

        // Perform the multiplication
        mpn_mul(
            &res_tmp_vec[0], // dst
            &op1_abs[0],     // src1
            op1_abs.size(),  // src1 limb vector length
            &op2_abs[0],     // src2
            op2_abs.size()   // src2 limb vector length
        );

        // Handle sign
        if (result_sign) {
            limb_vector_negate(
                res_tmp_vec.begin(),
                res_tmp_vec.begin() + result._itemsize,
                result._data.begin() + (i + 0) * result._itemsize
            );
        } else {
            // Copy into resulting vector
            std::copy_n(
                res_tmp_vec.begin(),
                result._itemsize,
                result._data.begin() + (i + 0) * result._itemsize
            );
        }
    }

    return result;
}

APyFixedArray APyFixedArray::operator<<(const int shift_val) const
{
    // Left and right shift of data only affects the binary point in the data
    APyFixedArray result = *this;
    result._int_bits += shift_val;
    return result;
}

APyFixedArray APyFixedArray::operator>>(const int shift_val) const
{
    // Left and right shift of data only affects the binary point in the data
    APyFixedArray result = *this;
    result._int_bits -= shift_val;
    return result;
}

APyFixedArray& APyFixedArray::operator<<=(const int shift_val)
{
    // Left shift in place
    _int_bits += shift_val;
    return *this;
}

APyFixedArray& APyFixedArray::operator>>=(const int shift_val)
{
    // Right shift in place
    _int_bits -= shift_val;
    return *this;
}

APyFixedArray APyFixedArray::matmul(const APyFixedArray& rhs) const
{
    if (ndim() == 1 && rhs.ndim() == 1) {
        if (_shape[0] == rhs._shape[0]) {
            // Dimensionality for a standard scalar inner product checks out. Perform
            // the checked inner product.
            int res_bits = bits() + rhs.bits() + bit_width(_shape[0] - 1);
            int res_int_bits = int_bits() + rhs.int_bits() + bit_width(_shape[0] - 1);
            APyFixedArray result({ 1 }, res_bits, res_int_bits);
            APyFixedArray hadamard_tmp(
                _shape, bits() + rhs.bits(), int_bits() + rhs.int_bits()
            );
            std::vector<mp_limb_t> prod_tmp(_itemsize + rhs._itemsize);
            std::vector<mp_limb_t> op1_abs(bits_to_limbs(bits()));
            std::vector<mp_limb_t> op2_abs(bits_to_limbs(rhs.bits()));
            _checked_inner_product(
                rhs,
                result, // dst
                hadamard_tmp,
                prod_tmp, // scratch memory: product
                op1_abs,  // scratch memory: operand 1 absolute value
                op2_abs   // scratch memroy: operand 2 absolute value
            );
            return result;
        }
    }
    if (ndim() == 2 && (rhs.ndim() == 2 || rhs.ndim() == 1)) {
        if (_shape[1] == rhs._shape[0]) {
            // Dimensionality for a standard 2D matrix mutliplication checks out.
            // Perform the checked 2D matrix
            return _checked_2d_matmul(rhs);
        }
    }

    // Unsupported `__matmul__` dimensionality, raise exception
    throw std::length_error(fmt::format(
        "APyFixedArray.__matmul__: input shape missmatch, lhs: ({}), rhs: ({})",
        string_from_vec(_shape),
        string_from_vec(rhs._shape)
    ));
}

APyFixedArray APyFixedArray::transpose() const
{
    if (ndim() > 2) {
        throw NotImplementedException(fmt::format(
            "Not implemented: high-dimensional (ndim={} > 2) tensor transposition",
            ndim()
        ));
    } else if (ndim() <= 1) {
        // Behave like `NumPy`, simply return `*this` if single-dimensional
        return *this;
    }

    // Resulting array with shape dimensions
    APyFixedArray result(_shape, bits(), int_bits());
    std::reverse(result._shape.begin(), result._shape.end());

    // Copy data
    for (std::size_t y = 0; y < _shape[0]; y++) {
        for (std::size_t x = 0; x < _shape[1]; x++) {
            std::copy_n(
                _data.begin() + (y * _shape[1] + x) * _itemsize,       // src
                _itemsize,                                             // limbs
                result._data.begin() + (x * _shape[0] + y) * _itemsize // dst
            );
        }
    }

    return result;
}

/* ********************************************************************************** *
 * *                               Other methods                                    * *
 * ********************************************************************************** */

std::string APyFixedArray::repr() const
{
    std::stringstream ss {};
    ss << "APyFixedArray([";
    if (_shape[0]) {
        // Setup hex printing which will properly display the BCD characters
        ss << std::hex;

        std::vector<mp_limb_t> data(_itemsize, 0);
        for (std::size_t offset = 0; offset < _data.size(); offset += _itemsize) {
            std::copy_n(_data.begin() + offset, _itemsize, data.begin());

            // Zero sign bits outside of bit-range
            if (bits() % _LIMB_SIZE_BITS) {
                mp_limb_t and_mask = (mp_limb_t(1) << (bits() % _LIMB_SIZE_BITS)) - 1;
                data.back() &= and_mask;
            }

            // Double-dabble for binary-to-BCD conversion
            ss << bcds_to_string(double_dabble(data));
            ss << ", ";
        }

        ss.seekp(-2, ss.cur);
    }
    ss << "], shape=(";
    ss << string_from_vec(_shape);
    ss << "), "
       << "bits=" << std::dec << bits() << ", "
       << "int_bits=" << std::dec << int_bits() << ")";
    return ss.str();
}

// The shape of the array
py::tuple APyFixedArray::shape() const
{
    py::tuple result(ndim());
    for (std::size_t i = 0; i < ndim(); i++) {
        result[i] = _shape[i];
    }
    return result;
}

// The dimension in the array
size_t APyFixedArray::ndim() const { return _shape.size(); }

size_t APyFixedArray::size() const { return _shape[0]; }

APyFixedArray APyFixedArray::abs() const
{
    APyFixedArray result = cast(bits() + 1, int_bits() + 1);
    for (std::size_t i = 0; i < fold_shape(_shape); i++) {
        limb_vector_abs(
            result._data.begin() + (i + 0) * result._itemsize,
            result._data.begin() + (i + 1) * result._itemsize,
            result._data.begin() + (i + 0) * result._itemsize
        );
    }
    return result;
}

APyFixedArray APyFixedArray::operator-() const
{
    APyFixedArray result = cast(bits() + 1, int_bits() + 1);
    for (std::size_t i = 0; i < fold_shape(_shape); i++) {
        limb_vector_negate(
            result._data.begin() + (i + 0) * result._itemsize,
            result._data.begin() + (i + 1) * result._itemsize,
            result._data.begin() + (i + 0) * result._itemsize
        );
    }
    return result;
}

APyFixedArray APyFixedArray::get_item(std::size_t idx) const
{
    if (idx >= _shape[0]) {
        throw std::out_of_range(fmt::format(
            "APyFixedArray.__getitem__: index {} is out of bounds for axis 0 with size "
            "{}",
            idx,
            _shape[0]
        ));
    }

    // New shape contains all dimensions except the very first one
    auto new_shape = ndim() > 1
        ? std::vector<std::size_t>(_shape.begin() + 1, _shape.end())
        : std::vector<std::size_t> { 1 };

    // Element stride is the new shape folded over multiplication
    std::size_t element_stride = fold_shape(new_shape);

    APyFixedArray result(new_shape, _bits, _int_bits);
    std::copy_n(
        _data.begin() + idx * _itemsize * element_stride,
        _itemsize * element_stride,
        result._data.begin()
    );
    return result;
}

py::array_t<double> APyFixedArray::to_numpy() const
{
    // The byte-strides of the NumPy object
    std::vector<std::size_t> numpy_stride = strides_from_shape<double>(_shape);

    // Resulting `NumPy` array of float64
    py::array_t<double, py::array::c_style> result(_shape, numpy_stride);

    // Type-cast data and store in the NumPy array
    APyFixed imm(bits(), int_bits());
    for (std::size_t i = 0; i < fold_shape(_shape); i++) {
        std::copy_n(
            std::begin(_data) + i * _itemsize, _itemsize, std::begin(imm._data)
        );
        result.mutable_data()[i] = double(imm);
    }

    return result;
}

bool APyFixedArray::is_identical(const APyFixedArray& other) const
{
    bool same_shape = _shape == other._shape;
    bool same_bit_spec = bits() == other.bits() && int_bits() == other.int_bits();
    return same_shape && same_bit_spec && (_data == other._data);
}

APyFixedArray APyFixedArray::cast(
    std::optional<int> bits,
    std::optional<int> int_bits,
    QuantizationMode quantization,
    OverflowMode overflow,
    std::optional<int> frac_bits
) const
{
    // Sanitize the input
    int new_bits = bits_from_optional(bits, int_bits, frac_bits);
    int new_int_bits = int_bits_from_optional(bits, int_bits, frac_bits);

    // The new result array (`bit_specifier_sanitize()` called in constructor)
    APyFixedArray result(
        _shape, std::max(new_bits, _bits), std::max(new_int_bits, _int_bits)
    );

    // `APyFixed` with the same word length as `*this` for reusing quantization methods
    APyFixed fixed(_bits, _int_bits);

    // For each scalar in the tensor...
    for (std::size_t i = 0; i < fold_shape(_shape); i++) {

        // Copy data into temporary `APyFixed` and possibly sign extend
        std::copy_n(
            _data.begin() + i * _itemsize, // src
            _itemsize,                     // limbs to copy
            fixed._data.begin()            // dst
        );
        if (fixed.vector_size() > _itemsize) {
            std::fill_n(
                fixed._data.begin() + _itemsize,
                fixed.vector_size() - _itemsize,
                mp_limb_signed_t(*--(_data.begin() + (i + 1) * _itemsize)) < 0 ? -1 : 0
            );
        }

        // Perform the resizing
        fixed._cast(
            result._data.begin() + (i + 0) * bits_to_limbs(new_bits), // output start
            result._data.begin() + (i + 1) * result._itemsize,        // output sentinel
            new_bits,
            new_int_bits,
            quantization,
            overflow
        );
    }

    result._bits = new_bits;
    result._int_bits = new_int_bits;
    result.buffer_resize(_shape, bits_to_limbs(new_bits));
    return result;
}

APyFixedArray APyFixedArray::resize(
    std::optional<int> bits,
    std::optional<int> int_bits,
    QuantizationMode quantization,
    OverflowMode overflow,
    std::optional<int> frac_bits
) const
{
    PyErr_WarnEx(
        PyExc_DeprecationWarning, "resize() is deprecated, use cast() instead.", 1
    );
    return cast(bits, int_bits, quantization, overflow, frac_bits);
}

/* ********************************************************************************** *
 * *                      Static conversion from other types                          *
 * ********************************************************************************** */

APyFixedArray APyFixedArray::from_double(
    const py::sequence& python_seq,
    std::optional<int> bits,
    std::optional<int> int_bits,
    std::optional<int> frac_bits
)
{
    APyFixedArray result(
        python_sequence_extract_shape(python_seq), bits, int_bits, frac_bits
    );

    // Extract all Python doubles and integers
    auto py_obj = python_sequence_walk<py::float_, py::int_>(python_seq);

    // Set data from doubles (reuse `APyFixed::from_double` conversion)
    APyFixed fix(result.bits(), result.int_bits());
    for (std::size_t i = 0; i < result._data.size() / result._itemsize; i++) {
        if (py::isinstance<py::float_>(py_obj[i])) {
            // Python double object
            double d = static_cast<double>(py::cast<py::float_>(py_obj[i]));
            fix.set_from_double(d);
            std::copy_n(
                fix._data.begin(),                          // src
                result._itemsize,                           // limbs to copy
                result._data.begin() + i * result._itemsize // dst
            );
        } else if (py::isinstance<py::int_>(py_obj[i])) {
            // Python integer object
            auto max_bits = result.frac_bits() < 0
                ? result.bits() - result.frac_bits() // Negative fractional bits
                : result.bits();                     // Non-negative fractional bits
            auto limb_vec = python_long_to_limb_vec(
                py::cast<py::int_>(py_obj[i]), bits_to_limbs(max_bits)
            );
            fix.set_from_double(double(APyFixed(max_bits, max_bits, limb_vec)));
            std::copy_n(
                fix._data.begin(),                          // src
                result._itemsize,                           // limbs to copy
                result._data.begin() + i * result._itemsize // dst
            );
        }
    }

    return result;
}

/* ********************************************************************************** *
 * *                            Private member functions                            * *
 * ********************************************************************************** */

void APyFixedArray::_checked_hadamard_product(
    const APyFixedArray& rhs,
    std::vector<mp_limb_t>::iterator res_out, // output iterator
    std::vector<mp_limb_t>& prod_tmp,         // scratch: product result
    std::vector<mp_limb_t>& op1_abs,          // scratch: absolute value operand 1
    std::vector<mp_limb_t>& op2_abs           // scratch: absolute value operand 2
) const
{
    // Perform multiplication for each element in the tensor. `mpn_mul` requires:
    // "The destination has to have space for `s1n` + `s2n` limbs, even if the product’s
    //  most significant limb is zero."
    for (std::size_t i = 0; i < fold_shape(_shape); i++) {
        // Current working operands
        auto op1_begin = _data.begin() + (i + 0) * _itemsize;
        auto op1_end = _data.begin() + (i + 1) * _itemsize;
        auto op2_begin = rhs._data.begin() + (i + 0) * rhs._itemsize;
        auto op2_end = rhs._data.begin() + (i + 1) * rhs._itemsize;

        // Evaluate resulting sign
        bool sign1 = mp_limb_signed_t(*(op1_end - 1)) < 0;
        bool sign2 = mp_limb_signed_t(*(op2_end - 1)) < 0;
        bool result_sign = sign1 ^ sign2;

        // Retrieve the absolute value of both operands, as required by GMP
        limb_vector_abs(op1_begin, op1_end, op1_abs.begin());
        limb_vector_abs(op2_begin, op2_end, op2_abs.begin());

        // Perform the multiplication
        mpn_mul(
            &prod_tmp[0],   // dst
            &op1_abs[0],    // src1
            op1_abs.size(), // src1 limb vector length
            &op2_abs[0],    // src2
            op2_abs.size()  // src2 limb vector length
        );

        int result_itemsize = bits_to_limbs(bits() + rhs.bits());
        if (result_sign) {
            // Negate result
            limb_vector_negate(
                prod_tmp.begin(),
                prod_tmp.begin() + result_itemsize,
                res_out + (i + 0) * result_itemsize
            );
        } else {
            // Copy into resulting vector
            std::copy(
                prod_tmp.begin(),
                prod_tmp.begin() + result_itemsize,
                res_out + (i + 0) * result_itemsize
            );
        }
    }
}

// Evaluate the inner between two vectors. This method assumes that the the shape of
// both `*this` and `rhs` are equally long. Anything else is undefined behaviour.
void APyFixedArray::_checked_inner_product(
    const APyFixedArray& rhs,         // rhs
    APyFixedArray& result,            // result
    APyFixedArray& hadamard_tmp,      // scratch: hadamard product
    std::vector<mp_limb_t>& prod_tmp, // scratch: product result
    std::vector<mp_limb_t>& op1_abs,  // scratch: absolute value operand 1
    std::vector<mp_limb_t>& op2_abs   // scratch: absolute value operand 2
) const
{
    // Early exit for empty arrays
    if (_shape[0] == 0) {
        return;
    }

    // Hadamard product of `*this` and `rhs`
    _checked_hadamard_product(
        rhs, hadamard_tmp._data.begin(), prod_tmp, op1_abs, op2_abs
    );

    // Handle possible global accumulator mode
    if (get_accumulator_mode().has_value()) {
        AccumulatorOption mode = *get_accumulator_mode();
        hadamard_tmp._cast(
            hadamard_tmp._data.begin(),
            hadamard_tmp._data.end(),
            mode.bits,
            mode.int_bits,
            mode.quantization,
            mode.overflow
        );
    }

    for (std::size_t i = 0; i < fold_shape(_shape); i++) {
        mpn_add(
            &*result._data.begin(),                          // dst
            &*result._data.begin(),                          // src1
            result._data.size(),                             // src1 limb length
            &hadamard_tmp._data[i * hadamard_tmp._itemsize], // src2
            hadamard_tmp._itemsize                           // src2 limb length
        );
    }
}

// Evaluate the matrix product between two 2D matrices. This method assumes that the
// shape of `*this` and `rhs` have been checked to match a 2d matrix multiplication.
APyFixedArray APyFixedArray::_checked_2d_matmul(const APyFixedArray& rhs) const
{
    // Resulting shape
    std::vector<std::size_t> res_shape = rhs._shape.size() > 1
        ? std::vector<std::size_t> { _shape[0], rhs._shape[1] } // rhs is 2-D
        : std::vector<std::size_t> { _shape[0] };               // rhs is 1-D
    const auto res_cols = rhs._shape.size() > 1 ? rhs._shape[1] : 1;

    // Resulting number of bits
    std::size_t res_bits = bits() + rhs.bits() + bit_width(_shape[1] - 1);
    std::size_t res_int_bits = int_bits() + rhs.int_bits() + bit_width(_shape[1] - 1);
    if (get_accumulator_mode().has_value()) {
        res_bits = get_accumulator_mode()->bits;
        res_int_bits = get_accumulator_mode()->int_bits;
    }

    // Resulting `APyFixedArray`
    APyFixedArray result(res_shape, res_bits, res_int_bits);

    // Scratch memories for avoiding memory allocation and de-allocation
    APyFixedArray hadamard_tmp(
        _shape, bits() + rhs.bits(), int_bits() + rhs.int_bits()
    );
    std::vector<mp_limb_t> prod_tmp(_itemsize + rhs._itemsize);
    std::vector<mp_limb_t> op1_abs(bits_to_limbs(bits()));
    std::vector<mp_limb_t> op2_abs(bits_to_limbs(rhs.bits()));

    // Perform the 2D matrix multiplication
    APyFixedArray current_column({ rhs._shape[0] }, rhs.bits(), rhs.int_bits());
    APyFixedArray current_row({ _shape[1] }, bits(), int_bits());
    APyFixedArray current_res({ 1 }, res_bits, res_int_bits);
    for (std::size_t x = 0; x < res_cols; x++) {

        // Copy column from `rhs` and use as the current working column. As reading
        // columns from `rhs` is cache-inefficient, we like to do this only once for
        // each element in the resulting matrix.
        for (std::size_t row = 0; row < rhs._shape[0]; row++) {
            std::copy_n(
                rhs._data.begin() + (x + row * res_cols) * rhs._itemsize,
                rhs._itemsize,
                current_column._data.begin() + row * rhs._itemsize
            );
        }

        for (std::size_t y = 0; y < res_shape[0]; y++) {

            // Copy current row from lhs (`*this`)
            std::copy_n(
                _data.begin() + (y * _shape[1] * _itemsize), // src
                _itemsize * _shape[1],                       // limbs to copy
                current_row._data.begin()                    // dst
            );

            // Perform the inner product
            current_column._checked_inner_product(
                current_row, current_res, hadamard_tmp, prod_tmp, op1_abs, op2_abs
            );

            // Copy into the resulting vector
            std::copy_n(
                current_res._data.begin(),
                current_res._data.size(),
                result._data.begin() + (y * res_cols + x) * result._itemsize
            );

            std::fill(current_res._data.begin(), current_res._data.end(), 0);
        }
    }

    return result;
}

APyFixedArray APyFixedArray::_cast_correct_wl(int new_bits, int new_int_bits) const
{

    // The new result array (`bit_specifier_sanitize()` called in constructor)
    APyFixedArray result(_shape, new_bits, new_int_bits);

    // `APyFixed` with the same word length as `*this` for reusing quantization methods
    APyFixed fixed(_bits, _int_bits);

    // For each scalar in the tensor...
    for (std::size_t i = 0; i < fold_shape(_shape); i++) {

        // Copy data into temporary `APyFixed`
        std::copy_n(
            _data.begin() + i * _itemsize, // src
            _itemsize,                     // limbs to copy
            fixed._data.begin()            // dst
        );

        // Perform the resizing
        fixed.cast_correct_wl(
            result._data.begin() + (i + 0) * result._itemsize, // output start
            result._data.begin() + (i + 1) * result._itemsize, // output sentinel
            new_bits,
            new_int_bits
        );
    }

    return result;
}

void APyFixedArray::_cast(
    std::vector<mp_limb_t>::iterator it_begin,
    std::vector<mp_limb_t>::iterator it_end,
    int new_bits,
    int new_int_bits,
    QuantizationMode quantization,
    OverflowMode overflow
) const
{
    // `APyFixed` with the same word length as `*this` for reusing quantization methods
    APyFixed fixed(_bits, _int_bits);

    // For each scalar in the tensor...
    for (std::size_t i = 0; i < fold_shape(_shape); i++) {

        // Copy data into temporary `APyFixed`
        std::copy_n(
            _data.begin() + i * _itemsize, // src
            _itemsize,                     // limbs to copy
            fixed._data.begin()            // dst
        );

        // Perform the resizing
        std::size_t result_itemsize = bits_to_limbs(new_bits);
        fixed._cast(
            it_begin + (i + 0) * result_itemsize, // output start
            it_begin + (i + 1) * result_itemsize, // output sentinel
            new_bits,
            new_int_bits,
            quantization,
            overflow
        );
    }
}
