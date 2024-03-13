#include "apytypes_common.h"
#include <nanobind/nanobind.h>
#include <optional>

namespace nb = nanobind;

void bind_context_manager(nb::module_& m)
{
    nb::class_<ContextManager>(m, "ContextManager");
}

void context_enter_handler(ContextManager& cm) { cm.enter_context(); }

void context_exit_handler(
    ContextManager& cm,
    // const std::optional<nanobind::type>& exc_type,
    const std::optional<nanobind::object>& exc_value,
    const std::optional<nanobind::object>& traceback
)
{
    cm.exit_context();
}

void bind_float_context(nb::module_& m)
{
    nb::class_<QuantizationContext, ContextManager>(m, "QuantizationContext")
        .def(
            nb::init<QuantizationMode, std::optional<std::uint64_t>>(),
            nb::arg("quantization"),
            nb::arg("quantization_seed") = std::nullopt
        )

        .def("__enter__", &context_enter_handler)
        .def("__exit__", &context_exit_handler);
}

void bind_accumulator_context(nb::module_& m)
{
    nb::class_<AccumulatorContext, ContextManager>(m, "AccumulatorContext")
        .def(
            nb::init<
                std::optional<int>,
                std::optional<int>,
                std::optional<int>,
                std::optional<QuantizationMode>,
                std::optional<OverflowMode>,
                std::optional<std::uint8_t>,
                std::optional<std::uint8_t>,
                std::optional<exp_t>>(),
            // nb::kw_only() is added in NanoBind v2.0.0
            // nb::kw_only(), // All parameters are keyword only
            nb::arg("bits") = std::nullopt,
            nb::arg("int_bits") = std::nullopt,
            nb::arg("frac_bits") = std::nullopt,
            nb::arg("quantization") = std::nullopt,
            nb::arg("overflow") = std::nullopt,
            nb::arg("exp_bits") = std::nullopt,
            nb::arg("man_bits") = std::nullopt,
            nb::arg("bias") = std::nullopt
        )
        .def("__enter__", &context_enter_handler)
        .def("__exit__", &context_exit_handler);
}
