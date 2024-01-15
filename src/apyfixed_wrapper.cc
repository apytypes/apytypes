#include "apyfixed.h"
#include "apytypes_util.h"

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <optional> // std::optional, std::nullopt

namespace py = pybind11;

void bind_fixed(py::module& m)
{
    py::class_<APyFixed>(m, "APyFixed")

        /*
         * Constructor: construct from another APyFixed number
         */
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

        /*
         * Constructor: construct from a Python arbitrary long integer object
         */
        .def(
            py::init<
                py::int_,
                std::optional<int>,
                std::optional<int>,
                std::optional<int>>(),
            py::arg("bit_pattern"),
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
        .def("bit_pattern_to_dec_string", &APyFixed::bit_pattern_to_dec_string)
        .def("bits", &APyFixed::bits)
        .def("frac_bits", &APyFixed::frac_bits)
        .def(
            "from_string", &APyFixed::from_string, py::arg("str"), py::arg("base") = 10
        )
        .def("from_float", &APyFixed::from_double)
        .def("increment_lsb", &APyFixed::increment_lsb)
        .def("int_bits", &APyFixed::int_bits)
        .def("is_negative", &APyFixed::is_negative)
        .def("is_zero", &APyFixed::is_zero)
        .def("to_string", &APyFixed::to_string, py::arg("base") = 10)
        .def("to_string_dec", &APyFixed::to_string_dec)
        .def("to_string_hex", &APyFixed::to_string_hex)
        .def("to_string_oct", &APyFixed::to_string_oct)
        .def("vector_size", &APyFixed::vector_size)

        /*
         * Dunder methods
         */
        .def("__abs__", &APyFixed::abs)
        .def("__float__", &APyFixed::to_double)
        .def("__neg__", [](APyFixed& fix) { return -fix; })
        .def("__repr__", &APyFixed::repr)
        .def("__str__", &APyFixed::to_string, py::arg("base") = 10)
        .def("__lshift__", &APyFixed::operator<<, py::arg("shift_amnt"))
        .def("__rshift__", &APyFixed::operator>>, py::arg("shift_amnt"))

        /*
         * Static methods
         */
        //.def_static("float_float", &APyFixed::from_double)
        ;
}
