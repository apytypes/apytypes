// Python object access through Pybind
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
#include <iostream>
#include <iterator>  // std::back_inserter
#include <optional>  // std::optional
#include <sstream>   // std::stringstream
#include <stdexcept> // std::domain_error
#include <string>    // std::string
#include <vector>    // std::vector, std::swap

#include "apyfixedarray.h"
#include "apytypes_util.h"
#include "ieee754.h"
#include "python_util.h"

// GMP should be included after all other includes
#include <gmp.h>

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
    // Currently we only support initialization from Python ints
    std::size_t limbs_per_element = bits_to_limbs(_bits);
    _data = python_sequence_walk_ints(bit_pattern_sequence, _shape, limbs_per_element);

    ///// TODO: Two's complement overflowing
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
    _data = std::vector<mp_limb_t>(
        bits_to_limbs(_bits)
            * std::accumulate(_shape.begin(), _shape.end(), 1, std::multiplies {}),
        0
    );
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

        // mpz_clear(tmp);
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
