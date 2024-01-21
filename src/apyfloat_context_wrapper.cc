#include "apyfloat_contextmanager.h"
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
        .def(
            py::init<
                RoundingMode, std::optional<std::uint64_t>>(), 
            py::arg("rounding_mode"),
            py::arg("rounding_seed") = std::nullopt
        )

        .def("__enter__", &context_enter_handler)
        .def("__exit__", &context_exit_handler);
}
