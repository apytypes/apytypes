#include <pybind11/pybind11.h>

namespace py = pybind11;

void bind_fixed(py::module &m);
void bind_float(py::module &m);

PYBIND11_MODULE(apy_types, m) {
    bind_fixed(m);
    bind_float(m);
}