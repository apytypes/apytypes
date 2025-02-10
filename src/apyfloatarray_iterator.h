#ifndef _APYFLOAT_ARRAY_ITERATOR_H
#define _APYFLOAT_ARRAY_ITERATOR_H

#include "apyfloat.h"
#include "apyfloatarray.h"

#include <nanobind/nanobind.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)

struct APyFloatArrayIterator {
public:
    APyFloatArrayIterator(const APyFloatArray& array, nanobind::object ref);

    std::variant<APyFloatArray, APyFloat> next();
    const APyFloatArray& array;

private:
    nanobind::object ref; // keep a reference
    std::size_t index;
};
#endif
