#include "apyfloatarray_iterator.h"
#include "apyfloatarray.h"

#include <nanobind/nanobind.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)

namespace nb = nanobind;

APyFloatArrayIterator::APyFloatArrayIterator(const APyFloatArray& array, nb::object ref)
    : array(array)
    , ref(ref)
    , index(0)
{
}

std::variant<APyFloatArray, APyFloat> APyFloatArrayIterator::next()
{
    if (index == array.size())
        throw nb::stop_iteration();
    return array.get_item_integer(index++);
}
