#include "apyfixed.h"
#include "apyfloat.h"
#include "apyfloatarray.h" // Needed by: APyFloat::is_identical

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/optional.h>

#include <functional>
#include <type_traits>

namespace nb = nanobind;

/*
 * Binding function of a custom R-operator (e.g., `__rmul__`) with non APyFloat type
 */
template <typename OP, typename L_TYPE>
static auto R_OP(const APyFloat& rhs, const L_TYPE& lhs) -> decltype(OP()(rhs, rhs))
{
    [[maybe_unused]] int exp_bits = rhs.get_exp_bits();
    [[maybe_unused]] int man_bits = rhs.get_man_bits();
    [[maybe_unused]] exp_t bias = rhs.get_bias();
    if constexpr (std::is_floating_point_v<L_TYPE>) {
        return OP()(APyFloat::from_double(lhs, exp_bits, man_bits, bias), rhs);
    } else {
        return OP()(APyFloat::from_integer(lhs, exp_bits, man_bits, bias), rhs);
    }
}

/*
 * Binding function of a custom L-operator (e.g., `__sub__`) with non APyFloat type
 */
template <typename OP, typename R_TYPE>
static auto L_OP(const APyFloat& lhs, const R_TYPE& rhs) -> decltype(OP()(lhs, lhs))
{
    [[maybe_unused]] int exp_bits = lhs.get_exp_bits();
    [[maybe_unused]] int man_bits = lhs.get_man_bits();
    [[maybe_unused]] exp_t bias = lhs.get_bias();
    if constexpr (std::is_floating_point_v<R_TYPE>) {
        return OP()(lhs, APyFloat::from_double(rhs, exp_bits, man_bits, bias));
    } else {
        return OP()(lhs, APyFloat::from_integer(rhs, exp_bits, man_bits, bias));
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
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloat` object.

            Parameters
            ----------
            sign : :class:`bool` or int
                The sign of the float. False/0 means positive. True/non-zero means
                negative.
            exp : :class:`int`
                Exponent of the float as stored, i.e., actual value + bias.
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
            :class:`APyFloat`
            )pbdoc"
        )

        /*
         * Copy
         */
        .def("copy", &APyFloat::python_copy, R"pbdoc(
            Create a copy of the object.

            .. versionadded:: 0.3
            )pbdoc")
        .def("__copy__", &APyFloat::python_copy)
        .def("__deepcopy__", &APyFloat::python_deepcopy, nb::arg("memo"))

        /*
         * Arithmetic operations
         */
        .def(nb::self == nb::self)
        .def(nb::self != nb::self)
        .def(nb::self < nb::self)
        .def(nb::self > nb::self)
        .def(nb::self <= nb::self)
        .def(nb::self >= nb::self)

        .def(nb::self + nb::self, NB_NARG())
        .def(nb::self - nb::self, NB_NARG())
        .def(nb::self * nb::self, NB_NARG())
        .def(nb::self / nb::self, NB_NARG())
        .def(-nb::self)
        .def(+nb::self)

        .def("__abs__", &APyFloat::abs)
        .def("__pow__", &APyFloat::pow)
        .def("__pow__", &APyFloat::pown)

        .def("__and__", BIN_OP<std::bit_and<>, APyFloat, APyFloat>)
        .def("__or__", BIN_OP<std::bit_or<>, APyFloat, APyFloat>)
        .def("__xor__", BIN_OP<std::bit_xor<>, APyFloat, APyFloat>)
        .def("__invert__", UN_OP<std::bit_not<>, APyFloat>)

        /*
         * Arithmetic operations with integers
         */
        .def("__add__", L_OP<STD_ADD<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__radd__", R_OP<STD_ADD<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<STD_SUB<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rmul__", R_OP<STD_MUL<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<STD_DIV<>, nb::int_>, NB_OP(), NB_NARG())

        /*
         * Arithmetic with floats
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

        /*
         * Arithmetic operations with APyFixed
         */
        .def("__eq__", BIN_OP<std::equal_to<>, APyFloat, APyFixed>)
        .def("__ne__", BIN_OP<std::not_equal_to<>, APyFloat, APyFixed>)
        .def("__le__", BIN_OP<std::less_equal<>, APyFloat, APyFixed>)
        .def("__lt__", BIN_OP<std::less<>, APyFloat, APyFixed>)
        .def("__ge__", BIN_OP<std::greater_equal<>, APyFloat, APyFixed>)
        .def("__gt__", BIN_OP<std::greater<>, APyFloat, APyFixed>)

        /*
         * Conversion methods
         */
        .def_static(
            "from_float",
            &APyFloat::from_number,
            nb::arg("value"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloat` object from an :class:`int`, :class:`float`,
            :class:`APyFixed`, or :class:`APyFloat`.

            .. attention::
                It is in all cases better to use :func:`~apytypes.APyFloat.cast` to
                create an :class:`APyFloat` from another :class:`APyFloat`.

            The quantization mode used is :class:`QuantizationMode.TIES_EVEN`.

            Parameters
            ----------
            value : :class:`int`, :class:`float`
                Floating-point value to initialize from.
            exp_bits : :class:`int`
                Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Examples
            --------

            >>> import apytypes as apy

            `a`, initialized from floating-point values.

            >>> a = apy.APyFloat.from_float(1.35, exp_bits=10, man_bits=15)

            Returns
            -------
            :class:`APyFloat`

            See Also
            --------
            from_bits
            )pbdoc"
        )
        .def("__float__", &APyFloat::operator double)
        .def_static(
            "from_bits",
            &APyFloat::from_bits,
            nb::arg("bits"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloat` object from a bit-representation.

            Parameters
            ----------
            bits : :class:`int`
                The bit-representation for the float.
            exp_bits : :class:`int`
                Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Examples
            --------

            >>> import apytypes as apy

            `a`, initialized to -1.5 from a bit pattern.

            >>> a = apy.APyFloat.from_bits(0b1_01111_10, exp_bits=5, man_bits=2)

            Returns
            -------
            :class:`APyFloat`

            See Also
            --------
            to_bits
            from_float
            )pbdoc"
        )
        .def("to_bits", &APyFloat::to_bits, R"pbdoc(
            Get the bit-representation of an :class:`APyFloat`.

            Examples
            --------

            >>> import apytypes as apy

            `a`, initialized to -1.5 from a bit pattern.

            >>> a = apy.APyFloat.from_bits(0b1_01111_10, exp_bits=5, man_bits=2)
            >>> a
            APyFloat(sign=1, exp=15, man=2, exp_bits=5, man_bits=2)
            >>> a.to_bits() == 0b1_01111_10
            True

            Returns
            -------
            :class:`int`

            See Also
            --------
            from_bits
            )pbdoc")
        .def("__str__", &APyFloat::str)
        .def("__repr__", &APyFloat::repr)
        .def("_repr_latex_", &APyFloat::latex)
        .def(
            "cast",
            &APyFloat::cast,
            nb::arg("exp_bits") = nb::none(),
            nb::arg("man_bits") = nb::none(),
            nb::arg("bias") = nb::none(),
            nb::arg("quantization") = nb::none(),
            R"pbdoc(
            Change format of the floating-point number.

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
            :class:`APyFloat`

            )pbdoc"
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
        .def(
            "is_identical",
            &APyFloat::is_identical,
            nb::arg("other"),
            nb::arg("ignore_zero_sign") = false,
            R"pbdoc(
            Test if two :py:class:`APyFloat` objects are identical.

            Two :py:class:`APyFloat` objects are considered identical if, and only if,
            they have the same sign, exponent, mantissa, and format.

            Parameters
            ----------
            other : :class:`APyFloat`
                The floating-point object to test identicality against.

            ignore_zero_sign : :class:`bool`, default: :code:`False`
                If :code:`True`, plus and minus zero are considered identical. If
                :code:`False`, plus and minus zero are considered non-identical.

            Returns
            -------
            :class:`bool`
            )pbdoc"
        )

        /*
         * Properties
         */
        .def_prop_ro("sign", &APyFloat::get_sign, R"pbdoc(
            Sign bit.

            See Also
            --------
            true_sign

            Returns
            -------
            :class:`bool`
            )pbdoc")
        .def_prop_ro(
            "true_sign",
            [](const APyFloat& rhs) { return rhs.get_sign() ? -1 : 1; },
            R"pbdoc(
            Sign value.

            See Also
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

            See Also
            --------
            true_man

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("true_man", &APyFloat::true_man, R"pbdoc(
            Mantissa value.

            These are with a possible hidden one.

            See Also
            --------
            man

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro("exp", &APyFloat::get_exp, R"pbdoc(
            Exponent bits with bias.

            See Also
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

            See Also
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
                see :func:`get_float_quantization_mode`, is used.
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
                see :func:`get_float_quantization_mode`, is used.
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
                see :func:`get_float_quantization_mode`, is used.
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
                see :func:`get_float_quantization_mode`, is used.
            )pbdoc"
        )
        .def("next_up", &APyFloat::next_up, R"pbdoc(
            Get the next larger number.

            That means the smallest floating-point number in the same format
            that compares greater.

            See Also
            --------
            next_down

            Returns
            -------
            :class:`APyFloat`

            )pbdoc")
        .def("next_down", &APyFloat::next_down, R"pbdoc(
            Get the next smaller number.

            That means the largest floating-point number in the same format that
            compares less.

            See Also
            --------
            next_up

            Returns
            -------
            :class:`APyFloat`

            )pbdoc")
        .def_static(
            "zero",
            &APyFloat::zero,
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloat` object that is initialized to positive zero.

            .. versionadded:: 0.4

            Parameters
            ----------
            exp_bits : :class:`int`
                Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Examples
            --------

            >>> import apytypes as apy

            `a`, initialized to positive zero.

            >>> a = apy.APyFloat.zero(exp_bits=10, man_bits=15)

            Returns
            -------
            :class:`APyFloat`

            See Also
            --------
            inf
            nan

            )pbdoc"
        )
        .def_static(
            "inf",
            &APyFloat::inf,
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloat` object that is initialized to positive infinity.

            .. versionadded:: 0.3.1

            Parameters
            ----------
            exp_bits : :class:`int`
                Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Examples
            --------

            >>> import apytypes as apy

            `a`, initialized to positive infinity.

            >>> a = apy.APyFloat.inf(exp_bits=10, man_bits=15)

            Returns
            -------
            :class:`APyFloat`

            See Also
            --------
            zero
            nan

            )pbdoc"
        )
        .def_static(
            "nan",
            &APyFloat::nan,
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloat` object that is initialized to NaN.

            .. versionadded:: 0.3.1

            Parameters
            ----------
            exp_bits : :class:`int`
                Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Examples
            --------

            >>> import apytypes as apy

            `a`, initialized to NaN.

            >>> a = apy.APyFloat.nan(exp_bits=10, man_bits=15)

            Returns
            -------
            :class:`APyFloat`

            See Also
            --------
            zero
            inf

            )pbdoc"
        );
}
