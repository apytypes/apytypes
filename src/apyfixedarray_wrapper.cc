#include "apyfixed.h"
#include "apyfixedarray.h"
#include "apyfixedarray_iterator.h"

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/optional.h>

#include <functional>
#include <type_traits>

namespace nb = nanobind;

/*
 * Binding function of a custom R-operator (e.g., `__rmul__`) with non APyFixedArray
 * type
 */
template <auto FUNC, typename L_TYPE>
static APyFixedArray R_OP(const APyFixedArray& rhs, const L_TYPE& lhs)
{
    if constexpr (std::is_floating_point_v<L_TYPE>) {
        return (rhs.*FUNC)(APyFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits()));
    } else if constexpr (std::is_same_v<remove_cvref_t<L_TYPE>, APyFixed>) {
        return (rhs.*FUNC)(lhs);
    } else {
        return (rhs.*FUNC)(
            APyFixed::from_integer(lhs, rhs.int_bits(), rhs.frac_bits())
        );
    }
}

/*
 * Binding function of a custom L-operator (e.g., `__sub__`) with non APyFixedArray type
 */
template <typename OP, typename R_TYPE>
static APyFixedArray L_OP(const APyFixedArray& lhs, const R_TYPE& rhs)
{
    if constexpr (std::is_floating_point_v<R_TYPE>) {
        return OP()(lhs, APyFixed::from_double(rhs, lhs.int_bits(), lhs.frac_bits()));
    } else if constexpr (std::is_same_v<remove_cvref_t<R_TYPE>, APyFixed>) {
        return OP()(lhs, rhs);
    } else if constexpr (std::is_same_v<
                             remove_cvref_t<R_TYPE>,
                             nb::ndarray<nb::c_contig>>) {
        return OP()(
            lhs, APyFixedArray::from_array(rhs, lhs.int_bits(), lhs.frac_bits())
        );
    } else {
        return OP()(lhs, APyFixed::from_integer(rhs, lhs.int_bits(), lhs.frac_bits()));
    }
}

