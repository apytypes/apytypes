#include "apyfixed.h"
#include "apyfixedarray.h" // Needed by: APyFixed::is_identical

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/optional.h>

#include <functional>
#include <type_traits>

namespace nb = nanobind;

/*
 * Binding function of a custom R-operator (e.g., `__rmul__`) with non APyFixed type
 */
template <typename OP, typename L_TYPE>
static auto R_OP(const APyFixed& rhs, const L_TYPE& lhs) -> decltype(OP()(rhs, rhs))
{
    if constexpr (std::is_floating_point_v<L_TYPE>) {
        return OP()(APyFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits()), rhs);
    } else {
        return OP()(APyFixed::from_integer(lhs, rhs.int_bits(), rhs.frac_bits()), rhs);
    }
}

/*
 * Binding function of a custom L-operator (e.g., `__sub__`) with non APyFixed type
 */
template <typename OP, typename R_TYPE>
static auto L_OP(const APyFixed& lhs, const R_TYPE& rhs) -> decltype(OP()(lhs, lhs))
{
    if constexpr (std::is_floating_point_v<R_TYPE>) {
        return OP()(lhs, APyFixed::from_double(rhs, lhs.int_bits(), lhs.frac_bits()));
    } else {
        return OP()(lhs, APyFixed::from_integer(rhs, lhs.int_bits(), lhs.frac_bits()));
    }
}

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
         * Copy
         */
        .def("copy", &APyFixed::python_copy, R"pbdoc(
            Create a copy of the object.

            .. versionadded:: 0.3
            )pbdoc")
        .def("__copy__", &APyFixed::python_copy)
        .def("__deepcopy__", &APyFixed::python_deepcopy, nb::arg("memo"))

        /*
         * Arithmetic operations
         */
        .def(nb::self == nb::self)
        .def(nb::self != nb::self)
        .def(nb::self < nb::self)
        .def(nb::self <= nb::self)
        .def(nb::self > nb::self)
        .def(nb::self >= nb::self)

        .def(nb::self + nb::self, NB_NARG())
        .def(nb::self - nb::self, NB_NARG())
        .def(nb::self * nb::self, NB_NARG())
        .def(nb::self / nb::self, NB_NARG())
        .def(-nb::self)
        .def(+nb::self)
        .def(nb::self <<= int(), nb::rv_policy::none)
        .def(nb::self >>= int(), nb::rv_policy::none)
        /*
         * Logic operations
         */
        .def(~nb::self)

        /*
         * Arithmetic operations with integers
         */
        .def(nb::self == nb::int_())
        .def(nb::self != nb::int_())
        .def(nb::self < nb::int_())
        .def(nb::self <= nb::int_())
        .def(nb::self > nb::int_())
        .def(nb::self >= nb::int_())

        .def(nb::self + nb::int_(), NB_NARG())
        .def(nb::self - nb::int_(), NB_NARG())
        .def(nb::self * nb::int_(), NB_NARG())
        .def(nb::self / nb::int_(), NB_NARG())
        .def("__radd__", R_OP<STD_ADD<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<STD_SUB<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rmul__", R_OP<STD_MUL<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<STD_DIV<>, nb::int_>, NB_OP(), NB_NARG())

        /*
         * Arithmetic operations with floats
         */
        .def(nb::self == double())
        .def(nb::self != double())
        .def(nb::self < double())
        .def(nb::self > double())
        .def(nb::self <= double())
        .def(nb::self >= double())

        .def("__add__", L_OP<STD_ADD<>, double>, NB_OP(), NB_NARG())
        .def("__radd__", R_OP<STD_ADD<>, double>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, double>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<STD_SUB<>, double>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, double>, NB_OP(), NB_NARG())
        .def("__rmul__", R_OP<STD_MUL<>, double>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, double>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<STD_DIV<>, double>, NB_OP(), NB_NARG())
        .def("__pow__", &APyFixed::pown, NB_OP())

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
            >>> import apytypes as apy

            Create fixed-point number `fx_a` of value -5.75

            >>> fx_a = apy.fx(-5.75, int_bits=4, frac_bits=4)

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
            >>> import apytypes as apy
            >>> fx_a = apy.fx(2.0, int_bits=3, frac_bits=3)
            >>> fx_b = apy.fx(2.0, int_bits=4, frac_bits=3)

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
            int_bits : :class:`int`, optional
                Number of integer bits in the result.
            frac_bits : :class:`int`, optional
                Number of fractional bits in the result.
            quantization : :class:`QuantizationMode`, optional
                Quantization mode to use in this cast.
            overflow : :class:`OverflowMode`, optional
                Overflowing mode to use in this cast.
            bits : :class:`int`, optional
                Total number of bits in the result.

            Examples
            --------
            >>> import apytypes as apy
            >>> fx = apy.fx(2.125, int_bits=3, frac_bits=3)

            Truncation (2.0)

            >>> fx.cast(int_bits=3, frac_bits=2, quantization=apy.QuantizationMode.TRN)
            APyFixed(8, bits=5, int_bits=3)

            Rounding (2.25)

            >>> fx.cast(int_bits=3, frac_bits=2, quantization=apy.QuantizationMode.RND)
            APyFixed(9, bits=5, int_bits=3)

            Two's complement overflowing (-1.875)

            >>> fx.cast(int_bits=2, frac_bits=3, overflow=apy.OverflowMode.WRAP)
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
        .def("__lshift__", &APyFixed::operator<<, nb::arg("shift_amnt"), NB_OP())
        .def("__rshift__", &APyFixed::operator>>, nb::arg("shift_amnt"), NB_OP())

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
            Create an :class:`APyFixed` object from an :class:`int`, :class:`float`,
            :class:`APyFixed`, or :class:`APyFloat`.

            .. attention::
                It is in all cases better to use :func:`~apytypes.APyFixed.cast` to
                create an :class:`APyFixed` from another :class:`APyFixed`.

            The input is quantized using :class:`QuantizationMode.RND_INF` and overflow
            is handled using the :class:`OverflowMode.WRAP` mode. Exactly two of the
            three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

            Parameters
            ----------
            value : :class:`int`, :class:`float`
                Floating point value to initialize from
            int_bits : :class:`int`, optional
                Number of integer bits in the created fixed-point object
            frac_bits : :class:`int`, optional
                Number of fractional bits in the created fixed-point object
            bits : :class:`int`, optional
                Total number of bits in the created fixed-point object

            Examples
            --------
            >>> import apytypes as apy
            >>> fx_a = apy.APyFixed.from_float(1.234, int_bits=2, frac_bits=2)

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
            int_bits : :class:`int`, optional
                Number of integer bits in the created fixed-point object
            frac_bits : :class:`int`, optional
                Number of fractional bits in the created fixed-point object
            base : :class:`int`, default: 10
                Numeric base used in `string_value`
            bits : :class:`int`, optional
                Total number of bits in the created fixed-point object

            Examples
            --------
            >>> import apytypes as apy

            Larger fixed-point value initialization from a string (base-10)

            >>> fx_a = apy.APyFixed.from_str(
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
