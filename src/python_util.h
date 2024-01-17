/*
 * Python utility functions and helpers
 */

// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h>

// Standard includes
#include <cstddef> // offsetof

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

/*
 * Creation of a new PyLongObject that can be returned to Python
 */
static inline PyLongObject* PyLong_New(std::size_t python_digits)
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
