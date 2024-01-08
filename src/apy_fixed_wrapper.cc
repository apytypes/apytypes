#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "apy_fixed.h"
#include "apy_util.h"

namespace py = pybind11;

void bind_fixed(py::module &m) {

    
    py::class_<APyFixed>(m, "APyFixed")
        
        /*
         * Constructors
         */

        .def(
            py::init<int, int>(),
            py::arg("bits"),
            py::arg("int_bits")
        )
        //.def(
        //    py::init<int, int, py::int_>(),
        //    py::arg("bits"),
        //    py::arg("int_bits"),
        //    py::arg("obj")
        //)
        .def(
            py::init<int, int, double>(),
            py::arg("bits"),
            py::arg("int_bits"),
            py::arg("value")
        )
        .def(
            py::init<int, int, const char *, int>(),
            py::arg("bits"),
            py::arg("int_bits"),
            py::arg("value"),
            py::arg("base") = 10
        )
        .def(
            py::init<int, int, const APyFixed &>(),
            py::arg("bits"),
            py::arg("int_bits"),
            py::arg("other")
        )
        //.def(
        //  py::init<int, int, PyLong_Type>(),
        //  //py::init<int, int, PyLong_Type>(),
        //  py::arg("bits"),
        //  py::arg("int_bits"),
        //  py::arg("value")
        ///

        /*
         * Arithmetic operators
         */
        // .def(py::self == py::self)
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def("assign", &APyFixed::operator=)
        .def(-py::self)

        /*
         * Methods
         */
        .def("bits", &APyFixed::bits)
        .def("int_bits", &APyFixed::int_bits)
        .def("frac_bits", &APyFixed::frac_bits)
        .def("vector_size", &APyFixed::vector_size)
        .def("is_negative", &APyFixed::is_negative)
        .def("increment_lsb", &APyFixed::increment_lsb)
        .def("to_string", &APyFixed::to_string, py::arg("base") = 10)
        .def("__str__", &APyFixed::to_string, py::arg("base") = 10)
        .def("to_string_hex", &APyFixed::to_string_hex)
        .def("to_string_oct", &APyFixed::to_string_oct)
        .def("to_string_dec", &APyFixed::to_string_dec)
        .def("__repr__", &APyFixed::repr)
        ;
    
}
