#ifndef _APYCFLOATARRAY_H
#define _APYCFLOATARRAY_H

#include "apyarray.h"
#include "apytypes_fwd.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h>    // nanobind::object
#include <nanobind/ndarray.h>     // nanobind::ndarray
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)

class APyCFloatArray : public APyArray<APyFloatData, APyCFloatArray> { };

#endif
