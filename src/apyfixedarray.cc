// Python object access through Pybind
#include "apyfixed.h"
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h> // PYLONG_BITS_IN_DIGIT, PyLongObject

// Standard header includes
#include <algorithm>  // std::copy, std::max, std::transform, etc...
#include <cstddef>    // std::size_t
#include <cstdlib>    // std::malloc, std::free
#include <cstring>    // std::memcpy
#include <functional> // std::bit_not
#include <iterator>   // std::back_inserter
#include <optional>   // std::optional
#include <sstream>    // std::stringstream
#include <stdexcept>  // std::domain_error
#include <string>     // std::string
#include <vector>     // std::vector, std::swap

#include "apyfixedarray.h"
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
        throw std::runtime_error("In APyFixedArray.__add__: shape missmatch");
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

APyFixedArray APyFixedArray::operator-(const APyFixedArray& rhs) const
{
    // Make sure `_shape` of `*this` and `rhs` are the same
    if (_shape != rhs._shape) {
        throw std::runtime_error("In APyFixedArray.__sub__: shape missmatch");
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

APyFixedArray APyFixedArray::operator*(const APyFixedArray& rhs) const
{
    // Make sure `_shape` of `*this` and `rhs` are the same
    if (_shape != rhs._shape) {
        throw std::runtime_error("In APyFixedArray.__mul__: shape missmatch");
    }

    const int res_int_bits = int_bits() + rhs.int_bits();
    const int res_frac_bits = frac_bits() + rhs.frac_bits();

    // Resulting `APyFixedArray` fixed-point tensor
    APyFixedArray result(_shape, res_int_bits + res_frac_bits, res_int_bits);

    // Perform multiplication for each element in the tensor. `mpn_mul` requires:
    // "The destination has to have space for `s1n` + `s2n` limbs, even if the product’s
    //  most significant limb is zero."
    std::vector<mp_limb_t> res_tmp_vec(_data.size() + rhs._data.size(), 0);
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

        // Retrieve the absolut value of both operands, as required by GMP
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
    if (_shape.size() == 1 && rhs._shape.size() == 1) {
        if (_shape[0] == rhs._shape[0]) {
            // Dimensionality for a standard scalar inner product checks out. Perform
            // the checked inner product.
            return _checked_inner_product(rhs);
        }
    }
    if (_shape.size() == 2 && rhs._shape.size() == 2) {
        if (_shape[1] == rhs._shape[0]) {
            // Dimensionality for a standard 2D matrix mutliplication checks out.
            // Perform the checked 2D matrix
            return _checked_2d_matmul(rhs);
        }
    }

    // Unsupported `__matmul__` dimensionality, raise exception
    throw std::runtime_error(
        std::string("APyFixedArray.__matmul__: input shape missmatch, ")
        + std::string("lhs: (") + string_from_vec(_shape) + "), "
        + std::string("rhs: (") + string_from_vec(rhs._shape) + ")"
    );
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
            std::vector<mp_limb_t> bcds = double_dabble(data);

            // The limbs can be converted to characters normally
            for (auto limb_it = bcds.crbegin(); limb_it != bcds.crend(); ++limb_it) {
                ss << *limb_it;
            }

            ss << ", ";
        }

        // Back to decimal printing
        ss << std::dec;

        ss.seekp(-2, ss.cur);
    }
    ss << "], shape=(";
    ss << string_from_vec(_shape);
    ss << "), "
       << "bits=" << bits() << ", "
       << "int_bits=" << int_bits() << ")";
    return ss.str();
}

// The shape of the array
pybind11::tuple APyFixedArray::shape() const
{
    py::tuple result(_shape.size());
    for (std::size_t i = 0; i < _shape.size(); i++) {
        result[i] = _shape[i];
    }
    return result;
}

// The dimension in the array
int APyFixedArray::ndim() const { return _shape.size(); }

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
    const pybind11::sequence& double_seq,
    std::optional<int> bits,
    std::optional<int> int_bits,
    std::optional<int> frac_bits
)
{
    APyFixedArray result(
        python_sequence_extract_shape(double_seq), bits, int_bits, frac_bits
    );

    // Extract all of the doubles
    std::vector<py::float_> doubles = python_sequence_walk<py::float_>(double_seq);

    // Set data from doubles (reuse `APyFixed::from_double` conversion)
    for (std::size_t i = 0; i < result._data.size() / bits_to_limbs(result.bits());
         i++) {
        double d = static_cast<double>(doubles[i]);
        auto fix = APyFixed::from_double(d, result.bits(), result.int_bits());
        std::copy_n(
            fix.read_data().begin(),                                // src
            bits_to_limbs(result.bits()),                           // limb count
            result._data.begin() + i * bits_to_limbs(result.bits()) // dst
        );
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
    for (std::size_t i = 0; i < result._data.size(); i += bits_to_limbs(new_bits)) {

        // Copy the scalar into an intermediate
        std::copy_n(
            _data.begin() + i * bits_to_limbs(new_bits), // src
            bits_to_limbs(new_bits),                     // number limbs to copy
            tmp.begin()                                  // dst
        );

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
    std::cout << "lsh._shape: (" << string_from_vec(this->_shape) << ")" << std::endl;
    std::cout << "rhs._shape: (" << string_from_vec(rhs._shape) << ")" << std::endl;

    // Resulting parameters
    std::vector<std::size_t> res_shape { _shape[0], rhs._shape[1] };
    const auto res_bits = bits() + rhs.bits() + bit_width(_shape[1] - 1);
    const auto res_int_bits = int_bits() + rhs.int_bits() + bit_width(_shape[1] - 1);

    // Resulting `APyFixedArray`
    APyFixedArray result(res_shape, res_bits, res_int_bits);

    std::cout << "res._shape: (" << string_from_vec(result._shape) << ")" << std::endl;

    for (std::size_t x = 0; x < res_shape[1]; x++) {

        // Copy column from `rhs` and use as the current working column. As reading
        // columns from `rhs` is cache-inefficient, we like to do this only once for
        // each element in the resulting matrix.
        APyFixedArray current_column({ rhs._shape[0] }, rhs.bits(), rhs.int_bits());
        for (std::size_t i = 0; i < rhs._shape[0]; i++) {
            current_column._data[i]
                = rhs._data[(x + i * rhs._shape[1]) * rhs._scalar_limbs()];
        }
        std::cout << "Column " << x << ": " << current_column.repr() << std::endl;

        for (std::size_t y = 0; y < res_shape[0]; y++) {

            // Current row from lhs (`*this`)
            APyFixedArray current_row({ _shape[1] }, bits(), int_bits());
            for (std::size_t i = 0; i < _shape[1]; i++) {
                current_row._data[i] = _data[(y * _shape[1] + i) * _scalar_limbs()];
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
                result._data.begin()
                    + (y * result._shape[1] + x) * result._scalar_limbs()
            );
        }
    }

    return result;
}
