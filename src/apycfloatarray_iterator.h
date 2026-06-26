#ifndef _APYCFLOAT_ARRAY_ITERATOR_H
#define _APYCFLOAT_ARRAY_ITERATOR_H

#include "apycfloat.h"
#include "apycfloatarray.h"

#include <nanobind/nanobind.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)

struct APyCFloatArrayIterator {
public:
    APyCFloatArrayIterator(const APyCFloatArray& array, nanobind::object ref);

    std::variant<APyCFloatArray, APyCFloat> next();
    const APyCFloatArray& array;

private:
    nanobind::object ref; // keep a reference
    std::size_t index;
};
#endif
