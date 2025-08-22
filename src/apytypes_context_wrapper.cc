#include "apytypes_common.h"
#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/optional.h>

namespace nb = nanobind;

void bind_context_manager(nb::module_& m)
{
    nb::class_<ContextManager>(m, "ContextManager");
}

void context_enter_handler(ContextManager& cm) { cm.enter_context(); }

void context_exit_handler(
    ContextManager& cm,
    std::optional<nanobind::object> exc_type,
    std::optional<nanobind::object> exc_value,
    std::optional<nanobind::object> traceback
)
{
    (void)exc_type;
    (void)exc_value;
    (void)traceback;
    cm.exit_context();
}

void bind_quantization_context(nb::module_& m)
{
    nb::class_<APyFloatQuantizationContext, ContextManager>(
        m, "APyFloatQuantizationContext"
    )
        .def(
            nb::init<QuantizationMode, std::optional<std::uint64_t>>(),
            nb::arg("quantization"),
            nb::arg("seed") = nb::none()
        )

        .def("__enter__", &context_enter_handler)
        .def(
            "__exit__",
            &context_exit_handler,
            nb::arg("exc_type") = nb::none(),
            nb::arg("exc_value") = nb::none(),
            nb::arg("traceback") = nb::none()
        );
}

void bind_cast_context(nb::module_& m)
{
    nb::class_<APyFixedCastContext, ContextManager>(m, "APyFixedCastContext")
        .def(
            nb::init<std::optional<QuantizationMode>, std::optional<OverflowMode>>(),
            nb::arg("quantization") = nb::none(),
            nb::arg("overflow") = nb::none()
        )
        .def("__enter__", &context_enter_handler)
        .def(
            "__exit__",
            &context_exit_handler,
            nb::arg("exc_type") = nb::none(),
            nb::arg("exc_value") = nb::none(),
            nb::arg("traceback") = nb::none()
        );
}

void bind_accumulator_context(nb::module_& m)
{
    nb::class_<APyFixedAccumulatorContext, ContextManager>(
        m, "APyFixedAccumulatorContext"
    )
        .def(
            nb::init<
                std::optional<int>,
                std::optional<int>,
                std::optional<QuantizationMode>,
                std::optional<OverflowMode>,
                std::optional<int>>(),
            nb::arg("int_bits") = nb::none(),
            nb::arg("frac_bits") = nb::none(),
            nb::arg("quantization") = nb::none(),
            nb::arg("overflow") = nb::none(),
            nb::arg("bits") = nb::none()
        )
        .def("__enter__", &context_enter_handler)
        .def(
            "__exit__",
            &context_exit_handler,
            nb::arg("exc_type") = nb::none(),
            nb::arg("exc_value") = nb::none(),
            nb::arg("traceback") = nb::none()
        );

    nb::class_<APyFloatAccumulatorContext, ContextManager>(
        m, "APyFloatAccumulatorContext"
    )
        .def(
            nb::init<
                std::optional<int>,
                std::optional<int>,
                std::optional<exp_t>,
                std::optional<QuantizationMode>>(),
            nb::arg("exp_bits") = nb::none(),
            nb::arg("man_bits") = nb::none(),
            nb::arg("bias") = nb::none(),
            nb::arg("quantization") = nb::none()
        )
        .def("__enter__", &context_enter_handler)
        .def(
            "__exit__",
            &context_exit_handler,
            nb::arg("exc_type") = nb::none(),
            nb::arg("exc_value") = nb::none(),
            nb::arg("traceback") = nb::none()
        );
}
