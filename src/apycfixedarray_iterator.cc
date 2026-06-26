#include "apycfixedarray_iterator.h"
#include "apycfixed.h"
#include "apycfixedarray.h"

#include <nanobind/nanobind.h>
#include <nanobind/stl/variant.h>

namespace nb = nanobind;

APyCFixedArrayIterator::APyCFixedArrayIterator(
    const APyCFixedArray& array, nb::object ref
)
    : array(array)
    , ref(ref)
    , index(0)
{
}

std::variant<APyCFixedArray, APyCFixed> APyCFixedArrayIterator::next()
{
    if (index == array.size())
        throw nb::stop_iteration();
    return array.get_item_integer(index++);
}
