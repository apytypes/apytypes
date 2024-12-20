#include "apycfloat.h"

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/variant.h>
#include <optional>
#include <variant>

namespace nb = nanobind;

/*
 * Binding function of a custom R-operator (e.g., `__rmul__`) with non APyCFloat type
 */
template <typename OP, typename L_TYPE>
static auto R_OP(const APyCFloat& rhs, const L_TYPE& lhs) -> decltype(OP()(rhs, rhs))
{
    [[maybe_unused]] int exp_bits = rhs.get_exp_bits();
    [[maybe_unused]] int man_bits = rhs.get_man_bits();
    [[maybe_unused]] exp_t bias = rhs.get_bias();
    if constexpr (std::is_floating_point_v<L_TYPE>) {
        return OP()(APyCFloat::from_double(lhs, exp_bits, man_bits, bias), rhs);
    } else {
        return OP()(APyCFloat::from_integer(lhs, exp_bits, man_bits, bias), rhs);
    }
}

/*
 * Binding function of a custom L-operator (e.g., `__sub__`) with non APyCFloat type
 */
template <typename OP, typename R_TYPE>
static auto L_OP(const APyCFloat& lhs, const R_TYPE& rhs) -> decltype(OP()(lhs, lhs))
{
    [[maybe_unused]] int exp_bits = lhs.get_exp_bits();
    [[maybe_unused]] int man_bits = lhs.get_man_bits();
    [[maybe_unused]] exp_t bias = lhs.get_bias();
    if constexpr (std::is_floating_point_v<R_TYPE>) {
        return OP()(lhs, APyCFloat::from_double(rhs, exp_bits, man_bits, bias));
    } else {
        return OP()(lhs, APyCFloat::from_integer(rhs, exp_bits, man_bits, bias));
    }
}

template <typename OP, typename TYPE>
static auto UN_OP(const TYPE& in) -> decltype(OP()(in))
{
    return OP()(in);
}

template <typename OP, typename L_TYPE, typename R_TYPE>
static auto BIN_OP(const L_TYPE& lhs, const R_TYPE& rhs) -> decltype(OP()(lhs, rhs))
{
    return OP()(lhs, rhs);
}

