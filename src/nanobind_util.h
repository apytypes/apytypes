#ifndef _NANOBIND_UTIL_H
#define _NANOBIND_UTIL_H

#include <nanobind/nanobind.h>

#include <functional>

//! Mark Python-exposed parameter which does *not* support implicit conversion on call.
//! An optional name parameter can be passed to overwrite the implicit parameter name.
#define NB_NARG(...) nb::arg(__VA_ARGS__).noconvert()

//! Mark a special double-underscore nanobind operator (e.g., `__add__`) that implements
//! an arithmetic operation. In Python, when a bound function with this annotation is
//! called with incompatible arguments, it will return `NotImplemented` rather than
//! raising a `TypeError` which is the default. No implicit conversion will be attempted
//! on the operand (`NB_NARG()`).
#define NB_OP(...) nb::is_operator(), NB_NARG(__VA_ARGS__)

//! Short-hand C++ arithmetic functors with Python dunder-function naming scheme.
#define STD_ADD std::plus
#define STD_SUB std::minus
#define STD_MUL std::multiplies
#define STD_DIV std::divides
#define STD_EQ std::equal_to
#define STD_NE std::not_equal_to

#endif