void bind_fixed_array(nb::module_& m)
{
    nb::class_<APyFixedArray>(m, "APyFixedArray")

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
            &APyFixedArray::python_copy,
            R"pbdoc(
            Create a copy of the object.

            .. versionadded:: 0.3
            )pbdoc"
        )
        .def("__copy__", &APyFixedArray::python_copy)
        .def("__deepcopy__", &APyFixedArray::python_deepcopy, nb::arg("memo"))

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
         * Arithmetic operations with integers
         */
        .def("__add__", L_OP<STD_ADD<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyFixedArray::rsub, nb::int_>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyFixedArray::rdiv, nb::int_>, NB_OP(), NB_NARG())

        /*
         * Arithmetic operation with floats
         */
        .def("__add__", L_OP<STD_ADD<>, double>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, double>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, double>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyFixedArray::rsub, double>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, double>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, double>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, double>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyFixedArray::rdiv, double>, NB_OP(), NB_NARG())

        /*
         * Arithmetic operations with APyFixed
         */
        .def("__add__", L_OP<STD_ADD<>, APyFixed>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, APyFixed>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, APyFixed>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyFixedArray::rsub, APyFixed>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, APyFixed>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, APyFixed>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, APyFixed>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyFixedArray::rdiv, APyFixed>, NB_OP(), NB_NARG())

        /*
         * Arithmetic operations with NumPy arrays
         * The right-hand versions are not used since Numpy will convert the
         * APyFixedArray to a Numpy array before they are invoked.
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
         * Logic operations
         */
        .def(~nb::self)

        /*
         * Properties and methods
         */
        .def_prop_ro("bits", &APyFixedArray::bits, R"pbdoc(
            Total number of bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("int_bits", &APyFixedArray::int_bits, R"pbdoc(
            Number of integer bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("frac_bits", &APyFixedArray::frac_bits, R"pbdoc(
            Number of fractional bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("shape", &APyFixedArray::python_get_shape, R"pbdoc(
            The shape of the array.

            Returns
            -------
            :class:`tuple` of :class:`int`
            )pbdoc")

        .def_prop_ro("ndim", &APyFixedArray::ndim, R"pbdoc(
            Number of dimensions in the array.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro(
            "T", [](const APyFixedArray& self) { return self.transpose(); }, R"pbdoc(
            The transposition of the array.

            Equivalent to calling :func:`APyFixedArray.transpose`.

            Returns
            -------
            :class:`APyFixedArray`
            )pbdoc"
        )

        .def(
            "to_numpy",
            &APyFixedArray::to_numpy,
            nb::arg("dtype") = nb::none(),
            nb::arg("copy") = nb::none(),
            R"pbdoc(
            Return array as a :class:`numpy.ndarray` of :class:`numpy.float64`.

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
            "to_bits",
            &APyFixedArray::to_bits,
            nb::arg("numpy") = false,
            R"pbdoc(
            Return the underlying bit representations.

            When `numpy` is true, the bit representations are returned in a
            :class:`numpy.ndarray`. Otherwise, they are returned in a :class:`list`.

            Returns
            -------
            :class:`list` or :class:`numpy.ndarray`
            )pbdoc"
        )
        .def(
            "_to_signed_bits",
            &APyFixedArray::to_signed_bits,
            R"pbdoc(
            Return the underlying bit representations as signed integers.

            Returns
            -------
            :class:`list`
            )pbdoc"
        )

        .def("reshape", &APyFixedArray::python_reshape, nb::arg("new_shape"), R"pbdoc(
            Reshape the APyFixedArray to the specified shape without changing its data.

            Parameters
            ----------
            new_shape : :class:`int` or :class:`tuple` of :class:`int`
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
            >>> from apytypes import fx
            >>> a = fx([2, 3, 4, 5], int_bits=5, frac_bits=1)
            >>> a
            APyFixedArray([ 4,  6,  8, 10], int_bits=5, frac_bits=1)

            >>> a.reshape((2, 2))
            APyFixedArray([[ 4,  6],
                           [ 8, 10]], int_bits=5, frac_bits=1)

            >>> a.reshape((4, 1))
            APyFixedArray([[ 4],
                           [ 6],
                           [ 8],
                           [10]], int_bits=5, frac_bits=1)

            Returns
            -------
            :class:`APyFixedArray`
            )pbdoc")

        .def("flatten", &APyFixedArray::flatten, R"pbdoc(
            Return a copy of the array collapsed into one dimension.

            Examples
            --------
            >>> from apytypes import fx
            >>> a = fx([[2, 3],
            ...         [4, 5]], int_bits=5, frac_bits=1)
            >>> a
            APyFixedArray([[ 4,  6],
                           [ 8, 10]], int_bits=5, frac_bits=1)

            >>> a.flatten()
            APyFixedArray([ 4,  6,  8, 10], int_bits=5, frac_bits=1)

            Returns
            -------
            :class:`APyFixedArray`
        )pbdoc")

        .def("ravel", &APyFixedArray::ravel, R"pbdoc(
            Return a copy of the array collapsed into one dimension. Same as flatten
            with current memory-copy model.

            Examples
            --------
            >>> from apytypes import fx
            >>> a = fx([[2, 3],
            ...         [4, 5]], int_bits=5, frac_bits=1)
            >>> a
            APyFixedArray([[ 4,  6],
                           [ 8, 10]], int_bits=5, frac_bits=1)

            >>> a.ravel()
            APyFixedArray([ 4,  6,  8, 10], int_bits=5, frac_bits=1)

            Returns
            -------
            :class:`APyFixedArray`
        )pbdoc")

        .def("is_identical", &APyFixedArray::is_identical, nb::arg("other"), R"pbdoc(
            Test if two :class:`APyFixedArray` objects are identical.

            Two :class:`APyFixedArray` objects are considered identical if, and only if:
              * They represent exactly the same tensor shape
              * They store the exact same fixed-point values in all tensor elements
              * They have the exact same bit specification (`bits`, `int_bits`, and
                `frac_bits` are all equal)

            Returns
            -------
            :class:`bool`

            )pbdoc")
        .def(
            "swapaxes",
            &APyFixedArray::swapaxes,
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
            >>> a = apy.fx([[1, 2, 3]], bits=5, frac_bits=0)
            >>> a
            APyFixedArray([[1, 2, 3]], int_bits=5, frac_bits=0)
            >>> a.swapaxes(0,1)
            APyFixedArray([[1],
                           [2],
                           [3]], int_bits=5, frac_bits=0)

            >>> b = apy.fx(
            ...     [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            ...     bits=5,
            ...     frac_bits=0
            ... )
            >>> b
            APyFixedArray([[[0, 1],
                            [2, 3]],
            <BLANKLINE>
                           [[4, 5],
                            [6, 7]]], int_bits=5, frac_bits=0)
            >>> b.swapaxes(0,2)
            APyFixedArray([[[0, 4],
                            [2, 6]],
            <BLANKLINE>
                           [[1, 5],
                            [3, 7]]], int_bits=5, frac_bits=0)

            Returns
            -------
            a_swapped : :class:`APyFixedArray`
                Copy of `a` with axes swapped
        )pbdoc"
        )
        .def(
            "transpose",
            &APyFixedArray::transpose,
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
                If specified, it must be a tuple or list which contains a permutation
                of [0,1,...,N-1] where N is the number of axes of `a`. The `i`'th axis
                of the returned array will correspond to the axis numbered ``axes[i]``
                of the input. If not specified, defaults to ``range(a.ndim)[::-1]``,
                which reverses the order of the axes.

            Examples
            --------
            >>> import apytypes as apy
            >>> a = apy.fx(
            ...     [[1.0, 2.0, 3.0],
            ...      [4.0, 5.0, 6.0]],
            ...     bits=5,
            ...     frac_bits=0
            ... )
            >>> a
            APyFixedArray([[1, 2, 3],
                           [4, 5, 6]], int_bits=5, frac_bits=0)
            >>> a.transpose()
            APyFixedArray([[1, 4],
                           [2, 5],
                           [3, 6]], int_bits=5, frac_bits=0)

            >>> b = apy.ones((1, 2, 3), bits=5, frac_bits=0)
            >>> b.transpose((1, 0, 2)).shape
            (2, 1, 3)
            >>> b.transpose((-2, -3, -1)).shape
            (2, 1, 3)

            Returns
            -------
            :class:`APyFixedArray`
                `a` with its axes permuted.
            )pbdoc"
        )
        .def(
            "cast",
            &APyFixedArray::cast,
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
            :class:`APyFixedArray`
            )pbdoc"
        )
        .def(
            "broadcast_to",
            &APyFixedArray::broadcast_to_python,
            nb::arg("shape"),
            R"pbdoc(
            Broadcast array to new shape.

            .. versionadded:: 0.2

            Parameters
            ----------
            shape : :class:`tuple` of :class:`int` or :class:`int`
                The shape to broadcast to. A single integer ``i`` is interpreted as
                ``(i,)``.

            Returns
            -------
            :class:`APyFixedArray`

            )pbdoc"
        )
        .def(
            "convolve",
            &APyFixedArray::convolve,
            nb::arg("other"),
            nb::arg("mode") = "full",
            R"pbdoc(
            Return the discrete linear convolution with another one-dimensional array.

            Requires that ``ndim = 1`` for both `self` and `other`.

            Parameters
            ----------
            other : :class:`APyFixedArray`
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
            convolved : :class:`APyFixedArray`
                The convolved array.

            )pbdoc"
        )

        .def(
            "squeeze",
            &APyFixedArray::squeeze,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Remove axes of size one at the specified axis/axes.

            If no axis is given, remove all dimensions with size one.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axes to squeeze, a given axis with a size other than one will result
                in an error. No given axes  will be remove all dimensions of size one.

            Returns
            -------
            :class:`APyFixedArray`

            Raises
            ------
            :class:`ValueError`
                If given an axis of a size other than one a ValueError will be thrown.
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            )pbdoc"
        )

        .def(
            "sum",
            &APyFixedArray::sum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the sum of the elements along specified axis/axes.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis/axes to summate across. Will summate the whole array if no int
                or tuple is specified.

            Returns
            -------
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------
            >>> import apytypes as apy
            >>> a = apy.APyFixedArray(
            ...     [1, 2, 3, 4, 5, 6],
            ...     int_bits=10,
            ...     frac_bits=0
            ... )
            >>> a.sum()
            APyFixed(21, bits=13, int_bits=13)

            -------
            )pbdoc"
        )

        .def(
            "cumsum",
            &APyFixedArray::cumsum,
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
            :class:`APyFixedArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------
            >>> from apytypes import fx
            >>> a = fx([[1, 2, 3], [4, 5, 6]], int_bits=10, frac_bits=0)
            >>> a
            APyFixedArray([[1, 2, 3],
                           [4, 5, 6]], int_bits=10, frac_bits=0)

            >>> a.cumsum()
            APyFixedArray([ 1,  3,  6, 10, 15, 21], int_bits=13, frac_bits=0)

            >>> a.cumsum(0)
            APyFixedArray([[1, 2, 3],
                           [5, 7, 9]], int_bits=11, frac_bits=0)

            >>> a.cumsum(1)
            APyFixedArray([[ 1,  3,  6],
                           [ 4,  9, 15]], int_bits=12, frac_bits=0)

            -------
            )pbdoc"
        )

        .def(
            "nansum",
            &APyFixedArray::sum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the sum of the elements along specified axis/axes treating NaN as 0.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis/axes to summate across. Will summate the whole array if no int
                or tuple is specified.

            Returns
            -------
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            )pbdoc"
        )

        .def(
            "nancumsum",
            &APyFixedArray::cumsum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the cumulative sum of the elements along a given axis treating NaN as
            0.

            Parameters
            ----------
            axis : :class:`int`, optional
                The axis to summate across. If not given an axis it will return the
                cumulative sum of the flattened array.

            Returns
            -------
            :class:`APyFixedArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            )pbdoc"
        )

        .def(
            "max",
            &APyFixedArray::max,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the maximum value from an array or the maximum values along an axis.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis to get the maximum along.

            Returns
            -------
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------
            >>> from apytypes import fx
            >>> a = fx([[1, 2, 3], [4, 5, 6]], int_bits=10, frac_bits=0)
            >>> a
            APyFixedArray([[1, 2, 3],
                           [4, 5, 6]], int_bits=10, frac_bits=0)

            >>> a.max()
            APyFixed(6, bits=10, int_bits=10)

            >>> a.max(0)
            APyFixedArray([4, 5, 6], int_bits=10, frac_bits=0)

            >>> a.max(1)
            APyFixedArray([3, 6], int_bits=10, frac_bits=0)

            -------

            )pbdoc"
        )

        .def(
            "min",
            &APyFixedArray::min,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the minimum value from an array or the minimum values along an axis.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis to get the minimum along.

            Returns
            -------
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------
            >>> from apytypes import fx
            >>> a = fx([[1, 2, 3], [4, 5, 6]], int_bits=10, frac_bits=0)
            >>> a
            APyFixedArray([[1, 2, 3],
                           [4, 5, 6]], int_bits=10, frac_bits=0)

            >>> a.min()
            APyFixed(1, bits=10, int_bits=10)

            >>> a.min(0)
            APyFixedArray([1, 2, 3], int_bits=10, frac_bits=0)

            >>> a.min(1)
            APyFixedArray([1, 4], int_bits=10, frac_bits=0)

            -------
            )pbdoc"
        )

        .def(
            "nanmax",
            &APyFixedArray::max,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the maximum value from an array or the maximum values along an axis,
            ignoring NaN.

            Issues a warning when encountering an all-nan slice or axis.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis to get the maximum along.

            Returns
            -------
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.
            )pbdoc"
        )

        .def(
            "nanmin",
            &APyFixedArray::min,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the minimum value from an array or the minimum values along an axis,
            ignoring NaN.

            Issues a warning when encountering an all-nan slice or axis.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis to get the minimum along.

            Returns
            -------
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.
            )pbdoc"
        )

        .def(
            "prod",
            &APyFixedArray::prod,
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
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------
            >>> import apytypes as apy
            >>> a = apy.APyFixedArray(
            ...     [1, 2, 3, 4, 5, 6],
            ...     int_bits=10,
            ...     frac_bits=0
            ... )
            >>> a.prod()
            APyFixed(720, bits=60, int_bits=60)

            -------
            )pbdoc"
        )

        .def(
            "cumprod",
            &APyFixedArray::cumprod,
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
            :class:`APyFixedArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------
            >>> from apytypes import fx
            >>> a = fx([[1, 2, 3], [4, 5, 6]], int_bits=10, frac_bits=0)
            >>> a
            APyFixedArray([[1, 2, 3],
                           [4, 5, 6]], int_bits=10, frac_bits=0)

            >>> a.cumprod()
            APyFixedArray([  1,   2,   6,  24, 120, 720], int_bits=60, frac_bits=0)

            >>> a.cumprod(0)
            APyFixedArray([[ 1,  2,  3],
                           [ 4, 10, 18]], int_bits=20, frac_bits=0)

            >>> a.cumprod(1)
            APyFixedArray([[  1,   2,   6],
                           [  4,  20, 120]], int_bits=30, frac_bits=0)

            -------
            )pbdoc"
        )

        .def(
            "nanprod",
            &APyFixedArray::prod,
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
                :class:`APyFixedArray` or :class:`APyFixed`

                Raises
                ------
                :class:`IndexError`
                    If a specified axis is outside of the existing number of dimensions
                    for the array.
            )pbdoc"
        )

        .def(
            "nancumprod",
            &APyFixedArray::cumprod,
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
                :class:`APyFixedArray`

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
            "from_float",
            &APyFixedArray::from_numbers,
            nb::arg("number_seq"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFixedArray` from iterable sequence of numbers.

            The input is quantized using :class:`QuantizationMode.RND_INF` and overflow
            is handled using the :class:`OverflowMode.WRAP` mode. Exactly two of the
            three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

            Using NumPy arrays as input is in general faster than e.g. lists.

            Parameters
            ----------
            number_seq : :class:`~collections.abc.Iterable` of numbers.
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
            >>> a = apy.APyFixedArray.from_float(
            ...         [1.0, 1.25, 1.49], int_bits=2, frac_bits=2
            ... )
            >>> a
            APyFixedArray([4, 5, 6], int_bits=2, frac_bits=2)

            >>> b = apy.APyFixedArray.from_float(
            ...     [
            ...         [1.0, 2.0, 3.0],
            ...         [4.0, 5.0, 6.0],
            ...     ],
            ...     bits=5,
            ...     frac_bits=0
            ... )
            >>> b
            APyFixedArray([[1, 2, 3],
                           [4, 5, 6]], int_bits=5, frac_bits=0)


            Returns
            -------
            :class:`APyFixedArray`

            See Also
            --------
            fx
            )pbdoc"
        )
        .def_static(
            "from_array",
            &APyFixedArray::from_array,
            nb::arg("ndarray"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFixedArray` object from an ndarray.

            The input is quantized using :class:`QuantizationMode.RND_INF` and overflow
            is handled using the :class:`OverflowMode.WRAP` mode. Exactly two of the
            three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

            .. hint:: Using NumPy arrays as input is in general faster than e.g. lists.

            Parameters
            ----------
            ndarray : ndarray
                Values to initialize from. The tensor shape will be taken from the
                ndarray shape.
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
            >>> a = apy.APyFixedArray.from_array(
            ...     np.array([
            ...         [1.0, 2.0, 3.0],
            ...         [4.0, 5.0, 6.0],
            ...     ]),
            ...     int_bits=10,
            ...     frac_bits=0
            ... )
            >>> a
            APyFixedArray([[1, 2, 3],
                           [4, 5, 6]], int_bits=10, frac_bits=0)

            Returns
            -------
            :class:`APyFixedArray`

            See Also
            --------
            fx

            )pbdoc"
        )
        .def_static(
            "zeros",
            &APyFixedArray::zeros,
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
            :class:`APyFixedArray`
                An array initialized with zeros.
            )pbdoc"
        )
        .def_static(
            "ones",
            &APyFixedArray::ones,
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
            :class:`APyFixedArray`
                An array initialized with ones.
            )pbdoc"
        )
        .def_static(
            "eye",
            &APyFixedArray::eye,
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
            :class:`APyFixedArray`
                An array with ones on the diagonal.
            )pbdoc"
        )
        .def_static(
            "identity",
            &APyFixedArray::identity,
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
            :class:`APyFixedArray`
                An identity matrix with ones on the diagonal.
            )pbdoc"
        )
        .def_static(
            "full",
            &APyFixedArray::full,
            nb::arg("shape"),
            nb::arg("fill_value"),
            R"pbdoc(
            Initialize an array with the specified value.

            Parameters
            ----------
            shape : :class:`tuple`
                Shape of the array.
            fill_value : :class:`APyFixed`
                Value to fill the array.

            Returns
            -------
            :class:`APyFixedArray`
                An array filled with the specified value.
            )pbdoc"
        )
        .def_static(
            "_arange",
            &APyFixedArray::arange,
            nb::arg("start"),
            nb::arg("stop"),
            nb::arg("step"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Create an array with evenly spaced values within a given interval.

            The function can be called with varying number of positional arguments:

            * ``arange(stop)``: Values are generated within the half-open interval
              ``[0, stop)`` (in other words, the interval including ``start`` but
              excluding ``stop``).
            * ``arange(start, stop)``: Values are generated within the half-open
              interval ``[start, stop)``.
            * ``arange(start, stop, step)``: Values are generated within the half-open
              interval ``[start, stop)``, with spacing between values given by ``step``.

            Parameters
            ----------
            start : :class:`int`, :class:`float`, :class:`APyFloat`, :class:`APyFixed`
                Start number.
            stop : :class:`int`, :class:`float`, :class:`APyFloat`, :class:`APyFixed`
                Stop number.
            step : :class:`int`, :class:`float`, :class:`APyFloat`, :class:`APyFixed`
                Step size in range.
            int_bits : :class:`int`, optional
                Number of integer bits.
            frac_bits : :class:`int`, optional
                Number of fractional bits.
            bits : :class:`int`, optional
                Total number of bits.

            Returns
            -------
            :class:`APyFixedArray`
            )pbdoc"
        )
        .def_static(
            "_fullrange",
            &APyFixedArray::fullrange,
            nb::arg("start"),
            nb::arg("stop"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Create an array with all values within a given interval.

            The function can be called with varying number of positional arguments:

            * ``arange(stop)``: Values are generated within the half-open interval
              ``[0, stop)`` (in other words, the interval including ``start`` but
              excluding ``stop``).
            * ``arange(start, stop)``: Values are generated within the half-open
              interval ``[start, stop)``.

            Parameters
            ----------
            start : :class:`int`, :class:`float`, :class:`APyFloat`, :class:`APyFixed`
                Start number.
            stop : :class:`int`, :class:`float`, :class:`APyFloat`, :class:`APyFixed`
                Stop number.
            int_bits : :class:`int`, optional
                Number of integer bits.
            frac_bits : :class:`int`, optional
                Number of fractional bits.
            bits : :class:`int`, optional
                Total number of bits.

            Returns
            -------
            :class:`APyFixedArray`
            )pbdoc"
        )
        .def_static(
            "_meshgrid",
            &APyFixedArray::meshgrid,
            nb::arg("arrays"),
            nb::arg("indexing") = "xy",
            R"pbdoc(
            Create a list of coordinate arrays based on several coordinate vectors (1-D arrays).
            The input vectors must all have the same bit specifiers.

            .. versionadded:: 0.4

            Parameters
            ----------
            arrays : :class:`APyFixedArray`
                The coordinate vectors (1-D arrays).
            indexing : {"xy", "ij"}, default: "xy"
                The indexing mode to use for the meshgrid. "xy" is the standard Cartesian indexing,
                while "ij" is the matrix indexing.

            Returns
            -------
            result : list of :class:`APyFixedArray`
                List of 2-D arrays representing the meshgrid.

            Examples
            --------
            >>> import apytypes as apy
            >>> x = apy.fx(range(3), int_bits=5, frac_bits=0)
            >>> y = apy.fx(range(4, 7), int_bits=5, frac_bits=0)
            >>> xx, yy = apy.meshgrid(x, y)
            >>> xx
            APyFixedArray([[0, 1, 2],
                           [0, 1, 2],
                           [0, 1, 2]], int_bits=5, frac_bits=0)
            >>> yy
            APyFixedArray([[4, 4, 4],
                           [5, 5, 5],
                           [6, 6, 6]], int_bits=5, frac_bits=0)
            )pbdoc"
        )

        /*
         * Dunder methods
         */
        .def("__matmul__", &APyFixedArray::matmul, nb::arg("rhs"))
        .def("__repr__", &APyFixedArray::repr)
        .def("__abs__", &APyFixedArray::abs)
        .def("__str__", &APyFixedArray::to_string, nb::arg("base") = 10)

        // Iteration and friends
        .def("__getitem__", &APyFixedArray::get_item, nb::arg("key"))
        .def("__setitem__", &APyFixedArray::set_item, nb::arg("key"), nb::arg("val"))
        .def("__len__", &APyFixedArray::size)
        .def(
            "__iter__",
            [](nb::iterable array) {
                return APyFixedArrayIterator(
                    nb::cast<const APyFixedArray&>(array), array
                );
            }
        )
        .def(
            "__array__",
            &APyFixedArray::to_numpy,
            nb::arg("dtype") = nb::none(),
            nb::arg("copy") = nb::none()
        )

        ;

    nb::class_<APyFixedArrayIterator>(m, "APyFixedArrayIterator")
        .def(
            "__iter__",
            [](APyFixedArrayIterator& it) -> APyFixedArrayIterator& { return it; }
        )
        .def("__next__", &APyFixedArrayIterator::next);
}
