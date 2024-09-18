#include "apyfixed.h"
#include "apyfixedarray.h"
#include "apyfixedarray_iterator.h"

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/optional.h>

namespace nb = nanobind;

void bind_fixed_array(nb::module_& m)
{
    nb::class_<APyFixedArray>(m, "APyFixedArray")

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
         * Arithmetic operations
         */
        .def(nb::self + nb::self)
        .def(
            "__add__",
            [](const APyFixedArray& a, const nb::int_& b) {
                return a + APyFixed::from_integer(b, a.int_bits(), a.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFixedArray& a, const nb::int_& b) {
                return a + APyFixed::from_integer(b, a.int_bits(), a.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__add__",
            [](const APyFixedArray& lhs, double rhs) {
                return lhs
                    + APyFixed::from_double(rhs, lhs.int_bits(), lhs.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFixedArray& rhs, double lhs) {
                return rhs
                    + APyFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__add__",
            [](const APyFixedArray& a, const APyFixed& b) { return a + b; },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFixedArray& a, const APyFixed& b) { return a + b; },
            nb::is_operator()
        )
        .def(nb::self - nb::self)
        .def(
            "__sub__",
            [](const APyFixedArray& a, const nb::int_& b) {
                return a - APyFixed::from_integer(b, a.int_bits(), a.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFixedArray& a, const nb::int_& b) {
                return a.rsub(APyFixed::from_integer(b, a.int_bits(), a.frac_bits()));
            },
            nb::is_operator()
        )
        .def(
            "__sub__",
            [](const APyFixedArray& lhs, double rhs) {
                return lhs
                    - APyFixed::from_double(rhs, lhs.int_bits(), lhs.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFixedArray& rhs, double lhs) {
                return rhs.rsub(
                    APyFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits())
                );
            },
            nb::is_operator()
        )
        .def(
            "__sub__",
            [](const APyFixedArray& a, const APyFixed& b) { return a - b; },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFixedArray& a, const APyFixed& b) { return a.rsub(b); },
            nb::is_operator()
        )
        .def(nb::self * nb::self)
        .def(
            "__mul__",
            [](const APyFixedArray& a, const APyFixed& b) { return a * b; },
            nb::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFixedArray& a, const APyFixed& b) { return a * b; },
            nb::is_operator()
        )
        .def(
            "__mul__",
            [](const APyFixedArray& a, const nb::int_& b) {
                return a * APyFixed::from_integer(b, a.int_bits(), a.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFixedArray& a, const nb::int_& b) {
                return a * APyFixed::from_integer(b, a.int_bits(), a.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__mul__",
            [](const APyFixedArray& lhs, double rhs) {
                return lhs
                    * APyFixed::from_double(rhs, lhs.int_bits(), lhs.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFixedArray& rhs, double lhs) {
                return rhs
                    * APyFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits());
            },
            nb::is_operator()
        )
        .def(nb::self / nb::self)
        .def(
            "__truediv__",
            [](const APyFixedArray& a, const APyFixed& b) { return a / b; },
            nb::is_operator()
        )
        .def(
            "__rtruediv__",
            [](const APyFixedArray& a, const APyFixed& b) { return a.rdiv(b); },
            nb::is_operator()

        )
        .def(
            "__truediv__",
            [](const APyFixedArray& a, const nb::int_& b) {
                return a / APyFixed::from_integer(b, a.int_bits(), a.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__rtruediv__",
            [](const APyFixedArray& a, const nb::int_& b) {
                return a.rdiv(APyFixed::from_integer(b, a.int_bits(), a.frac_bits()));
            },
            nb::is_operator()
        )
        .def(
            "__truediv__",
            [](const APyFixedArray& lhs, double rhs) {
                return lhs
                    / APyFixed::from_double(rhs, lhs.int_bits(), lhs.frac_bits());
            },
            nb::is_operator()
        )
        .def(
            "__rtruediv__",
            [](const APyFixedArray& rhs, double lhs) {
                return rhs.rdiv(
                    APyFixed::from_double(lhs, rhs.int_bits(), rhs.frac_bits())
                );
            },
            nb::is_operator()
        )
        .def(-nb::self)
        .def(nb::self <<= int(), nb::rv_policy::none)
        .def(nb::self >>= int(), nb::rv_policy::none)

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

        .def("to_numpy", &APyFixedArray::to_numpy, R"pbdoc(
            Return array as a :class:`numpy.ndarray` of :class:`numpy.float64`.

            The returned array has the same `shape` and values as `self`. This
            method rounds away from infinity on ties.

            Returns
            -------
            :class:`numpy.ndarray`
            )pbdoc")

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

        .def("reshape", &APyFixedArray::reshape, nb::arg("number_sequence"), R"pbdoc(
        Reshape the APyFixedArray to the specified shape without changing its data.

        Parameters
        ----------
        new_shape : `tuple`
            The new shape should be compatible with the original shape. If a dimension is -1, its value will be inferred from the length of the array and remaining dimensions. Only one dimension can be -1.

        Raises
        ------
        ValueError
            If negative dimensions less than -1 are provided, if the total size of the new array is not unchanged and divisible by the known dimensions, or if the total number of elements does not match the original array.

        Examples
        --------
        >>> from apytypes import APyFixedArray
        >>> a = APyFixedArray([2, 3, 4, 5], int_bits=2, frac_bits=1)
        >>> a.to_numpy()
        array([ 1. ,  1.5, -2. , -1.5])

        >>> a.reshape((2, 2)).to_numpy()
        array([[ 1. ,  1.5],
               [-2. , -1.5]])

        >>> a.reshape((4,)).to_numpy()
        array([ 1. ,  1.5, -2. , -1.5])

        >>> a.reshape((2, -1)).to_numpy()
        array([[ 1. ,  1.5],
               [-2. , -1.5]])

        Returns
        -------
        :class:`APyFixedArray`
             )pbdoc")

        .def("flatten", &APyFixedArray::flatten, R"pbdoc(
        Return a copy of the array collapsed into one dimension.


        Examples
        --------
        >>> from apytypes import APyFixedArray
        >>> arr = APyFixedArray([[2, 3], [4, 5]], int_bits=2, frac_bits=1)
        >>> arr.to_numpy()
        array([[ 1. ,  1.5],
               [-2. , -1.5]])

        >>> arr.flatten().to_numpy()
        array([ 1. ,  1.5, -2. , -1.5])

        Returns
        -------
        :class:`APyFixedArray`
             )pbdoc")

        .def("ravel", &APyFixedArray::ravel, R"pbdoc(
        Return a copy of the array collapsed into one dimension. Same as flatten with
        current memory-copy model.


        Examples
        --------
        >>> from apytypes import APyFixedArray
        >>> arr = APyFixedArray([[2, 3], [4, 5]], int_bits=2, frac_bits=1)
        >>> arr.to_numpy()
        array([[ 1. ,  1.5],
               [-2. , -1.5]])

        >>> arr.ravel().to_numpy()
        array([ 1. ,  1.5, -2. , -1.5])

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
          axis1 : int
              First axis.
          axis2 : int
              Second axis.

          Examples
          --------
          >>> from apytypes import APyFixedArray
          >>> x = APyFixedArray.from_float([[1, 2, 3]], bits=5, frac_bits=0)
          >>> x.swapaxes(0,1).to_numpy()
          array([[1.],
                 [2.],
                 [3.]])

          >>> x = APyFixedArray.from_float([[[0, 1], [2, 3]], [[4, 5], [6, 7]]], bits=5, frac_bits=0)
          >>> x.to_numpy()
          array([[[0., 1.],
                  [2., 3.]],
          <BLANKLINE>
                 [[4., 5.],
                  [6., 7.]]])

          >>> x.swapaxes(0,2).to_numpy()
          array([[[0., 4.],
                  [2., 6.]],
          <BLANKLINE>
                 [[1., 5.],
                  [3., 7.]]])

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
            axes : tuple of ints, optional
                If specified, it must be a tuple or list which contains a permutation
                of [0,1,...,N-1] where N is the number of axes of `a`. The `i`'th axis
                of the returned array will correspond to the axis numbered ``axes[i]``
                of the input. If not specified, defaults to ``range(a.ndim)[::-1]``,
                which reverses the order of the axes.

            Examples
            --------
            >>> from apytypes import APyFixedArray
            >>> a = APyFixedArray.from_float([[1.0, 2.0, 3.0], [-4.0, -5.0, -6.0]], bits=5, frac_bits=0)
            >>> a.to_numpy()
            array([[ 1.,  2.,  3.],
                   [-4., -5., -6.]])
            >>> a = a.transpose()
            >>> a.to_numpy()
            array([[ 1., -4.],
                   [ 2., -5.],
                   [ 3., -6.]])

            >>> a = APyFixedArray.from_float([1.0] * 6, bits=5, frac_bits=0).reshape((1, 2, 3))
            >>> a.transpose((1, 0, 2)).shape
            (2, 1, 3)

            >>> a.transpose((-2, -3, -1)).shape
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

            This is the primary method for performing quantization and overflowing/saturation
            when dealing with APyTypes fixed-point arrays.

            Exactly two of three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must
            be set.

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
            shape : tuple or int
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
            Removes axes of size one at the specified axis/axes, if no axis is given removes all dimensions with size one.

            Parameters
            ----------
            axis : tuple or int, optional
                The axes to squeeze, a given axis with a size other than one will result in an error. No given axes  will be remove all dimensions of size one.

            Returns
            -------
            :class:`APyFixedArray`

            Raises
            ------
            :class:`ValueError`
                If given an axis of a size other than one a ValueError will be thrown.
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            )pbdoc"
        )

        .def(
            "sum",
            &APyFixedArray::sum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the sum of the elements along specified axis/axes treating Not a Number as 0.

            Parameters
            ----------
            axis : tuple or int, optional
                The axis/axes to summate across. Will summate the whole array if no int or tuple is specified.

            Returns
            -------
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            --------

            >>> from apytypes import APyFixedArray

            >>> a = APyFixedArray(
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
            axis : int, optional
                The axes to summate across. If not given an axis it will return the cumulative sum of the flattened array.

            Returns
            -------
            :class:`APyFixedArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            --------

            >>> from apytypes import APyFixedArray

            Array `a`, array to summate across.

            >>> a = APyFixedArray(
            ...     [[1,2,3],[4,5,6]],
            ...     int_bits=10,
            ...     frac_bits=0
            ... )

            >>> a.cumsum()
            APyFixedArray([1, 3, 6, 10, 15, 21], shape=(6,), bits=13, int_bits=13)

            >>> a.cumsum(0)
            APyFixedArray([1, 2, 3, 5, 7, 9], shape=(2, 3), bits=11, int_bits=11)

            >>> a.cumsum(1)
            APyFixedArray([1, 3, 6, 4, 9, 15], shape=(2, 3), bits=12, int_bits=12)

            -------
            )pbdoc"
        )

        .def(
            "nansum",
            &APyFixedArray::nansum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the sum of the elements along specified axis/axes treating Not a Number as 0.

            Parameters
            ----------
            axis : tuple or int, optional
                The axis/axes to summate across. Will summate the whole array if no int or tuple is specified.

            Returns
            -------
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            )pbdoc"
        )

        .def(
            "nancumsum",
            &APyFixedArray::nancumsum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the cumulative sum of the elements along a given axis treating Not a Number as 0.

            Parameters
            ----------
            axis : int, optional
                The axis to summate across. If not given an axis it will return the cumulative sum of the flattened array.

            Returns
            -------
            :class:`APyFixedArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            )pbdoc"
        )

        .def(
            "max",
            &APyFixedArray::max,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Returns the maximum value from an array or the maximum along an axis

            Parameters
            ----------
            axis : tuple or int, optional
                The axis to get the maximum along.

            Returns
            -------
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            -------

            >>> from apytypes import APyFixedArray

            Array `a`, array to get the maximum along.

            >>> a = APyFixedArray(
            ...     [[1,2,3],[4,5,6]],
            ...     int_bits=10,
            ...     frac_bits=0
            ... )

            >>> a.max()
            APyFixed(6, bits=10, int_bits=10)

            >>> a.max(0)
            APyFixedArray([4, 5, 6], shape=(3,), bits=10, int_bits=10)

            >>> a.max(1)
            APyFixedArray([3, 6], shape=(2,), bits=10, int_bits=10)

            -------

            )pbdoc"
        )

        .def(
            "min",
            &APyFixedArray::min,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Returns the minimum value from an array or the minimum along an axis

            Parameters
            ----------
            axis : tuple or int, optional
                The axis to get the minimum along.

            Returns
            -------
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            -------

            >>> from apytypes import APyFixedArray

            Array `a`, array to get the minimum along.

            >>> a = APyFixedArray(
            ...     [[1,2,3],[4,5,6]],
            ...     int_bits=10,
            ...     frac_bits=0
            ... )

            >>> a.min()
            APyFixed(1, bits=10, int_bits=10)

            >>> a.min(0)
            APyFixedArray([1, 2, 3], shape=(3,), bits=10, int_bits=10)

            >>> a.min(1)
            APyFixedArray([1, 4], shape=(2,), bits=10, int_bits=10)

            -------
            )pbdoc"
        )

        .def(
            "nanmax",
            &APyFixedArray::nanmax,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Returns the maximum value from an array or the maximum along an axis. Ignoring nan.
            Issues a warning when encountering an all-nan slice or axis.

            Parameters
            ----------
            axis : tuple or int, optional
                The axis to get the maximum along.

            Returns
            -------
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.
            )pbdoc"
        )

        .def(
            "nanmin",
            &APyFixedArray::nanmin,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Returns the minimum value from an array or the minimum along an axis. Ignoring nan.
            Issues a warning when encountering an all-nan slice or axis.

            Parameters
            ----------
            axis : tuple or int, optional
                The axis to get the minimum along.

            Returns
            -------
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.
            )pbdoc"
        )

        .def(
            "prod",
            &APyFixedArray::prod,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Returns the product of the elements along specified axis/axes.

            Parameters
            ----------
            axis : tuple, int, optional
                The axis/axes to calculate the product across. If not given an axis it will return the product of the flattened array.

            Returns
            -------
            :class:`APyFixedArray` or :class:`APyFixed`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            --------

            >>> from apytypes import APyFixedArray

            >>> a = APyFixedArray(
            ...     [1,2,3,4,5,6],
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
            axis : int, optional
                The axes to calculate the product across. If not given an axis it will return the cumulative product of the flattened array.

            Returns
            -------
            :class:`APyFixedArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            --------

            >>> from apytypes import APyFixedArray


            >>> a = APyFixedArray(
            ...     [[1,2,3],[4,5,6]],
            ...     int_bits=10,
            ...     frac_bits=0
            ... )

            >>> a.cumprod()
            APyFixedArray([1, 2, 6, 24, 120, 720], shape=(6,), bits=60, int_bits=60)

            >>> a.cumprod(0)
            APyFixedArray([1, 2, 3, 4, 10, 18], shape=(2, 3), bits=20, int_bits=20)

            >>> a.cumprod(1)
            APyFixedArray([1, 2, 6, 4, 20, 120], shape=(2, 3), bits=30, int_bits=30)

            -------
            )pbdoc"
        )

        .def(
            "nanprod",
            &APyFixedArray::nanprod,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
                Return the product of the elements along a given axis treating Not a Number as 0.

                Parameters
                ----------
                axis : int, optional
                    The axis to calculate the product across. If not given an axis it will return the product of the flattened array.

                Returns
                -------
                :class:`APyFixedArray` or :class:`APyFixed`

                Raises
                ------
                :class:`IndexError`
                    If a specified axis is outside of the existing number of dimensions for the array.
            )pbdoc"
        )

        .def(
            "nancumprod",
            &APyFixedArray::nancumprod,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
                Return the cumulative product of the elements along a given axis treating Not a Number as 0.

                Parameters
                ----------
                axis : int, optional
                    The axis to calculate the product across. If not given an axis it will return the cumulative product of the flattened array.

                Returns
                -------
                :class:`APyFixedArray`

                Raises
                ------
                :class:`IndexError`
                    If a specified axis is outside of the existing number of dimensions for the array.

            )pbdoc"
        )

        /*
         * Static methods
         */
        .def_static(
            "from_float",
            &APyFixedArray::from_double,
            nb::arg("float_sequence"),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("bits") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFixedArray` object from a sequence of :class:`float`.

            The initialized fixed-point values are the one closest to the
            input floating-point values, rounded away from zero on ties. Exactly two of
            the three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

            Using NumPy arrays as input is in general faster than e.g. lists.

            Parameters
            ----------
            float_sequence : sequence of float
                Floating point values to initialize from. The tensor shape will be taken
                from the sequence shape.
            int_bits : int, optional
                Number of integer bits in the created fixed-point tensor.
            frac_bits : int, optional
                Number of fractional bits in the created fixed-point tensor.
            bits : int, optional
                Total number of bits in the created fixed-point tensor.

            Examples
            --------

            >>> from apytypes import APyFixedArray

            Array `a`, initialized from floating-point values.

            >>> a = APyFixedArray.from_float([1.0, 1.25, 1.49], int_bits=2, frac_bits=2)

            Array `b` (2 x 3 matrix), initialized from floating-point values.

            >>> b = APyFixedArray.from_float(
            ...     [
            ...         [1.0, 2.0, 3.0],
            ...         [4.0, 5.0, 6.0],
            ...     ],
            ...     bits=5,
            ...     frac_bits=0
            ... )

            Returns
            -------
            :class:`APyFixedArray`
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

            The initialized fixed-point values are the one closest to the
            input floating-point value, rounded away from zero on ties. Exactly two of
            the three bit-specifiers (`bits`, `int_bits`, `frac_bits`) must be set.

            Using NumPy arrays as input is in general faster than e.g. lists.

            Parameters
            ----------
            ndarray : ndarray
                Values to initialize from. The tensor shape will be taken from the ndarray shape.
            int_bits : int, optional
                Number of integer bits in the created fixed-point tensor.
            frac_bits : int, optional
                Number of fractional bits in the created fixed-point tensor.
            bits : int, optional
                Total number of bits in the created fixed-point tensor.

            Examples
            --------

            >>> from apytypes import APyFixedArray
            >>> import numpy as np

            Array `a`, initialized from NumPy ndarray

            >>> a = APyFixedArray.from_array(
            ...     np.array([
            ...         [1.0, 2.0, 3.0],
            ...         [4.0, 5.0, 6.0],
            ...     ]),
            ...     int_bits=10,
            ...     frac_bits=0
            ... )

            Returns
            -------
            :class:`APyFixedArray`
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
            Initializes an array with zeros.

            Parameters
            ----------
            shape : tuple
                Shape of the array.
            int_bits : int, optional
                Number of integer bits.
            frac_bits : int, optional
                Number of fractional bits.
            bits : int, optional
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
            Initializes an array with ones.

            Parameters
            ----------
            shape : tuple
                Shape of the array.
            int_bits : int, optional
                Number of integer bits.
            frac_bits : int, optional
                Number of fractional bits.
            bits : int, optional
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
            Initializes an array with ones on the diagonal.

            Parameters
            ----------
            n : int
                Number of rows.
            m : int, optional
                Number of columns.
            int_bits : int, optional
                Number of integer bits.
            frac_bits : int, optional
                Number of fractional bits.
            bits : int, optional
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
            Initializes an identity matrix with ones on the diagonal.

            Parameters
            ----------
            n : int
                Number of rows (and columns) in *n* x *n* output.
            int_bits : int, optional
                Number of integer bits.
            frac_bits : int, optional
                Number of fractional bits.
            bits : int, optional
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
            Initializes an array with the specified value.

            Parameters
            ----------
            shape : tuple
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

            * ``arange(stop)``: Values are generated within the half-open interval ``[0, stop)`` (in other words, the interval including ``start`` but excluding ``stop``).
            * ``arange(start, stop)``: Values are generated within the half-open interval ``[start, stop)``.
            * ``arange(start, stop, step)``: Values are generated within the half-open interval ``[start, stop)``, with spacing between values given by ``step``.

            Parameters
            ----------
            start : int, float, :class:`APyFloat`, :class:`APyFixed`
                Start number.
            stop : int, float, :class:`APyFloat`, :class:`APyFixed`
                Stop number.
            step : int, float, :class:`APyFloat`, :class:`APyFixed`
                Step size in range.
            int_bits : int, optional
                Number of integer bits.
            frac_bits : int, optional
                Number of fractional bits.
            bits : int, optional
                Total number of bits.

            Returns
            -------
            :class:`APyFixedArray`
            )pbdoc"
        )
        /*
         * Dunder methods
         */
        .def("__lshift__", &APyFixedArray::operator<<, nb::arg("shift_amnt"))
        .def("__matmul__", &APyFixedArray::matmul, nb::arg("rhs"))
        .def("__repr__", &APyFixedArray::repr)
        .def("__rshift__", &APyFixedArray::operator>>, nb::arg("shift_amnt"))
        .def("__abs__", &APyFixedArray::abs)

        // Iteration and friends
        .def("__getitem__", &APyFixedArray::get_item, nb::arg("idx"))
        .def("__len__", &APyFixedArray::size)
        .def(
            "__iter__",
            [](nb::iterable array) {
                return APyFixedArrayIterator(
                    nb::cast<const APyFixedArray&>(array), array
                );
            }
        )
        .def("__array__", &APyFixedArray::to_numpy)

        ;

    nb::class_<APyFixedArrayIterator>(m, "APyFixedArrayIterator")
        .def(
            "__iter__",
            [](APyFixedArrayIterator& it) -> APyFixedArrayIterator& { return it; }
        )
        .def("__next__", &APyFixedArrayIterator::next);
}
