// Python object access through Pybind
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
#include <cstdlib>   // std::malloc, std::free
#include <cstring>   // std::memcpy
#include <ios>       // std::dec
#include <numeric>   // std::accumulate
#include <optional>  // std::optional
#include <sstream>   // std::stringstream
#include <stdexcept> // std::domain_error
#include <string>    // std::string
#include <vector>    // std::vector, std::swap

#include <fmt/format.h>

#include "apyfixed.h"
#include "apyfixedarray.h"
#include "apyfixedarray_iterator.h"
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
    for (std::size_t i = 0; i < _data.size() / bits_to_limbs(_bits); i++) {

        // Two's complements overflowing in `APyFixed` constructor
        auto limb_vec = python_long_to_limb_vec(python_ints[i], bits_to_limbs(_bits));
        APyFixed fixed(_bits, _int_bits, limb_vec);
        std::copy_n(
            fixed.read_data().begin(),               // src
            bits_to_limbs(_bits),                    // limbs to copy
            _data.begin() + i * bits_to_limbs(_bits) // dst
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
    : _bits {}
    , _int_bits {}
    , _shape { shape }
    , _data {}
{
    set_bit_specifiers_from_optional(_bits, _int_bits, bits, int_bits, frac_bits);
    bit_specifier_sanitize_bits(_bits, _int_bits);
    _data = std::vector<mp_limb_t>(bits_to_limbs(_bits) * _fold_shape(), 0);
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
    APyFixedArray result = _bit_resize(res_int_bits + res_frac_bits, res_int_bits);
    APyFixedArray imm = rhs._bit_resize(res_int_bits + res_frac_bits, res_int_bits);

    // Perform addition
    for (std::size_t i = 0; i < result._data.size(); i += result._scalar_limbs()) {
        mpn_add_n(
            &result._data[i],      // dst
            &result._data[i],      // src1
            &imm._data[i],         // src2
            result._scalar_limbs() // limb vector length
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
    APyFixedArray result = _bit_resize(res_int_bits + res_frac_bits, res_int_bits);
    APyFixed imm = rhs.resize(res_int_bits + res_frac_bits, res_int_bits);

    // Perform addition
    for (std::size_t i = 0; i < result._data.size(); i += result._scalar_limbs()) {
        mpn_add_n(
            &result._data[i],      // dst
            &result._data[i],      // src1
            &imm._data[0],         // src2
            result._scalar_limbs() // limb vector length
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
    APyFixedArray result = _bit_resize(res_int_bits + res_frac_bits, res_int_bits);
    APyFixedArray imm = rhs._bit_resize(res_int_bits + res_frac_bits, res_int_bits);

    // Perform addition
    for (std::size_t i = 0; i < result._data.size(); i += result._scalar_limbs()) {
        mpn_sub_n(
            &result._data[i],      // dst
            &result._data[i],      // src1
            &imm._data[i],         // src2
            result._scalar_limbs() // limb vector length
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
    APyFixedArray result = _bit_resize(res_int_bits + res_frac_bits, res_int_bits);
    APyFixed imm = rhs.resize(res_int_bits + res_frac_bits, res_int_bits);

    // Perform addition
    for (std::size_t i = 0; i < result._data.size(); i += result._scalar_limbs()) {
        mpn_sub_n(
            &result._data[i],      // dst
            &result._data[i],      // src1
            &imm._data[0],         // src2
            result._scalar_limbs() // limb vector length
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

    // Perform multiplication for each element in the tensor. `mpn_mul` requires:
    // "The destination has to have space for `s1n` + `s2n` limbs, even if the product’s
    //  most significant limb is zero."
    std::vector<mp_limb_t> res_tmp_vec(_scalar_limbs() + rhs._scalar_limbs(), 0);
    for (std::size_t i = 0; i < _fold_shape(); i++) {
        // Current working operands
        auto op1_begin = _data.begin() + (i + 0) * _scalar_limbs();
        auto op1_end = _data.begin() + (i + 1) * _scalar_limbs();
        auto op2_begin = rhs._data.begin() + (i + 0) * rhs._scalar_limbs();
        auto op2_end = rhs._data.begin() + (i + 1) * rhs._scalar_limbs();

        // Evaluate resulting sign
        bool sign1 = mp_limb_signed_t(*(op1_end - 1)) < 0;
        bool sign2 = mp_limb_signed_t(*(op2_end - 1)) < 0;
        bool result_sign = sign1 ^ sign2;

        // Retrieve the absolute value of both operands, as required by GMP
        std::vector<mp_limb_t> op1_abs = limb_vector_abs(op1_begin, op1_end);
        std::vector<mp_limb_t> op2_abs = limb_vector_abs(op2_begin, op2_end);

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
            res_tmp_vec = limb_vector_negate(res_tmp_vec.begin(), res_tmp_vec.end());
        }

        // Copy into resulting vector
        std::copy_n(
            res_tmp_vec.begin(),
            result._scalar_limbs(),
            result._data.begin() + (i + 0) * result._scalar_limbs()
        );
    }

    return result;
}

APyFixedArray APyFixedArray::operator*(const APyFixed& rhs) const
{
    const int res_int_bits = int_bits() + rhs.int_bits();
    const int res_frac_bits = frac_bits() + rhs.frac_bits();

    // Resulting `APyFixedArray` fixed-point tensor
    APyFixedArray result(_shape, res_int_bits + res_frac_bits, res_int_bits);

    // Perform multiplication for each element in the tensor. `mpn_mul` requires:
    // "The destination has to have space for `s1n` + `s2n` limbs, even if the product’s
    //  most significant limb is zero."
    std::vector<mp_limb_t> res_tmp_vec(_scalar_limbs() + rhs.vector_size(), 0);
    for (std::size_t i = 0; i < _fold_shape(); i++) {
        // Current working operands
        auto op1_begin = _data.begin() + (i + 0) * _scalar_limbs();
        auto op1_end = _data.begin() + (i + 1) * _scalar_limbs();
        auto op2_begin = rhs._data.begin() + (0) * rhs.vector_size();
        auto op2_end = rhs._data.begin() + (1) * rhs.vector_size();

        // Evaluate resulting sign
        bool sign1 = mp_limb_signed_t(*(op1_end - 1)) < 0;
        bool sign2 = mp_limb_signed_t(*(op2_end - 1)) < 0;
        bool result_sign = sign1 ^ sign2;

        // Retrieve the absolute value of both operands, as required by GMP
        std::vector<mp_limb_t> op1_abs = limb_vector_abs(op1_begin, op1_end);
        std::vector<mp_limb_t> op2_abs = limb_vector_abs(op2_begin, op2_end);

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
            res_tmp_vec = limb_vector_negate(res_tmp_vec.begin(), res_tmp_vec.end());
        }

        // Copy into resulting vector
        std::copy_n(
            res_tmp_vec.begin(),
            result._scalar_limbs(),
            result._data.begin() + (i + 0) * result._scalar_limbs()
        );
    }

    return result;
}

APyFixedArray APyFixedArray::matmul(const APyFixedArray& rhs) const
{
    if (ndim() == 1 && rhs.ndim() == 1) {
        if (_shape[0] == rhs._shape[0]) {
            // Dimensionality for a standard scalar inner product checks out. Perform
            // the checked inner product.
            return _checked_inner_product(rhs);
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
                _data.begin() + (y * _shape[1] + x) * _scalar_limbs(),       // src
                _scalar_limbs(),                                             // limbs
                result._data.begin() + (x * _shape[0] + y) * _scalar_limbs() // dst
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

        std::size_t n_limbs = bits_to_limbs(bits());
        std::vector<mp_limb_t> data(n_limbs, 0);
        for (std::size_t offset = 0; offset < _data.size(); offset += n_limbs) {
            std::copy_n(_data.begin() + offset, n_limbs, data.begin());

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
pybind11::tuple APyFixedArray::shape() const
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
    std::size_t element_stride
        = std::accumulate(new_shape.begin(), new_shape.end(), 1, std::multiplies {});

    APyFixedArray result(new_shape, _bits, _int_bits);
    std::copy_n(
        _data.begin() + idx * _scalar_limbs() * element_stride,
        _scalar_limbs() * element_stride,
        result._data.begin()
    );
    return result;
}

py::array_t<double> APyFixedArray::to_numpy() const
{
    // Shape of NumPy object is same as `APyFixedArray` object
    std::vector<py::ssize_t> numpy_shape(_shape.begin(), _shape.end());

    // The strides of the NumPy object
    std::vector<py::ssize_t> numpy_stride(numpy_shape.size(), 0);
    for (std::size_t i = 0; i < numpy_shape.size(); i++) {
        numpy_stride[i] = std::accumulate(
            _shape.crbegin(), _shape.crbegin() + i, sizeof(double), std::multiplies {}
        );
    }
    std::reverse(numpy_stride.begin(), numpy_stride.end());

    // Resulting `NumPy` array of float64
    py::array_t<double, py::array::c_style> result(numpy_shape, numpy_stride);

    // Type-cast data and store in the NumPy array
    APyFixed imm(bits(), int_bits());
    double* numpy_data = result.mutable_data();
    for (std::size_t i = 0; i < _fold_shape(); i++) {
        std::copy_n(
            std::begin(_data) + i * _scalar_limbs(),
            _scalar_limbs(),
            std::begin(imm._data)
        );
        numpy_data[i] = double(imm);
    }

    return result;
}

bool APyFixedArray::is_identical(const APyFixedArray& other) const
{
    bool same_spec = (_shape == other._shape) && (bits() == other.bits())
        && (int_bits() == other.int_bits());

    return same_spec && (_data == other._data);
}

/* ********************************************************************************** *
 * *                      Static conversion from other types                          *
 * ********************************************************************************** */

APyFixedArray APyFixedArray::from_double(
    const pybind11::sequence& python_seq,
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
    for (std::size_t i = 0; i < result._data.size() / bits_to_limbs(result.bits());
         i++) {
        if (py::isinstance<py::float_>(py_obj[i])) {
            //
            // Python double object
            //
            double d = static_cast<double>(py::cast<py::float_>(py_obj[i]));
            auto fix = APyFixed::from_double(d, result.bits(), result.int_bits());
            std::copy_n(
                fix.read_data().begin(),                                // src
                bits_to_limbs(result.bits()),                           // limb count
                result._data.begin() + i * bits_to_limbs(result.bits()) // dst
            );
        } else if (py::isinstance<py::int_>(py_obj[i])) {
            //
            // Python integer object
            //
            auto max_bits = result.frac_bits() < 0
                ? result.bits() - result.frac_bits() // Negative fractional bits
                : result.bits();                     // Non-negative fractional bits
            auto limb_vec = python_long_to_limb_vec(
                py::cast<py::int_>(py_obj[i]), bits_to_limbs(max_bits)
            );
            APyFixed fixed(max_bits, max_bits, limb_vec);
            double d = double(fixed);
            auto fix = APyFixed::from_double(d, result.bits(), result.int_bits());
            std::copy_n(
                fix.read_data().begin(),                                // src
                bits_to_limbs(result.bits()),                           // limb count
                result._data.begin() + i * bits_to_limbs(result.bits()) // dst
            );
        }
    }

    return result;
}

/* ********************************************************************************** *
 * *                            Private member functions                            * *
 * ********************************************************************************** */

APyFixedArray APyFixedArray::_bit_resize(int new_bits, int new_int_bits) const
{
    APyFixedArray result(_shape, new_bits, new_int_bits);
    std::vector<mp_limb_t> tmp(bits_to_limbs(new_bits), 0);

    // For each scalar in the tensor...
    for (std::size_t i = 0; i < result._data.size() / bits_to_limbs(new_bits); i++) {

        // Copy the scalar into an intermediate
        std::copy_n(
            _data.begin() + i * bits_to_limbs(_bits),
            std::min(bits_to_limbs(new_bits), bits_to_limbs(_bits)),
            tmp.begin()
        );

        // Extend intermediate if bigger
        if (bits_to_limbs(new_bits) > bits_to_limbs(_bits)) {
            auto sentinel_limb_it = tmp.begin() + bits_to_limbs(_bits);
            bool is_negative = mp_limb_signed_t(*(sentinel_limb_it - 1)) < 0;
            std::fill_n(
                sentinel_limb_it,
                bits_to_limbs(new_bits) - bits_to_limbs(_bits),
                is_negative ? mp_limb_t(-1) : 0
            );
        }

        // Adjust binary point of intermediate
        if (frac_bits() <= result.frac_bits()) {
            limb_vector_lsl(tmp, result.frac_bits() - frac_bits());
        } else { // frac_bits() > result.frac_bits()
            limb_vector_asr(tmp, frac_bits() - result.frac_bits());
        }

        // Copy scalar into new tensor
        std::copy_n(
            tmp.begin(),                                       // src
            bits_to_limbs(new_bits),                           // number limbs to copy
            result._data.begin() + i * bits_to_limbs(new_bits) // dst
        );
    }

    return result;
}

std::size_t APyFixedArray::_fold_shape() const
{
    // Fold the shape over multiplication
    return std::accumulate(_shape.begin(), _shape.end(), 1, std::multiplies {});
}

// Evaluate the inner between two vectors. This method assumes that the the shape of
// both `*this` and `rhs` are equally long. Anything else is undefined behaviour.
APyFixedArray APyFixedArray::_checked_inner_product(const APyFixedArray& rhs) const
{
    const int res_bits = bits() + rhs.bits() + bit_width(_shape[0] - 1);
    const int res_int_bits = int_bits() + rhs.int_bits() + bit_width(_shape[0] - 1);
    if (_shape[0] == 0) {
        return APyFixedArray({ 0 }, res_bits, res_int_bits);
    }

    // Hadamard product of `*this` and `rhs`
    APyFixedArray hadamard = *this * rhs;
    APyFixedArray result({ 1 }, res_bits, res_int_bits);
    for (std::size_t i = 0; i < hadamard._fold_shape(); i++) {
        mpn_add(
            &result._data[0],                              // dst
            &result._data[0],                              // src1
            result._data.size(),                           // src1 limb length
            &hadamard._data[i * hadamard._scalar_limbs()], // src2
            hadamard._scalar_limbs()                       // src2 limb length
        );
    }
    return result;
}

// Evaluate the matrix product between two 2D matrices. This method assumes that the
// shape of `*this` and `rhs` have been checked to match a 2d matrix multiplication.
APyFixedArray APyFixedArray::_checked_2d_matmul(const APyFixedArray& rhs) const
{
    // Resulting parameters
    std::vector<std::size_t> res_shape = rhs._shape.size() > 1
        ? std::vector<std::size_t> { _shape[0], rhs._shape[1] } // rhs is 2-D
        : std::vector<std::size_t> { _shape[0] };               // rhs is 1-D
    const auto res_bits = bits() + rhs.bits() + bit_width(_shape[1] - 1);
    const auto res_int_bits = int_bits() + rhs.int_bits() + bit_width(_shape[1] - 1);
    const auto res_cols = rhs._shape.size() > 1 ? rhs._shape[1] : 1;

    // Resulting `APyFixedArray`
    APyFixedArray result(res_shape, res_bits, res_int_bits);

    for (std::size_t x = 0; x < res_cols; x++) {

        // Copy column from `rhs` and use as the current working column. As reading
        // columns from `rhs` is cache-inefficient, we like to do this only once for
        // each element in the resulting matrix.
        APyFixedArray current_column({ rhs._shape[0] }, rhs.bits(), rhs.int_bits());
        for (std::size_t col = 0; col < rhs._shape[0]; col++) {
            std::copy_n(
                rhs._data.begin() + (x + col * res_cols) * rhs._scalar_limbs(),
                rhs._scalar_limbs(),
                current_column._data.begin() + col * rhs._scalar_limbs()
            );
        }

        for (std::size_t y = 0; y < res_shape[0]; y++) {

            // Current row from lhs (`*this`)
            APyFixedArray current_row({ _shape[1] }, bits(), int_bits());
            for (std::size_t i = 0; i < _shape[1]; i++) {
                std::copy_n(
                    _data.begin() + (y * _shape[1] + i) * _scalar_limbs(),
                    _scalar_limbs(),
                    current_row._data.begin() + i * _scalar_limbs()
                );
            }

            // Perform the inner product
            APyFixedArray current_res
                = current_column._checked_inner_product(current_row);
            assert(current_res._shape == std::vector<std::size_t> { 1 });
            assert(current_res._scalar_limbs() == result._scalar_limbs());
            assert(current_res.bits() == result.bits());
            assert(current_res.int_bits() == result.int_bits());

            // Copy into the resulting vector
            std::copy_n(
                current_res._data.begin(), // src
                current_res._data.size(),  // limbs to copy
                result._data.begin() + (y * res_cols + x) * result._scalar_limbs()
            );
        }
    }

    return result;
}
