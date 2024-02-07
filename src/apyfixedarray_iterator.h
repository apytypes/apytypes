#include "apyfixedarray.h"
#include <pybind11/pybind11.h>

#ifndef _APYFIXED_ARRAY_ITERATOR_H
#define _APYFIXED_ARRAY_ITERATOR_H
struct APyFixedArrayIterator {
public:
    APyFixedArrayIterator(const APyFixedArray& array, pybind11::object ref);

    APyFixedArray next();
    const APyFixedArray& array;

private:
    pybind11::object ref; // keep a reference
    size_t index;
};
#endif
