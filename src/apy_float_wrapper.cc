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
            .def(py::self - py::self)
            .def(py::self * py::self)
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def(py::self < py::self)
            .def(py::self > py::self)
            .def(py::self <= py::self)
            .def(py::self >= py::self)
            .def("__float__", &APyFloat::operator double)

            // Non-computational operations
            .def("is_normal", &APyFloat::is_normal)
            .def("is_subnormal", &APyFloat::is_subnormal)
            .def("is_zero", &APyFloat::is_zero)
            .def("is_finite", &APyFloat::is_finite)
            .def("is_nan", &APyFloat::is_nan)
            .def("is_inf", &APyFloat::is_inf)
            .def("is_sign_neg", &APyFloat::is_sign_neg)

            // Getters
            .def("get_sign", &APyFloat::get_sign)
            .def("get_man", &APyFloat::get_man)
            .def("get_exp", &APyFloat::get_exp)
            ;
}
