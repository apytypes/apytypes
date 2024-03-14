#include "apyfloatarray_iterator.h"
#include "apyfloatarray.h"
#include <nanobind/nanobind.h>

namespace nb = nanobind;

APyFloatArrayIterator::APyFloatArrayIterator(const APyFloatArray& array, nb::object ref)
    : array(array)
    , ref(ref)
    , index(0)
{
}

APyFloatArray APyFloatArrayIterator::next()
{
    if (index == array.get_size())
        throw nb::stop_iteration();
    return array.get_item(index++);
}
