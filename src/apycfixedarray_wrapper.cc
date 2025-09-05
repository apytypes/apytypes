#include "apycfixed.h"
#include "apycfixedarray.h"
// #include "apycfixedarray_iterator.h"

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/optional.h>

namespace nb = nanobind;

#include <functional>
#include <type_traits>

/*
 * Binding function of a custom R-operator (e.g., `__rmul__`) with non APyCFixedArray
 * type
 */
template <auto FUNC, typename L_TYPE>
static APyCFixedArray R_OP(const APyCFixedArray& rhs, const L_TYPE& lhs)
{
    if constexpr (std::is_same_v<std::complex<double>, L_TYPE>) {
        return (rhs.*FUNC)(
            APyCFixed::from_complex(lhs, rhs.int_bits(), rhs.frac_bits())
        );
    } else if constexpr (std::is_floating_point_v<L_TYPE>) {
        return (rhs.*FUNC)(
            APyCFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits())
        );
    } else if constexpr (std::is_same_v<remove_cvref_t<L_TYPE>, APyCFixed>) {
        return (rhs.*FUNC)(lhs);
    } else if constexpr (std::is_same_v<remove_cvref_t<L_TYPE>, APyFixed>) {
        return (rhs.*FUNC)(
            APyCFixed::from_apyfixed(lhs, lhs.int_bits(), lhs.frac_bits())
        );
    } else {
        return (rhs.*FUNC)(
            APyCFixed::from_integer(lhs, rhs.int_bits(), rhs.frac_bits())
        );
    }
}

/*
 * Binding function of a custom L-operator (e.g., `__sub__`) with non APyCFixedArray
 * type
 */
template <typename OP, typename R_TYPE>
static APyCFixedArray L_OP(const APyCFixedArray& lhs, const R_TYPE& rhs)
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

using complex_t = std::complex<double>;

