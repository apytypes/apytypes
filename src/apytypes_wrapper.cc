#include "apytypes_common.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

void bind_accumulator_context(py::module& m);
void bind_common(py::module& m);
void bind_context_manager(py::module& m);
void bind_fixed(py::module& m);
void bind_fixed_array(py::module& m);
void bind_float(py::module& m);
void bind_float_array(py::module& m);
void bind_float_context(py::module& m);

PYBIND11_MODULE(_apytypes, m)
{
    bind_common(m);
    bind_fixed(m);
    bind_fixed_array(m);
    bind_float(m);
    bind_float_array(m);
    bind_context_manager(m);
    bind_float_context(m);
    bind_accumulator_context(m);
}

void bind_common(py::module& m)
{
    py::enum_<QuantizationMode>(m, "QuantizationMode")
        /*
         * Base quantization modes
         */
        .value("TRN", QuantizationMode::TRN)
        .value("TRN_ZERO", QuantizationMode::TRN_ZERO)
        .value("TRN_INF", QuantizationMode::TRN_INF)
        .value("RND", QuantizationMode::RND)
        .value("RND_ZERO", QuantizationMode::RND_ZERO)
        .value("RND_INF", QuantizationMode::RND_INF)
        .value("RND_MIN_INF", QuantizationMode::RND_MIN_INF)
        .value("RND_CONV", QuantizationMode::RND_CONV)
        .value("RND_CONV_ODD", QuantizationMode::RND_CONV_ODD)
        .value("JAM", QuantizationMode::JAM)
        .value("JAM_UNBIASED", QuantizationMode::JAM_UNBIASED)
        .value("STOCHASTIC_WEIGHTED", QuantizationMode::STOCHASTIC_WEIGHTED)
        .value("STOCHASTIC_EQUAL", QuantizationMode::STOCHASTIC_EQUAL)

        /*
         * Floating point aliases
         */
        .value("TO_NEG", QuantizationMode::TRN)
        .value("TO_ZERO", QuantizationMode::TRN_ZERO)
        .value("TO_POS", QuantizationMode::TRN_INF)
        .value("TIES_ZERO", QuantizationMode::RND_ZERO)
        .value("TIES_AWAY", QuantizationMode::RND_INF)
        .value("TIES_EVEN", QuantizationMode::RND_CONV)
        .value("TIES_ODD", QuantizationMode::RND_CONV_ODD)
        .value("TIES_NEG", QuantizationMode::RND_MIN_INF)
        .value("TIES_POS", QuantizationMode::RND);

    py::enum_<OverflowMode>(m, "OverflowMode")
        .value("WRAP", OverflowMode::WRAP)
        .value("SAT", OverflowMode::SAT)
        .value("NUMERIC_STD", OverflowMode::NUMERIC_STD);

    m.def("set_quantization_mode", &set_quantization_mode);
    m.def("get_quantization_mode", &get_quantization_mode);
    m.def("set_quantization_seed", &set_quantization_seed);
    m.def("get_quantization_seed", &get_quantization_seed);
}
