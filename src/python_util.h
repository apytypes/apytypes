/*
 * Python utility functions and helpers
 */

#ifndef _PYTHON_UTIL_H
#define _PYTHON_UTIL_H

// Python object access through Pybind
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h>

// Standard includes
#include <algorithm> // std::copy
#include <cassert>   // assert
#include <cstddef>   // offsetof
#include <cstring>   // std::memcpy
#include <numeric>   // std::accumulate
#include <optional>  // std::optional, std::nullopt
#include <stack>     // std::stack
#include <stdexcept> // std::runtime_error
#include <vector>    // std::vector

// Custom includes
#include "apytypes_util.h"

// GMP should be included after all other includes
#include "../extern/mini-gmp/mini-gmp.h"

/*
 * Access of information of PyLongObjects (Python arbitrary length integer objects).
 * -------------------------------------------------------------------------------------
 * Courtesy of `gmpy2`, specifically: gmpy2/src/gmpy2_convert_gmp.c
 *   `https://github.com/aleaxit/gmpy/blob/master/src/gmpy2_convert.h`
 */

#if PY_VERSION_HEX >= 0x030C0000
#define TAG_FROM_SIGN_AND_SIZE(is_neg, size)                                           \
    ((is_neg ? 2 : (size == 0)) | (((size_t)size) << 3))
#define PyLong_SetSignAndDigitCount(obj, is_neg, size)                                 \
    (obj->long_value.lv_tag = TAG_FROM_SIGN_AND_SIZE(is_neg, size))
#elif PY_VERSION_HEX >= 0x030900A4
#define PyLong_SetSignAndDigitCount(obj, is_neg, size)                                 \
    (Py_SET_SIZE(obj, (is_neg ? -1 : 1) * size))
#else
#define PyLong_SetSignAndDigitCount(obj, is_neg, size)                                 \
    (Py_SIZE(obj) = (is_neg ? -1 : 1) * size)
#endif

#if PY_VERSION_HEX >= 0x030C0000 // Python version >= 3.12.0
#define GET_OB_DIGIT(obj) obj->long_value.ob_digit
#define PyLong_IsNegative(obj) ((obj->long_value.lv_tag & 3) == 2)
#define PyLong_DigitCount(obj) (obj->long_value.lv_tag >> 3)
#else // Python version < 3.12.0
#define GET_OB_DIGIT(obj) obj->ob_digit
#define PyLong_IsNegative(obj) (Py_SIZE(obj) < 0)
#define PyLong_DigitCount(obj) (PyLong_IsNegative(obj) ? -Py_SIZE(obj) : Py_SIZE(obj))
#endif

/*!
 * Creation of a new PyLongObject that can be returned to Python
 */
[[maybe_unused]] static APY_INLINE PyLongObject* PyLong_New(std::size_t python_digits)
{
    std::size_t n_digits = python_digits ? python_digits : 1;

    // Allocate data for the new Python type
    PyLongObject* result = (PyLongObject*)PyObject_Malloc(
#if PY_VERSION_HEX >= 0x030C0000 // Python version >= 3.12.0
        offsetof(PyLongObject, long_value.ob_digit) + n_digits * sizeof(digit)
#else // Python version < 3.12.0
        offsetof(PyLongObject, ob_digit) + n_digits * sizeof(digit)
#endif
    );

    if (!result) {
        PyErr_NoMemory();
        return nullptr;
    }

    // Initialize object and return
    PyLong_SetSignAndDigitCount(result, python_digits != 0, python_digits);
    PyObject_Init((PyObject*)result, &PyLong_Type);
    GET_OB_DIGIT(result)[0] = 0;

    // Return the newly created Python integer
    return result;
}

/*!
 * Test if Python long integer (`nb::int_`) is negative
 */
[[maybe_unused]] static APY_INLINE bool
python_long_is_negative(const nanobind::int_& py_long_int)
{
    const PyLongObject* py_long = (const PyLongObject*)py_long_int.ptr();
    return PyLong_IsNegative(py_long);
}

