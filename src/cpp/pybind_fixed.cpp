#include <pybind11/pybind11.h>
#include "apy_fixed_point.hpp"

namespace py = pybind11;

PYBIND11_MODULE(apy_types, m) {
    py::class_<aPyFixed>(m, "aPyFixed")
        .def(py::init<std::uint8_t, std::uint8_t, double>(),
        py::arg("total_bits"), py::arg("integer_bits")=1, py::arg("value")=0.0)
        .def("__repr__", &aPyFixed::to_string)
        .def("__add__", &aPyFixed::operator+)
        .def("__iadd__", &aPyFixed::operator+=)
        .def("__sub__", &aPyFixed::operator-)
        .def("__isub__", &aPyFixed::operator-=)
        .def("__mul__", &aPyFixed::operator*)
        .def("__imul__", &aPyFixed::operator*=)
        .def("__truediv__", &aPyFixed::operator/)
        .def("__itruediv__", &aPyFixed::operator/=)
        .def("__float__", &aPyFixed::operator double)
        ;

}