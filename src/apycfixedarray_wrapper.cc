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
        return (rhs.*FUNC)(APyCFixed::from_complex(lhs, rhs.int_bits(), rhs.frac_bits())
        );
    } else if constexpr (std::is_floating_point_v<L_TYPE>) {
        return (rhs.*FUNC)(APyCFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits())
        );
    } else if constexpr (std::is_same_v<std::remove_cv_t<L_TYPE>, APyCFixed>) {
        return (rhs.*FUNC)(lhs);
    } else if constexpr (std::is_same_v<std::remove_cv_t<L_TYPE>, APyFixed>) {
        return (rhs.*FUNC)(
            APyCFixed::from_apyfixed(lhs, lhs.int_bits(), lhs.frac_bits())
        );
    } else {
        return (rhs.*FUNC)(APyCFixed::from_integer(lhs, rhs.int_bits(), rhs.frac_bits())
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

using std::complex;
using is_op = nb::is_operator;

void bind_cfixed_array(nb::module_& m)
{
    nb::class_<APyCFixedArray>(m, "APyCFixedArray")

        /*
         * Constructor: construct from a Python sequence of integers
         */
        .def(
            nb::init<
                const nb::sequence&,
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
        .def(nb::self + nb::self)
        .def(nb::self - nb::self)
        .def(nb::self * nb::self)
        .def(nb::self / nb::self)
        .def(-nb::self)
        .def(+nb::self)
        .def(nb::self <<= int(), nb::rv_policy::none)
        .def(nb::self >>= int(), nb::rv_policy::none)
        .def(nb::self << int())
        .def(nb::self >> int())

        /*
         * Arithmetic operations with `APyCFixed`
         */
        .def("__add__", L_OP<std::plus<>, APyCFixed>, is_op())
        .def("__radd__", L_OP<std::plus<>, APyCFixed>, is_op())
        .def("__sub__", L_OP<std::minus<>, APyCFixed>, is_op())
        .def("__rsub__", R_OP<&APyCFixedArray::rsub, APyCFixed>, is_op())
        .def("__rmul__", L_OP<std::multiplies<>, APyCFixed>, is_op())
        .def("__mul__", L_OP<std::multiplies<>, APyCFixed>, is_op())
        .def("__truediv__", L_OP<std::divides<>, APyCFixed>, is_op())
        .def("__rtruediv__", R_OP<&APyCFixedArray::rdiv, APyCFixed>, is_op())

        /*
         * Arithmetic operation with complex
         */
        .def("__add__", L_OP<std::plus<>, complex<double>>, is_op())
        .def("__radd__", L_OP<std::plus<>, complex<double>>, is_op())
        .def("__sub__", L_OP<std::minus<>, complex<double>>, is_op())
        .def("__rsub__", R_OP<&APyCFixedArray::rsub, complex<double>>, is_op())
        .def("__mul__", L_OP<std::multiplies<>, complex<double>>, is_op())
        .def("__rmul__", L_OP<std::multiplies<>, complex<double>>, is_op())
        .def("__truediv__", L_OP<std::divides<>, complex<double>>, is_op())
        .def("__rtruediv__", R_OP<&APyCFixedArray::rdiv, complex<double>>, is_op())

        /*
         * Arithmetic operation with floats
         */
        .def("__add__", L_OP<std::plus<>, double>, is_op())
        .def("__radd__", L_OP<std::plus<>, double>, is_op())
        .def("__sub__", L_OP<std::minus<>, double>, is_op())
        .def("__rsub__", R_OP<&APyCFixedArray::rsub, double>, is_op())
        .def("__mul__", L_OP<std::multiplies<>, double>, is_op())
        .def("__rmul__", L_OP<std::multiplies<>, double>, is_op())
        .def("__truediv__", L_OP<std::divides<>, double>, is_op())
        .def("__rtruediv__", R_OP<&APyCFixedArray::rdiv, double>, is_op())

        /*
         * Arithmetic operations with integers
         */
        .def("__add__", L_OP<std::plus<>, nb::int_>, is_op())
        .def("__radd__", L_OP<std::plus<>, nb::int_>, is_op())
        .def("__sub__", L_OP<std::minus<>, nb::int_>, is_op())
        .def("__rsub__", R_OP<&APyCFixedArray::rsub, nb::int_>, is_op())
        .def("__mul__", L_OP<std::multiplies<>, nb::int_>, is_op())
        .def("__rmul__", L_OP<std::multiplies<>, nb::int_>, is_op())
        .def("__truediv__", L_OP<std::divides<>, nb::int_>, is_op())
        .def("__rtruediv__", R_OP<&APyCFixedArray::rdiv, nb::int_>, is_op())

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

        .def("to_numpy", &APyCFixedArray::to_numpy, R"pbdoc(
            Return array as a :class:`numpy.ndarray` of :class:`numpy.complex128`.

            The returned array has the same `shape` and values as `self`. This
            method rounds away from infinity on ties.

            Returns
            -------
            :class:`numpy.ndarray`
            )pbdoc")

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

        .def("reshape", &APyCFixedArray::reshape, nb::arg("number_sequence"), R"pbdoc(
            Reshape the APyCFixedArray to the specified shape without changing its data.

            Parameters
            ----------
            new_shape : `tuple`
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
            >>> a = apy.APyCFixedArray([2, 3, 4, 5], int_bits=2, frac_bits=1)
            >>> a.to_numpy()
            array([ 1. +0.j,  1.5+0.j, -2. +0.j, -1.5+0.j])

            >>> a.reshape((2, 2)).to_numpy()
            array([[ 1. +0.j,  1.5+0.j],
                   [-2. +0.j, -1.5+0.j]])

            >>> a.reshape((4,)).to_numpy()
            array([ 1. +0.j,  1.5+0.j, -2. +0.j, -1.5+0.j])

            >>> a.reshape((2, -1)).to_numpy()
            array([[ 1. +0.j,  1.5+0.j],
                   [-2. +0.j, -1.5+0.j]])

            Returns
            -------
            :class:`APyCFixedArray`
        )pbdoc")

        .def("flatten", &APyCFixedArray::flatten, R"pbdoc(
            Return a copy of the array collapsed into one dimension.

            Examples
            --------
            >>> import apytypes as apy
            >>> arr = apy.APyCFixedArray([[2, 3], [4, 5]], int_bits=2, frac_bits=1)
            >>> arr.to_numpy()
            array([[ 1. +0.j,  1.5+0.j],
                   [-2. +0.j, -1.5+0.j]])

            >>> arr.flatten().to_numpy()
            array([ 1. +0.j,  1.5+0.j, -2. +0.j, -1.5+0.j])

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
            >>> arr = apy.APyCFixedArray([[2, 3], [4, 5]], int_bits=2, frac_bits=1)
            >>> arr.to_numpy()
            array([[ 1. +0.j,  1.5+0.j],
                   [-2. +0.j, -1.5+0.j]])

            >>> arr.ravel().to_numpy()
            array([ 1. +0.j,  1.5+0.j, -2. +0.j, -1.5+0.j])

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
            >>> x = apy.APyCFixedArray.from_float([[1, 2, 3]], bits=5, frac_bits=0)
            >>> x.swapaxes(0,1).to_numpy()
            array([[1.+0.j],
                   [2.+0.j],
                   [3.+0.j]])

            >>> x = apy.APyCFixedArray.from_float(
            ...         [[[0, 1], [2, 3]], [[4, 5], [6, 7]]],
            ...         bits=5,
            ...         frac_bits=0
            ...     )
            >>> x.to_numpy()
            array([[[0.+0.j, 1.+0.j],
                    [2.+0.j, 3.+0.j]],
            <BLANKLINE>
                   [[4.+0.j, 5.+0.j],
                    [6.+0.j, 7.+0.j]]])

            >>> x.swapaxes(0,2).to_numpy()
            array([[[0.+0.j, 4.+0.j],
                    [2.+0.j, 6.+0.j]],
            <BLANKLINE>
                   [[1.+0.j, 5.+0.j],
                    [3.+0.j, 7.+0.j]]])

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
            >>> a.transpose().to_numpy()
            array([[ 1.+0.j, -4.+0.j],
                   [ 2.+0.j, -5.+0.j],
                   [ 3.+0.j, -6.+0.j]])

            >>> a = apy.APyCFixedArray.from_float(
            ...         [1.0] * 6,
            ...         bits=5,
            ...         frac_bits=0
            ...     ).reshape((1, 2, 3))
            >>> a.transpose((1, 0, 2)).shape
            (2, 1, 3)

            >>> a.transpose((-2, -3, -1)).shape
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

        //     .def(
        //         "convolve",
        //         &APyCFixedArray::convolve,
        //         nb::arg("other"),
        //         nb::arg("mode") = "full",
        //         R"pbdoc(
        //         Return the discrete linear convolution with another one-dimensional
        //         array.

        //         Requires that ``ndim = 1`` for both `self` and `other`.

        //         Parameters
        //         ----------
        //         other : :class:`APyCFixedArray`
        //             The one-dimensional array of length :code:`N` to convolve with.

        //         mode : {'full', 'same', 'valid'}, default: 'full'
        //             'full':
        //                 Return the full convolution for each point of overlap. The
        //                 resulting single-dimensional shape will have length :code:`N
        //                 + M - 1`. Boundary effects occurs for points where the `a`
        //                 and `v` do not overlap completely.
        //             'same':
        //                 Return a convolution of length :code:`max(M, N)`. Boundary
        //                 effects still occur around the edges of the result.
        //             'valid':
        //                 Return the convolution for each point of full overlap. The
        //                 resulting single-dimensional shape will have length
        //                 :code:`max(M, N)
        //                 - min(M, N) + 1`

        //         Returns
        //         -------
        //         convolved : :class:`APyCFixedArray`
        //             The convolved array.

        //         )pbdoc"
        //     )

        .def(
            "squeeze",
            &APyCFixedArray::squeeze,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Removes axes of size one at the specified axis/axes, if no axis is
            given removes all dimensions with size one.

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
            >>> a.cumsum().to_numpy()
            array([ 1.+1.j,  3.+4.j,  6.+2.j, 10.+1.j, 15.+3.j, 21.+3.j])

            >>> a.cumsum(0).to_numpy()
            array([[1.+1.j, 2.+3.j, 3.-2.j],
                   [5.+0.j, 7.+5.j, 9.-2.j]])

            >>> a.cumsum(1).to_numpy()
            array([[ 1.+1.j,  3.+4.j,  6.+2.j],
                   [ 4.-1.j,  9.+1.j, 15.+1.j]])

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
            Return the cumulative sum of the elements along a given axis treating NaN as
            0.

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
            Returns the product of the elements along specified axis/axes.

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
            >>>
            >>> a = apy.APyCFixedArray.from_complex(
            ...     [1, 2 + 1j, 3 + 3j, -4 + 1j, 5 - 2j, 6 - 3j],
            ...     int_bits=10,
            ...     frac_bits=0
            ... )
            >>> complex(a.prod())
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
            >>>
            >>> a = apy.APyCFixedArray.from_complex(
            ...     [[1+1j, 2+1j, 3+2j],
            ...      [4-2j, 5+2j, 6+1j]],
            ...     int_bits=10,
            ...     frac_bits=0
            ... )
            >>> a.cumprod().to_numpy()
            array([   1.+1.00e+00j,    1.+3.00e+00j,   -3.+1.10e+01j,   10.+5.00e+01j,
                    -50.+2.70e+02j, -570.+1.57e+03j])

            >>> a.cumprod(0).to_numpy()
            array([[ 1. +1.j,  2. +1.j,  3. +2.j],
                   [ 6. +2.j,  8. +9.j, 16.+15.j]])

            >>> a.cumprod(1).to_numpy()
            array([[  1. +1.j,   1. +3.j,  -3.+11.j],
                   [  4. -2.j,  24. -2.j, 146.+12.j]])

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
            &APyCFixedArray::from_complex,
            nb::arg("complex_sequence"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Create an :class:`APyCFixedArray` object from a sequence of :class:`int`, :class:`float`, :class:`complex`, :class:`APyFixed`, :class:`APyFloat`, or :class:`APyCFixed`.

            The input is quantized using :class:`QuantizationMode.RND_INF` and overflow is handled using the :class:`OverflowMode.WRAP` mode.
            Exactly two of the three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

            Using NumPy arrays as input is in general faster than e.g. lists.

            Parameters
            ----------
            complex_sequence : sequence of float, int, or complex
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
            >>> b = apy.APyCFixedArray.from_complex(
            ...         [
            ...             [1.0 + 1.0j, 2.0 - 3.0j, 3.0 - 1.0j],
            ...             [4.0 - 2.0j, 5.0 + 2.0j, 6.0 + 1j],
            ...         ],
            ...         bits=5,
            ...         frac_bits=0
            ...     )

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
            Create an :class:`APyCFixedArray` object from a sequence of :class:`int`, :class:`float`, :class:`complex`, :class:`APyFixed`, :class:`APyFloat`, or :class:`APyCFixed`.
            This is an alias for :func:`~apytypes.APyCFixedArray.from_complex`, look there for more documentation.

            Parameters
            ----------
            number_seq : sequence of numbers
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
            >>>
            >>> a = apy.APyCFixedArray.from_array(
            ...     np.array([
            ...         [1.0, 2.0, 3.0],
            ...         [4.0, 5.0, 6.0],
            ...     ]),
            ...     int_bits=10,
            ...     frac_bits=0
            ... )

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
            Initializes an array with zeros.

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
            Initializes an array with ones.

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
            Initializes an array with ones on the diagonal.

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
            Initializes an identity matrix with ones on the diagonal.

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
            Initializes an array with the specified value.

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
        /*
         * Dunder methods
         */
        //     .def("__matmul__", &APyCFixedArray::matmul, nb::arg("rhs"))
        .def("__repr__", &APyCFixedArray::repr)

        // Iteration and friends
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
        .def("__array__", &APyCFixedArray::to_numpy)

        ;

    // nb::class_<APyCFixedArrayIterator>(m, "APyCFixedArrayIterator")
    //     .def(
    //         "__iter__",
    //         [](APyCFixedArrayIterator& it) -> APyCFixedArrayIterator& { return it; }
    //     )
    //     .def("__next__", &APyCFixedArrayIterator::next);
}
