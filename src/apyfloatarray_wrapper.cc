#include "apyfloatarray.h"
// #include "apyfloatarray_iterator.h"
#include "apytypes_common.h"
#include "apytypes_util.h"

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <optional> // std::optional, std::nullopt

namespace py = pybind11;

void bind_float_array(py::module& m)
{
    py::class_<APyFloatArray>(m, "APyFloatArray")

        /*
         * Constructor: construct from a list of Python integers
         */
        .def(
            py::init<
                const py::sequence&,
                const py::sequence&,
                const py::sequence&,
                std::uint8_t,
                std::uint8_t,
                std::optional<int>>(),
            py::arg("signs"),
            py::arg("exps"),
            py::arg("mans"),
            py::arg("exp_bits"),
            py::arg("man_bits"),
            py::arg("bias") = std::nullopt
        )

        /*
         * Methods
         */
        .def_property_readonly("exp_bits", &APyFloatArray::get_exp_bits, R"pbdoc(
            Number of exponent bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_property_readonly("man_bits", &APyFloatArray::get_man_bits, R"pbdoc(
            Number of mantissa bits.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_property_readonly("bias", &APyFloatArray::get_bias, R"pbdoc(
            Bias.

            Returns
            -------
            :class:`int`
            )pbdoc")

        .def_property_readonly("shape", &APyFloatArray::get_shape, R"pbdoc(
            The shape of the array.

            Returns
            -------
            :class:`tuple` of :class:`int`
            )pbdoc")

        .def_property_readonly("ndim", &APyFloatArray::get_ndim, R"pbdoc(
            Number of dimensions in the array.

            Returns
            -------
            :class:`int`
            )pbdoc")

        /*
         * Dunder methods
         */
        .def("__repr__", &APyFloatArray::repr)
        .def("__len__", &APyFloatArray::get_size);
}
