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
            nb::init<
                const nb::sequence&,
                const nb::sequence&,
                const nb::sequence&,
                std::uint8_t,
                std::uint8_t,
                std::optional<int>>(),
            nb::arg("signs"),
            nb::arg("exps"),
            nb::arg("mans"),
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none()
        )

        /*
         * Arithmetic operations
         */
        .def(nb::self + nb::self)
        .def(
            "__add__",
            [](const APyFloatArray& a, int b) {
                if (b == 0) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot add with int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFloatArray& a, int b) {
                if (b == 0) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot add with int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__add__",
            [](const APyFloatArray& a, float b) {
                if (b == 0.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot add with float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFloatArray& a, float b) {
                if (b == 0.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot add with float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__add__",
            [](const APyFloatArray& a, APyFloat& b) { return a + b; },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFloatArray& a, APyFloat& b) { return a + b; },
            nb::is_operator()
        )
        .def(nb::self - nb::self)
        .def(-nb::self)
        .def(
            "__sub__",
            [](const APyFloatArray& a, int b) {
                if (b == 0) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot add with int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFloatArray& a, int b) {
                if (b == 0) {
                    return -a;
                } else {
                    throw nanobind::type_error("Cannot sub with int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__sub__",
            [](const APyFloatArray& a, float b) {
                if (b == 0.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot add with float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFloatArray& a, float b) {
                if (b == 0.) {
                    return -a;
                } else {
                    throw nanobind::type_error("Cannot sub with float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__sub__",
            [](const APyFloatArray& a, APyFloat& b) { return a - b; },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFloatArray& a, APyFloat& b) { return (-a) + b; },
            nb::is_operator()
        )
        .def(nb::self * nb::self)
        .def(
            "__mul__",
            [](const APyFloatArray& a, int b) {
                if (b == 1) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot multiply with int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFloatArray& a, int b) {
                if (b == 1) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot multiply with int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__mul__",
            [](const APyFloatArray& a, float b) {
                if (b == 1.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot multiply with float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFloatArray& a, float b) {
                if (b == 1.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot multiply with float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__mul__",
            [](const APyFloatArray& a, APyFloat& b) { return a * b; },
            nb::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFloatArray& a, APyFloat& b) { return a * b; },
            nb::is_operator()
        )
        .def(nb::self / nb::self)
        .def(
            "__truediv__",
            [](const APyFloatArray& a, int b) {
                if (b == 1) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot divide with int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__truediv__",
            [](const APyFloatArray& a, float b) {
                if (b == 1.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot divide with float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__truediv__",
            [](const APyFloatArray& a, APyFloat& b) { return a / b; },
            nb::is_operator()
        )
        .def(
            "__rtruediv__",
            [](const APyFloatArray& a, APyFloat& b) { return a.rtruediv(b); },
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

        .def_prop_ro("bias", &APyFloatArray::get_bias, R"pbdoc(
            Bias.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_prop_ro("shape", &APyFloatArray::get_shape, R"pbdoc(
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
        .def_prop_ro("T", &APyFloatArray::transpose, R"pbdoc(
            The transposition of the array.

            Equivalent to calling :func:`APyFloatArray.transpose`.

            Returns
            -------
            :class:`APyFloatArray`
            )pbdoc")
        .def("to_numpy", &APyFloatArray::to_numpy, R"pbdoc(
            Return array as a :class:`numpy.ndarray` of :class:`numpy.float64`.

            The returned array has the same `shape` and stored value as `self`. This
            method rounds away from infinity on ties.

            Returns
            -------
            :class:`numpy.ndarray`
            )pbdoc")

        /*
         * Static methods
         */
        .def_static(
            "from_float",
            &APyFloatArray::from_double,
            nb::arg("float_sequence"),
            nb::arg("exp_bits") = nb::none(),
            nb::arg("man_bits") = nb::none(),
            nb::arg("bias") = nb::none(),
            nb::arg("quantization") = nb::none(),
            R"pbdoc(
            Create an :class:`APyFloatArray` object from a sequence of :class:`float`.

            Parameters
            ----------
            float_sequence : sequence of float
                Floating point values to initialize from. The tensor shape will be taken
                from the sequence shape.
            exp_bits : int
                Number of exponent bits in the created fixed-point tensor
            man_bits : int, optional
                Number of mantissa bits in the created fixed-point tensor
            bias : int, optional
                Bias in the created fixed-point tensor
            quantization : :class:`QuantizationMode`, optional
                Quantization mode to use. If not provided, the global mode,
                see :func:`get_quantization_mode`, is used.

            Returns
            -------
            :class:`APyFloatArray`

            Examples
            --------

            .. code-block:: python

                from apytypes import APyFloatArray

                # Array `a`, initialized from floating-point values.
                a = APyFloatArray.from_float([1.0, 1.25, 1.49], exp_bits=10, man_bits=15)

                # Array `b` (2 x 3 matrix), initialized from floating-point values.
                b = APyFloatArray.from_float(
                    [
                        [1.0, 2.0, 3.0],
                        [4.0, 5.0, 6.0],
                    ],
                    exp_bits=5,
                    man_bits=2
                )
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
              * They represent exatly the same tensor shape
              * They store the exact same floating-ppint values in all tensor elements
              * They have the exact same bit format (`exp_bits`, `man_bits`, and `bias`)

            Returns
            -------
            :class:`bool`
            )pbdoc")

        .def("transpose", &APyFloatArray::transpose, R"pbdoc(
            Return the transposition of the array.

            If the dimension of `self` is one, this method returns the a copy of `self`.
            If the dimension of `self` is two, this method returns the matrix
            transposition of `self`.

            Higher order transposition has not been implemented and will raise a
            `NotImplementedException`.

            Returns
            -------
            :class:`APyFloatArray`
            )pbdoc")

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
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            nb::arg("quantization") = nb::none()
        )
        .def(
            "resize",
            &APyFloatArray::resize,
            nb::arg("exp_bits"),
            nb::arg("man_bits"),
            nb::arg("bias") = nb::none(),
            nb::arg("quantization") = nb::none(),
            R"pbdoc(
            .. deprecated:: 0.1.pre
               Use :func:`~APyFloatArray.cast` instead.
            )pbdoc"
        ) /*
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
                see :func:`get_quantization_mode`, is used.
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
                see :func:`get_quantization_mode`, is used.
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
                see :func:`get_quantization_mode`, is used.
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
                see :func:`get_quantization_mode`, is used.
            )pbdoc"
        );

    nb::class_<APyFloatArrayIterator>(m, "APyFloatArrayIterator")
        .def(
            "__iter__",
            [](APyFloatArrayIterator& it) -> APyFloatArrayIterator& { return it; }
        )
        .def("__next__", &APyFloatArrayIterator::next);
}
