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
        .def(py::self - py::self)
        .def(py::self * py::self)

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
            Return the transposed version of the array.

            If the dimension of `self` is one, this method returns the a copy of `self`.
            If the dimension of `self` is two, this method returns the matrix
            transposition of `self`.

            Higher order transposition has not been implemented and will raise a
            `NotImplementedException`.

            Returns
            -------
            :class:`APyFixedArray`
            )pbdoc")
        .def_property_readonly("T", &APyFixedArray::transpose)

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
            Create an :class:`APyFixedArray` object and initialize its value from a
            a sequence of :class:`float`.

            The initialized fixed-point values are the one closest to the
            input floating-point value, rounded away from zero on ties. Exactly two of
            the three bit-specifiers (`bits`, `int_bits`, `frac_bits`) has to be set.

            Examples
            --------

            .. code-block:: python

                from apytypes import APyFixedArray

                # Tensor `a` from float, initialized from some floating-point values.
                # The last value `1.49` is rounded to its closes fixed-point
                # representable value `1.50`.
                a = APyFixedArray.from_float([1.0, 1.25, 1.49], int_bits=2, frac_bits=2)

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
            )pbdoc"
        )

        /*
         * Dunder methods
         */
        .def("__repr__", &APyFixedArray::repr)
        .def("__matmul__", &APyFixedArray::matmul, py::arg("rhs"))
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
