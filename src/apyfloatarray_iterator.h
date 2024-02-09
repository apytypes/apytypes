#include "apyfloatarray.h"
#include <pybind11/pybind11.h>

#ifndef _APYFLOAT_ARRAY_ITERATOR_H
#define _APYFLOAT_ARRAY_ITERATOR_H
struct APyFloatArrayIterator {
public:
    APyFloatArrayIterator(const APyFloatArray& array, pybind11::object ref);

    APyFloatArray next();
    const APyFloatArray& array;

private:
    pybind11::object ref; // keep a reference
    size_t index;
};
#endif
