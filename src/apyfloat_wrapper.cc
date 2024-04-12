#include "apyfixed.h"
#include "apyfloat.h"
#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/optional.h>

namespace nb = nanobind;

void bind_float(nb::module_& m)
{
    nb::class_<APyFloat>(m, "APyFloat")
        /*
         * Constructor
         */
        .def(
            "__init__",
            &APyFloat::create_in_place,
            nb::arg("sign"),
            nb::arg("exp"),
            nb::arg("man"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none()
        )

        /*
         * Conversion methods
         */
        .def_static(
            "from_float",
            &APyFloat::from_double,
            nb::arg("value"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloat` object from a :class:`float`.

            Parameters
            ----------
            float_sequence : float
                Floating-point value to initialize from.
            exp_bits : int
                Number of exponent bits.
            man_bits : int, optional
                Number of mantissa bits.
            bias : int, optional
                Bias.

            Returns
            -------
            :class:`APyFloat`

            Examples
            --------

            .. code-block:: python

                from apytypes import APyFloat

                # `a`, initialized from floating-point values.
                a = APyFloat.from_float(1.35, exp_bits=10, man_bits=15)
            )pbdoc"
        )
        .def("__float__", &APyFloat::operator double)
        .def_static(
            "from_bits",
            &APyFloat::from_bits,
            nb::arg("bits"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none()
        )
        .def("to_bits", &APyFloat::to_bits)
        .def("__str__", &APyFloat::str)
        .def("__repr__", &APyFloat::repr)
        .def("pretty_string", &APyFloat::pretty_string)
        .def("_repr_latex_", &APyFloat::latex)
        .def(
            "cast",
            &APyFloat::cast,
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            nb::arg("quantization") = nb::none(),
            R"pbdoc(
            Resize the floating-point number.

            This is the primary method for performing quantization when dealing with
            APyTypes floating-point numbers.

            Parameters
            ----------
            exp_bits : int
                Number of exponent bits in the result.
            man_bits : int
                Number of mantissa bits in the result.
            bias : int, optional
                Bias used in the result.
            quantization : :class:`QuantizationMode`, optional.
                Quantization mode to use in this cast. If None, use the global
                quantization mode.

            Returns
            -------
            :class:`APyFloat`

            )pbdoc"
        )
        /*
         * Arithmetic operators
         */
        .def(nb::self + nb::self)
        .def(-nb::self)
        .def(nb::self - nb::self)
        .def(nb::self * nb::self)
        .def(nb::self / nb::self)
        .def(
            "__add__",
            [](const APyFloat& a, int b) {
                if (b == 0) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot add with int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFloat& a, int b) {
                if (b == 0) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot add with int");
                };
            },
            nb::is_operator()
        )

        .def(
            "__add__",
            [](const APyFloat& a, float b) {
                if (b == 0.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot add with float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFloat& a, float b) {
                if (b == 0.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot add with float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__sub__",
            [](const APyFloat& a, int b) {
                if (b == 0) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot subtract with int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFloat& a, int b) {
                if (b == 0) {
                    return -a;
                } else {
                    throw nanobind::type_error("Cannot subtract with int");
                };
            },
            nb::is_operator()
        )

        .def(
            "__sub__",
            [](const APyFloat& a, float b) {
                if (b == 0.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot subtract with float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFloat& a, float b) {
                if (b == 0.) {
                    return -a;
                } else {
                    throw nanobind::type_error("Cannot subtract with float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__mul__",
            [](const APyFloat& a, int b) {
                if (b == 1) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot multiply with int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFloat& a, int b) {
                if (b == 1) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot multiply with int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__mul__",
            [](const APyFloat& a, float b) {
                if (b == 1.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot multiply with float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFloat& a, float b) {
                if (b == 1.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot multiply with float");
                };
            },
            nb::is_operator()
        )

        .def("__abs__", &APyFloat::abs)
        .def("__pow__", &APyFloat::pow)
        .def("__pow__", &APyFloat::pown)

        /*
         * Binary comparison operators
         */
        // APyFloat
        .def(nb::self == nb::self)
        .def(nb::self != nb::self)
        .def(nb::self < nb::self)
        .def(nb::self > nb::self)
        .def(nb::self <= nb::self)
        .def(nb::self >= nb::self)

        // float, double
        .def(nb::self == float())
        .def(nb::self != float())
        .def(nb::self < float())
        .def(nb::self > float())
        .def(nb::self <= float())
        .def(nb::self >= float())

        // APyFixed
        .def(
            "__eq__",
            [](const APyFloat& a, const APyFixed& b) { return a == b; },
            nb::is_operator()
        )
        .def(
            "__ne__",
            [](const APyFloat& a, const APyFixed& b) { return a != b; },
            nb::is_operator()
        )
        .def(
            "__le__",
            [](const APyFloat& a, const APyFixed& b) { return a <= b; },
            nb::is_operator()
        )
        .def(
            "__lt__",
            [](const APyFloat& a, const APyFixed& b) { return a < b; },
            nb::is_operator()
        )
        .def(
            "__ge__",
            [](const APyFloat& a, const APyFixed& b) { return a >= b; },
            nb::is_operator()
        )
        .def(
            "__gt__",
            [](const APyFloat& a, const APyFixed& b) { return a > b; },
            nb::is_operator()
        )

        /*
         * Logic operators
         */
        .def(
            "__and__", [](APyFloat& a, APyFloat& b) { return a & b; }, nb::is_operator()
        )
        .def(
            "__or__", [](APyFloat& a, APyFloat& b) { return a | b; }, nb::is_operator()
        )
        .def(
            "__xor__", [](APyFloat& a, APyFloat& b) { return a ^ b; }, nb::is_operator()
        )
        .def(
            "__invert__", [](APyFloat& a) { return ~a; }, nb::is_operator()
        )

        /*
         * Non-computational properties
         */
        .def_prop_ro("is_zero", &APyFloat::is_zero, R"pbdoc(
            True if and only if value is zero.

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_prop_ro("is_normal", &APyFloat::is_normal, R"pbdoc(
            True if and only if value is normal (not zero, subnormal, infinite, or NaN).

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_prop_ro("is_subnormal", &APyFloat::is_subnormal, R"pbdoc(
            True if and only if value is subnormal.

            Zero is considered subnormal.

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_prop_ro("is_finite", &APyFloat::is_finite, R"pbdoc(
            True if and only if value is zero, subnormal, or normal.

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_prop_ro("is_nan", &APyFloat::is_nan, R"pbdoc(
            True if and only if value is NaN.

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_prop_ro("is_inf", &APyFloat::is_inf, R"pbdoc(
            True if and only if value is infinite.

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def("is_identical", &APyFloat::is_identical, nb::arg("other"), R"pbdoc(
            Test if two :py:class:`APyFloat` objects are identical.

            Two :py:class:`APyFloat` objects are considered identical if, and only if,  they have
            the same sign, exponent, mantissa, and format.

            Returns
            -------
            :class:`bool`
            )pbdoc")

        /*
         * Properties
         */
        .def_prop_ro("sign", &APyFloat::get_sign, R"pbdoc(
            Sign bit.

            See also
            --------
            true_sign

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_prop_ro(
            "true_sign", [](APyFloat& a) { return a.get_sign() ? -1 : 1; }, R"pbdoc(
            Sign value.

            See also
            --------
            sign

            Returns
            -------
            :class:`int`
            )pbdoc"
        )
        .def_prop_ro("man", &APyFloat::get_man, R"pbdoc(
            Mantissa bits.

            These are without a possible hidden one.

            See also
            --------
            true_man

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("true_man", &APyFloat::true_man, R"pbdoc(
            Mantissa value.

            These are with a possible hidden one.

            See also
            --------
            man

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("exp", &APyFloat::get_exp, R"pbdoc(
            Exponent bits with bias.

            See also
            --------
            true_exp

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("true_exp", &APyFloat::true_exp, R"pbdoc(
            Exponent value.

            The bias value is subtracted and exponent adjusted in case of
            a subnormal number.

            See also
            --------
            exp

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("bias", &APyFloat::get_bias, R"pbdoc(
            Exponent bias.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("man_bits", &APyFloat::get_man_bits, R"pbdoc(
            Number of mantissa bits.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("exp_bits", &APyFloat::get_exp_bits, R"pbdoc(
            Number of exponent bits.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("bits", &APyFloat::get_bits, R"pbdoc(
            Total number of bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        /*
         * Convenience methods
         */
        .def(
            "cast_to_double",
            &APyFloat::cast_to_double,
            nb::arg("quantization") = nb::none(),
            R"pbdoc(
            Cast to IEEE 754 binary64 (double-precision) format.

            Convenience method corresponding to

            .. code-block:: python

               f.cast(exp_bits=11, man_bits=52)

            Parameters
            ----------

            quantization : :class:`QuantizationMode`, optional
                Quantization mode to use. If not provided, the global mode,
                see :func:`get_quantization_mode`, is used.
        )pbdoc"
        )
        .def(
            "cast_to_single",
            &APyFloat::cast_to_single,
            nb::arg("quantization") = nb::none(),
            R"pbdoc(
            Cast to IEEE 754 binary32 (single-precision) format.

            Convenience method corresponding to

            .. code-block:: python

               f.cast(exp_bits=8, man_bits=23)

            Parameters
            ----------

            quantization : :class:`QuantizationMode`, optional
                Quantization mode to use. If not provided, the global mode,
                see :func:`get_quantization_mode`, is used.
            )pbdoc"
        )
        .def(
            "cast_to_half",
            &APyFloat::cast_to_half,
            nb::arg("quantization") = nb::none(),
            R"pbdoc(
            Cast to IEEE 754 binary16 (half-precision) format.

            Convenience method corresponding to

            .. code-block:: python

               f.cast(exp_bits=5, man_bits=10)

            Parameters
            ----------

            quantization : :class:`QuantizationMode`, optional
                Quantization mode to use. If not provided, the global mode,
                see :func:`get_quantization_mode`, is used.
            )pbdoc"
        )
        .def(
            "cast_to_bfloat16",
            &APyFloat::cast_to_bfloat16,
            nb::arg("quantization") = nb::none(),
            R"pbdoc(
            Cast to bfloat16 format.

            Convenience method corresponding to

            .. code-block:: python

               f.cast(exp_bits=8, man_bits=7)

            Parameters
            ----------

            quantization : :class:`QuantizationMode`, optional
                Quantization mode to use. If not provided, the global mode,
                see :func:`get_quantization_mode`, is used.
            )pbdoc"
        );
}
