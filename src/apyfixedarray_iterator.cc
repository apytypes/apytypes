#include "apyfixedarray_iterator.h"
#include "apyfixedarray.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

APyFixedArrayIterator::APyFixedArrayIterator(const APyFixedArray& array, py::object ref)
    : array(array)
    , ref(ref)
    , index(0)
{
}

APyFixedArray APyFixedArrayIterator::next()
{
    if (index == array.size())
        throw py::stop_iteration();
    return array.get_item(index++);
}
