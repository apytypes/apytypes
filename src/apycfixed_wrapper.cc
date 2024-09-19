#include "apycfixed.h"

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/optional.h>

namespace nb = nanobind;

#include <functional>
#include <type_traits>

/*
 * Binding function of a custom R-operator (e.g., `__rmul__`) with non APyCFixed type
 */
template <typename OP, typename L_TYPE>
static auto R_OP(const APyCFixed& rhs, const L_TYPE& lhs) -> decltype(OP()(rhs, rhs))
{
    if constexpr (std::is_same_v<std::complex<double>, L_TYPE>) {
        return OP()(APyCFixed::from_complex(lhs, rhs.int_bits(), rhs.frac_bits()), rhs);
    } else if constexpr (std::is_floating_point_v<L_TYPE>) {
        return OP()(APyCFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits()), rhs);
    } else if constexpr (std::is_same_v<APyFixed, L_TYPE>) {
        return OP()(
            APyCFixed::from_apyfixed(lhs, rhs.int_bits(), rhs.frac_bits()), rhs
        );
    } else {
        return OP()(APyCFixed::from_integer(lhs, rhs.int_bits(), rhs.frac_bits()), rhs);
    }
}

/*
 * Binding function of a custom L-operator (e.g., `__sub__`) with non APyCFixed type
 */
template <typename OP, typename R_TYPE>
static auto L_OP(const APyCFixed& lhs, const R_TYPE& rhs) -> decltype(OP()(lhs, lhs))
{
    if constexpr (std::is_same_v<std::complex<double>, R_TYPE>) {
        return OP()(lhs, APyCFixed::from_complex(rhs, lhs.int_bits(), lhs.frac_bits()));
    } else if constexpr (std::is_floating_point_v<R_TYPE>) {
        return OP()(lhs, APyCFixed::from_double(rhs, lhs.int_bits(), lhs.frac_bits()));
    } else if constexpr (std::is_same_v<nb::int_, R_TYPE>) {
        return OP()(lhs, APyCFixed::from_integer(rhs, lhs.int_bits(), lhs.frac_bits()));
    } else if constexpr (std::is_same_v<APyFixed, R_TYPE>) {
        return OP()(
            lhs, APyCFixed::from_apyfixed(rhs, lhs.int_bits(), lhs.frac_bits())
        );
    } else {
        return OP()(lhs, rhs);
    }
}

using std::complex;

