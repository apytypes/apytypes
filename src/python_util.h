/*
 * Python utility functions and helpers
 */

#ifndef _PYTHON_UTIL_H
#define _PYTHON_UTIL_H

// Python object access through Pybind
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/string.h>

// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h>

// Standard includes
#include <cassert>     // assert
#include <cstddef>     // offsetof
#include <optional>    // std::optional, std::nullopt
#include <stack>       // std::stack
#include <stdexcept>   // std::runtime_error
#include <string_view> // std::stirng_view
#include <vector>      // std::vector

// Custom includes
#include "apytypes_mp.h"
#include "apytypes_util.h"

/*
 * Access of information of PyLongObjects (Python arbitrary length integer objects).
 * -------------------------------------------------------------------------------------
 * Courtesy of `gmpy2`, specifically: gmpy2/src/gmpy2_convert_gmp.c
 *   `https://github.com/aleaxit/gmpy/blob/master/src/gmpy2_convert.h`
 */

#if PY_VERSION_HEX >= 0x030C0000 // Python version >= 3.12.0
#define TAG_FROM_SIGN_AND_SIZE(is_neg, size)                                           \
    ((is_neg ? 2 : (size == 0)) | (((std::size_t)size) << 3))
#define PyLong_SetSignAndDigitCount(obj, is_neg, size)                                 \
    (obj->long_value.lv_tag = TAG_FROM_SIGN_AND_SIZE(is_neg, size))
#elif PY_VERSION_HEX >= 0x030900A4 // Python version >= 3.9.x
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

#define WHOLE_BYTES (PYLONG_BITS_IN_DIGIT / POSIX_CHAR_BITS)
#define REMAINING_BITS (PYLONG_BITS_IN_DIGIT % POSIX_CHAR_BITS)
#define REMAINING_BITS_MASK ((((apy_limb_t)1) << REMAINING_BITS) - 1)
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
 * Convert Python limbs (of length PYLONG_BITS_IN_DIGIT) to apy_type_t limbs
 * (of length APY_LIMB_SIZE_BITS).
 */
static std::vector<apy_limb_t>
limb_vec_from_py_long_vec(const std::size_t count, const PyLongObject* py_long)
{
    auto data = GET_OB_DIGIT(py_long);
    constexpr std::size_t data_size = sizeof(data[0]);

    assert(data_size * POSIX_CHAR_BITS - PYLONG_BITS_IN_DIGIT > 0);

    std::size_t limb_vec_size = bits_to_limbs(count * PYLONG_BITS_IN_DIGIT);
    std::vector<apy_limb_t> limb_vec = std::vector<apy_limb_t>(limb_vec_size);
    apy_limb_t* limb_vec_tmp_ptr = &limb_vec[0];
    const int endian = HOST_ENDIAN;

    /* offset to get to the next word after processing WHOLE_BYTES and REMAINING_BITS */
    constexpr std::size_t woffset_const
        = (PYLONG_BITS_IN_DIGIT + POSIX_CHAR_BITS - 1) / POSIX_CHAR_BITS;
    apy_size_t woffset
        = (endian >= 0 ? data_size + woffset_const : data_size - woffset_const);

    /* least significant byte */
    unsigned char* dp = (unsigned char*)data + (endian >= 0 ? data_size - 1 : 0);

    apy_limb_t limb = 0;
    int lbits = 0;
    apy_limb_t byte;
    for (std::size_t i = 0; i < count; i++) {
        for (std::size_t j = 0; j < WHOLE_BYTES; j++) {
            byte = *dp;
            dp -= endian;
            assert(lbits < (int)APY_LIMB_SIZE_BITS);
            assert(limb <= (((apy_limb_t)1) << lbits) - 1);

            limb |= (apy_limb_t)byte << lbits;
            lbits += POSIX_CHAR_BITS;
            if (lbits >= (int)APY_LIMB_SIZE_BITS) {
                *limb_vec_tmp_ptr++ = limb & APY_NUMBER_MASK;
                lbits -= APY_LIMB_SIZE_BITS;
                assert(lbits < POSIX_CHAR_BITS);
                limb = byte >> (POSIX_CHAR_BITS - lbits);
            }
        }
        // Process remaining bits
        assert(REMAINING_BITS != 0);
        byte = *dp & REMAINING_BITS_MASK;
        dp -= endian;
        assert(lbits < (int)APY_LIMB_SIZE_BITS);
        assert(limb <= (((apy_limb_t)1) << lbits) - 1);

        limb |= (apy_limb_t)byte << lbits;
        lbits += REMAINING_BITS;
        if (lbits >= (int)APY_LIMB_SIZE_BITS) {
            *limb_vec_tmp_ptr++ = limb & APY_NUMBER_MASK;
            lbits -= APY_LIMB_SIZE_BITS;
            assert(lbits < REMAINING_BITS);
            limb = byte >> (REMAINING_BITS - lbits);
        }
        dp += woffset;
    }

    if (lbits != 0) {
        assert(lbits <= (int)APY_LIMB_SIZE_BITS);
        *limb_vec_tmp_ptr++ = limb;
    }

    assert(limb_vec_tmp_ptr == &limb_vec[0] + limb_vec_size);

    /* low byte of word after most significant */
    assert(
        dp
        == (unsigned char*)data + count * data_size
            + (endian >= 0 ? (apy_size_t)data_size - 1 : 0)
    );

    // Normalize
    while (!limb_vec.empty() > 0 && (limb_vec.back() == 0)) {
        limb_vec.pop_back();
    }
    return limb_vec;
}

