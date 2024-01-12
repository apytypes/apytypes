/*
 * Python utility functions and helpers
 */

#include <Python.h>

/*
 * Access of information of PyLongObjects (Python arbitrary length integer objects).
 * -------------------------------------------------------------------------------------
 * Courtesy of `gmpy2`, specifically: gmpy2/src/gmpy2_convert_gmp.c
 *   `https://github.com/aleaxit/gmpy/blob/master/src/gmpy2_convert.h`
 */

#if PY_VERSION_HEX >= 0x030C0000 // Python version >= 3.12.0
#define GET_OB_DIGIT(obj) obj->long_value.ob_digit
#define _PyLong_IsNegative(obj) ((obj->long_value.lv_tag & 3) == 2)
#define _PyLong_DigitCount(obj) (obj->long_value.lv_tag >> 3)
#else // Python version < 3.12.0
#define GET_OB_DIGIT(obj) obj->ob_digit
#define _PyLong_IsNegative(obj) (Py_SIZE(obj) < 0)
#define _PyLong_DigitCount(obj) (_PyLong_IsNegative(obj) ? -Py_SIZE(obj) : Py_SIZE(obj))
#endif