/*!
 * Python arbitrary long integer object to GMP limb vector. If `n_min_limbs` is set, at
 * least that many limbs will be available in the result.
 */
[[maybe_unused]] static APY_INLINE std::vector<mp_limb_t> python_long_to_limb_vec(
    const nanobind::int_& py_long_int,
    std::optional<std::size_t> n_min_limbs = std::nullopt
)
{
    static_assert(
        PYLONG_BITS_IN_DIGIT <= _LIMB_SIZE_BITS,
        "`_LIMB_SIZE_BITS` need to at least as long as a single CPython digit"
    );

    const PyLongObject* py_long = (const PyLongObject*)py_long_int.ptr();
    long py_long_digits = PyLong_DigitCount(py_long);
    bool py_long_is_negative = PyLong_IsNegative(py_long);

    std::vector<mp_limb_t> result;
    if (py_long_digits == 0) {
        // Python integer is zero
        result = { 0 };
    } else if (py_long_digits == 1) {
        // Python integer fits in one Python digit
        result = { GET_OB_DIGIT(py_long)[0] };
    } else {
        // Python integer is stored using multiple Python digits. Import data from
        // multi-digit Python long integer.
        mpz_t mpz_from_py_long;
        mpz_init(mpz_from_py_long);
        mpz_import(
            mpz_from_py_long,                    // Destination operand
            py_long_digits,                      // Words to read
            -1,                                  // LSWord first
            sizeof(GET_OB_DIGIT(py_long)[0]),    // Word size in bytes
            0,                                   // Machine endianness
            sizeof(GET_OB_DIGIT(py_long)[0]) * 8 // Nail bits
                - PYLONG_BITS_IN_DIGIT,          //
            GET_OB_DIGIT(py_long)                // Source operand
        );

        // Compute how many limbs to copy
        std::size_t limb_copy_count {};
        if (n_min_limbs.has_value()) {
            limb_copy_count = std::min(*n_min_limbs, mpz_size(mpz_from_py_long));
        } else {
            limb_copy_count = mpz_size(mpz_from_py_long);
        }

        // Copy limbs into a result-vector
        result = std::vector<mp_limb_t>(limb_copy_count, 0);
        std::memcpy(
            &result[0],                       // dst
            mpz_limbs_read(mpz_from_py_long), // src
            limb_copy_count * _LIMB_SIZE_BYTES
        );

        // Clear MPZ resources
        mpz_clear(mpz_from_py_long);
    }

    // Possibly extend the vector
    if (n_min_limbs.has_value() && *n_min_limbs > result.size()) {
        std::fill_n(std::back_inserter(result), *n_min_limbs - result.size(), 0);
    }

    // Negate limb vector if negative
    if (py_long_is_negative) {
        limb_vector_negate(std::begin(result), std::end(result), std::begin(result));
    }

    return result;
}