/*!
 * Python arbitrary long integer object to apy_limb_t vector. If `n_min_limbs` is set,
 * at least that many limbs will be available in the result.
 */
[[maybe_unused]] static APY_INLINE std::vector<apy_limb_t> python_long_to_limb_vec(
    const nanobind::int_& py_long_int,
    std::optional<std::size_t> n_min_limbs = std::nullopt
)
{
    static_assert(
        PYLONG_BITS_IN_DIGIT <= APY_LIMB_SIZE_BITS,
        "`APY_LIMB_SIZE_BITS` need to at least as long as a single CPython digit"
    );

    const PyLongObject* py_long = (const PyLongObject*)py_long_int.ptr();
    const long py_long_digits = PyLong_DigitCount(py_long);

    std::vector<apy_limb_t> result;
    if (py_long_digits == 0) {
        // Python integer is zero
        result = { 0 };
    } else if (py_long_digits == 1) {
        // Python integer fits in one Python digit
        result = { GET_OB_DIGIT(py_long)[0] };
    } else {
        // Python integer is stored using multiple Python digits. Import data from
        // multi-digit Python long integer.
        std::vector<apy_limb_t> limb_vec = limb_vec_from_py_long_vec(
            py_long_digits, // Words to read
            py_long         // Source operand
        );

        // Compute how many limbs to copy
        std::size_t limb_copy_count
            = (n_min_limbs.has_value() ? std::min(*n_min_limbs, limb_vec.size())
                                       : limb_vec.size());

        // Copy limbs into a result-vector
        result = { limb_vec.begin(), limb_vec.begin() + limb_copy_count };
    }

    // Possibly extend the vector
    if (n_min_limbs.has_value() && *n_min_limbs > result.size()) {
        std::fill_n(std::back_inserter(result), *n_min_limbs - result.size(), 0);
    }

    // Negate limb vector if negative
    if (PyLong_IsNegative(py_long)) {
        limb_vector_negate_inplace(std::begin(result), std::end(result));
    }

    return result;
}