void bind_cfixed_array(nb::module_& m)
{
    nb::class_<APyCFixedArray>(m, "APyCFixedArray")

        /*
         * Constructor: construct from a Python sequence of integers
         */
        .def(
            nb::init<
                const nb::typed<nb::iterable, nb::any>&,
                std::optional<int>,
                std::optional<int>,
                std::optional<int>>(),
            nb::arg("bit_pattern_sequence"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none()
        )

        /*
         * Copy
         */
        .def(
            "copy",
            &APyCFixedArray::python_copy,
            R"pbdoc(
            Create a copy of the object.

            .. versionadded:: 0.3
            )pbdoc"
        )
        .def("__copy__", &APyCFixedArray::python_copy)
        .def("__deepcopy__", &APyCFixedArray::python_deepcopy, nb::arg("memo"))

        /*
         * Arithmetic operations
         */
        .def(nb::self + nb::self, NB_NARG())
        .def(nb::self - nb::self, NB_NARG())
        .def(nb::self * nb::self, NB_NARG())
        .def(nb::self / nb::self, NB_NARG())
        .def(-nb::self)
        .def(+nb::self)
        .def(nb::self <<= int(), nb::rv_policy::none)
        .def(nb::self >>= int(), nb::rv_policy::none)
        .def(nb::self << int())
        .def(nb::self >> int())

        /*
         * Arithmetic operations with `APyCFixed`
         */
        .def("__add__", L_OP<STD_ADD<>, APyCFixed>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, APyCFixed>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, APyCFixed>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyCFixedArray::rsub, APyCFixed>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, APyCFixed>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, APyCFixed>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, APyCFixed>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyCFixedArray::rdiv, APyCFixed>, NB_OP(), NB_NARG())

        /*
         * Arithmetic operation with complex
         */
        .def("__add__", L_OP<STD_ADD<>, complex_t>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, complex_t>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, complex_t>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyCFixedArray::rsub, complex_t>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, complex_t>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, complex_t>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, complex_t>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyCFixedArray::rdiv, complex_t>, NB_OP(), NB_NARG())

        /*
         * Arithmetic operation with floats
         */
        .def("__add__", L_OP<STD_ADD<>, double>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, double>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, double>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyCFixedArray::rsub, double>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, double>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, double>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, double>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyCFixedArray::rdiv, double>, NB_OP(), NB_NARG())

        /*
         * Arithmetic operations with integers
         */
        .def("__add__", L_OP<STD_ADD<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyCFixedArray::rsub, nb::int_>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyCFixedArray::rdiv, nb::int_>, NB_OP(), NB_NARG())

        /*
         * Logic operations
         */
        .def(~nb::self)

        /*
         * Get real and imaginary part
         */
        .def_prop_ro("real", &APyCFixedArray::get_real, R"pbdoc(
            Real part.

            Returns
            -------
            :class:`APyFixedArray`
            )pbdoc")
        .def_prop_ro("imag", &APyCFixedArray::get_imag, R"pbdoc(
            Imaginary part.

            Returns
            -------
            :class:`APyFixedArray`
            )pbdoc")

        /*
         * Properties and methods
         */
        .def_prop_ro("bits", &APyCFixedArray::bits, R"pbdoc(
            Total number of bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("int_bits", &APyCFixedArray::int_bits, R"pbdoc(
            Number of integer bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("frac_bits", &APyCFixedArray::frac_bits, R"pbdoc(
            Number of fractional bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("shape", &APyCFixedArray::python_get_shape, R"pbdoc(
            The shape of the array.

            Returns
            -------
            :class:`tuple` of :class:`int`
            )pbdoc")

        .def_prop_ro("ndim", &APyCFixedArray::ndim, R"pbdoc(
            Number of dimensions in the array.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro(
            "T",
            [](const APyCFixedArray& self) { return self.transpose(); },
            R"pbdoc( The transposition of the array.

            Equivalent to calling :func:`APyCFixedArray.transpose`.

            Returns
            -------
            :class:`APyCFixedArray`
            )pbdoc"
        )

        .def(
            "to_numpy",
            &APyCFixedArray::to_numpy,
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

        //.def(
        //    "to_bits",
        //    &APyCFixedArray::to_bits,
        //    nb::arg("numpy") = false,
        //    R"pbdoc(
        //    Return the underlying bit representations.

        //    When `numpy` is true, the bit representations are returned in a
        //    :class:`numpy.ndarray`. Otherwise, they are returned in a
        //    :class:`list`.

        //    Returns
        //    -------
        //    :class:`list` or :class:`numpy.ndarray`
        //    )pbdoc"
        //)

        .def("reshape", &APyCFixedArray::python_reshape, nb::arg("new_shape"), R"pbdoc(
            Reshape the APyCFixedArray to the specified shape without changing its data.

            Parameters
            ----------
            new_shape : :class:`tuple` of :class:`int`
                The new shape should be compatible with the original shape. If a
                dimension is -1, its value will be inferred from the length of the
                array and remaining dimensions. Only one dimension can be -1.

            Raises
            ------
            :class:`ValueError`
                If negative dimensions less than -1 are provided, if the total size
                of the new array is not unchanged and divisible by the known
                dimensions, or if the total number of elements does not match the
                original array.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.APyCFixedArray([2, 3, 4, 5], int_bits=5, frac_bits=0)
            >>> print(a)
            [2+0j, 3+0j, 4+0j, 5+0j]
            >>> print(a.reshape((2, 2)))
            [[2+0j, 3+0j],
             [4+0j, 5+0j]]
            >>> print(a.reshape((4, 1)))
            [[2+0j],
             [3+0j],
             [4+0j],
             [5+0j]]
            >>> print(a.reshape((2, -1)))
            [[2+0j, 3+0j],
             [4+0j, 5+0j]]

            Returns
            -------
            :class:`APyCFixedArray`
        )pbdoc")

        .def("flatten", &APyCFixedArray::flatten, R"pbdoc(
            Return a copy of the array collapsed into one dimension.

            Examples
            --------
            >>> import apytypes as apy
            >>> a = apy.APyCFixedArray.from_complex(
            ...     [[2, 3],
            ...      [4, 5]], int_bits=7, frac_bits=0
            ... )
            >>> a
            APyCFixedArray([[(2, 0), (3, 0)],
                            [(4, 0), (5, 0)]], int_bits=7, frac_bits=0)
            >>> a.flatten()
            APyCFixedArray([(2, 0), (3, 0), (4, 0), (5, 0)], int_bits=7, frac_bits=0)

            Returns
            -------
            :class:`APyCFixedArray`
        )pbdoc")

        .def("ravel", &APyCFixedArray::ravel, R"pbdoc(
            Return a copy of the array collapsed into one dimension. Same as flatten
            with current memory-copy model.

            Examples
            --------
            >>> import apytypes as apy
            >>> a = apy.APyCFixedArray.from_complex(
            ...     [[2, 3],
            ...      [4, 5]], int_bits=7, frac_bits=0
            ... )
            >>> a
            APyCFixedArray([[(2, 0), (3, 0)],
                            [(4, 0), (5, 0)]], int_bits=7, frac_bits=0)

            >>> a.ravel()
            APyCFixedArray([(2, 0), (3, 0), (4, 0), (5, 0)], int_bits=7, frac_bits=0)

            Returns
            -------
            :class:`APyCFixedArray`
        )pbdoc")

        .def(
            "is_identical",
            &APyCFixedArray::is_identical,
            nb::arg("other"),
            R"pbdoc(
            Test if two :class:`APyCFixedArray` objects are identical.

            Two :class:`APyCFixedArray` objects are considered identical if, and only
            if:
              * They represent exactly the same tensor shape
              * They store the exact same fixed-point values in all tensor elements
              * They have the exact same bit specification (`bits`, `int_bits`, and
                `frac_bits`) are all equal)

            Returns
            -------
            :class:`bool`
        )pbdoc"
        )

        .def(
            "swapaxes",
            &APyCFixedArray::swapaxes,
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
            >>> a = apy.APyCFixedArray.from_float([[1, 2, 3]], bits=5, frac_bits=0)
            >>> print(a)
            [[1+0j, 2+0j, 3+0j]]
            >>> print(a.swapaxes(0,1))
            [[1+0j],
             [2+0j],
             [3+0j]]

            >>> b = apy.APyCFixedArray.from_float(
            ...         [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            ...         bits=5,
            ...         frac_bits=0
            ...     )
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
            a_swapped : :class:`APyCFixedArray`
                Copy of `a` with axes swapped
            )pbdoc"
        )

        .def(
            "transpose",
            &APyCFixedArray::transpose,
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
            >>> a = apy.APyCFixedArray.from_float(
            ...         [[1.0,   2.0,  3.0],
            ...          [-4.0, -5.0, -6.0]],
            ...         bits=5,
            ...         frac_bits=0
            ...     )
            >>> print(a)
            [[ 1+0j,  2+0j,  3+0j],
             [-4+0j, -5+0j, -6+0j]]
            >>> print(a.transpose())
            [[ 1+0j, -4+0j],
             [ 2+0j, -5+0j],
             [ 3+0j, -6+0j]]

            >>> b = apy.APyCFixedArray.from_float(
            ...         [1.0] * 6,
            ...         bits=5,
            ...         frac_bits=0
            ...     ).reshape((1, 2, 3))
            >>> b.transpose((1, 0, 2)).shape
            (2, 1, 3)
            >>> b.transpose((-2, -3, -1)).shape
            (2, 1, 3)

            Returns
            -------
            :class:`APyCFixedArray`
                `a` with its axes permuted.
            )pbdoc"
        )

        .def(
            "cast",
            &APyCFixedArray::cast,
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("quantization") = nb::none(),
            nb::arg("overflow") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Change format of the fixed-point array.

            This is the primary method for performing quantization and
            overflowing/saturation when dealing with APyTypes fixed-point arrays.

            Exactly two of three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must
            be set.

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

            Returns
            -------
            :class:`APyCFixedArray`
            )pbdoc"
        )
        .def(
            "broadcast_to",
            &APyCFixedArray::broadcast_to_python,
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
            :class:`APyCFixedArray`
            )pbdoc"
        )
        .def(
            "convolve",
            &APyCFixedArray::convolve,
            nb::arg("other"),
            nb::arg("mode") = "full",
            R"pbdoc(
            Return the discrete linear convolution with another one-dimensional array.

            Requires that ``ndim = 1`` for both `self` and `other`.

            Parameters
            ----------
            other : :class:`APyCFixedArray`
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
            convolved : :class:`APyCFixedArray`
                The convolved array.
            )pbdoc"
        )

        .def(
            "squeeze",
            &APyCFixedArray::squeeze,
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
            :class:`APyCFixedArray`

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
            "sum",
            &APyCFixedArray::sum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the sum of the elements along specified axis/axes.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis/axes to summate across. Will summate the whole array if
                no int or tuple is specified.

            Returns
            -------
            :class:`APyCFixedArray` or :class:`APyCFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of
                dimensions for the array.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.APyCFixedArray(
            ...     [1, 2, 3, 4, 5, 6],
            ...     int_bits=10,
            ...     frac_bits=0
            ... )
            >>> a.sum()
            APyCFixed((21, 0), bits=13, int_bits=13)

            -------
            )pbdoc"
        )

        .def(
            "cumsum",
            &APyCFixedArray::cumsum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the cumulative sum of the elements along a given axes.

            Parameters
            ----------
            axis : :class:`int`, optional
                The axes to summate across. If not given an axis it will return the
                cumulative sum of the flattened array.

            Returns
            -------
            :class:`APyCFixedArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.APyCFixedArray.from_complex(
            ...     [
            ...         [1+1j, 2+3j, 3-2j],
            ...         [4-1j, 5+2j, 6+0j]
            ...     ],
            ...     int_bits=10,
            ...     frac_bits=0
            ... )
            >>> print(a)
            [[1+1j, 2+3j, 3-2j],
             [4-1j, 5+2j, 6+0j]]

            >>> print(a.cumsum())
            [ 1+1j,  3+4j,  6+2j, 10+1j, 15+3j, 21+3j]

            >>> print(a.cumsum(0))
            [[1+1j, 2+3j, 3-2j],
             [5+0j, 7+5j, 9-2j]]

            >>> print(a.cumsum(1))
            [[ 1+1j,  3+4j,  6+2j],
             [ 4-1j,  9+1j, 15+1j]]

            -------
            )pbdoc"
        )

        .def(
            "nansum",
            &APyCFixedArray::sum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the sum of the elements along specified axis/axes treating NaN as 0.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis/axes to summate across. Will summate the whole array if
                no int or tuple is specified.

            Returns
            -------
            :class:`APyCFixedArray` or :class:`APyCFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of
                dimensions for the array.

            )pbdoc"
        )

        .def(
            "nancumsum",
            &APyCFixedArray::cumsum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the cumulative sum of the elements along a given axis treating NaN as 0.

            Parameters
            ----------
            axis : :class:`int`, optional
                The axis to summate across. If not given an axis it will return the
                cumulative sum of the flattened array.

            Returns
            -------
            :class:`APyCFixedArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            )pbdoc"
        )

        .def(
            "prod",
            &APyCFixedArray::prod,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the product of the elements along specified axis/axes.

            Parameters
            ----------
            axis : :class:`tuple`, int, optional
                The axis/axes to calculate the product across. If not given an axis it
                will return the product of the flattened array.

            Returns
            -------
            :class:`APyCFixedArray` or :class:`APyCFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.APyCFixedArray.from_complex(
            ...     [1, 2 + 1j, 3 + 3j, -4 + 1j, 5 - 2j, 6 - 3j],
            ...     int_bits=10,
            ...     frac_bits=0
            ... )
            >>> a.prod()
            APyCFixed((36893488147419101837, 36893488147419103007), bits=65, int_bits=65)
            >>> print(a.prod())
            (-1395-225j)

            -------
            )pbdoc"
        )

        .def(
            "cumprod",
            &APyCFixedArray::cumprod,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the cumulative product of the elements along a given axes.

            Parameters
            ----------
            axis : :class:`int`, optional
                The axes to calculate the product across. If not given an axis it will
                return the cumulative product of the flattened array.

            Returns
            -------
            :class:`APyCFixedArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.APyCFixedArray.from_complex(
            ...     [[1+1j, 2+1j, 3+2j],
            ...      [4-2j, 5+2j, 6+1j]],
            ...     int_bits=10,
            ...     frac_bits=0
            ... )
            >>> print(a)
            [[1+1j, 2+1j, 3+2j],
             [4-2j, 5+2j, 6+1j]]
            >>> print(a.cumprod())
            [      1+1j,       1+3j,     -3+11j,     10+50j,   -50+270j, -570+1570j]

            >>> print(a.cumprod(0))
            [[  1+1j,   2+1j,   3+2j],
             [  6+2j,   8+9j, 16+15j]]

            >>> print(a.cumprod(1))
            [[   1+1j,    1+3j,  -3+11j],
             [   4-2j,   24-2j, 146+12j]]

            -------
            )pbdoc"
        )

        .def(
            "nanprod",
            &APyCFixedArray::prod,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
                Return the product of the elements along a given axis treating NaN as 0.

                Parameters
                ----------
                axis : :class:`int`, optional
                    The axis to calculate the product across. If not given an axis it
                    will return the product of the flattened array.

                Returns
                -------
                :class:`APyCFixedArray` or :class:`APyCFixed`

                Raises
                ------
                :class:`IndexError`
                    If a specified axis is outside of the existing number of dimensions
                    for the array.
            )pbdoc"
        )

        .def(
            "nancumprod",
            &APyCFixedArray::cumprod,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
                Return the cumulative product of the elements along a given axis
                treating NaN as 0.

                Parameters
                ----------
                axis : :class:`int`, optional
                    The axis to calculate the product across. If not given an axis it
                    will return the cumulative product of the flattened array.

                Returns
                -------
                :class:`APyCFixedArray`

                Raises
                ------
                :class:`IndexError`
                    If a specified axis is outside of the existing number of dimensions
                    for the array.

            )pbdoc"
        )

        /*
         * Static methods
         */
        .def_static(
            "from_complex",
            &APyCFixedArray::from_numbers,
            nb::arg("complex_sequence"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Create an :class:`APyCFixedArray` from iterable sequence of numbers.

            The input is quantized using :class:`QuantizationMode.RND_INF` and overflow
            is handled using the :class:`OverflowMode.WRAP` mode. Exactly two of the
            three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

            Using NumPy arrays as input is in general faster than using e.g. lists.

            Parameters
            ----------
            complex_sequence : :class:`~collections.abc.Iterable` of numbers
                Values to initialize from. The tensor shape will be taken from the
                sequence shape.
            int_bits : :class:`int`, optional
                Number of integer bits in the created fixed-point tensor.
            frac_bits : :class:`int`, optional
                Number of fractional bits in the created fixed-point tensor.
            bits : :class:`int`, optional
                Total number of bits in the created fixed-point tensor.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.APyCFixedArray.from_complex(
            ...         [1.0, 1.25j, 1.49 - 0.5j],
            ...         int_bits=2,
            ...         frac_bits=2
            ...     )
            >>> print(a)
            [    1+0j,  0+1.25j, 1.5-0.5j]

            >>> b = apy.APyCFixedArray.from_complex(
            ...         [
            ...             [1.0 + 1.0j, 2.0 - 3.0j, 3.0 - 1.0j],
            ...             [4.0 - 2.0j, 5.0 + 2.0j, 6.0 + 1j],
            ...         ],
            ...         bits=5,
            ...         frac_bits=0
            ...     )
            >>> print(b)
            [[1+1j, 2-3j, 3-1j],
             [4-2j, 5+2j, 6+1j]]

            Returns
            -------
            :class:`APyCFixedArray`
        )pbdoc"
        )

        .def_static(
            "from_float",
            &APyCFixedArray::from_numbers,
            nb::arg("number_seq"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Create an :class:`APyCFixedArray` from iterable sequence of numbers.

            This is an alias for :func:`~apytypes.APyCFixedArray.from_complex`, look
            there for more documentation.

            Parameters
            ----------
            number_seq : :class:`~collections.abc.Iterable` of numbers
                Values to initialize from. The tensor shape will be taken from the
                sequence shape.
            int_bits : :class:`int`, optional
                Number of integer bits in the created fixed-point tensor.
            frac_bits : :class:`int`, optional
                Number of fractional bits in the created fixed-point tensor.
            bits : :class:`int`, optional
                Total number of bits in the created fixed-point tensor.

            Returns
            -------
            :class:`APyCFixedArray`
        )pbdoc"
        )

        .def_static(
            "from_array",
            &APyCFixedArray::from_array,
            nb::arg("ndarray"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Create an :class:`APyCFixedArray` object from an ndarray.

            The initialized fixed-point values are the one closest to the
            input floating-point value, rounded away from zero on ties. Exactly
            two of the three bit-specifiers (`bits`, `int_bits`, `frac_bits`)
            must be set.

            Using NumPy arrays as input is in general faster than e.g. lists.

            Parameters
            ----------
            ndarray : ndarray
                Values to initialize from. The tensor shape will be taken from
                the ndarray shape.
            int_bits : :class:`int`, optional
                Number of integer bits in the created fixed-point tensor.
            frac_bits : :class:`int`, optional
                Number of fractional bits in the created fixed-point tensor.
            bits : :class:`int`, optional
                Total number of bits in the created fixed-point tensor.

            Examples
            --------

            >>> import apytypes as apy
            >>> import numpy as np
            >>> a = apy.APyCFixedArray.from_array(
            ...     np.array([
            ...         [1.0, 2.0, 3.0],
            ...         [4.0, 5.0, 6.0],
            ...     ]),
            ...     int_bits=10,
            ...     frac_bits=0
            ... )
            >>> print(a)
            [[1+0j, 2+0j, 3+0j],
             [4+0j, 5+0j, 6+0j]]

            Returns
            -------
            :class:`APyCFixedArray`
            )pbdoc"
        )
        .def_static(
            "zeros",
            &APyCFixedArray::zeros,
            nb::arg("shape"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Initialize an array with zeros.

            Parameters
            ----------
            shape : :class:`tuple`
                Shape of the array.
            int_bits : :class:`int`, optional
                Number of integer bits.
            frac_bits : :class:`int`, optional
                Number of fractional bits.
            bits : :class:`int`, optional
                Total number of bits.

            Returns
            -------
            :class:`APyCFixedArray`
                An array initialized with zeros.
            )pbdoc"
        )
        .def_static(
            "ones",
            &APyCFixedArray::ones,
            nb::arg("shape"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Initialize an array with ones.

            Parameters
            ----------
            shape : :class:`tuple`
                Shape of the array.
            int_bits : :class:`int`, optional
                Number of integer bits.
            frac_bits : :class:`int`, optional
                Number of fractional bits.
            bits : :class:`int`, optional
                Total number of bits.

            Returns
            -------
            :class:`APyCFixedArray`
                An array initialized with ones.
            )pbdoc"
        )
        .def_static(
            "eye",
            &APyCFixedArray::eye,
            nb::arg("n"),
            nb::arg("m") = nb::none(),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Initialize an array with ones on the diagonal.

            Parameters
            ----------
            n : :class:`int`
                Number of rows.
            m : :class:`int`, optional
                Number of columns.
            int_bits : :class:`int`, optional
                Number of integer bits.
            frac_bits : :class:`int`, optional
                Number of fractional bits.
            bits : :class:`int`, optional
                Total number of bits.

            Returns
            -------
            :class:`APyCFixedArray`
                An array with ones on the diagonal.
            )pbdoc"
        )
        .def_static(
            "identity",
            &APyCFixedArray::identity,
            nb::arg("n"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Initialize an identity matrix with ones on the diagonal.

            Parameters
            ----------
            n : :class:`int`
                Number of rows (and columns) in *n* x *n* output.
            int_bits : :class:`int`, optional
                Number of integer bits.
            frac_bits : :class:`int`, optional
                Number of fractional bits.
            bits : :class:`int`, optional
                Total number of bits.

            Returns
            -------
            :class:`APyCFixedArray`
                An identity matrix with ones on the diagonal.
            )pbdoc"
        )
        .def_static(
            "full",
            &APyCFixedArray::full,
            nb::arg("shape"),
            nb::arg("fill_value"),
            R"pbdoc(
            Initialize an array with the specified value.

            Parameters
            ----------
            shape : :class:`tuple`
                Shape of the array.
            fill_value : :class:`APyCFixed`
                Value to fill the array.

            Returns
            -------
            :class:`APyCFixedArray`
                An array filled with the specified value.
            )pbdoc"
        )
        .def_static(
            "_meshgrid",
            &APyCFixedArray::meshgrid,
            nb::arg("arrays"),
            nb::arg("indexing") = "xy",
            R"pbdoc(
            Create a list of coordinate arrays based on several coordinate vectors (1-D arrays).
            The input vectors must all have the same bit specifiers.

            .. versionadded:: 0.4

            Parameters
            ----------
            arrays : :class:`APyCFixedArray`
                The coordinate vectors (1-D arrays).
            indexing : {"xy", "ij"}, default: "xy"
                The indexing mode to use for the meshgrid. "xy" is the standard Cartesian indexing,
                while "ij" is the matrix indexing.

            Returns
            -------
            result : list of :class:`APyCFixedArray`
                List of 2-D arrays representing the meshgrid.

            Examples
            --------
            >>> import apytypes as apy
            >>> x = apy.fx(range(3), int_bits=5, frac_bits=0, force_complex=True)
            >>> y = apy.fx(range(4, 7), int_bits=5, frac_bits=0, force_complex=True)
            >>> xx, yy = apy.meshgrid(x, y)
            >>> xx
            APyCFixedArray([[(0, 0), (1, 0), (2, 0)],
                            [(0, 0), (1, 0), (2, 0)],
                            [(0, 0), (1, 0), (2, 0)]], int_bits=5, frac_bits=0)
            >>> yy
            APyCFixedArray([[(4, 0), (4, 0), (4, 0)],
                            [(5, 0), (5, 0), (5, 0)],
                            [(6, 0), (6, 0), (6, 0)]], int_bits=5, frac_bits=0)
            )pbdoc"
        )
        /*
         * Dunder methods
         */
        .def("__matmul__", &APyCFixedArray::matmul, nb::arg("rhs"))
        .def("__repr__", &APyCFixedArray::repr)
        .def("__str__", &APyCFixedArray::to_string, nb::arg("base") = 10)
        .def("__getitem__", &APyCFixedArray::get_item, nb::arg("key"))
        .def("__setitem__", &APyCFixedArray::set_item, nb::arg("key"), nb::arg("val"))
        .def("__len__", &APyCFixedArray::size)
        //     .def(
        //         "__iter__",
        //         [](nb::iterable array) {
        //             return APyCFixedArrayIterator(
        //                 nb::cast<const APyCFixedArray&>(array), array
        //             );
        //         }
        //     )
        .def(
            "__array__",
            &APyCFixedArray::to_numpy,
            nb::arg("dtype") = nb::none(),
            nb::arg("copy") = nb::none()
        )

        ;

    // nb::class_<APyCFixedArrayIterator>(m, "APyCFixedArrayIterator")
    //     .def(
    //         "__iter__",
    //         [](APyCFixedArrayIterator& it) -> APyCFixedArrayIterator& { return it; }
    //     )
    //     .def("__next__", &APyCFixedArrayIterator::next);
}
