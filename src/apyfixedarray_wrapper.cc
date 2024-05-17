#include "apyfixed.h"
#include "apyfixedarray.h"
#include "apyfixedarray_iterator.h"
#include "apytypes_common.h"

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
            [](const APyFixedArray& a, int b) {
                if (b == 0) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot add with non-zero int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFixedArray& a, int b) {
                if (b == 0) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot reverse add with non-zero int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__add__",
            [](const APyFixedArray& a, float b) {
                if (b == 0.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot add with non-zero float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFixedArray& a, float b) {
                if (b == 0.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot reverse add with non-zero float"
                    );
                };
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
            [](const APyFixedArray& a, const int b) {
                if (b == 0) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot subtract with non-zero int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFixedArray& a, const int b) {
                if (b == 0) {
                    return -a;
                } else {
                    throw nanobind::type_error(
                        "Cannot reverse subtract with non-zero int"
                    );
                };
            },
            nb::is_operator()
        )
        .def(
            "__sub__",
            [](const APyFixedArray& a, const float b) {
                if (b == 0.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot subtract with non-zero float");
                };
            },
            nb::is_operator()
        )
        .def(
            "__rsub__",
            [](const APyFixedArray& a, float const b) {
                if (b == 0.) {
                    return -a;
                } else {
                    throw nanobind::type_error(
                        "Cannot reverse subtract with non-zero float"
                    );
                };
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
            [](const APyFixedArray& a, const int b) {
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
            [](const APyFixedArray& a, const int b) {
                if (b == 1) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot reverse multiply with int");
                };
            },
            nb::is_operator()
        )
        .def(
            "__mul__",
            [](const APyFixedArray& a, const float b) {
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
            [](const APyFixedArray& a, const float b) {
                if (b == 1.) {
                    return a;
                } else {
                    throw nanobind::type_error("Cannot multiply with float");
                };
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

        .def_prop_ro("shape", &APyFixedArray::shape, R"pbdoc(
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
        .def_prop_ro("T", &APyFixedArray::transpose, R"pbdoc(
            The transposition of the array.

            Equivalent to calling :func:`APyFixedArray.transpose`.

            Returns
            -------
            :class:`APyFixedArray`
            )pbdoc")

        .def("to_numpy", &APyFixedArray::to_numpy, R"pbdoc(
            Return array as a :class:`numpy.ndarray` of :class:`numpy.float64`.

            The returned array has the same `shape` and values as `self`. This
            method rounds away from infinity on ties.

            Returns
            -------
            :class:`numpy.ndarray`
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

        .def("transpose", &APyFixedArray::transpose, R"pbdoc(
            Return the transposition of the array.

            If the dimension of `self` is one, this method returns the a copy of `self`.
            If the dimension of `self` is two, this method returns the matrix
            transposition of `self`.

            Higher order transposition has not been implemented and will raise a
            `NotImplementedException`.

            Returns
            -------
            :class:`APyFixedArray`
            )pbdoc")
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
        .def("broadcast_to", &APyFixedArray::broadcast_to_python, nb::arg("shape"))

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
                Number of integer bits in the created fixed-point tensor
            frac_bits : int, optional
                Number of fractional bits in the created fixed-point tensor
            bits : int, optional
                Total number of bits in the created fixed-point tensor

            Returns
            -------
            :class:`APyFixedArray`

            Examples
            --------

            .. code-block:: python

                from apytypes import APyFixedArray

                # Array `a`, initialized from floating-point values.
                a = APyFixedArray.from_float([1.0, 1.25, 1.49], int_bits=2, frac_bits=2)

                # Array `b` (2 x 3 matrix), initialized from floating-point values.
                b = APyFixedArray.from_float(
                    [
                        [1.0, 2.0, 3.0],
                        [4.0, 5.0, 6.0],
                    ],
                    bits=5,
                    frac_bits=0
                )
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
                Number of integer bits in the created fixed-point tensor
            frac_bits : int, optional
                Number of fractional bits in the created fixed-point tensor
            bits : int, optional
                Total number of bits in the created fixed-point tensor

            Returns
            -------
            :class:`APyFixedArray`

            Examples
            --------

            .. code-block:: python

                from apytypes import APyFixedArray
                import numpy as np

                # Array `a`, initialized from NumPy ndarray
                a = APyFixedArray.from_array(
                    np.array([
                        [1.0, 2.0, 3.0],
                        [4.0, 5.0, 6.0],
                    ]),
                    int_bits=10,
                    frac_bits=0
                )
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