/*!
 * Convert a limb vector (`std::vector<apy_limb_t>`) to a Python long integer object
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
    bool sign = vec_is_signed ? apy_limb_signed_t(*std::prev(end)) < 0 : false;

    // Take absolute value of limb vector
    std::vector<apy_limb_t> limb_vec_abs(begin, end);
    if (sign) {
        limb_vector_negate_inplace(limb_vec_abs.begin(), limb_vec_abs.end());
    }

    // Zero bits outside of range if printing as positive and `bits_last_limb` is
    // specified
    if (!vec_is_signed && bits_last_limb.has_value()) {
        if (*bits_last_limb % APY_LIMB_SIZE_BITS) {
            limb_vec_abs.back()
                &= (apy_limb_t(1) << (*bits_last_limb % APY_LIMB_SIZE_BITS)) - 1;
        }
    }

    // Normalize
    while (!limb_vec_abs.empty() > 0 && (limb_vec_abs.back() == 0)) {
        limb_vec_abs.pop_back();
    }
    if (limb_vec_abs.size() == 0) {
        // Value is 0
        return nanobind::steal<nanobind::int_>((PyObject*)PyLong_New(0));
    }

    std::size_t limb_vec_size = limb_vec_abs.size();
    assert(limb_vec_size > 0);
    // Number of significant bits in the absolute value limb vector
    std::size_t significant_bits
        = APY_LIMB_SIZE_BITS * limb_vec_size - leading_zeros(limb_vec_abs.back());

    // Number of resulting Python digits in the Python long
    std::size_t python_digits
        = (significant_bits + PYLONG_BITS_IN_DIGIT - 1) / PYLONG_BITS_IN_DIGIT;

    PyLongObject* result = PyLong_New(python_digits);
    if (!result) {
        throw std::runtime_error("Could not allocate memory for Python long integer");
    }

    // Export the intermediate data to the Python integer
    // Relevant parts from mpz_export
    const std::size_t py_limb_size = sizeof(GET_OB_DIGIT(result)[0]);
    assert(py_limb_size * POSIX_CHAR_BITS - PYLONG_BITS_IN_DIGIT > 0);

    apy_limb_t* limb_vec_tmp_ptr = &limb_vec_abs[0];

    assert(limb_vec_tmp_ptr[limb_vec_size - 1] != 0);

    int endian = HOST_ENDIAN;

    /* offset to get to the next word */
    std::size_t woffset = (endian >= 0 ? 2 * py_limb_size : 0);

    /* least significant byte */
    unsigned char* dp = (unsigned char*)&GET_OB_DIGIT(result)[0]
        + (endian >= 0 ? py_limb_size - 1 : 0);
    const apy_limb_t* zend = limb_vec_tmp_ptr + limb_vec_size;
    int lbits = 0;
    apy_limb_t limb = 0;
    std::size_t j;
    for (std::size_t i = 0; i < python_digits; i++) {
        for (j = 0; j < WHOLE_BYTES; j++) {
            if (lbits >= POSIX_CHAR_BITS) {
                *dp = limb;
                limb >>= POSIX_CHAR_BITS;
                lbits -= POSIX_CHAR_BITS;
            } else {
                apy_limb_t newlimb
                    = (limb_vec_tmp_ptr == zend ? 0 : *limb_vec_tmp_ptr++);
                *dp = (limb | (newlimb << lbits));
                limb = newlimb >> (POSIX_CHAR_BITS - lbits);
                lbits += APY_LIMB_SIZE_BITS - POSIX_CHAR_BITS;
            }
            dp -= endian;
        }
        // Process remaining bits
        assert(REMAINING_BITS != 0);
        if (lbits >= REMAINING_BITS) {
            *dp = limb & REMAINING_BITS_MASK;
            limb >>= REMAINING_BITS;
            lbits -= REMAINING_BITS;
        } else {
            apy_limb_t newlimb = (limb_vec_tmp_ptr == zend ? 0 : *limb_vec_tmp_ptr++);
            *dp = (limb | (newlimb << lbits)) & REMAINING_BITS_MASK;
            limb = newlimb >> (REMAINING_BITS - lbits);
            lbits += APY_LIMB_SIZE_BITS - REMAINING_BITS;
        }

        dp -= endian;
        j++;
        // TODO: Unused?
        for (; j < py_limb_size; j++) {
            *dp = '\0';
            dp -= endian;
        }
        dp += woffset;
    }

    assert(limb_vec_tmp_ptr == &limb_vec_abs[0] + limb_vec_size);

    /* low byte of word after most significant */
    assert(
        dp
        == (unsigned char*)&GET_OB_DIGIT(result)[0] + python_digits * py_limb_size
            + (endian >= 0 ? (apy_size_t)py_limb_size - 1 : 0)
    );

    // End of relevant parts from mpz_export

    // If this ever triggers, normalize result by removing trailing zero digits
    assert(GET_OB_DIGIT(result)[python_digits - 1] != 0);

    PyLong_SetSignAndDigitCount(result, sign, python_digits);

    // Do a nanobind steal of the raw Python object and return
    return nanobind::steal<nanobind::int_>((PyObject*)result);
}

