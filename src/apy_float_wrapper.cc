#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "apy_float.h"

namespace py = pybind11;

void bind_float(py::module &m) {
    py::class_<APyFloat>(m, "APyFloat")
            .def(py::init<std::uint8_t, std::uint8_t, double>(),
            py::arg("exp_bits"), py::arg("man_bits"), py::arg("value")=0.0)
            .def("__repr__", &APyFloat::repr)
            .def(py::self + py::self)
            .def(-py::self)
            // .def(py::self - py::self)
            // .def(py::self * py::self)
            // .def(py::self / py::self)
            .def(py::self == py::self)
            .def("__float__", &APyFloat::operator double)
            ;
}
