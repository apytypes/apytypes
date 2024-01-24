#include "apyfixed.h"
#include "apytypes_common.h"
#include "apytypes_util.h"

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <optional> // std::optional, std::nullopt

namespace py = pybind11;

void bind_fixed(py::module& m)
{
    py::class_<APyFixed>(m, "APyFixed")

        /*
         * Constructor: construct from another APyFixed number
         */
        .def(
            py::init<
                const APyFixed&,
                std::optional<int>,
                std::optional<int>,
                std::optional<int>>(),
            py::arg("other"),
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("frac_bits") = std::nullopt
        )

        /*
         * Constructor: construct from a Python arbitrary long integer object
         */
        .def(
            py::init<
                py::int_,
                std::optional<int>,
                std::optional<int>,
                std::optional<int>>(),
            py::arg("bit_pattern"),
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("frac_bits") = std::nullopt
        )

        /*
         * Arithmetic operators
         */
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self < py::self)
        .def(py::self <= py::self)
        .def(py::self > py::self)
        .def(py::self >= py::self)

        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def("assign", &APyFixed::operator=)
        .def(-py::self)

        /*
         * Methods
         */
        .def("bit_pattern_to_string_dec", &APyFixed::bit_pattern_to_string_dec, R"pbdoc(
            Retrieve the underlying bit-pattern as a :class:`str` object formated in
            base-10.

            Returns
            -------
            :class:`str`
            )pbdoc")
        .def(
            "bit_pattern_to_int",
            &APyFixed::bit_pattern_to_int,
            py::arg("allow_negative_return_value") = false,
            R"pbdoc(
            Retrieve the underlying bit-pattern in an :class:`int` object.

            Parameters
            ----------
            allow_negative_return_value : bool, default=False
                Allow returning a negative integer bit-pattern. See example.

            Examples
            --------

            .. code-block:: python

                from apytypes import APyFixed

                # Create fixed-point number `fx_a` of value -5.75
                fx_a = APyFixed.from_float(-5.75, int_bits=4, frac_bits=4)

                ### Returns: 164 == 0xa4 == 0b10100100
                fx_a.bit_pattern_to_int()

                ### Returns: -92 == -0x5C == -0b1011100
                fx_a.bit_pattern_to_int(allow_negative_return_value=True)

            Returns
            -------
            :class:`int`
            )pbdoc"
        )
        .def_property_readonly("bits", &APyFixed::bits, R"pbdoc(
            Total number of bits in this :class:`APyFixed` object.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_property_readonly("frac_bits", &APyFixed::frac_bits, R"pbdoc(
            Number of fractional bits in this :class:`APyFixed` object.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def(
            "set_from_string",
            &APyFixed::set_from_string,
            py::arg("str"),
            py::arg("base") = 10
        )
        .def("set_from_float", &APyFixed::set_from_double)
        .def("increment_lsb", &APyFixed::increment_lsb)
        .def_property_readonly("int_bits", &APyFixed::int_bits, R"pbdoc(
            Number of integer bits in this :class:`APyFixed` object.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_property_readonly("is_negative", &APyFixed::is_negative)
        .def_property_readonly("is_positive", &APyFixed::is_positive)
        .def("is_identical", &APyFixed::is_identical, py::arg("other"), R"pbdoc(
            Test if two `APyFixed` objects are identical.

            Two `APyFixed` objects are considered identical if, and only if,  they store
            the same fixed-point value and have exactly the same bit-specification
            (`bits`, `int_bits`, and `frac_bits` are all equal). This is a more
            restrictive test than `__eq__`,  that only tests equality of the fixed-point
            value.

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_property_readonly("is_zero", &APyFixed::is_zero)
        .def(
            "resize",
            &APyFixed::resize,
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("rounding_mode") = APyFixedRoundingMode::TRN,
            py::arg("overflow_mode") = APyFixedOverflowMode::TWOS_OVERFLOW,
            py::arg("frac_bits") = std::nullopt
        )
        .def("to_string", &APyFixed::to_string, py::arg("base") = 10)
        .def("to_string_dec", &APyFixed::to_string_dec)
        .def("to_string_hex", &APyFixed::to_string_hex)
        .def("to_string_oct", &APyFixed::to_string_oct)
        .def_property_readonly("vector_size", &APyFixed::vector_size)

        /*
         * Dunder methods
         */
        .def("__abs__", &APyFixed::abs)
        .def("__float__", &APyFixed::to_double)
        .def("__neg__", [](APyFixed& fix) { return -fix; })
        .def("__repr__", &APyFixed::repr)
        .def("__str__", &APyFixed::to_string, py::arg("base") = 10)
        .def("__lshift__", &APyFixed::operator<<, py::arg("shift_amnt"))
        .def("__rshift__", &APyFixed::operator>>, py::arg("shift_amnt"))

        /*
         * Static methods
         */
        .def_static(
            "from_float",
            &APyFixed::from_double,
            py::arg("value"),
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("frac_bits") = std::nullopt,
            R"pbdoc(
            Create an :class:`APyFixed` object and initialize its value from a
            :class:`float`.

            The initialized fixed-point value is the one closest to the
            input floating-point value, rounded away from zero on ties. Exactly two of
            the three bit-specifiers (`bits`, `int_bits`, `frac_bits`) has to be set.

            Examples
            --------

            .. code-block:: python

                from apytypes import APyFixed

                # Fixed-point `fx_a` from float, initialized from the floating-point
                # value 1.234, rounded to 1.25 as it is the closest representable number
                fx_a = APyFixed.from_float(1.234, int_bits=2, frac_bits=2)

            Parameters
            ----------
            value : float
                Floating point value to initialize from
            bits : int, optional
                Total number of bits in the created fixed-point object
            int_bits : int, optional
                Number of integer bits in the created fixed-point object
            frac_bits : int, optional
                Number of fractional bits in the created fixed-point object
            )pbdoc"
        )
        .def_static(
            "from_string",
            &APyFixed::from_string,
            py::arg("string_value"),
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("base") = 10,
            py::arg("frac_bits") = std::nullopt
        );
}