/*!
 * Convert a limb vector (`std::vector<mp_limb_t>`) to a Python long integer object
 * wrapped in a `nanobind::int_`.
 */
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE nanobind::int_ python_limb_vec_to_long(
    RANDOM_ACCESS_ITERATOR begin,
    RANDOM_ACCESS_ITERATOR end,
    bool vec_is_signed = false,
    std::optional<unsigned> bits_last_limb = std::nullopt
)
{
    // Guard for empty vectors
    auto size = std::distance(begin, end);
    if (size <= 0) {
        return nanobind::steal<nanobind::int_>((PyObject*)PyLong_New(0));
    }

    // Extract sign of limb vector
    bool sign = vec_is_signed ? mp_limb_signed_t(*std::prev(end)) < 0 : false;

    // Take absolute value of limb vector
    std::vector<mp_limb_t> limb_vec_abs(begin, end);
    if (sign) {
        std::transform(
            limb_vec_abs.cbegin(),
            limb_vec_abs.cend(),
            limb_vec_abs.begin(),
            [](auto limb) { return ~limb; }
        );
        mpn_add_1(&limb_vec_abs[0], &limb_vec_abs[0], limb_vec_abs.size(), 1);
    }

    // Zero bits outside of range if printing as positive and `bits_last_limb` is
    // specified
    if (!vec_is_signed && bits_last_limb.has_value()) {
        if (*bits_last_limb % _LIMB_SIZE_BITS) {
            limb_vec_abs.back()
                &= (mp_limb_t(1) << (*bits_last_limb % _LIMB_SIZE_BITS)) - 1;
        }
    }

    // Number of significant bits in the absolute value limb vector
    std::size_t significant_bits = _LIMB_SIZE_BITS * limb_vec_abs.size()
        - limb_vector_leading_zeros(limb_vec_abs.begin(), limb_vec_abs.end());

    // Number of resulting Python digits in the Python long
    std::size_t python_digits
        = (significant_bits + PYLONG_BITS_IN_DIGIT - 1) / PYLONG_BITS_IN_DIGIT;

    // Intermediate GMP `mpz` variable for import and export
    mpz_t mpz_to_py_long;
    mpz_init(mpz_to_py_long);
    mpz_import(
        mpz_to_py_long,      // Destination operand
        limb_vec_abs.size(), // Words to read
        -1,                  // LSWord first
        sizeof(mp_limb_t),   // Word size in bytes
        0,                   // Machine endianness
        0,                   // Number of nail bits
        &limb_vec_abs[0]     // Source operand
    );

    PyLongObject* result = PyLong_New(python_digits);
    if (!result) {
        throw std::runtime_error("Could not allocate memory for Python long integer");
    }

    // Export the intermediate data to the python integer
    std::size_t words_written = 0;
    mpz_export(
        &GET_OB_DIGIT(result)[0],           // Destination operand
        &words_written,                     // Number of words written
        -1,                                 // LSWord first
        sizeof(GET_OB_DIGIT(result)[0]),    // Word size in bytes
        0,                                  // Machine endianness
        sizeof(GET_OB_DIGIT(result)[0]) * 8 // Nail bits
            - PYLONG_BITS_IN_DIGIT,         //
        mpz_to_py_long                      // Source operand
    );
    if (!words_written) {
        GET_OB_DIGIT(result)[0] = 0;
    }

    // Clear the GMP `mpz` intermediate and finalize the Python long integer
    mpz_clear(mpz_to_py_long);
    while (python_digits > 0 && (GET_OB_DIGIT(result)[python_digits - 1] == 0)) {
        python_digits--;
    }
    PyLong_SetSignAndDigitCount(result, sign, python_digits);

    // Do a nanobind steal of the raw Python object and return
    return nanobind::steal<nanobind::int_>((PyObject*)result);
}

/*!
 * Retrieve the shape of a, possibly nested, Python sequence of iterable object.
 */
