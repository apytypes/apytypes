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

        .def(py::self == float())
        .def(py::self != float())
        .def(py::self < float())
        .def(py::self > float())
        .def(py::self <= float())
        .def(py::self >= float())

        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(-py::self)

        // Addition with integers
        .def(py::self + int())
        .def("__radd__", [](APyFixed& rhs, int lhs) { return rhs + lhs; })
        .def(py::self - int())
        .def(
            "__rsub__",
            [](APyFixed& rhs, int lhs) {
                if (lhs == 0) {
                    return -rhs;
                }
                throw NotImplementedException("NotImplemented: __rsub__(int)");
            }
        )
        .def(py::self * int())
        .def("__rmul__", [](APyFixed& rhs, int lhs) { return rhs * lhs; })
        //.def(py::self / int())

        /*
         * Methods
         */
        .def(
            "to_bits",
            &APyFixed::to_bits,
            R"pbdoc(
            Retrieve underlying bit-pattern in an :class:`int`.

            Returns
            -------
            :class:`int`

            Examples
            --------
            .. code-block:: python

                from apytypes import APyFixed

                # Create fixed-point number `fx_a` of value -5.75
                fx_a = APyFixed.from_float(-5.75, int_bits=4, frac_bits=4)

                # Returns: 164 == 0xA4 == 0b10100100
                fx_a.to_bits()
            )pbdoc"
        )
        .def_property_readonly("bits", &APyFixed::bits, R"pbdoc(
            Total number of bits.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_property_readonly("frac_bits", &APyFixed::frac_bits, R"pbdoc(
            Number of fractional bits.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_property_readonly("int_bits", &APyFixed::int_bits, R"pbdoc(
            Number of integer bits.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_property_readonly("_is_negative", &APyFixed::is_negative)
        .def_property_readonly("_is_positive", &APyFixed::is_positive)
        .def("is_identical", &APyFixed::is_identical, py::arg("other"), R"pbdoc(
            Test if two fixed-point objects are exactly identical.

            Two `APyFixed` objects are considered exactly identical if, and only if,
            they store the same fixed-point value, and have the exact same
            bit-specification (*bits*, *int_bits*, and *frac_bits*). This is a more
            restrictive test than ``==``,  that only tests equality of the stored
            fixed-point value.

            Parameters
            ----------
            other : :class:`APyFixed`
                The fixed-point number to test identicality against

            Returns
            -------
            :class:`bool`

            Examples
            --------
            .. code-block:: python

                from apytypes import APyFixed

                fx_a = APyFixed.from_float(2.0, int_bits=3, frac_bits=3)
                fx_b = APyFixed.from_float(2.0, int_bits=4, frac_bits=3)

                # `fx_a` and `fx_b` store the same fixed-point value
                assert fx_a == fx_b

                # `fx_a` and `fx_b` differ in the `int_bits` specifier
                assert not(fx_a.is_identical(fx_b))
            )pbdoc")
        .def_property_readonly("is_zero", &APyFixed::is_zero, R"pbdoc(
            True if the stored value equals zero, false otherwise.
            )pbdoc")
        .def(
            "resize",
            &APyFixed::resize,
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("rounding") = RoundingMode::TRN,
            py::arg("overflow") = OverflowMode::WRAP,
            py::arg("frac_bits") = std::nullopt,
            R"pbdoc(
            Create a new resized fixed-point number based on the bit pattern in this
            fixed-point number.

            This is the primary method for performing rounding, truncation, overflowing,
            and saturation when dealing with APyTypes fixed-point numbers.

            Exactly two of three bit-specifiers (*bits*, *int_bits*, *frac_bits*) needs
            to be set.

            Parameters
            ----------
            bits : int, optional
                Total number of bits in the created fixed-point object
            int_bits : int, optional
                Number of integer bits in the created fixed-point object
            rounding : RoundingMode, default: RoundingMode.TRN
                Rounding mode to use in this resize
            overflow : OverflowMode, default: OverflowMode.WRAP
                Overflowing mode to use in this resize
            frac_bits : int, optional
                Number of fractional bits in the created fixed-point object

            Returns
            -------
            :class:`APyFixed`

            Examples
            --------
            .. code-block:: python

                from apytypes import APyFixed
                from apytypes import RoundingMode
                from apytypes import OverflowMode

                fx = APyFixed.from_float(2.125, int_bits=3, frac_bits=3)

                # Truncation (fx_a == 2.0)
                fx_a = fx.resize(int_bits=3, frac_bits=2, rounding=RoundingMode.TRN)

                # Rounding (fx_b == 2.25)
                fx_b = fx.resize(int_bits=3, frac_bits=2, rounding=RoundingMode.RND)

                # Two's complement overflowing (fx_c == -1.875)
                fx_c = fx.resize(int_bits=2, frac_bits=3, overflow=OverflowMode.WRAP)
            )pbdoc"
        )
        .def_property_readonly("_vector_size", &APyFixed::vector_size)
        .def("_repr_latex_", &APyFixed::latex)

        /*
         * Dunder methods
         */
        .def("__abs__", &APyFixed::abs)
        .def("__float__", &APyFixed::operator double)
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
            Create an :class:`APyFixed` object from :class:`float`.

            The initialized fixed-point value is the one closest to the
            input floating-point value, rounded away from zero on ties. Exactly two of
            the three bit-specifiers (`bits`, `int_bits`, `frac_bits`) needs to be set.

            Exactly two of three bit-specifiers (*bits*, *int_bits*, *frac_bits*) needs
            to be set.

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

            Examples
            --------
            .. code-block:: python

                from apytypes import APyFixed

                # Fixed-point `fx_a`, initialized from the floating-point value 1.234,
                # rounded to 1.25 as it is the closest representable number
                fx_a = APyFixed.from_float(1.234, int_bits=2, frac_bits=2)
            )pbdoc"
        )
        .def_static(
            "from_str",
            &APyFixed::from_string,
            py::arg("string_value"),
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("base") = 10,
            py::arg("frac_bits") = std::nullopt,
            R"pbdoc(
            Create an :class:`APyFixed` object from :class:`str`.

            Exactly two of three bit-specifiers (*bits*, *int_bits*, *frac_bits*) needs
            to be set.

            Parameters
            ----------
            string_value : str
                String to initialize the value from
            bits : int, optional
                Total number of bits in the created fixed-point object
            int_bits : int, optional
                Number of integer bits in the created fixed-point object
            base : int, default: 10
                Numeric base used in `string_value`
            frac_bits : int, optional
                Number of fractional bits in the created fixed-point object

            Examples
            --------
            .. code-block:: python

                from apytypes import APyFixed

                # Larger fixed-point value initialization from a string (base-10)
                fx_a = APyFixed.from_str(
                    "-1376018206341311063223476816643087998331620501540496640."
                    "021222579872958058370179355618716816066859017361262100333952697594702"
                    "314679773970519809467311447652539955943903993200932791396783892142688"
                    "708904952458654442554723081083186210082207584128592922850820472478833"
                    "257136662269306798708182072507551281664490003441493733349403017982015"
                    "56238154807942919433116912841796875",
                    bits=511,
                    int_bits=199,
                    base=10
                )
            )pbdoc"
        );
}
