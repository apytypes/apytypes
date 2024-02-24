#include "apyfixed.h"
#include "apyfixedarray.h"
#include "apyfixedarray_iterator.h"
#include "apytypes_common.h"
#include "apytypes_util.h"

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <optional> // std::optional, std::nullopt

namespace py = pybind11;

void bind_fixed_array(py::module& m)
{
    py::class_<APyFixedArray>(m, "APyFixedArray")

        /*
         * Constructor: construct from a list of Python integers
         */
        .def(
            py::init<
                const py::sequence&,
                std::optional<int>,
                std::optional<int>,
                std::optional<int>>(),
            py::arg("bit_pattern_sequence"),
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("frac_bits") = std::nullopt
        )

        /*
         * Arithmetic operations
         */
        .def(py::self + py::self)
        .def(
            "__add__",
            [](const APyFixedArray& a, int b) {
                if (b == 0) {
                    return a;
                } else {
                    throw pybind11::type_error("Cannot add with int");
                };
            },
            py::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFixedArray& a, int b) {
                if (b == 0) {
                    return a;
                } else {
                    throw pybind11::type_error("Cannot add with int");
                };
            },
            py::is_operator()
        )
        .def(
            "__add__",
            [](const APyFixedArray& a, float b) {
                if (b == 0.) {
                    return a;
                } else {
                    throw pybind11::type_error("Cannot add with float");
                };
            },
            py::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFixedArray& a, float b) {
                if (b == 0.) {
                    return a;
                } else {
                    throw pybind11::type_error("Cannot add with float");
                };
            },
            py::is_operator()
        )
        .def(
            "__add__",
            [](const APyFixedArray& a, APyFixed& b) { return a + b; },
            py::is_operator()
        )
        .def(
            "__radd__",
            [](const APyFixedArray& a, APyFixed& b) { return a + b; },
            py::is_operator()
        )
        .def(py::self - py::self)
        .def(
            "__sub__",
            [](const APyFixedArray& a, int b) {
                if (b == 0) {
                    return a;
                } else {
                    throw pybind11::type_error("Cannot subtract with int");
                };
            },
            py::is_operator()
        )
        /* .def("__rsub__", [](const APyFixedArray& a, int b) {
            if (b == 0) {
                return -a;
            } else {
                throw pybind11::type_error("Cannot subtract with int");
            };
        }, py::is_operator())
         */
        .def(
            "__sub__",
            [](const APyFixedArray& a, float b) {
                if (b == 0.) {
                    return a;
                } else {
                    throw pybind11::type_error("Cannot subtract with float");
                };
            },
            py::is_operator()
        )
        /* .def("__rsub__", [](const APyFixedArray& a, float b) {
            if (b == 0.) {
                return -a;
            } else {
                throw pybind11::type_error("Cannot subtract with float");
            };
        }, py::is_operator())
         */
        .def(
            "__sub__",
            [](const APyFixedArray& a, APyFixed& b) { return a - b; },
            py::is_operator()
        )
        /* .def("__rsub__", [](const APyFixedArray& a, APyFixed& b) {
            return (-a) + b;
        }, py::is_operator())
         */
        .def(py::self * py::self)
        .def(
            "__mul__",
            [](const APyFixedArray& a, APyFixed& b) { return a * b; },
            py::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFixedArray& a, APyFixed& b) { return a * b; },
            py::is_operator()
        )
        .def(
            "__mul__",
            [](const APyFixedArray& a, int b) {
                if (b == 1) {
                    return a;
                } else {
                    throw pybind11::type_error("Cannot multiply with int");
                };
            },
            py::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFixedArray& a, int b) {
                if (b == 1) {
                    return a;
                } else {
                    throw pybind11::type_error("Cannot multiply with int");
                };
            },
            py::is_operator()
        )
        .def(
            "__mul__",
            [](const APyFixedArray& a, float b) {
                if (b == 1.) {
                    return a;
                } else {
                    throw pybind11::type_error("Cannot multiply with float");
                };
            },
            py::is_operator()
        )
        .def(
            "__rmul__",
            [](const APyFixedArray& a, float b) {
                if (b == 1.) {
                    return a;
                } else {
                    throw pybind11::type_error("Cannot multiply with float");
                };
            },
            py::is_operator()
        )
        /*
         * Methods
         */
        .def_property_readonly("bits", &APyFixedArray::bits, R"pbdoc(
            Total number of bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_property_readonly("int_bits", &APyFixedArray::int_bits, R"pbdoc(
            Number of integer bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_property_readonly("frac_bits", &APyFixedArray::frac_bits, R"pbdoc(
            Number of fractional bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_property_readonly("shape", &APyFixedArray::shape, R"pbdoc(
            The shape of the array.

            Returns
            -------
            :class:`tuple` of :class:`int`
            )pbdoc")

        .def_property_readonly("ndim", &APyFixedArray::ndim, R"pbdoc(
            Number of dimensions in the array.

            Returns
            -------
            :class:`int`
            )pbdoc")
        .def_property_readonly("T", &APyFixedArray::transpose, R"pbdoc(
            The transposition of the array.

            Equivalent to calling :func:`APyFixedArray.transpose`.

            Returns
            -------
            :class:`APyFixedArray`
            )pbdoc")

        .def("to_numpy", &APyFixedArray::to_numpy, R"pbdoc(
            Return array as a :class:`numpy.ndarray` of :class:`numpy.float64`.

            The returned array has the same `shape` and stored value as `self`. This
            method rounds away from infinity on ties.

            Returns
            -------
            :class:`numpy.ndarray`
            )pbdoc")

        .def("is_identical", &APyFixedArray::is_identical, py::arg("other"), R"pbdoc(
            Test if two :class:`APyFixedArray` objects are identical.

            Two :class:`APyFixedArray` objects are considered identical if, and only if:
              * They represent exatly the same tensor shape
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
            "resize",
            &APyFixedArray::resize,
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("quantization") = QuantizationMode::TRN,
            py::arg("overflow") = OverflowMode::WRAP,
            py::arg("frac_bits") = std::nullopt,
            R"pbdoc(
            .. deprecated:: 0.1.pre
               Use :func:`~APyFixedArray.cast` instead.
            )pbdoc"
        )
        .def(
            "cast",
            &APyFixedArray::cast,
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("quantization") = QuantizationMode::TRN,
            py::arg("overflow") = OverflowMode::WRAP,
            py::arg("frac_bits") = std::nullopt,
            R"pbdoc(
            Create a new resized fixed-point array based on the bit pattern in this
            fixed-point array.

            This is the primary method for performing quantization, truncation, overflowing,
            and saturation when dealing with APyTypes fixed-point numbers.

            Exactly two of three bit-specifiers (*bits*, *int_bits*, *frac_bits*) needs
            to be set.

            Parameters
            ----------
            bits : int, optional
                Total number of bits in the created fixed-point array.
            int_bits : int, optional
                Number of integer bits in the created fixed-point array-
            quantization : :class:`QuantizationMode`, default: :class:`QuantizationMode.TRN`
                Quantization mode to use in this cast.
            overflow : :class:`OverflowMode`, default: :class:`OverflowMode.WRAP`
                Overflowing mode to use in this cast.
            frac_bits : int, optional
                Number of fractional bits in the created fixed-point array.

            Returns
            -------
            :class:`APyFixedArray`
            )pbdoc"
        )

        /*
         * Static methods
         */
        .def_static(
            "from_float",
            &APyFixedArray::from_double,
            py::arg("float_sequence"),
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("frac_bits") = std::nullopt,
            R"pbdoc(
            Create an :class:`APyFixedArray` object from a sequence of :class:`float`.

            The initialized fixed-point values are the one closest to the
            input floating-point value, rounded away from zero on ties. Exactly two of
            the three bit-specifiers (`bits`, `int_bits`, `frac_bits`) has to be set.

            Parameters
            ----------
            float_sequence : sequence of float
                Floating point values to initialize from. The tensor shape will be taken
                from the sequence shape.
            bits : int, optional
                Total number of bits in the created fixed-point tensor
            int_bits : int, optional
                Number of integer bits in the created fixed-point tensor
            frac_bits : int, optional
                Number of fractional bits in the created fixed-point tensor

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

        /*
         * Dunder methods
         */
        .def("__lshift__", &APyFixedArray::operator<<, py::arg("shift_amnt"))
        .def("__matmul__", &APyFixedArray::matmul, py::arg("rhs"))
        .def("__repr__", &APyFixedArray::repr)
        .def("__rshift__", &APyFixedArray::operator>>, py::arg("shift_amnt"))

        // Iteration and friends
        .def("__getitem__", &APyFixedArray::get_item, py::arg("idx"))
        .def("__len__", &APyFixedArray::size)
        .def(
            "__iter__",
            [](py::object array) {
                return APyFixedArrayIterator(array.cast<const APyFixedArray&>(), array);
            }
        )
        .def("__array__", &APyFixedArray::to_numpy)

        ;

    py::class_<APyFixedArrayIterator>(m, "APyFixedArrayIterator")
        .def(
            "__iter__",
            [](APyFixedArrayIterator& it) -> APyFixedArrayIterator& { return it; }
        )
        .def("__next__", &APyFixedArrayIterator::next);
}
