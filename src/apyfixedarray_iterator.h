#ifndef _APYFIXED_ARRAY_ITERATOR_H
#define _APYFIXED_ARRAY_ITERATOR_H

#include "apyfixed.h"
#include "apyfixedarray.h"

#include <nanobind/nanobind.h>
#include <nanobind/stl/variant.h>

struct APyFixedArrayIterator {
public:
    APyFixedArrayIterator(const APyFixedArray& array, nanobind::object ref);

    std::variant<APyFixedArray, APyFixed> next();
    const APyFixedArray& array;

private:
    nanobind::object ref; // keep a reference
    std::size_t index;
};
#endif
