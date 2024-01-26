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
    const py::sequence& bit_pattern_list,
    std::optional<int> bits,
    std::optional<int> int_bits,
    std::optional<int> frac_bits
)
    : APyFixedArray(bits, int_bits, frac_bits)
{
    _shape = python_sequence_extract_shape(bit_pattern_list);

    // Currently we only support initialization from Python ints
    std::size_t limbs_per_element = bits_to_limbs(_bits);
    _data = python_sequence_walk_ints(bit_pattern_list, _shape, limbs_per_element);

    ///// TODO: Two's complement overflowing
}

/* ********************************************************************************** *
 * *                       More C++ accessible constructors                         * *
 * ********************************************************************************** */

APyFixedArray::APyFixedArray(
    std::optional<int> bits, std::optional<int> int_bits, std::optional<int> frac_bits
)
    : _bits {}
    , _int_bits {}
    , _shape()
    , _data()

{
    set_bit_specifiers_from_optional(_bits, _int_bits, bits, int_bits, frac_bits);
    bit_specifier_sanitize_bits(_bits, _int_bits);
}

/* ********************************************************************************** *
 * *                            Binary arithmetic operators                         * *
 * ********************************************************************************** */

APyFixedArray APyFixedArray::operator+(const APyFixedArray& rhs) const
{
    // Make sure `_shape` of `*this` and `rhs` are the same

    // Increase word length of result by one

    // Shift the binary point in position

    // Perform addition using `mpn_add_n` or `mpn_add`
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

        std::size_t n_limbs = bits_to_limbs(_bits);
        std::vector<mp_limb_t> data(n_limbs, 0);
        for (std::size_t offset = 0; offset < _data.size(); offset += n_limbs) {
            std::copy_n(_data.begin() + offset, n_limbs, data.begin());

            // Zero sign bits outside of bit-range
            if (_bits % _LIMB_SIZE_BITS) {
                mp_limb_t and_mask = (mp_limb_t(1) << (_bits % _LIMB_SIZE_BITS)) - 1;
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
       << "bits=" << _bits << ", "
       << "int_bits=" << _int_bits << ")";
    return ss.str();
}
