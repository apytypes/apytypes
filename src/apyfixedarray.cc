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
 * *                               Other methods                                    * *
 * ********************************************************************************** */

std::string APyFixedArray::repr() const
{
    std::stringstream ss {};
    ss << "APyFixedArray([";
    if (_shape[0]) {
        mpz_t tmp;
        mpz_init(tmp);
        for (std::size_t i = 0; i < _data.size(); i += bits_to_limbs(_bits)) {
            mpz_import(
                tmp,                  // Destination operand
                bits_to_limbs(_bits), // Words to read
                -1,                   // LSWord first
                sizeof(mp_limb_t),    // Word size in bytes
                0,                    // Machine endianness
                0,                    // Number of nail bits
                &_data[i]             // Source operand
            );

            // Allocation using `malloc()` happens in `gmp_asprintf()`, therefore we
            // need to free the memory using `free()`. See 10.2 Functions, of GMP
            // documentation.
            char* pp = nullptr;
            gmp_asprintf(&pp, "%Zd", tmp);
            ss << pp << ", ";
            std::free(pp);
        }
        mpz_clear(tmp);
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
