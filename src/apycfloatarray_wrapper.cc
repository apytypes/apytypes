#include "apycfloatarray.h"

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/optional.h>

namespace nb = nanobind;

void bind_cfloat_array(nb::module_& m)
{
    nb::class_<APyCFloatArray>(m, "APyCFloatArray")

        ;
}
