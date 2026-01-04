#include "apycfloatarray.h"
#include "apyfloatarray.h"

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/optional.h>

namespace nb = nanobind;

/*
 * Binding function of a custom R-operator (e.g., `__rmul__`) with non APyCFloatArray
 * type by first promotong the left-hand side.
 */
template <auto FUNC, typename L_TYPE>
static auto R_OP(const APyCFloatArray& rhs, const L_TYPE& lhs) -> APyCFloatArray
{
    [[maybe_unused]] int exp_bits = rhs.get_exp_bits();
    [[maybe_unused]] int man_bits = rhs.get_man_bits();
    [[maybe_unused]] exp_t bias = rhs.get_bias();
    if constexpr (std::is_same_v<std::complex<double>, L_TYPE>) {
        auto lhs_cplx = APyCFloat::from_complex(lhs, exp_bits, man_bits, bias);
        return (rhs.*FUNC)(lhs_cplx);
    } else if constexpr (std::is_floating_point_v<L_TYPE>) {
        auto lhs_cplx = APyCFloat::from_double(lhs, exp_bits, man_bits, bias);
        return (rhs.*FUNC)(lhs_cplx);
    } else if constexpr (std::is_same_v<remove_cvref_t<L_TYPE>, APyCFloat>) {
        return (rhs.*FUNC)(lhs);
    } else if constexpr (std::is_same_v<remove_cvref_t<L_TYPE>, nb::int_>) {
        auto lhs_cplx = APyCFloat::from_integer(lhs, exp_bits, man_bits, bias);
        return (rhs.*FUNC)(lhs_cplx);
    } else if constexpr (std::is_same_v<remove_cvref_t<L_TYPE>, APyFloat>) {
        auto [exp_bits, man_bits, bias] = lhs.spec();
        auto lhs_cplx = APyCFloat(lhs.get_data(), exp_bits, man_bits, bias);
        return (rhs.*FUNC)(lhs_cplx);
    } else {
        static_assert(
            always_false_v<L_TYPE>, "Need to add another R_OP specialization?"
        );
    }
}

template <typename L_FUNC, typename L_TYPE>
static auto R_OP(const APyCFloatArray& rhs, const L_TYPE& lhs) -> APyCFloatArray
{
    [[maybe_unused]] int exp_bits = rhs.get_exp_bits();
    [[maybe_unused]] int man_bits = rhs.get_man_bits();
    [[maybe_unused]] exp_t bias = rhs.get_bias();
    if constexpr (std::is_same_v<remove_cvref_t<L_TYPE>, APyFloatArray>) {
        return L_FUNC()(APyCFloatArray(lhs), rhs);
    } else {
        static_assert(
            always_false_v<L_TYPE>, "Need to add another R_OP specialization?"
        );
    }
}

/*
 * Binding function of a custom L-operator (e.g., `__sub__`) with non APyCFloatArray
 * type by first promotong the left-hand side.
 */
template <typename L_FUNC, typename R_TYPE>
static auto L_OP(const APyCFloatArray& lhs, const R_TYPE& rhs)
    -> decltype(L_FUNC()(lhs, lhs))
{
    using std::is_same_v;
    [[maybe_unused]] int exp_bits = lhs.get_exp_bits();
    [[maybe_unused]] int man_bits = lhs.get_man_bits();
    [[maybe_unused]] exp_t bias = lhs.get_bias();
    if constexpr (is_same_v<std::complex<double>, R_TYPE>) {
        auto rhs_cplx = APyCFloat::from_complex(rhs, exp_bits, man_bits, bias);
        return L_FUNC()(lhs, rhs_cplx);
    } else if constexpr (std::is_floating_point_v<R_TYPE>) {
        auto rhs_cplx = APyCFloat::from_double(rhs, exp_bits, man_bits, bias);
        return L_FUNC()(lhs, rhs_cplx);
    } else if constexpr (is_same_v<nb::int_, R_TYPE>) {
        auto rhs_cplx = APyCFloat::from_integer(rhs, exp_bits, man_bits, bias);
        return L_FUNC()(lhs, rhs_cplx);
    } else if constexpr (is_same_v<nb::ndarray<nb::c_contig>, remove_cvref_t<R_TYPE>>) {
        auto rhs_cplx = APyCFloatArray::from_array(rhs, exp_bits, man_bits, bias);
        return L_FUNC()(lhs, rhs_cplx);
    } else if constexpr (is_same_v<remove_cvref_t<R_TYPE>, APyFloat>) {
        auto [exp_bits, man_bits, bias] = rhs.spec();
        auto rhs_cplx = APyCFloat(rhs.get_data(), exp_bits, man_bits, bias);
        return L_FUNC()(lhs, rhs_cplx);
    } else if constexpr (is_same_v<remove_cvref_t<R_TYPE>, APyFloatArray>) {
        APyCFloatArray rhs_cplx(rhs);
        return L_FUNC()(lhs, rhs_cplx);
    } else {
        return L_FUNC()(lhs, rhs);
    }
}

