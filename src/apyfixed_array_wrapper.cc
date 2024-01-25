#include "apyfixed_array.h"
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
         * Dunder methods
         */
        .def("__repr__", &APyFixedArray::repr)

        ;
}