/*!
 * Test whether a `nb::object` is a Python iterable, and simultaneously that the object
 * is *not* any of the `PyArgs` types.
 */
template <typename... PyTypes>
[[maybe_unused]] static APY_INLINE bool
is_iterable_and_exclude(const nanobind::handle& obj)
{
    namespace nb = nanobind;
    return nb::isinstance<nb::iterable>(obj) && !(nb::isinstance<PyTypes>(obj) || ...);
}

/*!
 * Retrieve the sequence of a python sequence object through recursive descent along the
 * dimensions.
 */
[[maybe_unused]] static APY_INLINE std::vector<std::size_t>
_python_iterable_extract_shape_recursive_descent(
    const nanobind::iterable& sequence, std::string_view err_prefix
)
{
    namespace nb = nanobind;
    const auto inhomogenous_shape_err = [name = err_prefix]() -> std::domain_error {
        return std::domain_error(fmt::format("{}: inhomogeneous sequence shape", name));
    };

    if (sequence.begin() == sequence.end()) {
        // An empty sequence constitutes one dimension with shape zero
        return { 0 };
    } else if (is_iterable_and_exclude<nb::str, nb::set, nb::dict>(*sequence.begin())) {
        // First element along this dimension is another sequence. Make sure all
        // elements along this dimensions are also sequences and recursively evaluate
        // their shapes.
        std::vector<std::vector<std::size_t>> shapes {};
        for (auto&& element : sequence) {
            if (!nb::isinstance<nb::iterable>(element)) {
                throw inhomogenous_shape_err();
            }

            auto next = nb::cast<nb::iterable>(element);
            shapes.emplace_back(
                _python_iterable_extract_shape_recursive_descent(next, err_prefix)
            );
        }

        // Make sure all recursively found shapes are equal
        for (auto&& shape : shapes) {
            if (shape != shapes[0]) {
                throw inhomogenous_shape_err();
            }
        }

        // Return the recursive shape
        std::vector<std::size_t> result { std::size_t(shapes.size()) };
        result.insert(result.end(), shapes[0].begin(), shapes[0].end());
        return result;
    } else {
        // First element along this dimension is not a sequence. Make sure all elements
        // along this dimension are non-sequence.
        std::size_t sequence_len = 0;
        for (auto&& element : sequence) {
            if (is_iterable_and_exclude<nb::str, nb::set, nb::dict>(element)) {
                throw inhomogenous_shape_err();
            }
            sequence_len++;
        }

        // Return the size along this dimension
        return std::vector<std::size_t> { std::size_t(sequence_len) };
    }
}

/*!
 * Retrieve the shape of a possibly nested Python sequence of iterable object. When
 * `IS_COMPLEX_COLLAPSE` is true, the last dimension is truncated if it is exactly
 * equal to two.
 */
