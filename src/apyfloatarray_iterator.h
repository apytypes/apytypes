#include "apyfloatarray.h"
#include <nanobind/nanobind.h>

#ifndef _APYFLOAT_ARRAY_ITERATOR_H
#define _APYFLOAT_ARRAY_ITERATOR_H
struct APyFloatArrayIterator {
public:
    APyFloatArrayIterator(const APyFloatArray& array, nanobind::object ref);

    APyFloatArray next();
    const APyFloatArray& array;

private:
    nanobind::object ref; // keep a reference
    size_t index;
};
#endif
