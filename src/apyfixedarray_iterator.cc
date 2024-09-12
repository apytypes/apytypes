#include "apyfixedarray_iterator.h"
#include "apyfixed.h"
#include "apyfixedarray.h"

#include <nanobind/nanobind.h>
#include <nanobind/stl/variant.h>

namespace nb = nanobind;

APyFixedArrayIterator::APyFixedArrayIterator(const APyFixedArray& array, nb::object ref)
    : array(array)
    , ref(ref)
    , index(0)
{
}

std::variant<APyFixedArray, APyFixed> APyFixedArrayIterator::next()
{
    if (index == array.size())
        throw nb::stop_iteration();
    return array.get_item_integer(index++);
}
