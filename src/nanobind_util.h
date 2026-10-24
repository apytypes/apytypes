#ifndef _NANOBIND_UTIL_H
#define _NANOBIND_UTIL_H

#include <nanobind/nanobind.h>

#include <functional>
#include <optional>

APY_INLINE void apply_fixed_spec(
    const nb::object& spec,
    std::optional<int>& int_bits,
    std::optional<int>& frac_bits,
    std::optional<int>& bits
)
{
    if (spec.is_none()) {
        return;
    }
    if (int_bits || frac_bits || bits) {
        throw nb::value_error(
            "`spec` cannot be combined with fixed-point bit-specifiers"
        );
    }
    if (!nb::hasattr(spec, "bits") || !nb::hasattr(spec, "int_bits")) {
        throw nb::type_error("`spec` must be a fixed-point scalar or array");
    }
    bits = nb::cast<int>(spec.attr("bits"));
    int_bits = nb::cast<int>(spec.attr("int_bits"));
}

template <typename Bias>
APY_INLINE void apply_float_spec(
    const nb::object& spec,
    std::optional<int>& exp_bits,
    std::optional<int>& man_bits,
    std::optional<Bias>& bias
)
{
    if (spec.is_none()) {
        return;
    }
    if (exp_bits || man_bits || bias) {
        throw nb::value_error(
            "`spec` cannot be combined with floating-point bit-specifiers"
        );
    }
    if (!nb::hasattr(spec, "exp_bits") || !nb::hasattr(spec, "man_bits")
        || !nb::hasattr(spec, "bias")) {
        throw nb::type_error("`spec` must be a floating-point scalar or array");
    }
    exp_bits = nb::cast<int>(spec.attr("exp_bits"));
    man_bits = nb::cast<int>(spec.attr("man_bits"));
    bias = nb::cast<Bias>(spec.attr("bias"));
}

APY_INLINE void require_float_bit_spec(
    const std::optional<int>& exp_bits, const std::optional<int>& man_bits
)
{
    if (!exp_bits || !man_bits) {
        throw nb::value_error(
            "Floating-point format requires `exp_bits` and `man_bits`"
        );
    }
}

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
