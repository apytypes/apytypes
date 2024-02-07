#include "apyfloat.h"
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void bind_float(py::module& m)
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
            py::arg("rounding_mode") = std::nullopt
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
            "resize",
            &APyFloat::resize,
            py::arg("exp_bits"),
            py::arg("man_bits"),
            py::arg("bias") = std::nullopt,
            py::arg("rounding_mode") = std::nullopt
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
        .def_property_readonly("is_normal", &APyFloat::is_normal, R"pbdoc(
            True if and only if x is normal (not zero, subnormal, infinite, or NaN).

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_property_readonly("is_subnormal", &APyFloat::is_subnormal, R"pbdoc(
            True if and only if x is normal (not zero, subnormal, infinite, or NaN).

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_property_readonly("is_finite", &APyFloat::is_finite, R"pbdoc(
            True if and only if x is zero, subnormal, or normal.

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_property_readonly("is_nan", &APyFloat::is_nan, R"pbdoc(
            True if and only if x is NaN.

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_property_readonly("is_inf", &APyFloat::is_inf, R"pbdoc(
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
        .def_property_readonly("sign", &APyFloat::get_sign)
        .def_property_readonly("man", &APyFloat::get_man)
        .def_property_readonly("exp", &APyFloat::get_exp)
        .def_property_readonly("bias", &APyFloat::get_bias)
        .def_property_readonly("man_bits", &APyFloat::get_man_bits)
        .def_property_readonly("exp_bits", &APyFloat::get_exp_bits);
}
