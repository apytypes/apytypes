#include "apy_float_contextmanager.h"
#include <optional>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void context_enter_handler(ContextManager& cm) { cm.enter_context(); }

void context_exit_handler(
    ContextManager& cm,
    const std::optional<pybind11::type>& exc_type,
    const std::optional<pybind11::object>& exc_value,
    const std::optional<pybind11::object>& traceback
)
{
    cm.exit_context();
}

void bind_float_context(py::module& m)
{
    py::class_<ContextManager>(m, "ContextManager");
    py::class_<RoundingContext, ContextManager>(m, "RoundingContext")
        .def(py::init<RoundingMode>(), py::arg("rounding_mode"))
        .def("__enter__", &context_enter_handler)
        .def("__exit__", &context_exit_handler);

    m.def("set_rounding_mode", &set_rounding_mode);
    m.def("get_rounding_mode", &get_rounding_mode);
}