void bind_cfixed(nb::module_& m)
{
    nb::class_<APyCFixed>(m, "APyCFixed")

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

        .def(
            nb::init<
                nb::tuple,
                std::optional<int>,
                std::optional<int>,
                std::optional<int>>(),
            nb::arg("bit_pattern"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none()
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
        .def(nb::self <<= int(), nb::rv_policy::none)
        .def(nb::self >>= int(), nb::rv_policy::none)

        /*
         * Arithmetic operations with integers
         */
        .def(nb::self == nb::int_())
        .def(nb::self != nb::int_())

        .def("__add__", L_OP<std::plus<>, nb::int_>, nb::is_operator())
        .def("__sub__", L_OP<std::minus<>, nb::int_>, nb::is_operator())
        .def("__mul__", L_OP<std::multiplies<>, nb::int_>, nb::is_operator())
        .def("__truediv__", L_OP<std::divides<>, nb::int_>, nb::is_operator())
        .def("__radd__", R_OP<std::plus<>, nb::int_>, nb::is_operator())
        .def("__rsub__", R_OP<std::minus<>, nb::int_>, nb::is_operator())
        .def("__rmul__", R_OP<std::multiplies<>, nb::int_>, nb::is_operator())
        .def("__rtruediv__", R_OP<std::divides<>, nb::int_>, nb::is_operator())

        /*
         * Arithmetic operations with floats
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
         * Arithmetic operations with complex
         */
        .def(nb::self == complex<double>())
        .def(nb::self != complex<double>())

        .def("__add__", L_OP<std::plus<>, complex<double>>, nb::is_operator())
        .def("__radd__", R_OP<std::plus<>, complex<double>>, nb::is_operator())
        .def("__sub__", L_OP<std::minus<>, complex<double>>, nb::is_operator())
        .def("__rsub__", R_OP<std::minus<>, complex<double>>, nb::is_operator())
        .def("__mul__", L_OP<std::multiplies<>, complex<double>>, nb::is_operator())
        .def("__rmul__", R_OP<std::multiplies<>, complex<double>>, nb::is_operator())
        .def("__truediv__", L_OP<std::divides<>, complex<double>>, nb::is_operator())
        .def("__rtruediv__", R_OP<std::divides<>, complex<double>>, nb::is_operator())

        /*
         * Arithmetic operations with APyFixed
         */
        .def("__eq__", L_OP<std::equal_to<>, APyFixed>, nb::is_operator())
        .def("__ne__", L_OP<std::not_equal_to<>, APyFixed>, nb::is_operator())

        .def("__add__", L_OP<std::plus<>, APyFixed>, nb::is_operator())
        .def("__radd__", R_OP<std::plus<>, APyFixed>, nb::is_operator())
        .def("__sub__", L_OP<std::minus<>, APyFixed>, nb::is_operator())
        .def("__rsub__", R_OP<std::minus<>, APyFixed>, nb::is_operator())
        .def("__mul__", L_OP<std::multiplies<>, APyFixed>, nb::is_operator())
        .def("__rmul__", R_OP<std::multiplies<>, APyFixed>, nb::is_operator())
        .def("__truediv__", L_OP<std::divides<>, APyFixed>, nb::is_operator())
        .def("__rtruediv__", R_OP<std::divides<>, APyFixed>, nb::is_operator())

        /*
         * Get real and imaginary part
         */
        .def_prop_ro("real", &APyCFixed::get_real, R"pbdoc(
            Real part.

            Returns
            -------
            :class:`APyFixed`
            )pbdoc")
        .def_prop_ro("imag", &APyCFixed::get_imag, R"pbdoc(
            Imaginary part.

            Returns
            -------
            :class:`APyFixed`
            )pbdoc")

        /*
         * Methods
         */
        .def(
            "to_bits",
            &APyCFixed::to_bits,
            R"pbdoc(
            Retrieve underlying bit-pattern in a :class:`tuple` of :class:`int`.

            Examples
            --------
            Create complex-valued fixed-point number `fx_a` of value -5.75 + 2j and
            show it's bit pattern (real, imag)

            >>> from apytypes import APyCFixed
            >>>
            >>> fx_a = APyCFixed.from_complex(-5.75 + 2j, int_bits=4, frac_bits=4)
            >>> fx_a.to_bits()
            (164, 32)

            Returns
            -------
            :tuple:`tuple` of :class:`int`
            )pbdoc"
        )
        .def_prop_ro("bits", &APyCFixed::bits, R"pbdoc(
             Total number of bits.

             Returns
             -------
             :class:`int`
             )pbdoc")
        .def_prop_ro("frac_bits", &APyCFixed::frac_bits, R"pbdoc(
             Number of fractional bits.

             Returns
             -------
             :class:`int`
             )pbdoc")
        .def_prop_ro("int_bits", &APyCFixed::int_bits, R"pbdoc(
             Number of integer bits.

             Returns
             -------
             :class:`int`
             )pbdoc")
        .def("is_identical", &APyCFixed::is_identical, nb::arg("other"), R"pbdoc(
            Test if two fixed-point objects are exactly identical.

            Two `APyCFixed` objects are considered exactly identical if, and only if,
            they represent the same fixed-point value, and have the exact same
            bit-specification (`bits`, `int_bits`, and `frac_bits`). This is a more
            restrictive test than ``==``,  that only tests equality of the numerical
            fixed-point value.

            Parameters
            ----------
            other : :class:`APyCFixed`
                The fixed-point number to test identicality against

            Examples
            --------
            Complex-valued fixed-point `fx_a` and `fx_b`. They are equal, as they store
            the same value, but they are *not* identical as the differ in the `int_bits`
            specifier.

            >>> from apytypes import APyCFixed
            >>>
            >>> fx_a = APyCFixed.from_complex(2.0 + 3.0j, int_bits=3, frac_bits=3)
            >>> fx_b = APyCFixed.from_complex(2.0 + 3.0j, int_bits=4, frac_bits=3)
            >>> fx_a == fx_b
            True
            >>> fx_a.is_identical(fx_b)
            False

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_prop_ro("is_zero", &APyCFixed::is_zero, R"pbdoc(
            True if the value equals zero, false otherwise.
            )pbdoc")
        .def(
            "cast",
            &APyCFixed::cast,
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("quantization") = nb::none(),
            nb::arg("overflow") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Change format of the complex-valued fixed-point number.

            This is the primary method for performing quantization and
            overflowing/saturation when dealing with APyTypes fixed-point numbers.
            The specified quatization and overflow are applied piecewise to the real and
            imaginary part.

            Exactly two of three bit-specifiers (`bits`, `int_bits`, `frac_bits`)
            needs to be set.

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
            >>> from apytypes import APyCFixed
            >>> from apytypes import QuantizationMode
            >>> from apytypes import OverflowMode
            >>>
            >>> fx = APyCFixed.from_complex(2.125 + 1.625j, int_bits=3, frac_bits=3)
            >>>
            >>> # Truncation: 2.0 + 1.5j
            >>> fx.cast(int_bits=3, frac_bits=2, quantization=QuantizationMode.TRN)
            APyCFixed((8, 6), bits=5, int_bits=3)
            >>>
            >>> # Fixed-point rounding: 2.25 + 1.75j
            >>> fx.cast(int_bits=3, frac_bits=2, quantization=QuantizationMode.RND)
            APyCFixed((9, 7), bits=5, int_bits=3)
            >>>
            >>> # Two's complement overflowing: -1.875 + 1.625j
            >>> fx.cast(int_bits=2, frac_bits=3, overflow=OverflowMode.WRAP)
            APyCFixed((17, 13), bits=5, int_bits=2)

            Returns
            -------
            :class:`APyCFixed`
            )pbdoc"
        )
        /*
         * Dunder methods
         */
        .def("__repr__", &APyCFixed::repr)
        .def("__str__", &APyCFixed::to_string, nb::arg("base") = 10)
        .def("__complex__", &APyCFixed::to_complex)
        .def(
            "__lshift__",
            &APyCFixed::operator<<,
            nb::arg("shift_amnt"),
            nb::is_operator()
        )
        .def(
            "__rshift__",
            &APyCFixed::operator>>,
            nb::arg("shift_amnt"),
            nb::is_operator()
        )

        /*
         * Static methods
         */
        .def_static(
            "from_complex",
            &APyCFixed::from_number,
            nb::arg("value"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Create an :class:`APyCFixed` object from an :class:`int`, :class:`float`,
            :class:`complex`, :class:`APyFixed`, :class:`APyFloat`, or :class:`APyCFixed`.

            .. note:: It is in all cases better to use :func:`~apytypes.APyCFixed.cast` to create an :class:`APyCFixed` from an :class:`APyCFixed`.

            The input is quantized using :class:`QuantizationMode.RND_INF` and overflow is handled using the :class:`OverflowMode.WRAP` mode.
            Exactly two of the three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

            Parameters
            ----------
            value : int, float, complex
                Value to initialize from
            int_bits : int, optional
                Number of integer bits in the created fixed-point object
            frac_bits : int, optional
                Number of fractional bits in the created fixed-point object
            bits : int, optional
                Total number of bits in the created fixed-point object

            Examples
            --------
            Complex-valued fixed-point `fx_a`, initialized from the complex number 1.234
            + 0.4j, rounded to 1.25 + 0.5j as it is the closest representable number.

            >>> from apytypes import APyCFixed
            >>>
            >>> fx_a = APyCFixed.from_complex(1.234 + 0.4j, int_bits=2, frac_bits=2)
            >>> fx_a
            APyCFixed((5, 2), bits=4, int_bits=2)
            >>> str(fx_a)
            '1.25+0.5j'

            Returns
            -------
            :class:`APyCFixed`
            )pbdoc"
        )
        .def_static(
            "from_float",
            &APyCFixed::from_number,
            nb::arg("value"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Create an :class:`APyCFixed` object from an :class:`int`, :class:`float`,
            :class:`complex`, :class:`APyFixed`, :class:`APyFloat`, or :class:`APyCFixed`.
            This is an alias for :func:`~apytypes.APyCFixed.from_complex`, look there for more documentation.

            .. note:: It is in all cases better to use :func:`~apytypes.APyCFixed.cast` to create an :class:`APyCFixed` from an :class:`APyCFixed`.

            Parameters
            ----------
            value : int, float, complex
                Value to initialize from
            int_bits : int, optional
                Number of integer bits in the created fixed-point object
            frac_bits : int, optional
                Number of fractional bits in the created fixed-point object
            bits : int, optional
                Total number of bits in the created fixed-point object

            Returns
            -------
            :class:`APyCFixed`
            )pbdoc"
        );
}
