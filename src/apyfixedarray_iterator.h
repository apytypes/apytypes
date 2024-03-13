#include "apyfixedarray.h"
#include <nanobind/nanobind.h>

#ifndef _APYFIXED_ARRAY_ITERATOR_H
#define _APYFIXED_ARRAY_ITERATOR_H
struct APyFixedArrayIterator {
public:
    APyFixedArrayIterator(const APyFixedArray& array, nanobind::object ref);

    APyFixedArray next();
    const APyFixedArray& array;

private:
    nanobind::object ref; // keep a reference
    size_t index;
};
#endif
