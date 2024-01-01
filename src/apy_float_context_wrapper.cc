#include <optional>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "apy_float_contextmanager.h"

namespace py = pybind11;

void bind_float_context(py::module &m) {
    py::class_<RoundingContext>(m, "RoundingContext")
            .def(py::init<RoundingMode>(),
            py::arg("rounding_mode"))
            .def("__enter__", [] (RoundingContext &rc) { rc.enter_context(); })
            .def("__exit__",
            [] (RoundingContext& rc,
                const std::optional<pybind11::type> &exc_type,
                const std::optional<pybind11::object> &exc_value,
                const std::optional<pybind11::object> &traceback)
            { 
                    rc.exit_context(); 
            })
            ;

    m.def("set_rounding_mode", &set_rounding_mode);
    m.def("get_rounding_mode", &get_rounding_mode);
}