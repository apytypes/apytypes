#include "apyfloat.h"
#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

namespace py = nanobind;

void bind_float(py::module_& m)
{
    py::class_<APyFloat>(m, "APyFloat")
        /*
         * Constructors
         */
        .def(
            py::init<
                bool,
                exp_t,
                man_t,
                std::uint8_t,
                std::uint8_t,
                std::optional<exp_t>>(),
            py::arg("sign"),
            py::arg("exp"),
            py::arg("man"),
            py::arg("exp_bits"),
            py::arg("man_bits"),
            py::arg("bias") = std::nullopt
        )

        /*
         * Conversion methods
         */
        .def_static(
            "from_float",
            &APyFloat::from_double,
            py::arg("value"),
            py::arg("exp_bits"),
            py::arg("man_bits"),
            py::arg("bias") = std::nullopt,
            py::arg("quantization") = std::nullopt,
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
            quantization : :class:`QuantizationMode`, optional
                Quantization mode to use. If not provided, the global mode,
                see :func:`get_quantization_mode`, is used.

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
            py::arg("bits"),
            py::arg("exp_bits"),
            py::arg("man_bits"),
            py::arg("bias") = std::nullopt
        )
        .def("to_bits", &APyFloat::to_bits)
        .def("__str__", &APyFloat::str)
        .def("__repr__", &APyFloat::repr)
        .def("pretty_string", &APyFloat::pretty_string)
        .def("_repr_latex_", &APyFloat::latex)
        .def(
            "cast",
            &APyFloat::cast,
            py::arg("exp_bits"),
            py::arg("man_bits"),
            py::arg("bias") = std::nullopt,
            py::arg("quantization") = std::nullopt
        )
        // Deprecated, but keep around for paper review
        .def(
            "resize",
            &APyFloat::resize,
            py::arg("exp_bits"),
            py::arg("man_bits"),
            py::arg("bias") = std::nullopt,
            py::arg("quantization") = std::nullopt,
            R"pbdoc(
            .. deprecated:: 0.1.pre
               Use :func:`~APyFloat.cast` instead.
            )pbdoc"

        )
        /*
         * Arithmetic operators
         */
        .def(py::self + py::self)
        .def(-py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)

        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self < py::self)
        .def(py::self > py::self)
        .def(py::self <= py::self)
        .def(py::self >= py::self)

        .def(py::self == float())
        .def(py::self != float())
        .def(py::self < float())
        .def(py::self > float())
        .def(py::self <= float())
        .def(py::self >= float())
        .def(
            "__add__",
            [](const APyFloat& a, int b) {
                if (b == 0) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot add with int");
                };
            },
            py::is_operator()
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
            py::is_operator()
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
            py::is_operator()
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
            py::is_operator()
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
            py::is_operator()
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
            py::is_operator()
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
            py::is_operator()
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
            py::is_operator()
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
            py::is_operator()
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
            py::is_operator()
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
            py::is_operator()
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
            py::is_operator()
        )

        .def("__abs__", &APyFloat::abs)
        .def("__pow__", &APyFloat::pow)
        .def("__pow__", &APyFloat::pown)

        .def(
            "__and__", [](APyFloat& a, APyFloat& b) { return a & b; }, py::is_operator()
        )
        .def(
            "__or__", [](APyFloat& a, APyFloat& b) { return a | b; }, py::is_operator()
        )
        .def(
            "__xor__", [](APyFloat& a, APyFloat& b) { return a ^ b; }, py::is_operator()
        )
        .def(
            "__invert__", [](APyFloat& a) { return ~a; }, py::is_operator()
        )

        /*
         * Non-computational properties
         */
        .def_prop_ro("is_normal", &APyFloat::is_normal, R"pbdoc(
            True if and only if x is normal (not zero, subnormal, infinite, or NaN).

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_prop_ro("is_subnormal", &APyFloat::is_subnormal, R"pbdoc(
            True if and only if x is normal (not zero, subnormal, infinite, or NaN).

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_prop_ro("is_finite", &APyFloat::is_finite, R"pbdoc(
            True if and only if x is zero, subnormal, or normal.

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_prop_ro("is_nan", &APyFloat::is_nan, R"pbdoc(
            True if and only if x is NaN.

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_prop_ro("is_inf", &APyFloat::is_inf, R"pbdoc(
            True if and only if x is infinite.

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def("is_identical", &APyFloat::is_identical, py::arg("other"), R"pbdoc(
            Test if two `APyFloat` objects are identical.

            Two `APyFixed` objects are considered identical if, and only if,  they have
            the same sign, exponent, mantissa, and format.

            Returns
            -------
            :class:`bool`
            )pbdoc")

        /*
         * Properties
         */
        .def_prop_ro("sign", &APyFloat::get_sign)
        .def_prop_ro("man", &APyFloat::get_man)
        .def_prop_ro("exp", &APyFloat::get_exp)
        .def_prop_ro("bias", &APyFloat::get_bias)
        .def_prop_ro("man_bits", &APyFloat::get_man_bits)
        .def_prop_ro("exp_bits", &APyFloat::get_exp_bits)

        /*
         * Convenience methods
         */
        .def(
            "cast_to_double",
            &APyFloat::cast_to_double,
            py::arg("quantization") = std::nullopt,
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
            py::arg("quantization") = std::nullopt,
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
            py::arg("quantization") = std::nullopt,
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
            py::arg("quantization") = std::nullopt,
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
