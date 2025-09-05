#include "apyfloatarray.h"
#include "apyfloatarray_iterator.h"

#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/optional.h>

namespace nb = nanobind;

/*
 * Binding function of a custom R-operator (e.g., `__rmul__`) with non APyFloatArray
 * type
 */
template <auto FUNC, typename L_TYPE>
static APyFloatArray R_OP(const APyFloatArray& rhs, const L_TYPE& lhs)
{
    [[maybe_unused]] int exp_bits = rhs.get_exp_bits();
    [[maybe_unused]] int man_bits = rhs.get_man_bits();
    [[maybe_unused]] exp_t bias = rhs.get_bias();
    if constexpr (std::is_floating_point_v<L_TYPE>) {
        return (rhs.*FUNC)(APyFloat::from_double(lhs, exp_bits, man_bits, bias));
    } else if constexpr (std::is_same_v<remove_cvref_t<L_TYPE>, APyFloat>) {
        return (rhs.*FUNC)(lhs);
    } else {
        return (rhs.*FUNC)(APyFloat::from_integer(lhs, exp_bits, man_bits, bias));
    }
}

/*
 * Binding function of a custom L-operator (e.g., `__sub__`) with non APyFloatArray type
 */
template <typename OP, typename R_TYPE>
static APyFloatArray L_OP(const APyFloatArray& lhs, const R_TYPE& rhs)
{
    [[maybe_unused]] int exp_bits = lhs.get_exp_bits();
    [[maybe_unused]] int man_bits = lhs.get_man_bits();
    [[maybe_unused]] exp_t bias = lhs.get_bias();
    if constexpr (std::is_floating_point_v<R_TYPE>) {
        return OP()(lhs, APyFloat::from_double(rhs, exp_bits, man_bits, bias));
    } else if constexpr (std::is_same_v<remove_cvref_t<R_TYPE>, APyFloat>) {
        return OP()(lhs, rhs);
    } else if constexpr (std::is_same_v<
                             remove_cvref_t<R_TYPE>,
                             nb::ndarray<nb::c_contig>>) {
        return OP()(lhs, APyFloatArray::from_array(rhs, exp_bits, man_bits, bias));
    } else {
        return OP()(lhs, APyFloat::from_integer(rhs, exp_bits, man_bits, bias));
    }
}

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
            Create an :class:`APyFloatArray` object.

            Parameters
            ----------
            signs : :class:`~collections.abc.Iterable` of :class:`bool`s or :class:`int`s
                The sign of the float. False/0 means positive. True/non-zero means
                negative.
            exps : :class:`~collections.abc.Iterable` of :class:`int`s
                Exponents of the floats as stored, i.e., actual value + bias.
            mans : :class:`~collections.abc.Iterable` of :class:`int`s
                Mantissas of the floats as stored, i.e., without a hidden one.
            exp_bits : :class:`int`
                Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Returns
            -------
            :class:`APyFloatArray`
            )pbdoc"
        )

        /*
         * Copy
         */
        .def(
            "copy",
            &APyFloatArray::python_copy,
            R"pbdoc(
            Create a copy of the object.

            .. versionadded:: 0.3
            )pbdoc"
        )
        .def("__copy__", &APyFloatArray::python_copy)
        .def("__deepcopy__", &APyFloatArray::python_deepcopy, nb::arg("memo"))

        /*
         * Arithmetic operations
         */
        .def(nb::self + nb::self, NB_NARG())
        .def(nb::self - nb::self, NB_NARG())
        .def(nb::self * nb::self, NB_NARG())
        .def(nb::self / nb::self, NB_NARG())
        .def(-nb::self)
        .def(+nb::self)

        /*
         * Arithmetic operators with integers
         */
        .def("__add__", L_OP<STD_ADD<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyFloatArray::rsub, nb::int_>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, nb::int_>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyFloatArray::rdiv, nb::int_>, NB_OP(), NB_NARG())

        /*
         * Arithmetic operators with floats
         */
        .def("__add__", L_OP<STD_ADD<>, double>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, double>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, double>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyFloatArray::rsub, double>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, double>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, double>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, double>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyFloatArray::rdiv, double>, NB_OP(), NB_NARG())

        /*
         * Arithmetic operators with APyFloat
         */
        .def("__add__", L_OP<STD_ADD<>, APyFloat>, NB_OP(), NB_NARG())
        .def("__radd__", L_OP<STD_ADD<>, APyFloat>, NB_OP(), NB_NARG())
        .def("__sub__", L_OP<STD_SUB<>, APyFloat>, NB_OP(), NB_NARG())
        .def("__rsub__", R_OP<&APyFloatArray::rsub, APyFloat>, NB_OP(), NB_NARG())
        .def("__mul__", L_OP<STD_MUL<>, APyFloat>, NB_OP(), NB_NARG())
        .def("__rmul__", L_OP<STD_MUL<>, APyFloat>, NB_OP(), NB_NARG())
        .def("__truediv__", L_OP<STD_DIV<>, APyFloat>, NB_OP(), NB_NARG())
        .def("__rtruediv__", R_OP<&APyFloatArray::rdiv, APyFloat>, NB_OP(), NB_NARG())

        /*
         * Arithmetic operations with NumPy arrays
         * The right-hand versions are not used since Numpy will convert the
         * APyFloatArray to a Numpy array before they are invoked.
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
         * Methods
         */
        .def("__abs__", &APyFloatArray::abs)
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
            Exponent bias.

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

        .def_prop_ro("ndim", &APyFloatArray::ndim, R"pbdoc(
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
        .def(
            "to_numpy",
            &APyFloatArray::to_numpy,
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
        .def("reshape", &APyFloatArray::python_reshape, nb::arg("new_shape"), R"pbdoc(
            Reshape the APyFloatArray to the specified shape without changing its data.

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
            >>> arr = apy.fp([1, 2, -3, -4], exp_bits=8, man_bits=23)
            >>> print(arr)
            [ 1,  2, -3, -4]
            >>> print(arr.reshape((2, 2)))
            [[ 1,  2],
             [-3, -4]]
            >>> print(arr.reshape((4, 1)))
            [[ 1],
             [ 2],
             [-3],
             [-4]]

            Returns
            -------
            :class:`APyFloatArray`
            )pbdoc")

        .def("flatten", &APyFloatArray::flatten, R"pbdoc(
            Return a copy of the array collapsed into one dimension.

            Examples
            --------
            >>> import apytypes as apy
            >>> arr = apy.fp([[ 1,  2],
            ...               [-3, -4]], exp_bits=8, man_bits=23)
            >>> print(arr)
            [[ 1,  2],
             [-3, -4]]
            >>> print(arr.flatten())
            [ 1,  2, -3, -4]

            Returns
            -------
            :class:`APyFloatArray`
            )pbdoc")
        .def("ravel", &APyFloatArray::ravel, R"pbdoc(
            Return a copy of the array collapsed into one dimension. Same as flatten
            with current memory-copy model.

            Examples
            --------
            >>> import apytypes as apy
            >>> arr = apy.fp([[ 1,  2],
            ...               [-3, -4]], exp_bits=8, man_bits=23)
            >>> print(arr)
            [[ 1,  2],
             [-3, -4]]
            >>> print(arr.ravel())
            [ 1,  2, -3, -4]

            Returns
            -------
            :class:`APyFloatArray`
            )pbdoc")

        /*
         * Static methods
         */
        .def_static(
            "from_float",
            &APyFloatArray::from_numbers,
            nb::arg("number_sequence"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloatArray` from iterable sequence of numbers.

            Parameters
            ----------
            number_sequence : :class:`~collections.abc.Iterable` of numbers
                Floating point values to initialize from. The tensor shape will be taken
                from the sequence shape.
            exp_bits : :class:`int`
                Number of exponent bits in the created floating-point tensor
            man_bits : :class:`int`
                Number of mantissa bits in the created floating-point tensor
            bias : :class:`int`, optional
                Bias in the created floating-point tensor

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.APyFloatArray.from_float(
            ...     [1.0, 1.25, 1.49], exp_bits=10, man_bits=15
            ... )
            >>> a
            APyFloatArray(
                [    0,     0,     0],
                [  511,   511,   511],
                [    0,  8192, 16056],
                exp_bits=10,
                man_bits=15
            )
            >>> print(a)
            [      1,    1.25, 1.48999]

            >>> b = apy.APyFloatArray.from_float(
            ...     [
            ...         [1.0, 2.0, 3.0],
            ...         [4.0, 5.0, 6.0],
            ...     ],
            ...     exp_bits=5,
            ...     man_bits=2
            ... )
            >>> b
            APyFloatArray(
                [[ 0,  0,  0],
                 [ 0,  0,  0]],
            <BLANKLINE>
                [[15, 16, 16],
                 [17, 17, 17]],
            <BLANKLINE>
                [[ 0,  0,  2],
                 [ 0,  1,  2]],
                exp_bits=5,
                man_bits=2
            )
            >>> print(b)
            [[1, 2, 3],
             [4, 5, 6]]

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
            "from_array",
            &APyFloatArray::from_array,
            nb::arg("ndarray"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloatArray` from an ndarray.

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
            >>> a = apy.APyFloatArray.from_array(
            ...     np.array([
            ...         [1.0, 2.0, 3.0],
            ...         [4.0, 5.0, 6.0],
            ...     ]),
            ...     man_bits=10,
            ...     exp_bits=10
            ... )
            >>> print(a)
            [[1, 2, 3],
             [4, 5, 6]]
            >>> a
            APyFloatArray(
                [[  0,   0,   0],
                 [  0,   0,   0]],
            <BLANKLINE>
                [[511, 512, 512],
                 [513, 513, 513]],
            <BLANKLINE>
                [[  0,   0, 512],
                 [  0, 256, 512]],
                exp_bits=10,
                man_bits=10
            )

            Returns
            -------
            :class:`APyFloatArray`

            See Also
            --------
            fp
            from_float

            )pbdoc"
        )
        .def_static(
            "from_bits",
            &APyFloatArray::from_bits,
            nb::arg("bits"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloatArray` object from bit-representations.

            Parameters
            ----------
            bits : :class:`int`
                The bit-representations.
            exp_bits : :class:`int`
                Number of exponent bits in the created floating-point tensor
            man_bits : :class:`int`
                Number of mantissa bits in the created floating-point tensor
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.APyFloatArray.from_bits(
            ...     [[60, 61], [80, 82]],
            ...     exp_bits=5,
            ...     man_bits=2
            ... )
            >>> a
            APyFloatArray(
                [[ 0,  0],
                 [ 0,  0]],
            <BLANKLINE>
                [[15, 15],
                 [20, 20]],
            <BLANKLINE>
                [[ 0,  1],
                 [ 0,  2]],
                exp_bits=5,
                man_bits=2
            )
            >>> print(a)
            [[   1, 1.25],
             [  32,   48]]

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
            :class:`APyFloatArray`
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
            :class:`APyFloatArray`
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
            :class:`APyFloatArray`
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
            :class:`APyFloatArray`
                An identity matrix with ones on the diagonal.
            )pbdoc"
        )
        .def_static(
            "full",
            &APyFloatArray::full,
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
            :class:`APyFloatArray`
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
            :class:`APyFloatArray`
            )pbdoc"
        )
        .def_static(
            "_fullrange",
            &APyFloatArray::fullrange,
            nb::arg("start"),
            nb::arg("stop"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = std::nullopt,
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
            exp_bits : :class:`int`
                    Number of exponent bits.
            man_bits : :class:`int`
                Number of mantissa bits.
            bias : :class:`int`, optional
                Exponent bias. If not provided, *bias* is ``2**exp_bits - 1``.

            Returns
            -------
            :class:`APyFloatArray`
            )pbdoc"
        )
        .def_static(
            "_meshgrid",
            &APyFloatArray::meshgrid,
            nb::arg("arrays"),
            nb::arg("indexing") = "xy",
            R"pbdoc(
            Create a list of coordinate arrays based on several coordinate vectors (1-D arrays).
            The input vectors must all have the same bit specifiers.

            .. versionadded:: 0.4

            Parameters
            ----------
            arrays : :class:`APyFloatArray`
                The coordinate vectors (1-D arrays).
            indexing : {"xy", "ij"}, default: "xy"
                The indexing mode to use for the meshgrid. "xy" is the standard Cartesian indexing,
                while "ij" is the matrix indexing.

            Returns
            -------
            result : list of :class:`APyFloatArray`
                List of 2-D arrays representing the meshgrid.

            Examples
            --------
            >>> import apytypes as apy
            >>> x = apy.fp(range(3), exp_bits=5, man_bits=10)
            >>> y = apy.fp(range(4, 7), exp_bits=5, man_bits=10)
            >>> xx, yy = apy.meshgrid(x, y)
            >>> xx
            APyFloatArray([[0, 1, 2],
                           [0, 1, 2],
                           [0, 1, 2]], exp_bits=5, man_bits=10)
            >>> yy
            APyFloatArray([[4, 4, 4],
                           [5, 5, 5],
                           [6, 6, 6]], exp_bits=5, man_bits=10)
            )pbdoc"
        )

        /*
         * Dunder methods
         */
        .def("__matmul__", &APyFloatArray::matmul, nb::arg("rhs"))
        .def("__repr__", &APyFloatArray::repr)
        .def("__len__", &APyFloatArray::size)
        .def("__str__", &APyFloatArray::to_string, nb::arg("base") = 10)

        .def(
            "is_identical",
            &APyFloatArray::is_identical,
            nb::arg("other"),
            nb::arg("ignore_zero_sign") = false,
            R"pbdoc(
            Test if two :class:`APyFloatArray` objects are identical.

            Two :class:`APyFloatArray` objects are considered identical if, and only if:
                * They represent exactly the same tensor shape
                * They store the exact same floating-point values in all tensor elements
                * They have the exact same bit format (`exp_bits`, `man_bits`, and
                  `bias`)

            Parameters
            ----------
            other : :class:`APyFloatArray`
                The other array to compare against.

            ignore_zero_sign : :class:`bool`, default: :code:`False`
                If :code:`True`, plus and minus zero are considered identical. If
                :code:`False`, plus and minus zero are considered non-identical.

            Returns
            -------
            :class:`bool`
            )pbdoc"
        )
        .def(
            "swapaxes",
            &APyFloatArray::swapaxes,
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
            >>> a = apy.fp([[1 ,2, 3]], exp_bits=5, man_bits=2)
            >>> print(a)
            [[1, 2, 3]]
            >>> print(a.swapaxes(0,1))
            [[1],
             [2],
             [3]]

            >>> b = apy.fp([[[0, 1], [2, 3]], [[4, 5], [6, 7]]], exp_bits=5, man_bits=5)
            >>> print(b)
            [[[0, 1],
              [2, 3]],
            <BLANKLINE>
             [[4, 5],
              [6, 7]]]
            >>> print(b.swapaxes(0,2))
            [[[0, 4],
              [2, 6]],
            <BLANKLINE>
             [[1, 5],
              [3, 7]]]

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
            axes : :class:`tuple` of :class:`int`, optional
                If specified, it must be a tuple or list which contains a permutation
                of [0,1,...,N-1] where N is the number of axes of `a`. The `i`'th axis
                of the returned array will correspond to the axis numbered ``axes[i]``
                of the input. If not specified, defaults to ``range(a.ndim)[::-1]``,
                which reverses the order of the axes.

            Examples
            --------
            >>> import apytypes as apy
            >>> a = apy.fp([[1, 2, 3], [-4, -5, -6]], exp_bits=5, man_bits=2)
            >>> print(a)
            [[ 1,  2,  3],
             [-4, -5, -6]]
            >>> print(a.transpose())
            [[ 1, -4],
             [ 2, -5],
             [ 3, -6]]

            >>> b = apy.fp([1.0] * 6, exp_bits=5, man_bits=2).reshape((1, 2, 3))
            >>> b.shape
            (1, 2, 3)
            >>> b.transpose((1, 0, 2)).shape
            (2, 1, 3)
            >>> b.transpose((-2, -3, -1)).shape
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
            shape : :class:`tuple` of :class:`int` or :class:`int`
                The shape to broadcast to. A single integer ``i`` is interpreted as
                ``(i,)``.

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

        .def("squeeze", &APyFloatArray::squeeze, nb::arg("axis") = nb::none(), R"pbdoc(
            Remove axes of size one at the specified axis/axes.

            If no axis is given, remove all dimensions with size one.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis/axes to squeeze, a given axis with a size other than one will
                result in an error. No given axes  will be remove all dimensions of size
                1.

            Returns
            -------
            :class:`APyFloatArray`

            Raises
            ------
            :class:`ValueError`
                If given an axis of a size other than one, a ValueError will be thrown.
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            )pbdoc")

        .def("sum", &APyFloatArray::sum, nb::arg("axis") = nb::none(), R"pbdoc(
            Return the sum of the elements along specified axis/axes.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis/axes to summate across. Will summate the whole array if no int
                or tuple is specified.

            Returns
            -------
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.fp(
            ...     [1, 2, 3, 4, 5, 6],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )
            >>> a
            APyFloatArray(
                [  0,   0,   0,   0,   0,   0],
                [511, 512, 512, 513, 513, 513],
                [  0,   0, 512,   0, 256, 512],
                exp_bits=10,
                man_bits=10
            )
            >>> a.sum()
            APyFloat(sign=0, exp=515, man=320, exp_bits=10, man_bits=10)
            >>> print(a)
            [1, 2, 3, 4, 5, 6]
            >>> print(a.sum())
            21

            -------
            )pbdoc")

        .def("cumsum", &APyFloatArray::cumsum, nb::arg("axis") = nb::none(), R"pbdoc(
            Return the cumulative sum of the elements along a given axis.

            Parameters
            ----------
            axis : :class:`int`, optional
                The axis to summate across. If not given an axis it will return the
                cumulative sum of the flattened array.

            Returns
            -------
            :class:`APyFloatArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.fp(
            ...     [[1, 2, 3], [4, 5, 6]],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )
            >>> print(a)
            [[1, 2, 3],
             [4, 5, 6]]
            >>> print(a.cumsum())
            [ 1,  3,  6, 10, 15, 21]
            >>> print(a.cumsum(0))
            [[1, 2, 3],
             [5, 7, 9]]
            >>> print(a.cumsum(1))
            [[ 1,  3,  6],
             [ 4,  9, 15]]

            -------
            )pbdoc")

        .def("nansum", &APyFloatArray::nansum, nb::arg("axis") = nb::none(), R"pbdoc(
            Return the sum of the elements along specified axis/axes treating NaN as 0.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis/axes to summate across. Will summate the whole array if no int
                or tuple is specified.

            Returns
            -------
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------

            >>> import apytypes as apy
            >>> nan = float("nan")
            >>> a = apy.fp(
            ...     [1, 2, 3, 4, 5, nan],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )
            >>> print(a)
            [  1,   2,   3,   4,   5, nan]
            >>> print(a.sum())
            nan
            >>> print(a.nansum())
            15

            -------
            )pbdoc")

        .def(
            "nancumsum",
            &APyFloatArray::nancumsum,
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
            :class:`APyFloatArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------

            >>> import apytypes as apy
            >>> nan = float("nan")
            >>> a = apy.fp(
            ...     [[1, 2, 3], [4, 5, nan]],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )
            >>> print(a)
            [[  1,   2,   3],
             [  4,   5, nan]]
            >>> print(a.cumsum())
            [  1,   3,   6,  10,  15, nan]
            >>> print(a.nancumsum())
            [ 1,  3,  6, 10, 15, 15]

            >>> print(a.cumsum(0))
            [[  1,   2,   3],
             [  5,   7, nan]]
            >>> print(a.nancumsum(0))
            [[1, 2, 3],
             [5, 7, 3]]

            >>> print(a.cumsum(1))
            [[  1,   3,   6],
             [  4,   9, nan]]
            >>> print(a.nancumsum(1))
            [[1, 3, 6],
             [4, 9, 9]]

            -------
            )pbdoc"
        )

        .def("prod", &APyFloatArray::prod, nb::arg("axis") = nb::none(), R"pbdoc(
            Return product of elements along specified axis/axes.

            Parameters
            ----------
            axis : :class:`tuple`, int, optional
                The axis/axes to calculate the product across. If not given an axis it
                will return the product of the flattened array.

            Returns
            -------
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.fp(
            ...     [1, 2, 3, 4, 5, 6],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )
            >>> a
            APyFloatArray(
                [  0,   0,   0,   0,   0,   0],
                [511, 512, 512, 513, 513, 513],
                [  0,   0, 512,   0, 256, 512],
                exp_bits=10,
                man_bits=10
            )
            >>> a.prod()
            APyFloat(sign=0, exp=520, man=416, exp_bits=10, man_bits=10)
            >>> print(a)
            [1, 2, 3, 4, 5, 6]
            >>> print(a.prod())
            720

            -------
            )pbdoc")

        .def("cumprod", &APyFloatArray::cumprod, nb::arg("axis") = nb::none(), R"pbdoc(
            Return cumulative product of elements along a given axes.

            Parameters
            ----------
            axis : :class:`int`, optional
                The axes to calculate the product across. If not given an axis it will
                return the cumulative product of the flattened array.

            Returns
            -------
            :class:`APyFloatArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.fp(
            ...     [[1, 2, 3], [4, 5, 6]],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )
            >>> print(a)
            [[1, 2, 3],
             [4, 5, 6]]
            >>> print(a.cumprod())
            [  1,   2,   6,  24, 120, 720]
            >>> print(a.cumprod(0))
            [[ 1,  2,  3],
             [ 4, 10, 18]]
            >>> print(a.cumprod(1))
            [[  1,   2,   6],
             [  4,  20, 120]]

            -------
            )pbdoc")

        .def("nanprod", &APyFloatArray::nanprod, nb::arg("axis") = nb::none(), R"pbdoc(
            Return product of the elements along a given axis treating NaN as 0.

            Parameters
            ----------
            axis : :class:`int`, optional
                The axis to calculate the product across. If not given an axis it will
                return the product of the flattened array.

            Returns
            -------
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.
            )pbdoc")

        .def(
            "nancumprod",
            &APyFloatArray::nancumprod,
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
            :class:`APyFloatArray`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            )pbdoc"
        )

        .def("max", &APyFloatArray::max, nb::arg("axis") = nb::none(), R"pbdoc(
            Return maximum value of array or along an axis.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis to get the maximum along.

            Returns
            -------
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.fp(
            ...     [[1, 2, 3], [4, 5, 6]],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )
            >>> print(a)
            [[1, 2, 3],
             [4, 5, 6]]
            >>> print(a.max())
            6
            >>> print(a.max(0))
            [4, 5, 6]
            >>> print(a.max(1))
            [3, 6]

            -------
            )pbdoc")

        .def("min", &APyFloatArray::min, nb::arg("axis") = nb::none(), R"pbdoc(
            Return maximum value of array or along an axis.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis to get the minimum along.

            Returns
            -------
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            Examples
            --------

            >>> import apytypes as apy
            >>> a = apy.fp(
            ...     [[1, 2, 3], [4, 5, 6]],
            ...     exp_bits=10,
            ...     man_bits=10
            ... )
            >>> print(a)
            [[1, 2, 3],
             [4, 5, 6]]

            >>> print(a.min())
            1

            >>> print(a.min(0))
            [1, 2, 3]

            >>> print(a.min(1))
            [1, 4]

            -------
            )pbdoc")

        .def("nanmax", &APyFloatArray::nanmax, nb::arg("axis") = nb::none(), R"pbdoc(
            Return maximum value of array or along an axis ignoring NaN.

            Issues a warning when encountering an all-nan slice or axis.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis to get the maximum along.

            Returns
            -------
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.

            )pbdoc")

        .def("nanmin", &APyFloatArray::nanmin, nb::arg("axis") = nb::none(), R"pbdoc(
            Return minimum value of array or along an axis ignoring NaN.

            Issues a warning when encountering an all-nan slice or axis.

            Parameters
            ----------
            axis : :class:`tuple` of :class:`int` or :class:`int`, optional
                The axis to get the minimum along.

            Returns
            -------
            :class:`APyFloatArray` or :class:`APyFloat`

            Raises
            ------
            :class:`IndexError`
                If a specified axis is outside of the existing number of dimensions for
                the array.


            )pbdoc")

        // Iteration and friends
        .def("__getitem__", &APyFloatArray::get_item, nb::arg("key"))
        .def("__setitem__", &APyFloatArray::set_item, nb::arg("key"), nb::arg("val"))
        .def(
            "__iter__",
            [](nb::iterable array) {
                return APyFloatArrayIterator(
                    nb::cast<const APyFloatArray&>(array), array
                );
            }
        )
        .def(
            "__array__",
            &APyFloatArray::to_numpy,
            nb::arg("dtype") = nb::none(),
            nb::arg("copy") = nb::none()
        )

        .def(
            "cast",
            &APyFloatArray::cast,
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
            :class:`APyFloatArray`

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

            Returns
            -------
            :class:`APyFloatArray`
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

            Returns
            -------
            :class:`APyFloatArray`
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

            Returns
            -------
            :class:`APyFloatArray`
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

            Returns
            -------
            :class:`APyFloatArray`
            )pbdoc"
        );

    nb::class_<APyFloatArrayIterator>(m, "APyFloatArrayIterator")
        .def(
            "__iter__",
            [](APyFloatArrayIterator& it) -> APyFloatArrayIterator& { return it; }
        )
        .def("__next__", &APyFloatArrayIterator::next);
}