// Create a "cheat" comparison-function signature that is used *only* in the created
// stub files. The cheat signature tells nanobind that a comparison function returns a
// numpy array of booleans, even though the comparison function may return another
// third-party array-library array of booleans. This hack is needed until the stub
// generation is more developed upstream, or until the static type-checkers can do a
// better job of the currently generated signatures.
#define CMP(FUNC_NAME, RHS_TYPE)                                                       \
    nb::is_operator(), nb::arg().noconvert(),                                          \
        nb::sig(                                                                       \
            "def " FUNC_NAME "(self, arg: " RHS_TYPE ", /) -> NDArray[numpy.bool]"     \
        )

using complex_t = std::complex<double>;

void bind_cfloat_array(nb::module_& m)
{
    nb::class_<APyCFloatArray>(m, "APyCFloatArray")

        /*
         * Base constructor
         */
        .def(
            nb::init<
                const nb::typed<nb::iterable, nb::any>&,
                const nb::typed<nb::iterable, nb::any>&,
                const nb::typed<nb::iterable, nb::any>&,
                int,
                int,
                std::optional<exp_t>>(),
            nb::arg("sign_seq"),
            nb::arg("exp_seq"),
            nb::arg("man_seq"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none()
        )

        /*
         * Copy
         */
        .def(
            "copy",
            &APyCFloatArray::python_copy,
            R"pbdoc(
            Create a copy of the object.
            )pbdoc"
        )
        .def("__copy__", &APyCFloatArray::python_copy)
        .def("__deepcopy__", &APyCFloatArray::python_deepcopy, nb::arg("memo"))

        /*
         * Pickling support
         */
        .def("__getstate__", &APyCFloatArray::python_pickle)
        .def("__setstate__", &APyCFloatArray::python_unpickle)

        /*
         * Arithmetic operations
         */
        .def(nb::self + nb::self, NB_NARG())
        .def(nb::self - nb::self, NB_NARG())
        .def(nb::self * nb::self, NB_NARG())
        .def(nb::self / nb::self, NB_NARG())
        .def(-nb::self)
        .def(+nb::self)

        .def(nb::self == nb::self, CMP("__eq__", "APyCFloatArray"))
        .def(nb::self != nb::self, CMP("__ne__", "APyCFloatArray"))

        /*
         * Arithmetic operations with `APyCFloat`
         */
        .def("__add__", L_OP<STD_ADD<>, APyCFloat>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, APyCFloat>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, APyCFloat>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyCFloatArray::rsub, APyCFloat>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, APyCFloat>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, APyCFloat>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, APyCFloat>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyCFloatArray::rdiv, APyCFloat>, NB_OP(), NB_NARG())
        .def("__eq__", L_OP<STD_EQ<>, APyCFloat>, CMP("__eq__", "APyCFloat"))
        .def("__ne__", L_OP<STD_NE<>, APyCFloat>, CMP("__ne__", "APyCFloat"))

        /*
         * Arithmetic operations with `APyFloat`
         */
        .def("__add__", L_OP<STD_ADD<>, APyFloat>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, APyFloat>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, APyFloat>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyCFloatArray::rsub, APyFloat>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, APyFloat>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, APyFloat>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, APyFloat>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyCFloatArray::rdiv, APyFloat>, NB_OP(), NB_NARG())
        .def("__eq__", L_OP<STD_EQ<>, APyFloat>, CMP("__eq__", "APyFloat"))
        .def("__ne__", L_OP<STD_NE<>, APyFloat>, CMP("__ne__", "APyFloat"))

        /*
         * Arithmetic operations with `APyFloatArray`
         */
        .def("__add__", L_OP<STD_ADD<>, APyFloatArray>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, APyFloatArray>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, APyFloatArray>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<STD_SUB<>, APyFloatArray>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, APyFloatArray>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, APyFloatArray>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, APyFloatArray>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<STD_DIV<>, APyFloatArray>, NB_OP(), NB_NARG())
        .def("__eq__", L_OP<STD_EQ<>, APyFloatArray>, CMP("__eq__", "APyFloatArray"))
        .def("__ne__", L_OP<STD_NE<>, APyFloatArray>, CMP("__ne__", "APyFloatArray"))

        /*
         * Arithmetic operations with Python `int`
         */
        .def("__add__", L_OP<STD_ADD<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyCFloatArray::rsub, nb::int_>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyCFloatArray::rdiv, nb::int_>, NB_OP(), NB_NARG())
        .def("__eq__", L_OP<STD_EQ<>, nb::int_>, CMP("__eq__", "int"))
        .def("__ne__", L_OP<STD_NE<>, nb::int_>, CMP("__ne__", "int"))

        /*
         * Arithmetic operators with Python floats
         */
        .def("__add__", L_OP<STD_ADD<>, double>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, double>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, double>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyCFloatArray::rsub, double>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, double>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, double>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, double>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyCFloatArray::rdiv, double>, NB_OP(), NB_NARG())
        .def("__eq__", L_OP<STD_EQ<>, double>, CMP("__eq__", "float"))
        .def("__ne__", L_OP<STD_NE<>, double>, CMP("__ne__", "float"))

        /*
         * Arithmetic operators with Python complex
         */
        .def("__add__", L_OP<STD_ADD<>, complex_t>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, complex_t>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, complex_t>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyCFloatArray::rsub, complex_t>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, complex_t>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, complex_t>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, complex_t>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyCFloatArray::rdiv, complex_t>, NB_OP(), NB_NARG())
        .def("__eq__", L_OP<STD_EQ<>, complex_t>, CMP("__eq__", "complex"))
        .def("__ne__", L_OP<STD_NE<>, complex_t>, CMP("__ne__", "complex"))

        /*
         * Arithmetic operations with NumPy arrays
         * The right-hand versions are not used since Numpy will convert the
         * APyCFloatArray to a Numpy array before they are invoked.
         */
        .def("__add__", L_OP<STD_ADD<>, nb::ndarray<nb::c_contig>>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, nb::ndarray<nb::c_contig>>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, nb::ndarray<nb::c_contig>>, NB_OP(), NB_NARG())
        .def(
            "__truediv__",
            L_OP<STD_DIV<>, nb::ndarray<nb::c_contig>>,
            NB_OP(),
            NB_NARG()
        )

        /*
         * Properties
         */
        .def_prop_ro("exp_bits", &APyCFloatArray::get_exp_bits, R"pbdoc(
            Number of exponent bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("man_bits", &APyCFloatArray::get_man_bits, R"pbdoc(
            Number of mantissa bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("bits", &APyCFloatArray::get_bits, R"pbdoc(
            Total number of bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("bias", &APyCFloatArray::get_bias, R"pbdoc(
            Exponent bias.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("shape", &APyCFloatArray::python_get_shape, R"pbdoc(
            The shape of the array.

            Returns
            -------
            :class:`tuple` of :class:`int`
            )pbdoc")

        .def_prop_ro("ndim", &APyCFloatArray::ndim, R"pbdoc(
            Number of dimensions in the array.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro(
            "T", [](const APyCFloatArray& self) { return self.transpose(); }, R"pbdoc(
            The transposition of the array.

            Equivalent to calling :func:`transpose`.

            Returns
            -------
            :class:`APyCFloatArray`
            )pbdoc"
        )
        .def_prop_ro(
            "H",
            [](const APyCFloatArray& self) { return self.hermitian_transpose(); },
            R"pbdoc(
            The Hermitian transpose of the array.

            Equivalent to calling :func:`transpose` followed by :func:`conj`.

            .. versionadded:: 0.5

            Returns
            -------
            :class:`APyCFloatArray`
            )pbdoc"
        )

        /*
         * Static methods
         */
        .def_static(
            "from_complex",
            &APyCFloatArray::from_numbers,
            nb::arg("complex_sequence"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyCFloatArray` from iterable sequence of numbers.

            Using NumPy arrays as input is in general faster than using e.g. lists.

            Parameters
            ----------
            complex_sequence : :class:`~collections.abc.Iterable` of numbers.
                Values to initialize from. The tensor shape will be taken from the
                sequence shape.
            exp_bits : :class:`int`
                Number of exponent bits in the created floating-point tensor.
            man_bits : :class:`int`
                Number of mantissa bits in the created floating-point tensor.
            bias : :class:`int`, optional
                Bias in the created floating-point tensor.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.APyCFloatArray.from_complex(
            ...     [1.0, 1.25, 1.49], exp_bits=4, man_bits=6
            ... )
            >>> a
            APyCFloatArray(
                [ (0, 0),  (0, 0),  (0, 0)],
                [ (7, 0),  (7, 0),  (7, 0)],
                [ (0, 0), (16, 0), (31, 0)],
                exp_bits=4,
                man_bits=6
            )
            >>> print(a)
            [       1+0j,     1.25+0j, 1.484375+0j]

            >>> b = apy.APyCFloatArray.from_complex(
            ...     [
            ...         [1.0, 2.0, 3.0],
            ...         [4.0, 5.0, 6.0],
            ...     ],
            ...     exp_bits=5,
            ...     man_bits=2
            ... )
            >>> b
            APyCFloatArray(
                [[ (0, 0),  (0, 0),  (0, 0)],
                 [ (0, 0),  (0, 0),  (0, 0)]],
            <BLANKLINE>
                [[(15, 0), (16, 0), (16, 0)],
                 [(17, 0), (17, 0), (17, 0)]],
            <BLANKLINE>
                [[ (0, 0),  (0, 0),  (2, 0)],
                 [ (0, 0),  (1, 0),  (2, 0)]],
                exp_bits=5,
                man_bits=2
            )
            >>> print(b)
            [[1+0j, 2+0j, 3+0j],
             [4+0j, 5+0j, 6+0j]]

            Returns
            -------
            :class:`APyCFloatArray`

            See Also
            --------
            fp
            from_array

            )pbdoc"
        )

        .def_static(
            "from_float",
            &APyCFloatArray::from_numbers,
            nb::arg("complex_sequence"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyCFloatArray` from iterable sequence of numbers.

            This is an alias for :func:`~apytypes.APyCFloatArray.from_complex`, look
            there for more documentation.

            Parameters
            ----------
            complex_sequence : :class:`~collections.abc.Iterable` of numbers.
                Values to initialize from. The tensor shape will be taken from the
                sequence shape.
            exp_bits : :class:`int`
                Number of exponent bits in the created floating-point tensor.
            man_bits : :class:`int`
                Number of mantissa bits in the created floating-point tensor.
            bias : :class:`int`, optional
                Bias in the created floating-point tensor.

            Returns
            -------
            :class:`APyCFloatArray`
            )pbdoc"
        )
        .def_static(
            "from_array",
            &APyCFloatArray::from_array,
            nb::arg("ndarray"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyCFloatArray` from an ndarray.

            Parameters
            ----------
            ndarray : ndarray
                Values to initialize from. The tensor shape will be taken from the
                ndarray shape.
            exp_bits : :class:`int`
                Number of exponent bits in the created floating-point tensor
            man_bits : :class:`int`
                Number of mantissa bits in the created floating-point tensor
            bias : :class:`int`, optional
                Bias in the created floating-point tensor

            Examples
            --------

            >>> import apytypes as apy
            >>> import numpy as np
            >>> a = apy.APyCFloatArray.from_array(
            ...     np.array([
            ...         [1.0, 2.0, 3.0],
            ...         [4.0, 5.0, 6.0],
            ...     ]),
            ...     man_bits=10,
            ...     exp_bits=10
            ... )
            >>> print(a)
            [[1+0j, 2+0j, 3+0j],
             [4+0j, 5+0j, 6+0j]]

            Returns
            -------
            :class:`APyCFloatArray`

            See Also
            --------
            fp
            from_float

            )pbdoc"
        )

        /*
         * Public methods
         */
        .def(
            "is_identical",
            &APyCFloatArray::is_identical,
            nb::arg("other"),
            nb::arg("ignore_zero_sign") = false,
            R"pbdoc(
            Test if two :py:class:`APyCFloatArray` objects are identical.

            Two :class:`APyCFloatArray` objects are considered identical if, and only
            if

            - They represent exactly the same tensor shape
            - They store the exact same floating-point values in all elements
            - They have the exact same bit format (`exp_bits`, `man_bits`, and
                `bias`)

            Parameters
            ----------
            other : :class:`APyCFloatArray`
                The floating-point object to test identicality against.

            ignore_zero_sign : :class:`bool`, default: :code:`False`
                If :code:`True`, plus and minus zero are considered identical. If
                :code:`False`, plus and minus zero are considered different.

            Returns
            -------
            :class:`bool`
            )pbdoc"
        )

        .def("reshape", &APyCFloatArray::python_reshape, nb::arg("new_shape"), R"pbdoc(
            Reshape the APyCFloatArray to the specified shape without changing its data.

            Parameters
            ----------
            new_shape : :class:`tuple` of :class:`int`
                The new shape should be compatible with the original shape. If a
                dimension is -1, its value will be inferred from the length of the array
                and remaining dimensions. Only one dimension can be -1.

            Raises
            ------
            :class:`ValueError`
                If negative dimensions less than -1 are provided, if the total size of
                the new array is not unchanged and divisible by the known dimensions, or
                if the total number of elements does not match the original array.

            Examples
            --------
            >>> import apytypes as apy
            >>> arr = apy.APyCFloatArray.from_float(
            ...     [1, 2, -3, -4], exp_bits=8, man_bits=23
            ... )
            >>> print(arr)
            [ 1+0j,  2+0j, -3+0j, -4+0j]
            >>> print(arr.reshape((2, 2)))
            [[ 1+0j,  2+0j],
             [-3+0j, -4+0j]]
            >>> print(arr.reshape((4, 1)))
            [[ 1+0j],
             [ 2+0j],
             [-3+0j],
             [-4+0j]]

            Returns
            -------
            :class:`APyCFloatArray`
            )pbdoc")

        .def(
            "transpose",
            &APyCFloatArray::transpose,
            nb::arg("axes") = nb::none(),
            R"pbdoc(
            Return copy of array with axes transposed.

            For a 1-D array, this return the same array.
            For a 2-D array, this is the standard matrix transpose.
            For an n-D array, if axes are given, their order indicates how the
            axes are permuted (see Examples). If axes are not provided, then
            ``a.transpose(a).shape == a.shape[::-1]``.

            Parameters
            ----------
            axes : :class:`tuple` of :class:`int`, optional
                If specified, it must be a tuple or list which contains a
                permutation of [0,1,...,N-1] where N is the number of axes of
                `a`. The `i`'th axis of the returned array will correspond to the
                axis numbered ``axes[i]`` of the input. If not specified,
                defaults to ``range(a.ndim)[::-1]``, which reverses the order of
                the axes.

            Examples
            --------
            >>> import apytypes as apy
            >>> a = apy.APyCFloatArray.from_float(
            ...         [[ 1.0,  2.0,  3.0],
            ...          [-4.0, -5.0, -6.0]],
            ...         exp_bits=5,
            ...         man_bits=5
            ...     )
            >>> print(a)
            [[ 1+0j,  2+0j,  3+0j],
             [-4+0j, -5+0j, -6+0j]]
            >>> print(a.transpose())
            [[ 1+0j, -4+0j],
             [ 2+0j, -5+0j],
             [ 3+0j, -6+0j]]

            >>> b = apy.APyCFloatArray.from_float(
            ...         [1.0] * 6,
            ...         exp_bits=5,
            ...         man_bits=5
            ...     ).reshape((1, 2, 3))
            >>> b.transpose((1, 0, 2)).shape
            (2, 1, 3)
            >>> b.transpose((-2, -3, -1)).shape
            (2, 1, 3)

            Returns
            -------
            :class:`APyCFloatArray`
                `a` with its axes permuted.
            )pbdoc"
        )

        .def(
            "squeeze",
            &APyCFloatArray::squeeze,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Remove axes of size one at the specified axis/axes.

            If no axis is given, remove all dimensions with size one.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axes to squeeze, a given axis with a size other than one will
                result in an error. No given axes  will be remove all dimensions
                of size one.

            Returns
            -------
            :class:`APyCFloatArray`

            Raises
            ------
            :class:`ValueError`
                If given an axis of a size other than one a ValueError will be
                thrown.
            :class:`IndexError`
                If a specified axis is outside of the existing number of
                dimensions for the array.

            )pbdoc"
        )

        .def(
            "swapaxes",
            &APyCFloatArray::swapaxes,
            nb::arg("axis1"),
            nb::arg("axis2"),
            R"pbdoc(
            Interchange two axes of an array.

            Parameters
            ----------
            axis1 : :class:`int`
                First axis.
            axis2 : :class:`int`
                Second axis.

            Examples
            --------
            >>> import apytypes as apy
            >>> a = apy.APyCFloatArray.from_float(
            ...     [[1, 2, 3]], exp_bits=5, man_bits=2
            ... )
            >>> print(a)
            [[1+0j, 2+0j, 3+0j]]
            >>> print(a.swapaxes(0, 1))
            [[1+0j],
             [2+0j],
             [3+0j]]

            >>> b = apy.APyCFloatArray.from_float(
            ...     [[[0, 1],
            ...       [2, 3]],
            ...
            ...      [[4, 5],
            ...       [6, 7]]],
            ...     exp_bits=5,
            ...     man_bits=5
            ... )
            >>> print(b)
            [[[0+0j, 1+0j],
              [2+0j, 3+0j]],
            <BLANKLINE>
             [[4+0j, 5+0j],
              [6+0j, 7+0j]]]
            >>> print(b.swapaxes(0,2))
            [[[0+0j, 4+0j],
              [2+0j, 6+0j]],
            <BLANKLINE>
             [[1+0j, 5+0j],
              [3+0j, 7+0j]]]

            Returns
            -------
            a_swapped : :class:`APyCFloatArray`
                Copy of `a` with axes swapped
            )pbdoc"
        )
        .def("flatten", &APyCFloatArray::flatten, R"pbdoc(
            Return a copy of the array collapsed into one dimension.

            Examples
            --------
            >>> import apytypes as apy
            >>> arr = apy.APyCFloatArray.from_float([[ 1,  2],
            ...                                      [-3, -4]], exp_bits=8, man_bits=23)
            >>> print(arr)
            [[ 1+0j,  2+0j],
             [-3+0j, -4+0j]]
            >>> print(arr.flatten())
            [ 1+0j,  2+0j, -3+0j, -4+0j]

            Returns
            -------
            :class:`APyCFloatArray`
            )pbdoc")
        .def("ravel", &APyCFloatArray::ravel, R"pbdoc(
            Return a copy of the array collapsed into one dimension. Same as flatten
            with current memory-copy model.

            Examples
            --------
            >>> import apytypes as apy
            >>> arr = apy.APyCFloatArray.from_float([[ 1,  2],
            ...                                      [-3, -4]], exp_bits=8, man_bits=23)
            >>> print(arr)
            [[ 1+0j,  2+0j],
             [-3+0j, -4+0j]]
            >>> print(arr.ravel())
            [ 1+0j,  2+0j, -3+0j, -4+0j]

            Returns
            -------
            :class:`APyCFloatArray`
            )pbdoc")

        .def_static(
            "zeros",
            &APyCFloatArray::zeros,
            nb::arg("shape"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = std::nullopt,
            R"pbdoc(
            Initialize an array with zeros.

            Parameters
            ----------
            shape : :class:`tuple`
                Shape of the array.
            exp_bits : :class:`int`
                Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Returns
            -------
            :class:`APyCFloatArray`
                An array filled with zeros.
            )pbdoc"
        )
        .def_static(
            "ones",
            &APyCFloatArray::ones,
            nb::arg("shape"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = std::nullopt,
            R"pbdoc(
            Initialize an array with ones.

            Parameters
            ----------
            shape : :class:`tuple`
                Shape of the array.
            exp_bits : :class:`int`
                Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Returns
            -------
            :class:`APyCFloatArray`
                An array filled with ones.
            )pbdoc"
        )
        .def_static(
            "eye",
            &APyCFloatArray::eye,
            nb::arg("n"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("m") = nb::none(),
            nb::arg("bias") = std::nullopt,
            R"pbdoc(
            Initialize an array with ones on the diagonal.

            Parameters
            ----------
            n : :class:`int`
                Number of rows (and columns) in the n x n output.
            exp_bits : :class:`int`
                Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            m : :class:`int`, optional
                Number of columns. Default is None, which results in an n x n output.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Returns
            -------
            :class:`APyCFloatArray`
                An array with the specified value on the diagonal.
            )pbdoc"
        )
        .def_static(
            "identity",
            &APyCFloatArray::identity,
            nb::arg("n"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = std::nullopt,
            R"pbdoc(
            Initialize an identity matrix with ones on the diagonal.

            Parameters
            ----------
            n : :class:`int`
                Number of rows (and columns) in the n x n output.
            exp_bits : :class:`int`
                Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Returns
            -------
            :class:`APyCFloatArray`
                An identity matrix with ones on the diagonal.
            )pbdoc"
        )
        .def_static(
            "full",
            &APyCFloatArray::full,
            nb::arg("shape"),
            nb::arg("fill_value"),
            R"pbdoc(
            Initialize an array filled with the specified value.

            Parameters
            ----------
            shape : :class:`tuple`
                Shape of the array.
            fill_value : APyFloat
                Value to fill the array.

            Returns
            -------
            :class:`APyCFloatArray`
                An array filled with the specified value.
            )pbdoc"
        )
        .def_static(
            "_meshgrid",
            &APyCFloatArray::meshgrid,
            nb::arg("arrays"),
            nb::arg("indexing") = "xy",
            R"pbdoc(
            Create a list of coordinate arrays based on several coordinate vectors (1-D arrays).
            The input vectors must all have the same bit specifiers.

            .. versionadded:: 0.4

            Parameters
            ----------
            arrays : :class:`APyCFloatArray`
                The coordinate vectors (1-D arrays).
            indexing : {"xy", "ij"}, default: "xy"
                The indexing mode to use for the meshgrid. "xy" is the standard Cartesian indexing,
                while "ij" is the matrix indexing.

            Returns
            -------
            result : list of :class:`APyCFloatArray`
                List of 2-D arrays representing the meshgrid.

            Examples
            --------
            >>> import apytypes as apy
            >>> x = apy.fp(range(3), exp_bits=5, man_bits=10, force_complex=True)
            >>> y = apy.fp(range(4, 7), exp_bits=5, man_bits=10, force_complex=True)
            >>> xx, yy = apy.meshgrid(x, y)
            >>> xx
            APyCFloatArray(
                [[ (0, 0),  (0, 0),  (0, 0)],
                [ (0, 0),  (0, 0),  (0, 0)],
                [ (0, 0),  (0, 0),  (0, 0)]],

                [[ (0, 0), (15, 0), (16, 0)],
                [ (0, 0), (15, 0), (16, 0)],
                [ (0, 0), (15, 0), (16, 0)]],

                [[ (0, 0),  (0, 0),  (0, 0)],
                [ (0, 0),  (0, 0),  (0, 0)],
                [ (0, 0),  (0, 0),  (0, 0)]],
                exp_bits=5,
                man_bits=10
            )

            >>> yy
            APyCFloatArray(
                [[  (0, 0),   (0, 0),   (0, 0)],
                [  (0, 0),   (0, 0),   (0, 0)],
                [  (0, 0),   (0, 0),   (0, 0)]],

                [[ (17, 0),  (17, 0),  (17, 0)],
                [ (17, 0),  (17, 0),  (17, 0)],
                [ (17, 0),  (17, 0),  (17, 0)]],

                [[  (0, 0),   (0, 0),   (0, 0)],
                [(256, 0), (256, 0), (256, 0)],
                [(512, 0), (512, 0), (512, 0)]],
                exp_bits=5,
                man_bits=10
            )
            )pbdoc"
        )

        .def(
            "to_numpy",
            &APyCFloatArray::to_numpy,
            nb::arg("dtype") = nb::none(),
            nb::arg("copy") = nb::none(),
            R"pbdoc(
            Return array as a :class:`numpy.ndarray` of :class:`numpy.complex128`.

            The returned array has the same `shape` and values as `self`. This
            method rounds away from infinity on ties.

            Parameters
            ----------
            dtype : :std:doc:`numpy:dtype`
                The desired data type of the output array. This parameter is currently ignored.
            copy : :class:`bool`
                Whether to copy the data or not. Must be :code:`True` or :code:`None`.

            Returns
            -------
            :class:`numpy.ndarray`
            )pbdoc"
        )

        .def(
            "broadcast_to",
            &APyCFloatArray::broadcast_to_python,
            nb::arg("shape"),
            R"pbdoc(
            Broadcast array to new shape.

            Parameters
            ----------
            shape : :class:`tuple` of :class:`int` or :class:`int`
                The shape to broadcast to. A single integer ``i`` is interpreted as
                ``(i,)``.

            Returns
            -------
            :class:`APyCFloatArray`
            )pbdoc"
        )

        .def(
            "cast",
            &APyCFloatArray::cast,
            nb::arg("exp_bits") = nb::none(),
            nb::arg("man_bits") = nb::none(),
            nb::arg("bias") = nb::none(),
            nb::arg("quantization") = nb::none(),
            R"pbdoc(
            Change format of the floating-point array.

            This is the primary method for performing quantization when dealing with
            APyTypes floating-point arrays.

            Parameters
            ----------
            exp_bits : :class:`int`, optional
                Number of exponent bits in the result.
            man_bits : :class:`int`, optional
                Number of mantissa bits in the result.
            bias : :class:`int`, optional
                Bias used in the result.
            quantization : :class:`QuantizationMode`, optional.
                Quantization mode to use in this cast. If None, use the global
                quantization mode.

            Returns
            -------
            :class:`APyCFloatArray`
            )pbdoc"
        )

        .def("sum", &APyCFloatArray::sum, nb::arg("axis") = nb::none(), R"pbdoc(
            Return the sum of the elements along specified axis/axes.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis/axes to summate across. Will summate the whole array if no int
                or tuple is specified.

            Returns
            -------
            :class:`APyCFloatArray` or :class:`APyCFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------
            >>> import apytypes as apy
            >>> a = apy.APyCFloatArray.from_float(
            ...     [1, 2, 3, 4, 5, 6],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )
            >>> print(a)
            [1+0j, 2+0j, 3+0j, 4+0j, 5+0j, 6+0j]
            >>> print(a.sum())
            (21+0j)

            -------
            )pbdoc")

        .def("cumsum", &APyCFloatArray::cumsum, nb::arg("axis") = nb::none(), R"pbdoc(
            Return the cumulative sum of the elements along a given axis.

            Parameters
            ----------
            axis : :class:`int`, optional
                The axis to summate across. If not given an axis it will return the
                cumulative sum of the flattened array.

            Returns
            -------
            :class:`APyCFloatArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------
            >>> import apytypes as apy
            >>> a = apy.APyCFloatArray.from_float([[1, 2, 3],
            ...                                    [4, 5, 6]], exp_bits=10, man_bits=10)
            >>> print(a)
            [[1+0j, 2+0j, 3+0j],
             [4+0j, 5+0j, 6+0j]]
            >>> print(a.cumsum())
            [ 1+0j,  3+0j,  6+0j, 10+0j, 15+0j, 21+0j]
            >>> print(a.cumsum(0))
            [[1+0j, 2+0j, 3+0j],
             [5+0j, 7+0j, 9+0j]]
            >>> print(a.cumsum(1))
            [[ 1+0j,  3+0j,  6+0j],
             [ 4+0j,  9+0j, 15+0j]]

            -------
            )pbdoc")

        .def("nansum", &APyCFloatArray::nansum, nb::arg("axis") = nb::none(), R"pbdoc(
            Return the sum of the elements along specified axis/axes treating NaN as 0.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis/axes to summate across. Will summate the whole array if no int
                or tuple is specified.

            Returns
            -------
            :class:`APyCFloatArray` or :class:`APyCFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------
            >>> import apytypes as apy
            >>> nan = float("nan")
            >>> a = apy.APyCFloatArray.from_float(
            ...     [1, 2, 3, 4, 5, nan],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )
            >>> print(a)
            [  1+0j,   2+0j,   3+0j,   4+0j,   5+0j, nan+0j]
            >>> print(a.sum())
            (nan+0j)
            >>> print(a.nansum())
            (15+0j)

            -------
            )pbdoc")

        .def(
            "nancumsum",
            &APyCFloatArray::nancumsum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return cumulative sum of elements along a given axis treating NaN as 0.

            Parameters
            ----------
            axis : :class:`int`, optional
                The axis to summate across. If not given an axis it will return the
                cumulative sum of the flattened array.

            Returns
            -------
            :class:`APyCFloatArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------
            >>> import apytypes as apy
            >>> nan = float("nan")
            >>> a = apy.APyCFloatArray.from_float(
            ...     [[  1,   2,   3],
            ...      [  4,   nan, 6]],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )
            >>> print(a)
            [[  1+0j,   2+0j,   3+0j],
             [  4+0j, nan+0j,   6+0j]]
            >>> print(a.cumsum())
            [  1+0j,   3+0j,   6+0j,  10+0j, nan+0j, nan+0j]
            >>> print(a.nancumsum())
            [ 1+0j,  3+0j,  6+0j, 10+0j, 10+0j, 16+0j]

            >>> print(a.cumsum(0))
            [[  1+0j,   2+0j,   3+0j],
             [  5+0j, nan+0j,   9+0j]]
            >>> print(a.nancumsum(0))
            [[1+0j, 2+0j, 3+0j],
             [5+0j, 2+0j, 9+0j]]

            >>> print(a.cumsum(1))
            [[  1+0j,   3+0j,   6+0j],
             [  4+0j, nan+0j, nan+0j]]
            >>> print(a.nancumsum(1))
            [[ 1+0j,  3+0j,  6+0j],
             [ 4+0j,  4+0j, 10+0j]]

            -------
            )pbdoc"
        )

        .def("prod", &APyCFloatArray::prod, nb::arg("axis") = nb::none(), R"pbdoc(
            Return product of elements along specified axis/axes.

            Parameters
            ----------
            axis : :class:`tuple`, int, optional
                The axis/axes to calculate the product across. If not given an axis it
                will return the product of the flattened array.

            Returns
            -------
            :class:`APyCFloatArray` or :class:`APyCFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------
            >>> import apytypes as apy
            >>> a = apy.APyCFloatArray.from_float(
            ...     [1, 2, 3, 4, 5, 6], exp_bits=10, man_bits=10
            ... )
            >>> print(a)
            [1+0j, 2+0j, 3+0j, 4+0j, 5+0j, 6+0j]
            >>> print(a.prod())
            (720+0j)

            -------
            )pbdoc")

        .def("cumprod", &APyCFloatArray::cumprod, nb::arg("axis") = nb::none(), R"pbdoc(
            Return cumulative product of elements along a given axes.

            Parameters
            ----------
            axis : :class:`int`, optional
                The axes to calculate the product across. If not given an axis it will
                return the cumulative product of the flattened array.

            Returns
            -------
            :class:`APyCFloatArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------
            >>> import apytypes as apy
            >>> a = apy.APyCFloatArray.from_float(
            ...     [[1, 2, 3],
            ...      [4, 5, 6]],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )
            >>> print(a)
            [[1+0j, 2+0j, 3+0j],
             [4+0j, 5+0j, 6+0j]]
            >>> print(a.cumprod())
            [  1+0j,   2+0j,   6+0j,  24+0j, 120+0j, 720+0j]
            >>> print(a.cumprod(0))
            [[ 1+0j,  2+0j,  3+0j],
             [ 4+0j, 10+0j, 18+0j]]
            >>> print(a.cumprod(1))
            [[  1+0j,   2+0j,   6+0j],
             [  4+0j,  20+0j, 120+0j]]

            -------
            )pbdoc")

        .def("nanprod", &APyCFloatArray::nanprod, nb::arg("axis") = nb::none(), R"pbdoc(
            Return product of the elements along a given axis treating NaN as 0.

            Parameters
            ----------
            axis : :class:`int`, optional
                The axis to calculate the product across. If not given an axis it will
                return the product of the flattened array.

            Returns
            -------
            :class:`APyCFloatArray` or :class:`APyCFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.
            )pbdoc")

        .def(
            "nancumprod",
            &APyCFloatArray::nancumprod,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return cumulative product of elements along a given axis treating NaN as 0.

            Parameters
            ----------
            axis : :class:`int`, optional
                The axis to calculate the product across. If not given an axis it will
                return the cumulative product of the flattened array.

            Returns
            -------
            :class:`APyCFloatArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            )pbdoc"
        )
        .def(
            "convolve",
            &APyCFloatArray::convolve,
            nb::arg("other"),
            nb::arg("mode") = "full",
            R"pbdoc(
            Return the discrete linear convolution with another one-dimensional array.

            Requires that ``ndim = 1`` for both `self` and `other`.

            Parameters
            ----------
            other : :class:`APyCFloatArray`
                The one-dimensional array of length :code:`N` to convolve with.

            mode : {'full', 'same', 'valid'}, default: 'full'
                'full':
                    Return the full convolution for each point of overlap. The
                    resulting single-dimensional shape will have length :code:`N + M -
                    1`. Boundary effects occurs for points where the `a` and `v` do not
                    overlap completely.
                'same':
                    Return a convolution of length :code:`max(M, N)`. Boundary effects
                    still occur around the edges of the result.
                'valid':
                    Return the convolution for each point of full overlap. The
                    resulting single-dimensional shape will have length :code:`max(M, N)
                    - min(M, N) + 1`

            Returns
            -------
            convolved : :class:`APyCFloatArray`
                The convolved array.
            )pbdoc"
        )
        .def(
            "conj",
            &APyCFloatArray::conj,
            R"pbdoc(
            Return complex conjugate.

            .. versionadded:: 0.5

            Returns
            -------
            :class:`APyCFloatArray`
            )pbdoc"
        )
        .def(
            "hermitian_transpose",
            &APyCFloatArray::hermitian_transpose,
            R"pbdoc(
            Return the Hermitian transpose.

            This is equivalent to calling :func:`transpose` followed by :func:`conj`.

            .. versionadded:: 0.5

            Returns
            -------
            :class:`APyCFloatArray`
            )pbdoc"
        )
        .def(
            "outer",
            &APyCFloatArray::outer_product,
            nb::arg("rhs"),
            R"pbdoc(
            Compute the outer product between `self` and `rhs`.

            The outer product of 1-D arrays `a` and `b` of length :code:`M` and
            :code:`N`, respectively, is

            .. code-block:: python

                [[   a_0*b_0,     a_0*b_1,   ...     a_0*b_{N-1}   ],
                 [   a_1*b_0,        .                    .        ],
                 [      .                     .           .        ],
                 [ a_{M-1}*b_0,     ...      ...   a_{M-1}*b_{N-1} ]]

            .. versionadded:: 0.5

            Parameters
            ----------
            rhs : :class:`APyCFloatArray`
                Second 1-D input vector to outer product.

            Returns
            -------
            :class:`APyCFloatArray`
            )pbdoc"
        )

        /*
         * Dunder methods
         */
        .def("__matmul__", &APyCFloatArray::matmul, nb::arg("rhs"))
        .def("__repr__", &APyCFloatArray::repr)
        .def("__str__", &APyCFloatArray::to_string, nb::arg("base") = 10)
        .def("__getitem__", &APyCFloatArray::get_item, nb::arg("key"))
        .def("__setitem__", &APyCFloatArray::set_item, nb::arg("key"), nb::arg("val"))
        .def("__len__", &APyCFloatArray::size)
        .def(
            "__array__",
            &APyCFloatArray::to_numpy,
            nb::arg("dtype") = nb::none(),
            nb::arg("copy") = nb::none()
        );
}