void bind_cfloat(nb::module_& m)
{
    nb::class_<APyCFloat>(m, "APyCFloat")
        /*
         * Constructor
         */
        .def(
            nb::init<
                std::variant<nb::bool_, nb::int_>,
                nb::int_,
                nb::int_,
                nb::int_,
                nb::int_,
                std::optional<nb::int_>>(),
            nb::arg("sign"),
            nb::arg("exp"),
            nb::arg("man"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyCFloat` object and initialize its real part.

            The imaginary part is zero initialized.

            Parameters
            ----------
            sign : :class:`bool` or :class:`int`
                Sign of real part. `True`/non-zero equals negative.
            exp : :class:`int`
                Exponent of real part as stored, i.e., actual value + bias.
            man : :class:`int`
                Mantissa of the float as stored, i.e., without a hidden one.
            exp_bits : :class:`int`
                Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Returns
            -------
            :class:`APyCFloat`
            )pbdoc"
        )

        .def(
            nb::init<
                nb::tuple,
                nb::tuple,
                nb::tuple,
                nb::int_,
                nb::int_,
                std::optional<nb::int_>>(),
            nb::arg("sign"),
            nb::arg("exp"),
            nb::arg("man"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyCFloat` object and initialize both real and imaginary
            part.

            Parameters
            ----------
            sign : :class:`tuple` of :class:`int` or :class:`bool`
                Sign of real/imaginary parts. `True`/non-zero equals negative.
            exp : :class:`tuple` of :class:`int`
                Exponent of real/imaginary parts as stored, i.e., actual value + bias.
            man : :class:`tuple` of :class:`int`
                Mantissa of the real/imaginary parts as stored, i.e., without a hidden
                one.
            exp_bits : :class:`int`
                Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Returns
            -------
            :class:`APyCFloat`
            )pbdoc"
        )

        /*
         * Arithmetic operations
         */
        .def(nb::self == nb::self)
        .def(nb::self != nb::self)

        .def(nb::self + nb::self)
        .def(nb::self - nb::self)
        .def(nb::self * nb::self)
        .def(nb::self / nb::self)
        .def(-nb::self)

        // .def("__pow__", &APyCFloat::pow)
        // .def("__pow__", &APyCFloat::pown)

        // .def("__and__", BIN_OP<std::bit_and<>, APyCFloat, APyCFloat>)
        // .def("__or__", BIN_OP<std::bit_or<>, APyCFloat, APyCFloat>)
        // .def("__xor__", BIN_OP<std::bit_xor<>, APyCFloat, APyCFloat>)
        // .def("__invert__", UN_OP<std::bit_not<>, APyCFloat>)

        /*
         * Arithmetic operations with integers
         */
        .def("__add__", L_OP<std::plus<>, nb::int_>, nb::is_operator())
        .def("__radd__", R_OP<std::plus<>, nb::int_>, nb::is_operator())
        .def("__sub__", L_OP<std::minus<>, nb::int_>, nb::is_operator())
        .def("__rsub__", R_OP<std::minus<>, nb::int_>, nb::is_operator())
        .def("__mul__", L_OP<std::multiplies<>, nb::int_>, nb::is_operator())
        .def("__rmul__", R_OP<std::multiplies<>, nb::int_>, nb::is_operator())
        .def("__truediv__", L_OP<std::divides<>, nb::int_>, nb::is_operator())
        .def("__rtruediv__", R_OP<std::divides<>, nb::int_>, nb::is_operator())

        /*
         * Arithmetic with floats
         */
        .def(nb::self == double())
        .def(nb::self != double())

        .def("__add__", L_OP<std::plus<>, double>, nb::is_operator())
        .def("__radd__", R_OP<std::plus<>, double>, nb::is_operator())
        .def("__sub__", L_OP<std::minus<>, double>, nb::is_operator())
        .def("__rsub__", R_OP<std::minus<>, double>, nb::is_operator())
        .def("__mul__", L_OP<std::multiplies<>, double>, nb::is_operator())
        .def("__rmul__", R_OP<std::multiplies<>, double>, nb::is_operator())
        .def("__truediv__", L_OP<std::divides<>, double>, nb::is_operator())
        .def("__rtruediv__", R_OP<std::divides<>, double>, nb::is_operator())

        /*
         * Arithmetic operations with APyFixed
         */
        .def("__eq__", BIN_OP<std::equal_to<>, APyCFloat, APyFloat>)
        .def("__ne__", BIN_OP<std::not_equal_to<>, APyCFloat, APyFloat>)

        /*
         * Get real and imaginary part
         */
        .def_prop_ro("real", &APyCFloat::get_real, R"pbdoc(
            Real part.

            Returns
            -------
            :class:`APyFixed`
            )pbdoc")
        .def_prop_ro("imag", &APyCFloat::get_imag, R"pbdoc(
            Imaginary part.

            Returns
            -------
            :class:`APyFixed`
            )pbdoc")

        /*
         * Conversion methods
         */
        .def_static(
            "from_complex",
            &APyCFloat::from_number,
            nb::arg("value"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyCFloat` object from an :class:`int`, :class:`float`, or
            :class:`complex`.

            The initialize floating-point value is the one closest to `value`. Ties are
            rounded using :class:`QuantizationMode.TIES_EVEN`.

            Parameters
            ----------
            value : :class:`complex`, :class:`float`, :class:`int`
                Value to initialize from.
            exp_bits : :class:`int`
                Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Examples
            --------

            >>> from apytypes import APyCFloat
            >>>
            >>> a = APyCFloat.from_complex(1.375, exp_bits=10, man_bits=15)
            >>> a
            APyCFloat(sign=(0, 0), exp=(511, 0), man=(12288, 0), exp_bits=10, man_bits=15)
            >>> str(a)
            '(1.375+0j)'

            Returns
            -------
            :class:`APyCFloat`

            See also
            --------
            from_complex
            )pbdoc"
        )
        .def_static(
            "from_float",
            &APyCFloat::from_number,
            nb::arg("value"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyCFloat` object from an :class:`int`, :class:`float`, or
            :class:`complex`.

            The initialize floating-point value is the one closest to `value`. Ties are
            rounded using :class:`QuantizationMode.TIES_EVEN`.

            Parameters
            ----------
            value : int, float, complex
                Value to initialize from.
            exp_bits : int
                Number of exponent bits.
            man_bits : int
                Number of mantissa bits.
            bias : int, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Examples
            --------

            >>> from apytypes import APyCFloat
            >>>
            >>> a = APyCFloat.from_float(1.25, exp_bits=10, man_bits=15)
            >>> a
            APyCFloat(sign=(0, 0), exp=(511, 0), man=(8192, 0), exp_bits=10, man_bits=15)
            >>> str(a)
            '(1.25+0j)'

            Returns
            -------
            :class:`APyCFloat`

            See also
            --------
            from_complex
            )pbdoc"
        )
        .def("__complex__", &APyCFloat::to_complex)
        // .def_static(
        //     "from_bits",
        //     &APyCFloat::from_bits,
        //     nb::arg("bits"),
        //     nb::arg("exp_bits"),
        //     nb::arg("man_bits"),
        //     nb::arg("bias") = nb::none(),
        //     R"pbdoc(
        //     Create an :class:`APyCFloat` object from a bit-representation.

        //     Parameters
        //     ----------
        //     bits : :class:`int`
        //         The bit-representation for the float.
        //     exp_bits : :class:`int`
        //         Number of exponent bits.
        //     man_bits : :class:`int`
        //         Number of mantissa bits.
        //     bias : :class:`int`, optional
        //         Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

        //     Examples
        //     --------

        //     >>> from apytypes import APyCFloat

        //     `a`, initialized to -1.5 from a bit pattern.

        //     >>> a = APyCFloat.from_bits(0b1_01111_10, exp_bits=5, man_bits=2)

        //     Returns
        //     -------
        //     :class:`APyCFloat`

        //     See also
        //     --------
        //     to_bits
        //     from_float
        //     )pbdoc"
        // )
        // .def("to_bits", &APyCFloat::to_bits, R"pbdoc(
        //     Get the bit-representation of an :class:`APyCFloat`.

        //     Examples
        //     --------

        //     >>> from apytypes import APyCFloat

        //     `a`, initialized to -1.5 from a bit pattern.

        //     >>> a = APyCFloat.from_bits(0b1_01111_10, exp_bits=5, man_bits=2)
        //     >>> a
        //     APyCFloat(sign=1, exp=15, man=2, exp_bits=5, man_bits=2)
        //     >>> a.to_bits() == 0b1_01111_10
        //     True

        //     Returns
        //     -------
        //     :class:`int`

        //     See also
        //     --------
        //     from_bits
        //     )pbdoc")
        .def("__str__", &APyCFloat::to_string, nb::arg("base") = 10)
        .def("__repr__", &APyCFloat::repr)
        // .def("_repr_latex_", &APyCFloat::latex)
        .def(
            "cast",
            &APyCFloat::cast,
            nb::arg("exp_bits") = nb::none(),
            nb::arg("man_bits") = nb::none(),
            nb::arg("bias") = nb::none(),
            nb::arg("quantization") = nb::none(),
            R"pbdoc(
            Change format of the complex-valued floating-point number.

            This is the primary method for performing quantization when dealing with
            APyTypes floating-point numbers.

            Parameters
            ----------
            exp_bits : :class:`int`, optional
                Number of exponent bits in the result.
            man_bits : :class:`int`, optional
                Number of mantissa bits in the result.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.
            quantization : :class:`QuantizationMode`, optional.
                Quantization mode to use in this cast. If None, use the global
                quantization mode.

            Returns
            -------
            :class:`APyCFloat`

            )pbdoc"
        )

        /*
         * Non-computational properties
         */
        .def_prop_ro("is_zero", &APyCFloat::is_zero, R"pbdoc(
            True if, and only if, both the real and imaginary parts are zero.

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def("is_identical", &APyCFloat::is_identical, nb::arg("other"), R"pbdoc(
            Test if two :py:class:`APyCFloat` objects are identical.

            Two :py:class:`APyCFloat` objects are considered identical if, and only
            if,  they have the same sign, exponent, mantissa, and format.

            Returns
            -------
            :class:`bool`
            )pbdoc")

        /*
         * Properties
         */
        .def_prop_ro("man_bits", &APyCFloat::get_man_bits, R"pbdoc(
            Number of mantissa bits.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("exp_bits", &APyCFloat::get_exp_bits, R"pbdoc(
            Number of exponent bits.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("bits", &APyCFloat::get_bits, R"pbdoc(
            Total number of bits.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("bias", &APyCFloat::get_bias, R"pbdoc(
            Exponent bias.

            Returns
            -------
            :class:`int`
            )pbdoc")

        // /*
        //  * Convenience methods
        //  */
        // .def(
        //     "cast_to_double",
        //     &APyCFloat::cast_to_double,
        //     nb::arg("quantization") = nb::none(),
        //     R"pbdoc(
        //     Cast to IEEE 754 binary64 (double-precision) format.

        //     Convenience method corresponding to

        //     .. code-block:: python

        //        f.cast(exp_bits=11, man_bits=52)

        //     Parameters
        //     ----------
        //     quantization : :class:`QuantizationMode`, optional
        //         Quantization mode to use. If not provided, the global mode,
        //         see :func:`get_float_quantization_mode`, is used.
        // )pbdoc"
        // )
        // .def(
        //     "cast_to_single",
        //     &APyCFloat::cast_to_single,
        //     nb::arg("quantization") = nb::none(),
        //     R"pbdoc(
        //     Cast to IEEE 754 binary32 (single-precision) format.

        //     Convenience method corresponding to

        //     .. code-block:: python

        //        f.cast(exp_bits=8, man_bits=23)

        //     Parameters
        //     ----------
        //     quantization : :class:`QuantizationMode`, optional
        //         Quantization mode to use. If not provided, the global mode,
        //         see :func:`get_float_quantization_mode`, is used.
        //     )pbdoc"
        // )
        // .def(
        //     "cast_to_half",
        //     &APyCFloat::cast_to_half,
        //     nb::arg("quantization") = nb::none(),
        //     R"pbdoc(
        //     Cast to IEEE 754 binary16 (half-precision) format.

        //     Convenience method corresponding to

        //     .. code-block:: python

        //        f.cast(exp_bits=5, man_bits=10)

        //     Parameters
        //     ----------
        //     quantization : :class:`QuantizationMode`, optional
        //         Quantization mode to use. If not provided, the global mode,
        //         see :func:`get_float_quantization_mode`, is used.
        //     )pbdoc"
        // )
        // .def(
        //     "cast_to_bfloat16",
        //     &APyCFloat::cast_to_bfloat16,
        //     nb::arg("quantization") = nb::none(),
        //     R"pbdoc(
        //     Cast to bfloat16 format.

        //     Convenience method corresponding to

        //     .. code-block:: python

        //        f.cast(exp_bits=8, man_bits=7)

        //     Parameters
        //     ----------
        //     quantization : :class:`QuantizationMode`, optional
        //         Quantization mode to use. If not provided, the global mode,
        //         see :func:`get_float_quantization_mode`, is used.
        //     )pbdoc"
        // )
        ;
}
