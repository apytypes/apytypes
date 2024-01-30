// Python object access through Pybind
#include "apyfixed.h"
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
#include "ieee754.h"
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
    ss << "], "
       << "shape=(";
    for (auto d : _shape) {
        ss << d << ", ";
    }
    ss.seekp(-2, ss.cur);
    ss << "), "
       << "bits=" << bits() << ", "
       << "int_bits=" << int_bits() << ")";
    return ss.str();
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
        APyFixed fix(d, result.bits(), result.int_bits());
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
