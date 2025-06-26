#include "apycfloatarray.h"

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/optional.h>

namespace nb = nanobind;

void bind_cfloat_array(nb::module_& m)
{
    nb::class_<APyCFloatArray>(m, "APyCFloatArray")

        /*
         * Base constructor
         */
        .def(
            nb::init<
                const nb::typed<nb::sequence, nb::any>&,
                const nb::typed<nb::sequence, nb::any>&,
                const nb::typed<nb::sequence, nb::any>&,
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
         * Arithmetic operations
         */
        .def(nb::self + nb::self)
        .def(nb::self - nb::self)
        .def(nb::self * nb::self)
        .def(nb::self / nb::self)
        // .def(-nb::self)
        // .def(+nb::self)

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

            Equivalent to calling :func:`APyCFloatArray.transpose`.

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
            Create an :class:`APyCFloatArray` object from a sequence of :class:`int`,
            :class:`float`, :class:`complex`, :class:`APyFixed`, :class:`APyFloat`, or
            :class:`APyCFixed`.

            Using NumPy arrays as input is in general faster than using e.g. lists.

            Parameters
            ----------
            complex_sequence : sequence of :class:`complex`, :class:`float`, :class:`int`, :class:`APyCFloat`, :class:`APyCFixed`, :class:`APyFixed`, or :class:`APyFloat`.
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
            :class:`APyFloatArray`

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
            Create an :class:`APyCFloatArray` object from a sequence of :class:`int`,
            :class:`float`, :class:`complex`, :class:`APyFixed`, :class:`APyFloat`, or
            :class:`APyCFixed`.

            This is an alias for :func:`~apytypes.APyCFloatArray.from_complex`, look
            there for more documentation.

            Parameters
            ----------
            complex_sequence : sequence of :class:`complex`, :class:`float`, :class:`int`, :class:`APyCFloat`, :class:`APyCFixed`, :class:`APyFixed`, or :class:`APyFloat`.
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
            if:
                * They represent exactly the same tensor shape
                * They store the exact same floating-point values in all elements
                * They have the exact same bit format (`exp_bits`, `man_bits`, and
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
            "_arange",
            &APyCFloatArray::arange,
            nb::arg("start"),
            nb::arg("stop"),
            nb::arg("step"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = std::nullopt,
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
            exp_bits : :class:`int`
                    Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Returns
            -------
            :class:`APyCFloatArray`
            )pbdoc"
        )

        .def("to_numpy", &APyCFloatArray::to_numpy, R"pbdoc(
            Return array as a :class:`numpy.ndarray` of :class:`numpy.complex128`.

            The returned array has the same `shape` and values as `self`. This
            method rounds away from infinity on ties.

            Returns
            -------
            :class:`numpy.ndarray`
            )pbdoc")

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

        /*
         * Dunder methods
         */
        //     .def("__matmul__", &APyCFixedArray::matmul, nb::arg("rhs"))
        .def("__repr__", &APyCFloatArray::repr)
        .def("__str__", &APyCFloatArray::to_string, nb::arg("base") = 10)
        .def("__getitem__", &APyCFloatArray::get_item, nb::arg("key"))
        .def("__setitem__", &APyCFloatArray::set_item, nb::arg("key"), nb::arg("val"))
        .def("__len__", &APyCFloatArray::size)

        ;
}
