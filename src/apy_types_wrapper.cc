#include <pybind11/pybind11.h>
#include "apy_common.h"

namespace py = pybind11;

void bind_common(py::module &m);
void bind_fixed(py::module &m);
void bind_float(py::module &m);
void bind_float_context(py::module &m);

PYBIND11_MODULE(apy_types, m) {
    bind_common(m);
    bind_fixed(m);
    bind_float(m);
    bind_float_context(m);
}

void bind_common(py::module &m) {
    py::enum_<RoundingMode>(m, "RoundingMode")
        .value("TO_POSITIVE", RoundingMode::TO_POSITIVE)
        .value("TO_NEGATIVE", RoundingMode::TO_NEGATIVE)
        .value("TO_ZERO", RoundingMode::TO_ZERO)
        .value("TIES_TO_EVEN", RoundingMode::TIES_TO_EVEN)
        .value("TIES_TO_AWAY", RoundingMode::TIES_TO_AWAY)
        .value("JAMMING", RoundingMode::JAMMING);
    
    m.def("set_rounding_mode", &set_rounding_mode);
    m.def("get_rounding_mode", &get_rounding_mode);

    // APyTypes version number
    m.attr("__version__") = "0.0.1";
}
