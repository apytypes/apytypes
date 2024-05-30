#include "apyfixed.h"
#include "apytypes_common.h"

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/optional.h>

namespace nb = nanobind;

void bind_fixed(nb::module_& m)
{
    nb::class_<APyFixed>(m, "APyFixed")

        /*
         * Constructor: construct from a Python arbitrary long integer object
         */
        .def(
            nb::init<
                nb::int_,
                std::optional<int>,
                std::optional<int>,
                std::optional<int>>(),
            nb::arg("bit_pattern"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none()
        )

        /*
         * Arithmetic operators
         */
        .def(nb::self == nb::self)
        .def(nb::self != nb::self)
        .def(nb::self < nb::self)
        .def(nb::self <= nb::self)
        .def(nb::self > nb::self)
        .def(nb::self >= nb::self)

        .def(nb::self == nb::int_())
        .def(nb::self != nb::int_())
        .def(nb::self < nb::int_())
        .def(nb::self <= nb::int_())
        .def(nb::self > nb::int_())
        .def(nb::self >= nb::int_())

        .def(nb::self == double())
        .def(nb::self != double())
        .def(nb::self < double())
        .def(nb::self > double())
        .def(nb::self <= double())
        .def(nb::self >= double())

        .def(nb::self + nb::self)
        .def(nb::self - nb::self)
        .def(nb::self * nb::self)
        .def(nb::self / nb::self)
        .def(-nb::self)
        .def(nb::self <<= int(), nb::rv_policy::none)
        .def(nb::self >>= int(), nb::rv_policy::none)

        // Operations with integers
        .def(nb::self + nb::int_())
        .def(
            "__radd__",
            [](const APyFixed& rhs, const nb::int_& lhs) { return rhs + lhs; },
            nb::is_operator()
        )
        .def(nb::self - nb::int_())
        .def(
            "__rsub__",
            [](const APyFixed& rhs, const nb::int_& lhs) {
                return APyFixed::from_integer(lhs, rhs.int_bits(), rhs.frac_bits())
                    - rhs;
            },
            nb::is_operator()
        )
        .def(nb::self * nb::int_())
        .def(
            "__rmul__",
            [](const APyFixed& rhs, const nb::int_& lhs) { return rhs * lhs; },
            nb::is_operator()
        )
        .def(nb::self / nb::int_())
        .def(
            "__rtruediv__",
            [](const APyFixed& rhs, const nb::int_& lhs) {
                return APyFixed::from_integer(lhs, rhs.int_bits(), rhs.frac_bits())
                    / rhs;
            },
            nb::is_operator()
        )

        // Operations with floats
        .def(
            "__add__",
            [](const APyFixed& lhs, double rhs) {
                return lhs
                    + APyFixed::from_double(rhs, lhs.int_bits(), lhs.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFixed& rhs, double lhs) {
                return rhs
                    + APyFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__sub__",
            [](const APyFixed& lhs, double rhs) {
                return lhs
                    - APyFixed::from_double(rhs, lhs.int_bits(), lhs.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFixed& rhs, double lhs) {
                return APyFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits())
                    - rhs;
            },
            nb::is_operator()
        )
        .def(
            "__mul__",
            [](const APyFixed& lhs, double rhs) {
                return lhs
                    * APyFixed::from_double(rhs, lhs.int_bits(), lhs.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFixed& rhs, double lhs) {
                return rhs
                    * APyFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFixed& rhs, double lhs) {
                return rhs
                    * APyFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__truediv__",
            [](const APyFixed& lhs, double rhs) {
                return lhs
                    / APyFixed::from_double(rhs, lhs.int_bits(), lhs.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__rtruediv__",
            [](const APyFixed& rhs, double lhs) {
                return APyFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits())
                    / rhs;
            },
            nb::is_operator()
        )

        /*
         * Methods
         */
        .def(
            "to_bits",
            &APyFixed::to_bits,
            R"pbdoc(
            Retrieve underlying bit-pattern in an :class:`int`.

            Examples
            --------
            >>> from apytypes import APyFixed

            Create fixed-point number `fx_a` of value -5.75

            >>> fx_a = APyFixed.from_float(-5.75, int_bits=4, frac_bits=4)

            Returns: 164 == 0xA4 == 0b10100100

            >>> fx_a.to_bits()
            164

            Returns
            -------
            :class:`int`
            )pbdoc"
        )
        .def_prop_ro("bits", &APyFixed::bits, R"pbdoc(
            Total number of bits.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("frac_bits", &APyFixed::frac_bits, R"pbdoc(
            Number of fractional bits.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("int_bits", &APyFixed::int_bits, R"pbdoc(
            Number of integer bits.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("_is_negative", &APyFixed::is_negative)
        .def_prop_ro("_is_positive", &APyFixed::is_positive)
        .def("is_identical", &APyFixed::is_identical, nb::arg("other"), R"pbdoc(
            Test if two fixed-point objects are exactly identical.

            Two `APyFixed` objects are considered exactly identical if, and only if,
            they represent the same fixed-point value, and have the exact same
            bit-specification (`bits`, `int_bits`, and `frac_bits`). This is a more
            restrictive test than ``==``,  that only tests equality of the numerical
            fixed-point value.

            Parameters
            ----------
            other : :class:`APyFixed`
                The fixed-point number to test identicality against

            Examples
            --------
            >>> from apytypes import APyFixed
            >>> fx_a = APyFixed.from_float(2.0, int_bits=3, frac_bits=3)
            >>> fx_b = APyFixed.from_float(2.0, int_bits=4, frac_bits=3)

            `fx_a` and `fx_b` store the same fixed-point value

            >>> fx_a == fx_b
            True

            `fx_a` and `fx_b` differ in the `int_bits` specifier

            >>> fx_a.is_identical(fx_b)
            False

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_prop_ro("is_zero", &APyFixed::is_zero, R"pbdoc(
            True if the value equals zero, false otherwise.
            )pbdoc")
        .def(
            "cast",
            &APyFixed::cast,
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("quantization") = nb::none(),
            nb::arg("overflow") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Change format of the fixed-point number.

            This is the primary method for performing quantization and
            overflowing/saturation when dealing with APyTypes fixed-point numbers.

            Exactly two of three bit-specifiers (`bits`, `int_bits`, `frac_bits`) needs
            to be set.

            Parameters
            ----------
            int_bits : int, optional
                Number of integer bits in the result.
            frac_bits : int, optional
                Number of fractional bits in the result.
            quantization : :class:`QuantizationMode`, optional
                Quantization mode to use in this cast.
            overflow : :class:`OverflowMode`, optional
                Overflowing mode to use in this cast.
            bits : int, optional
                Total number of bits in the result.

            Examples
            --------
            >>> from apytypes import APyFixed
            >>> from apytypes import QuantizationMode
            >>> from apytypes import OverflowMode
            >>> fx = APyFixed.from_float(2.125, int_bits=3, frac_bits=3)

            Truncation (2.0)

            >>> fx.cast(int_bits=3, frac_bits=2, quantization=QuantizationMode.TRN)
            APyFixed(8, bits=5, int_bits=3)

            Rounding (2.25)

            >>> fx.cast(int_bits=3, frac_bits=2, quantization=QuantizationMode.RND) # doctest: +SKIP
            APyFixed(8, bits=5, int_bits=3)

            Two's complement overflowing (-1.875)

            >>> fx.cast(int_bits=2, frac_bits=3, overflow=OverflowMode.WRAP)
            APyFixed(17, bits=5, int_bits=2)

            Returns
            -------
            :class:`APyFixed`
            )pbdoc"
        )
        .def_prop_ro("_vector_size", &APyFixed::vector_size)
        .def("_repr_latex_", &APyFixed::latex)
        .def_prop_ro(
            "leading_ones",
            &APyFixed::leading_ones,
            R"pbdoc(
            Number of leading ones.

            Returns
            -------
            :class:`int`
            )pbdoc"
        )
        .def_prop_ro(
            "leading_zeros",
            &APyFixed::leading_zeros,
            R"pbdoc(
            Number of leading zeros.

            Returns
            -------
            :class:`int`
            )pbdoc"
        )
        .def_prop_ro(
            "leading_fractional_zeros",
            &APyFixed::leading_fractional_zeros,
            R"pbdoc(
            Number of leading zeros after the binary fixed-point.

            Returns
            -------
            :class:`int`
            )pbdoc"
        )
        .def_prop_ro(
            "leading_signs",
            &APyFixed::leading_signs,
            R"pbdoc(
            Number of leading signs.

            Returns
            -------
            :class:`int`
            )pbdoc"
        )

        /*
         * Dunder methods
         */
        .def("__abs__", &APyFixed::abs)
        .def("__float__", &APyFixed::operator double)
        .def("__repr__", &APyFixed::repr)
        .def("__str__", &APyFixed::to_string, nb::arg("base") = 10)
        .def(
            "__lshift__",
            &APyFixed::operator<<,
            nb::arg("shift_amnt"),
            nb::is_operator()
        )
        .def(
            "__rshift__",
            &APyFixed::operator>>,
            nb::arg("shift_amnt"),
            nb::is_operator()
        )

        /*
         * Static methods
         */
        .def_static(
            "from_float",
            &APyFixed::from_number,
            nb::arg("value"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFixed` object from an :class:`int` or :class:`float`.

            The initialized fixed-point value is the one closest to the input
            floating-point value, rounded away from zero on ties. Exactly two of the
            three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

            Parameters
            ----------
            value : int, float
                Floating point value to initialize from
            int_bits : int, optional
                Number of integer bits in the created fixed-point object
            frac_bits : int, optional
                Number of fractional bits in the created fixed-point object
            bits : int, optional
                Total number of bits in the created fixed-point object

            Examples
            --------
            >>> from apytypes import APyFixed
            >>> fx_a = APyFixed.from_float(1.234, int_bits=2, frac_bits=2)

            Fixed-point `fx_a`, initialized from the floating-point value 1.234,
            rounded to 1.25 as it is the closest representable number

            >>> fx_a
            APyFixed(5, bits=4, int_bits=2)
            >>> str(fx_a)
            '1.25'

            Returns
            -------
            :class:`APyFixed`
            )pbdoc"
        )
        .def_static(
            "from_str",
            &APyFixed::from_string,
            nb::arg("string_value"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("base") = 10,
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFixed` object from :class:`str`.

            Exactly two of three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must
            be set.

            Parameters
            ----------
            string_value : str
                String to initialize the value from
            int_bits : int, optional
                Number of integer bits in the created fixed-point object
            frac_bits : int, optional
                Number of fractional bits in the created fixed-point object
            base : int, default: 10
                Numeric base used in `string_value`
            bits : int, optional
                Total number of bits in the created fixed-point object

            Examples
            --------
            >>> from apytypes import APyFixed

            Larger fixed-point value initialization from a string (base-10)

            >>> fx_a = APyFixed.from_str(
            ...     "-1376018206341311063223476816643087998331620501540496640."
            ...     "021222579872958058370179355618716816066859017361262100333952697594702"
            ...     "314679773970519809467311447652539955943903993200932791396783892142688"
            ...     "708904952458654442554723081083186210082207584128592922850820472478833"
            ...     "257136662269306798708182072507551281664490003441493733349403017982015"
            ...     "56238154807942919433116912841796875",
            ...     bits=511,
            ...     int_bits=199,
            ...     base=10
            ... )

            Returns
            -------
            :class:`APyFixed`
            )pbdoc"
        );
}
