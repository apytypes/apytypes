#ifndef _APYCFIXED_ARRAY_ITERATOR_H
#define _APYCFIXED_ARRAY_ITERATOR_H

#include "apycfixed.h"
#include "apycfixedarray.h"

#include <nanobind/nanobind.h>
#include <nanobind/stl/variant.h>

struct APyCFixedArrayIterator {
public:
    APyCFixedArrayIterator(const APyCFixedArray& array, nanobind::object ref);

    std::variant<APyCFixedArray, APyCFixed> next();
    const APyCFixedArray& array;

private:
    nanobind::object ref; // keep a reference
    std::size_t index;
};
#endif