[[maybe_unused]] static APY_INLINE std::vector<std::size_t>
python_sequence_extract_shape(const nanobind::sequence& bit_pattern_sequence)
{
    namespace nb = nanobind;

    // Compute the length along the first dimension of this sequence
    auto nanobind_sequence_distance = [](const nanobind::sequence& seq) -> std::size_t {
        std::size_t res = 0;
        auto first = seq.begin();
        auto last = seq.end();
        while (first != last) {
            ++first;
            ++res;
        }
        return res;
    };
    std::size_t sequence_len = nanobind_sequence_distance(bit_pattern_sequence);

    // Early exit
    if (sequence_len == 0) {
        // Empyt Python sequence, array shape is ( 0, )
        return { 0 };
    }

    auto first_element_it = bit_pattern_sequence.begin();
    if (nb::isinstance<nb::str>(*first_element_it)) {
        // First element along this dimension is a string. We currently do not support
        // having strings sequence array structures
        throw std::domain_error(
            "python_sequence_extract_shape(): found string when extracting shape, "
            "which is currently unsupported"
        );
    } else if (nb::isinstance<nb::sequence>(*first_element_it)) {
        // First element along this dimension is another sequence. Make sure all
        // elements along this dimensions are also sequences and recursively evaluate
        // their shapes.
        std::vector<std::vector<std::size_t>> recursive_shapes;
        for (auto element : bit_pattern_sequence) {
            if (!nb::isinstance<nb::sequence>(element)) {
                // Non-sequence detected along dimension of sequences
                throw std::domain_error("Inhomogeneous sequence shape");
            }

            recursive_shapes.push_back(
                python_sequence_extract_shape(nb::cast<nb::sequence>(element))
            );
        }

        // Make sure all recursively found shapes are equal
        for (const auto& shape : recursive_shapes) {
            if (shape != recursive_shapes[0]) {
                // Inhomogeneous detected
                throw std::domain_error("Inhomogeneous sequence shape");
            }
        }

        // Return the recursive shape
        assert(sequence_len > 0);
        std::vector<std::size_t> result { std::size_t(sequence_len) };
        result.insert(
            result.end(), recursive_shapes[0].begin(), recursive_shapes[0].end()
        );
        return result;
    } else {
        // First element along this dimension is not a sequence. Make sure all elements
        // along this dimension are non-sequence.
        for (auto element : bit_pattern_sequence) {
            if (nb::isinstance<nb::sequence>(element)) {
                // Sequence detected along dimension of non-sequence
                throw std::domain_error("Inhomogeneous sequence shape");
            }
        }

        // Return the size along this dimension
        assert(sequence_len > 0);
        return std::vector<std::size_t> { std::size_t(sequence_len) };
    }
}

/*!
 * Walk a, possibly nested, Python sequence of iterable objects and convert every Python
 * object (of type `<T>`, via `nb::cast<T>()`) and return them in a `std::vector<T>`.
 * The sequence is walked in a depth-first search manner. If any object in the sequence
 * `bit_pattern_sequence` does not match `<T>` or another Python sequence a
 * `std::domain_error` exception to be raised.
 */
template <typename... PyTypes>
[[maybe_unused]] static std::vector<nanobind::object>
python_sequence_walk(const nanobind::sequence& py_seq)
{
    namespace nb = nanobind;

    // Result output iterator
    std::vector<nb::object> result {};

    // Walk the Python sequences and extract the data
    struct seq_it_pair {
        decltype(std::begin(py_seq)) iterator;
        decltype(std::end(py_seq)) sentinel;
    };
    std::stack<seq_it_pair, std::vector<seq_it_pair>> it_stack;
    it_stack.push({ py_seq.begin(), py_seq.end() });

    while (!it_stack.empty()) {
        if (it_stack.top().iterator == it_stack.top().sentinel) {
            // End of current iterator/sentinel pair. Pop it.
            it_stack.pop();
        } else {
            if (nb::isinstance<nb::sequence>(*it_stack.top().iterator)) {
                // New sequence found. We need to go deeper
                auto new_sequence = nb::cast<nb::sequence>(*it_stack.top().iterator++);
                it_stack.push({ new_sequence.begin(), new_sequence.end() });
            } else if ((nb::isinstance<PyTypes>(*it_stack.top().iterator) || ...)) {
                // Element matching one of the PyTypes found, store it in container
                result.push_back(nb::cast<nb::object>(*it_stack.top().iterator++));
            } else {
                nb::object obj = nb::cast<nb::object>(*it_stack.top().iterator);
                nb::type_object type = nb::cast<nb::type_object>(obj.type());
                nb::str type_string = nb::str(type);
                nb::str repr = nb::repr(obj);
                std::string repr_string = repr.c_str();
                throw std::domain_error(
                    std::string("Non <type>/sequence found when walking: '")
                    + repr_string + "' of type: '" + type_string.c_str()
                );
            }
        }
    }

    // Return the result
    return result;
}

#endif // _PYTHON_UTIL_H
