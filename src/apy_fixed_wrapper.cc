#include "apy_fixed.h"
#include "apy_util.h"
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <optional> // std::optional, std::nullopt

namespace py = pybind11;

void bind_fixed(py::module& m)
{

    py::class_<APyFixed>(m, "APyFixed")

        // Constructor: construct from another APyFixed number
        .def(
            py::init<
                const APyFixed&,
                std::optional<int>,
                std::optional<int>,
                std::optional<int>>(),
            py::arg("other"),
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("frac_bits") = std::nullopt
        )

        // Constructor: construct from a Python arbitrary long integer object
        .def(
            py::init<
                py::int_,
                std::optional<int>,
                std::optional<int>,
                std::optional<int>>(),
            py::arg("int_bit_pattern"),
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("frac_bits") = std::nullopt
        )

        /*
         * Arithmetic operators
         */
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self < py::self)
        .def(py::self <= py::self)
        .def(py::self > py::self)
        .def(py::self >= py::self)

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
        .def(
            "from_string", &APyFixed::from_string, py::arg("str"), py::arg("base") = 10
        )
        .def("__str__", &APyFixed::to_string, py::arg("base") = 10)
        .def("to_string_hex", &APyFixed::to_string_hex)
        .def("to_string_oct", &APyFixed::to_string_oct)
        .def("to_string_dec", &APyFixed::to_string_dec)
        .def("__repr__", &APyFixed::repr);
}
