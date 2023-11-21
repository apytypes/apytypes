#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "apy_fixed.h"
#include "apy_util.h"

namespace py = pybind11;

void bind_fixed(py::module &m) {
   py::class_<APyFixed>(m, "APyFixed")
        .def(py::init<int, int>(), py::arg("bits"), py::arg("int_bits"))
        .def(py::init<int, int, int>(), py::arg("bits"), py::arg("int_bits"), py::arg("value"))
        .def("bits", &APyFixed::bits)
        .def("int_bits", &APyFixed::int_bits)
        .def("vector_size", &APyFixed::vector_size)
        .def("is_negative", &APyFixed::is_negative)
        .def("increment_lsb", &APyFixed::increment_lsb)
        .def("to_string", &APyFixed::to_string)
        .def("to_string_hex", &APyFixed::to_string_hex)
        .def("to_string_oct", &APyFixed::to_string_oct)
        .def("to_string_dec", &APyFixed::to_string_dec)
        .def("__repr__", &APyFixed::repr)
        .def(py::self == py::self)
        // .def(py::self + py::self)
        // .def(py::self - py::self)
        // .def(py::self * py::self)
        // .def(py::self / py::self)
        .def(-py::self)
        ;

    py::enum_<STRING_TYPE>(m, "STRING_TYPE")
        .value("HEX", STRING_TYPE::HEX)
        .value("OCT", STRING_TYPE::OCT)
        .value("DEC", STRING_TYPE::DEC);
}
