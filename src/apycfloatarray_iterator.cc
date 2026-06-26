#include "apycfloatarray_iterator.h"
#include "apycfloatarray.h"

#include <nanobind/nanobind.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)

namespace nb = nanobind;

APyCFloatArrayIterator::APyCFloatArrayIterator(
    const APyCFloatArray& array, nb::object ref
)
    : array(array)
    , ref(ref)
    , index(0)
{
}

std::variant<APyCFloatArray, APyCFloat> APyCFloatArrayIterator::next()
{
    if (index == array.size())
        throw nb::stop_iteration();
    return array.get_item_integer(index++);
}
