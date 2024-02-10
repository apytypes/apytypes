#include "apytypes_common.h"
#include <optional>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void bind_context_manager(py::module& m)
{
    py::class_<ContextManager>(m, "ContextManager");
}

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
    py::class_<RoundingContext, ContextManager>(m, "RoundingContext")
        .def(
            py::init<RoundingMode, std::optional<std::uint64_t>>(),
            py::arg("rounding_mode"),
            py::arg("rounding_seed") = std::nullopt
        )

        .def("__enter__", &context_enter_handler)
        .def("__exit__", &context_exit_handler);
}

void bind_accumulator_context(py::module& m)
{
    py::class_<AccumulatorContext, ContextManager>(m, "AccumulatorContext")
        .def(
            py::init<
                std::optional<int>,
                std::optional<int>,
                std::optional<int>,
                std::optional<RoundingMode>,
                std::optional<OverflowMode>>(),
            py::arg("bits") = std::nullopt,
            py::arg("int_bits") = std::nullopt,
            py::arg("frac_bits") = std::nullopt,
            py::arg("rounding") = std::nullopt,
            py::arg("overflow") = std::nullopt
        )
        .def("__enter__", &context_enter_handler)
        .def("__exit__", &context_exit_handler);
}
