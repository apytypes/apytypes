#include "apyfloatarray_iterator.h"
#include "apyfloatarray.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

APyFloatArrayIterator::APyFloatArrayIterator(const APyFloatArray& array, py::object ref)
    : array(array)
    , ref(ref)
    , index(0)
{
}

APyFloatArray APyFloatArrayIterator::next()
{
    if (index == array.get_size())
        throw py::stop_iteration();
    return array.get_item(index++);
}
