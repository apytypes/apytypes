#include "apyfloatarray.h"
#include "apyfloatarray_iterator.h"

#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/optional.h>

namespace nb = nanobind;

void bind_float_array(nb::module_& m)
{
    nb::class_<APyFloatArray>(m, "APyFloatArray")

        /*
         * Constructor: construct from a list of Python integers
         */
        .def(
            "__init__",
            &APyFloatArray::create_in_place,
            nb::arg("signs"),
            nb::arg("exps"),
            nb::arg("mans"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloat` object.

            Parameters
            ----------
            signs : sequence of bools or ints
                The sign of the float. False/0 means positive. True/non-zero means negative.
            exps : sequence of ints
                Exponents of the floats as stored, i.e., actual value + bias.
            mans : sequence of ints
                Mantissas of the floats as stored, i.e., without a hidden one.
            exp_bits : int
                Number of exponent bits.
            man_bits : int
                Number of mantissa bits.
            bias : int, optional
                Bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Returns
            -------
            :class:`APyFloatArray`
            )pbdoc"
        )

        /*
         * Arithmetic operations
         */
        .def(nb::self + nb::self)
        .def(
            "__add__",
            [](const APyFloatArray& lhs, const nb::int_& rhs) {
                return lhs
                    + APyFloat::from_integer(
                           rhs, lhs.get_exp_bits(), lhs.get_man_bits(), lhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFloatArray& rhs, const nb::int_& lhs) {
                return rhs
                    + APyFloat::from_integer(
                           lhs, rhs.get_exp_bits(), rhs.get_man_bits(), rhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__add__",
            [](const APyFloatArray& lhs, double rhs) {
                return lhs
                    + APyFloat::from_double(
                           rhs, lhs.get_exp_bits(), lhs.get_man_bits(), lhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFloatArray& rhs, double lhs) {
                return rhs
                    + APyFloat::from_double(
                           lhs, rhs.get_exp_bits(), rhs.get_man_bits(), rhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__add__",
            [](const APyFloatArray& lhs, const APyFloat& rhs) { return lhs + rhs; },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFloatArray& rhs, const APyFloat& lhs) { return rhs + lhs; },
            nb::is_operator()
        )
        .def(nb::self - nb::self)
        .def(-nb::self)
        .def(
            "__sub__",
            [](const APyFloatArray& lhs, const nb::int_& rhs) {
                return lhs
                    - APyFloat::from_integer(
                           rhs, lhs.get_exp_bits(), lhs.get_man_bits(), lhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFloatArray& rhs, const nb::int_& lhs) {
                return (-rhs)
                    + APyFloat::from_integer(
                           lhs, rhs.get_exp_bits(), rhs.get_man_bits(), rhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__sub__",
            [](const APyFloatArray& lhs, double rhs) {
                return lhs
                    - APyFloat::from_double(
                           rhs, lhs.get_exp_bits(), lhs.get_man_bits(), lhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFloatArray& rhs, double lhs) {
                return (-rhs)
                    + APyFloat::from_double(
                           lhs, rhs.get_exp_bits(), rhs.get_man_bits(), rhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__sub__",
            [](const APyFloatArray& lhs, const APyFloat& rhs) { return lhs - rhs; },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFloatArray& rhs, const APyFloat& lhs) { return (-rhs) + lhs; },
            nb::is_operator()
        )
        .def(nb::self * nb::self)
        .def(
            "__mul__",
            [](const APyFloatArray& lhs, const nb::int_& rhs) {
                return lhs
                    * APyFloat::from_integer(
                           rhs, lhs.get_exp_bits(), lhs.get_man_bits(), lhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFloatArray& rhs, const nb::int_& lhs) {
                return rhs
                    * APyFloat::from_integer(
                           lhs, rhs.get_exp_bits(), rhs.get_man_bits(), rhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__mul__",
            [](const APyFloatArray& lhs, double rhs) {
                return lhs
                    * APyFloat::from_double(
                           rhs, lhs.get_exp_bits(), lhs.get_man_bits(), lhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFloatArray& rhs, double lhs) {
                return rhs
                    * APyFloat::from_double(
                           lhs, rhs.get_exp_bits(), rhs.get_man_bits(), rhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__mul__",
            [](const APyFloatArray& lhs, const APyFloat& rhs) { return lhs * rhs; },
            nb::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFloatArray& rhs, const APyFloat& lhs) { return rhs * lhs; },
            nb::is_operator()
        )
        .def(nb::self / nb::self)
        .def(
            "__truediv__",
            [](const APyFloatArray& lhs, const nb::int_& rhs) {
                return lhs
                    / APyFloat::from_integer(
                           rhs, lhs.get_exp_bits(), lhs.get_man_bits(), lhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__rtruediv__",
            [](const APyFloatArray& rhs, const nb::int_& lhs) {
                return rhs.rtruediv(APyFloat::from_integer(
                    lhs, rhs.get_exp_bits(), rhs.get_man_bits(), rhs.get_bias()
                ));
            },
            nb::is_operator()
        )
        .def(
            "__truediv__",
            [](const APyFloatArray& lhs, double rhs) {
                return lhs
                    / APyFloat::from_double(
                           rhs, lhs.get_exp_bits(), lhs.get_man_bits(), lhs.get_bias()
                    );
            },
            nb::is_operator()
        )
        .def(
            "__rtruediv__",
            [](const APyFloatArray& rhs, double lhs) {
                return rhs.rtruediv(APyFloat::from_double(
                    lhs, rhs.get_exp_bits(), rhs.get_man_bits(), rhs.get_bias()
                ));
            },
            nb::is_operator()
        )
        .def(
            "__truediv__",
            [](const APyFloatArray& lhs, const APyFloat& rhs) { return lhs / rhs; },
            nb::is_operator()
        )
        .def(
            "__rtruediv__",
            [](const APyFloatArray& rhs, const APyFloat& lhs) {
                return rhs.rtruediv(lhs);
            },
            nb::is_operator()
        )
        .def("__abs__", &APyFloatArray::abs)

        /*
         * Methods
         */
        .def_prop_ro("exp_bits", &APyFloatArray::get_exp_bits, R"pbdoc(
            Number of exponent bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("man_bits", &APyFloatArray::get_man_bits, R"pbdoc(
            Number of mantissa bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("bits", &APyFloatArray::get_bits, R"pbdoc(
            Total number of bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("bias", &APyFloatArray::get_bias, R"pbdoc(
            Bias.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("shape", &APyFloatArray::python_get_shape, R"pbdoc(
            The shape of the array.

            Returns
            -------
            :class:`tuple` of :class:`int`
            )pbdoc")

        .def_prop_ro("ndim", &APyFloatArray::get_ndim, R"pbdoc(
            Number of dimensions in the array.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_prop_ro(
            "T", [](const APyFloatArray& self) { return self.transpose(); }, R"pbdoc(
            The transposition of the array.

            Equivalent to calling :func:`APyFloatArray.transpose`.

            Returns
            -------
            :class:`APyFloatArray`
            )pbdoc"
        )
        .def("to_numpy", &APyFloatArray::to_numpy, R"pbdoc(
            Return array as a :class:`numpy.ndarray` of :class:`numpy.float64`.

            The returned array has the same `shape` and values as `self`. This
            method rounds away from infinity on ties.

            Returns
            -------
            :class:`numpy.ndarray`
            )pbdoc")
        .def(
            "to_bits",
            &APyFloatArray::to_bits,
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
        .def("reshape", &APyFloatArray::reshape, nb::arg("number_sequence"), R"pbdoc(
        Reshape the APyFloatArray to the specified shape without changing its data.

        Parameters
        ----------
        new_shape : `tuple`
            The new shape should be compatible with the original shape. If a dimension is -1, its value will be inferred from the length of the array and remaining dimensions. Only one dimension can be -1.

        Raises
        ------
        :class:`ValueError`
            If negative dimensions less than -1 are provided, if the total size of the new array is not unchanged and divisible by the known dimensions, or if the total number of elements does not match the original array.

        Examples
        --------
        >>> from apytypes import APyFloatArray
        >>> signs = [0, 0, 1, 1]
        >>> exps = [127, 128, 128, 129]
        >>> mans = [0, 0, 4194304, 0]
        >>> arr = APyFloatArray(signs=signs, exps=exps, mans=mans, exp_bits=8, man_bits=23)
        >>> arr.to_numpy()
        array([ 1.,  2., -3., -4.])

        >>> arr.reshape((2, 2)).to_numpy()
        array([[ 1.,  2.],
               [-3., -4.]])

        >>> arr.reshape((4,)).to_numpy()
        array([ 1.,  2., -3., -4.])

        >>> arr.reshape((2, -1)).to_numpy()
        array([[ 1.,  2.],
               [-3., -4.]])

        Returns
        -------
        :class:`APyFloatArray`
        )pbdoc")

        .def("flatten", &APyFloatArray::flatten, R"pbdoc(
        Return a copy of the array collapsed into one dimension.

        Examples
        --------
        >>> from apytypes import APyFloatArray
        >>> signs = [[0, 0], [1, 1]]
        >>> exps = [[127, 128], [128, 129]]
        >>> mans = [[0, 0], [4194304, 0]]
        >>> arr = APyFloatArray(signs=signs, exps=exps, mans=mans, exp_bits=8, man_bits=23)
        >>> arr.to_numpy()
        array([[ 1.,  2.],
               [-3., -4.]])

        >>> arr.flatten().to_numpy()
        array([ 1.,  2., -3., -4.])

        Returns
        -------
        :class:`APyFloatArray`
            )pbdoc")

        .def("ravel", &APyFloatArray::ravel, R"pbdoc(
        Return a copy of the array collapsed into one dimension. Same as flatten with current memory-copy model.

        Examples
        --------
        >>> from apytypes import APyFloatArray
        >>> signs = [[0, 0], [1, 1]]
        >>> exps = [[127, 128], [128, 129]]
        >>> mans = [[0, 0], [4194304, 0]]
        >>> arr = APyFloatArray(signs=signs, exps=exps, mans=mans, exp_bits=8, man_bits=23)
        >>> arr.to_numpy()
        array([[ 1.,  2.],
               [-3., -4.]])

        >>> arr.ravel().to_numpy()
        array([ 1.,  2., -3., -4.])

        Returns
        -------
        :class:`APyFloatArray`
            )pbdoc")

        /*
         * Static methods
         */
        .def_static(
            "from_float",
            &APyFloatArray::from_double,
            nb::arg("number_sequence"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloatArray` object from a sequence of :class:`int` or :class:`float`.

            Parameters
            ----------
            number_sequence : sequence of numbers
                Floating point values to initialize from. The tensor shape will be taken
                from the sequence shape.
            exp_bits : int
                Number of exponent bits in the created floating-point tensor
            man_bits : int
                Number of mantissa bits in the created floating-point tensor
            bias : int, optional
                Bias in the created floating-point tensor

            Examples
            --------

            >>> from apytypes import APyFloatArray

            Array `a`, initialized from floating-point values.

            >>> a = APyFloatArray.from_float([1.0, 1.25, 1.49], exp_bits=10, man_bits=15)

            Array `lhs` (2 x 3 matrix), initialized from floating-point values.

            >>> lhs = APyFloatArray.from_float(
            ...     [
            ...         [1.0, 2.0, 3.0],
            ...         [4.0, 5.0, 6.0],
            ...     ],
            ...     exp_bits=5,
            ...     man_bits=2
            ... )

            Returns
            -------
            :class:`APyFloatArray`
            )pbdoc"
        )
        .def_static(
            "from_array",
            &APyFloatArray::from_array,
            nb::arg("ndarray"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloatArray` object from an ndarray.

            Parameters
            ----------
            ndarray : ndarray
                Values to initialize from. The tensor shape will be taken from the ndarray shape.
            exp_bits : int
                Number of exponent bits in the created floating-point tensor
            man_bits : int
                Number of mantissa bits in the created floating-point tensor
            bias : int, optional
                Bias in the created floating-point tensor

            Examples
            --------

            >>> from apytypes import APyFloatArray
            >>> import numpy as np

            Array `a`, initialized from NumPy ndarray

            >>> a = APyFloatArray.from_array(
            ...     np.array([
            ...         [1.0, 2.0, 3.0],
            ...         [4.0, 5.0, 6.0],
            ...     ]),
            ...     man_bits=10,
            ...     exp_bits=10
            ... )

            Returns
            -------
            :class:`APyFloatArray`
            )pbdoc"
        )
        .def_static(
            "zeros",
            &APyFloatArray::zeros,
            nb::arg("shape"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = std::nullopt,
            R"pbdoc(

            Initializes an array with zeros.

            Parameters
            ----------
            shape : tuple
                Shape of the array.
            exp_bits : int
                Number of exponent bits.
            man_bits : int
                Number of mantissa bits.
            bias : optional
                Bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Returns
            -------
            APyFloatArray
                An array filled with zeros.
    )pbdoc"
        )
        .def_static(
            "ones",
            &APyFloatArray::ones,
            nb::arg("shape"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = std::nullopt,
            R"pbdoc(
        Initializes an array with ones.

        Parameters
        ----------
        shape : tuple
            Shape of the array.
        exp_bits : int
            Number of exponent bits.
        man_bits : int
            Number of mantissa bits.
        bias : optional
            Bias. If not provided, *bias* is ``2**exp_bits - 1``.

        Returns
        -------
        APyFloatArray
            An array filled with ones.
    )pbdoc"
        )
        .def_static(
            "eye",
            &APyFloatArray::eye,
            nb::arg("n"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("m") = nb::none(),
            nb::arg("bias") = std::nullopt,
            R"pbdoc(
        Initializes an array with ones on the diagonal.

        Parameters
        ----------
        n : int
            Number of rows (and columns) in the n x n output.
        exp_bits : int
            Number of exponent bits.
        man_bits : int
            Number of mantissa bits.
        m : int, optional
            Number of columns. Default is None, which results in an n x n output.
        bias : optional
            Bias. If not provided, *bias* is ``2**exp_bits - 1``.

        Returns
        -------
        APyFloatArray
            An array with the specified value on the diagonal.
    )pbdoc"
        )
        .def_static(
            "identity",
            &APyFloatArray::identity,
            nb::arg("n"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = std::nullopt,
            R"pbdoc(
        Initializes an identity matrix with ones on the diagonal.

        Parameters
        ----------
        n : int
            Number of rows (and columns) in the n x n output.
        exp_bits : int
            Number of exponent bits.
        man_bits : int
            Number of mantissa bits.
        bias : optional
            Bias. If not provided, *bias* is ``2**exp_bits - 1``.

        Returns
        -------
        APyFloatArray
            An identity matrix with ones on the diagonal.
    )pbdoc"
        )
        .def_static(
            "full",
            &APyFloatArray::full,
            nb::arg("shape"),
            nb::arg("fill_value"),
            R"pbdoc(
        Initializes an array filled with the specified value.

        Parameters
        ----------
        shape : tuple
            Shape of the array.
        fill_value : APyFloat
            Value to fill the array.

        Returns
        -------
        APyFloatArray
            An array filled with the specified value.
    )pbdoc"
        )
        .def_static(
            "_arange",
            &APyFloatArray::arange,
            nb::arg("start"),
            nb::arg("stop"),
            nb::arg("step"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = std::nullopt,
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
            exp_bits : int
                    Number of exponent bits.
            man_bits : int
                Number of mantissa bits.
            bias : int, optional
                Bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Returns
            -------
            :class:`APyFloatArray`
        )pbdoc"
        )
        /*
         * Dunder methods
         */
        .def("__matmul__", &APyFloatArray::matmul, nb::arg("rhs"))
        .def("__repr__", &APyFloatArray::repr)
        .def("__len__", &APyFloatArray::get_size)

        .def("is_identical", &APyFloatArray::is_identical, nb::arg("other"), R"pbdoc(
            Test if two :class:`APyFloatArray` objects are identical.

            Two :class:`APyFloatArray` objects are considered identical if, and only if:
              * They represent exactly the same tensor shape
              * They store the exact same floating-ppint values in all tensor elements
              * They have the exact same bit format (`exp_bits`, `man_bits`, and `bias`)

            Returns
            -------
            :class:`bool`
            )pbdoc")

        .def(
            "swapaxes",
            &APyFloatArray::swapaxes,
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
          >>> from apytypes import APyFloatArray
          >>> x = APyFloatArray.from_float([[1 ,2, 3]], exp_bits=5, man_bits=2)
          >>> x.swapaxes(0,1).to_numpy()
          array([[1.],
                 [2.],
                 [3.]])

          >>> x = APyFloatArray.from_float([[[0, 1], [2, 3]], [[4, 5], [6, 7]]], exp_bits=5, man_bits=5)
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
          a_swapped : :class:`APyFloatArray`
              Copy of `a` with axes swapped
                    )pbdoc"
        )

        .def(
            "transpose",
            &APyFloatArray::transpose,
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
            >>> from apytypes import APyFloatArray
            >>> a = APyFloatArray.from_float([[1.0, 2.0, 3.0], [-4.0, -5.0, -6.0]], exp_bits=5, man_bits=2)
            >>> a.to_numpy()
            array([[ 1.,  2.,  3.],
                   [-4., -5., -6.]])
            >>> a = a.transpose()
            >>> a.to_numpy()
            array([[ 1., -4.],
                   [ 2., -5.],
                   [ 3., -6.]])

            >>> a = APyFloatArray.from_float([1.0] * 6, exp_bits=5, man_bits=2).reshape((1, 2, 3))
            >>> a.transpose((1, 0, 2)).shape
            (2, 1, 3)

            >>> a.transpose((-2, -3, -1)).shape
            (2, 1, 3)

            Returns
            -------
            :class:`APyFloatArray`
                `a` with its axes permuted.
            )pbdoc"
        )
        .def(
            "broadcast_to",
            &APyFloatArray::broadcast_to_python,
            nb::arg("shape"),
            R"pbdoc(
            Broadcast array to new shape.

            .. versionadded:: 0.2

            Parameters
            ----------
            shape : tuple or int
                The shape to broadcast to. A single integer ``i`` is interpreted as ``(i,)``.

            Returns
            -------
            :class:`APyFloatArray`

            )pbdoc"
        )
        .def(
            "convolve",
            &APyFloatArray::convolve,
            nb::arg("other"),
            nb::arg("mode") = "full",
            R"pbdoc(
            Return the discrete linear convolution with another one-dimensional array.

            Requires that ``ndim = 1`` for both `self` and `other`.

            Parameters
            ----------
            other : :class:`APyFloatArray`
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
            convolved : :class:`APyFloatArray`
                The convolved array.

            )pbdoc"
        )

        .def(
            "squeeze",
            &APyFloatArray::squeeze,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Remove axes of size one at the specified axis/axes, if no axís is given removes all dimensions with size one.

            Parameters
            ----------
            axis : tuple or int, optional
                The axis/axes to squeeze, a given axis with a size other than one will result in an error. No given axes  will be remove all dimensions of size 1.

            Returns
            -------
            :class:`APyFloatArray`

            Raises
            ------
            :class:`ValueError`
                If given an axis of a size other than one, a ValueError will be thrown.
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            )pbdoc"
        )

        .def(
            "sum",
            &APyFloatArray::sum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the sum of the elements along specified axis/axes.

            Parameters
            ----------
            axis : tuple or int, optional
                The axis/axes to summate across. Will summate the whole array if no int or tuple is specified.

            Returns
            -------
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            -------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            --------

            >>> from apytypes import APyFloatArray

            >>> a = APyFloatArray.from_float(
            ...     [1,2,3,4,5,6],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )

            >>> a.sum()
            APyFloat(sign=0, exp=515, man=320, exp_bits=10, man_bits=10)

            -------
            )pbdoc"
        )

        .def(
            "cumsum",
            &APyFloatArray::cumsum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the cumulative sum of the elements along a given axis.

            Parameters
            ----------
            axis : int, optional
                The axis to summate across. If not given an axis it will return the cumulative sum of the flattened array.

            Returns
            -------
            :class:`APyFloatArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            --------

            >>> from apytypes import APyFloatArray

            >>> a = APyFloatArray.from_float(
            ...     [[1,2,3],[4,5,6]],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )

            >>> a.cumsum()
            APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 513, 514, 514, 515], [0, 512, 512, 256, 896, 320], shape=(6,), exp_bits=10, man_bits=10, bias=511)

            >>> a.cumsum(0)
            APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 512, 513, 513, 514], [0, 0, 512, 256, 768, 128], shape=(2, 3), exp_bits=10, man_bits=10, bias=511)

            >>> a.cumsum(1)
            APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 513, 513, 514, 514], [0, 512, 512, 0, 128, 896], shape=(2, 3), exp_bits=10, man_bits=10, bias=511)

            -------
            )pbdoc"
        )

        .def(
            "nansum",
            &APyFloatArray::nansum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the sum of the elements along specified axis/axes treating Not a Number as 0.

            Parameters
            ----------
            axis : tuple or int, optional
                The axis/axes to summate across. Will summate the whole array if no int or tuple is specified.

            Returns
            -------
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            --------

            >>> from apytypes import APyFloatArray

            >>> nan = float("nan")

            >>> a = APyFloatArray.from_float(
            ...     [1,2,3,4,5,nan],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )

            >>> a.nansum()
            APyFloat(sign=0, exp=514, man=896, exp_bits=10, man_bits=10)



            -------
            )pbdoc"
        )

        .def(
            "nancumsum",
            &APyFloatArray::nancumsum,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the cumulative sum of the elements along a given axis treating Not a Number as 0.

            Parameters
            ----------
            axis : int, optional
                The axis to summate across. If not given an axis it will return the cumulative sum of the flattened array.

            Returns
            -------
            :class:`APyFloatArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            --------


            >>> from apytypes import APyFloatArray

            >>> nan = float("nan")

            >>> a = APyFloatArray.from_float(
            ...     [[1,2,3],[4,5,6]],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )

            >>> a.nancumsum()
            APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 513, 514, 514, 515], [0, 512, 512, 256, 896, 320], shape=(6,), exp_bits=10, man_bits=10, bias=511)

            >>> a.nancumsum(0)
            APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 512, 513, 513, 514], [0, 0, 512, 256, 768, 128], shape=(2, 3), exp_bits=10, man_bits=10, bias=511)

            >>> a.nancumsum(1)
            APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 513, 513, 514, 514], [0, 512, 512, 0, 128, 896], shape=(2, 3), exp_bits=10, man_bits=10, bias=511)


            -------
            )pbdoc"
        )

        .def(
            "prod",
            &APyFloatArray::prod,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the product of the elements along specified axis/axes.

            Parameters
            ----------
            axis : tuple, int, optional
                The axis/axes to calculate the product across. If not given an axis it will return the product of the flattened array.

            Returns
            -------
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            --------

            >>> from apytypes import APyFloatArray

            >>> a = APyFloatArray.from_float(
            ...     [1,2,3,4,5,6],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )

            >>> a.prod()
            APyFloat(sign=0, exp=520, man=416, exp_bits=10, man_bits=10)

            -------
            )pbdoc"
        )

        .def(
            "cumprod",
            &APyFloatArray::cumprod,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Return the cumulative product of the elements along a given axes.

            Parameters
            ----------
            axis : int, optional
                The axes to calculate the product across. If not given an axis it will return the cumulative product of the flattened array.

            Returns
            -------
            :class:`APyFloatArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            --------

            >>> from apytypes import APyFloatArray


            >>> a = APyFloatArray.from_float(
            ...     [[1,2,3],[4,5,6]],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )

            >>> a.cumprod()
            APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 513, 515, 517, 520], [0, 0, 512, 512, 896, 416], shape=(6,), exp_bits=10, man_bits=10, bias=511)

            >>> a.cumprod(0)
            APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 512, 513, 514, 515], [0, 0, 512, 0, 256, 128], shape=(2, 3), exp_bits=10, man_bits=10, bias=511)

            >>> a.cumprod(1)
            APyFloatArray([0, 0, 0, 0, 0, 0], [511, 512, 513, 513, 515, 517], [0, 0, 512, 0, 256, 896], shape=(2, 3), exp_bits=10, man_bits=10, bias=511)

            -------
            )pbdoc"
        )

        .def(
            "nanprod",
            &APyFloatArray::nanprod,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
                Return the product of the elements along a given axis treating Not a Number as 0.

                Parameters
                ----------
                axis : int, optional
                    The axis to calculate the product across. If not given an axis it will return the product of the flattened array.

                Returns
                -------
                :class:`APyFloatArray` or :class:`APyFloat`

                Raises
                ------
                :class:`IndexError`
                    If a specified axis is outside of the existing number of dimensions for the array.
            )pbdoc"
        )

        .def(
            "nancumprod",
            &APyFloatArray::nancumprod,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
                Return the cumulative product of the elements along a given axis treating Not a Number as 0.

                Parameters
                ----------
                axis : int, optional
                    The axis to calculate the product across. If not given an axis it will return the cumulative product of the flattened array.

                Returns
                -------
                :class:`APyFloatArray`

                Raises
                ------
                :class:`IndexError`
                    If a specified axis is outside of the existing number of dimensions for the array.

            )pbdoc"
        )

        .def(
            "max",
            &APyFloatArray::max,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Returns the maximum value from an array or the maximum along an axis

            Parameters
            ----------
            axis : tuple or int, optional
                The axis to get the maximum along.

            Returns
            -------
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            -------

            >>> from apytypes import APyFloatArray

            Array `a`, array to get the maximum along.

            >>> a = APyFloatArray.from_float(
            ...     [[1, 2, 3], [4, 5, 6]],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )

            >>> a.max()
            APyFloat(sign=0, exp=513, man=512, exp_bits=10, man_bits=10)

            >>> a.max(0)
            APyFloatArray([0, 0, 0], [513, 513, 513], [0, 256, 512], shape=(3,), exp_bits=10, man_bits=10, bias=511)

            >>> a.max(1)
            APyFloatArray([0, 0], [512, 513], [512, 512], shape=(2,), exp_bits=10, man_bits=10, bias=511)

            -------

            )pbdoc"
        )

        .def(
            "min",
            &APyFloatArray::min,
            nb::arg("axis") = nb::none(),
            R"pbdoc(
            Returns the minimum value from an array or the minimum along an axis

            Parameters
            ----------
            axis : tuple or int, optional
                The axis to get the minimum along.

            Returns
            -------
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            Examples
            -------

            >>> from apytypes import APyFloatArray

            Array `a`, array to get the minimum along.

            >>> a = APyFloatArray.from_float(
            ...     [[1, 2, 3], [4, 5, 6]],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )

            >>> a.min()
            APyFloat(sign=0, exp=511, man=0, exp_bits=10, man_bits=10)

            >>> a.min(0)
            APyFloatArray([0, 0, 0], [511, 512, 512], [0, 0, 512], shape=(3,), exp_bits=10, man_bits=10, bias=511)

            >>> a.min(1)
            APyFloatArray([0, 0], [511, 513], [0, 0], shape=(2,), exp_bits=10, man_bits=10, bias=511)

            -------
            )pbdoc"
        )

        .def(
            "nanmax",
            &APyFloatArray::nanmax,
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
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.

            )pbdoc"
        )

        .def(
            "nanmin",
            &APyFloatArray::nanmin,
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
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for the array.


            )pbdoc"
        )

        // Iteration and friends
        .def("__getitem__", &APyFloatArray::get_item, nb::arg("idx"))
        .def(
            "__iter__",
            [](nb::iterable array) {
                return APyFloatArrayIterator(
                    nb::cast<const APyFloatArray&>(array), array
                );
            }
        )
        .def("__array__", &APyFloatArray::to_numpy)

        .def(
            "cast",
            &APyFloatArray::cast,
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
            exp_bits : int, optional
                Number of exponent bits in the result.
            man_bits : int, optional
                Number of mantissa bits in the result.
            bias : int, optional
                Bias used in the result.
            quantization : :class:`QuantizationMode`, optional.
                Quantization mode to use in this cast. If None, use the global
                quantization mode.

            Returns
            -------
            :class:`APyFloat`

            )pbdoc"
        )
        /*
         * Convenience methods
         */
        .def(
            "cast_to_double",
            &APyFloatArray::cast_to_double,
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
            &APyFloatArray::cast_to_single,
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
            &APyFloatArray::cast_to_half,
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
            &APyFloatArray::cast_to_bfloat16,
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
        );

    nb::class_<APyFloatArrayIterator>(m, "APyFloatArrayIterator")
        .def(
            "__iter__",
            [](APyFloatArrayIterator& it) -> APyFloatArrayIterator& { return it; }
        )
        .def("__next__", &APyFloatArrayIterator::next);
}
