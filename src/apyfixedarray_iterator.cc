#include "apyfixedarray_iterator.h"
#include "apyfixedarray.h"
#include <nanobind/nanobind.h>

namespace nb = nanobind;

APyFixedArrayIterator::APyFixedArrayIterator(const APyFixedArray& array, nb::object ref)
    : array(array)
    , ref(ref)
    , index(0)
{
}

APyFixedArray APyFixedArrayIterator::next()
{
    if (index == array.size())
        throw nb::stop_iteration();
    return array.get_item(index++);
}