template <bool IS_COMPLEX_COLLAPSE = false>
[[maybe_unused]] static APY_INLINE std::vector<std::size_t>
python_iterable_extract_shape(
    const nanobind::iterable& seq, std::string_view err_prefix
)
{
    namespace nb = nanobind;
    std::vector<std::size_t> result;
    if (nb::isinstance<nb::ndarray<>>(seq)) {
        const auto& ndarray = nb::cast<nb::ndarray<>>(seq);
        result = std::vector<std::size_t>(ndarray.ndim());
        for (std::size_t i = 0; i < ndarray.ndim(); i++) {
            result[i] = ndarray.shape_ptr()[i];
        }
        assert(result.size());
    } else {
        result = _python_iterable_extract_shape_recursive_descent(seq, err_prefix);
        assert(result.size());
    }

    assert(result.size());
    if constexpr (IS_COMPLEX_COLLAPSE) {
        if (result.back() == 2) {
            result.pop_back();
            if (!result.size()) {
                result.push_back(1);
            }
        }
    }
    return result;
}

/*!
 * Walk a (possibly nested) Python iterable sequence, `py_seq`, of iterable objects and
 * store handles to them them in a `std::vector<nb::object>`. The sequence is walked in
 * a depth-first manner and all elements must match `(nb::isinstance<PyTypes> || ...)`.
 * If any object in the sequence `py_seq` does not match any `PyTypes` (or another
 * Python sequence) a `std::domain_error` exception is raised.
 */
template <typename... PyTypes>
[[maybe_unused]] static std::vector<nanobind::object>
python_iterable_walk(const nanobind::iterable& py_seq, std::string_view err_prefix)
{
    namespace nb = nanobind;

    // Walk the Python sequences and extract the data
    using iterator_t = decltype(std::begin(py_seq));
    struct seq_it_pair {
        iterator_t iterator;
        iterator_t sentinel;
    };
    std::stack<seq_it_pair, std::vector<seq_it_pair>> it_stack;
    it_stack.push({ py_seq.begin(), py_seq.end() });

    std::vector<nb::object> result {};
    while (!it_stack.empty()) {
        auto&& iterator = it_stack.top().iterator;
        auto&& sentinel = it_stack.top().sentinel;
        if (iterator == sentinel) {
            // End of current iterator/sentinel pair. Pop it.
            it_stack.pop();
        } else {
            using namespace nb;
            if (is_iterable_and_exclude<str, set, dict, PyTypes...>(*iterator)) {
                // New sequence found. We need to go deeper
                auto&& new_sequence = nb::cast<nb::iterable>(*iterator++);
                it_stack.push({ std::begin(new_sequence), std::end(new_sequence) });
            } else if ((nb::isinstance<PyTypes>(*iterator) || ...)) {
                // Element matching one of the PyTypes found, store it in container
                result.emplace_back(nb::cast<nb::object>(*iterator++));
            } else {
                auto&& err_obj = nb::cast<nb::object>(*iterator);
                std::string err_msg = fmt::format(
                    "{}: unexpected type when traversing iterable sequence: {}",
                    err_prefix,
                    nb::repr(nb::cast<nb::type_object>(err_obj.type())).c_str()
                );
                throw std::domain_error(err_msg);
            }
        }
    }

    // Return the result
    return result;
}

//! See: `python_iterable_walk()`
template <typename... PyTypes>
[[maybe_unused]] static std::vector<nanobind::object>
python_sequence_walk(const nanobind::sequence& py_seq, std::string_view err_prefix)
{
    namespace nb = nanobind;
    return python_iterable_walk<PyTypes...>(nb::cast<nb::iterable>(py_seq), err_prefix);
}

//! See: `python_iterable_extract_shape()`
template <bool IS_COMPLEX_COLLAPSE = false>
[[maybe_unused]] static APY_INLINE std::vector<std::size_t>
python_sequence_extract_shape(
    const nanobind::sequence& py_seq, std::string_view err_prefix
)
{
    namespace nb = nanobind;
    return python_iterable_extract_shape<IS_COMPLEX_COLLAPSE>(
        nb::cast<nb::iterable>(py_seq), err_prefix
    );
}

#endif // _PYTHON_UTIL_H
