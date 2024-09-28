/*
 * APyTypes array base class, for sharing functionality between array types. All the
 * member functions of the APyTypes array types I don't want to write twice...
 */

#ifndef _APYARRAY_H
#define _APYARRAY_H

#include "apybuffer.h"
#include "apytypes_util.h"
#include "array_utils.h"
#include "broadcast.h"

// Python object access through Nanobind
#include <nanobind/nanobind.h>
#include <nanobind/stl/variant.h> // std::variant (with nanobind support)
namespace nb = nanobind;

#include <iterator> // std::begin
#include <set>      // std::set
#include <variant>  // std::variant

template <typename T, typename ARRAY_TYPE> class APyArray : public APyBuffer<T> {

public:
    //! Base constructor for creating a new `APyArray`
    APyArray(const std::vector<std::size_t>& shape, std::size_t itemsize = 1)
        : APyBuffer<T>(shape, itemsize)
    {
    }

    using APyBuffer<T>::_shape;
    using APyBuffer<T>::_ndim;
    using APyBuffer<T>::_data;
    using APyBuffer<T>::_itemsize;
    using APyBuffer<T>::_nitems;

    /* ****************************************************************************** *
     * *                    `__getitem__` family of methods                         * *
     * ****************************************************************************** */

    //! Retrieve item from integer index
    auto get_item_integer(std::ptrdiff_t idx) const
    {
        using SCALAR_TYPE = typename ARRAY_TYPE::SCALAR_VARIANT;
        using RESULT_TYPE = std::variant<ARRAY_TYPE, SCALAR_TYPE>;

        // Bounds checking
        if (idx >= std::ptrdiff_t(_shape[0]) || idx < -std::ptrdiff_t(_shape[0])) {
            std::string msg = fmt::format(
                "{}.__getitem__: index {} is out of bounds for axis 0 with size {}",
                ARRAY_TYPE::ARRAY_NAME,
                idx,
                _shape[0]
            );
            throw std::out_of_range(msg);
        }

        // Adjust for negative index
        idx = idx < 0 ? idx + _shape[0] : idx;

        if (_ndim == 1) {
            SCALAR_TYPE result = static_cast<const ARRAY_TYPE*>(this)->create_scalar();
            result.copy_n_from(std::begin(_data) + idx * _itemsize, _itemsize);
            return RESULT_TYPE(result);
        } else {
            // New shape contains all dimensions except the very first one
            auto new_shape
                = std::vector<std::size_t>(std::begin(_shape) + 1, std::end(_shape));

            // Element stride is the new shape folded over multiplication
            std::size_t element_stride = fold_shape(new_shape);

            ARRAY_TYPE result
                = static_cast<const ARRAY_TYPE*>(this)->create_array(new_shape);
            std::copy_n(
                std::begin(_data) + idx * _itemsize * element_stride,
                _itemsize * element_stride,
                std::begin(result._data)
            );
            return RESULT_TYPE(result);
        }
    }

    //! Retrieve item from a slice index
    auto get_item_slice(nb::slice slice) const
    {
        auto [start, stop, step, len] = slice.compute(_shape[0]);

        // New resulting shape
        std::vector<std::size_t> new_shape = _shape;
        new_shape[0] = len;

        // Result array
        ARRAY_TYPE result
            = static_cast<const ARRAY_TYPE*>(this)->create_array(new_shape);

        auto size = _itemsize * fold_shape(std::begin(_shape) + 1, std::end(_shape));
        std::ptrdiff_t src_i = start;
        std::ptrdiff_t dst_i = 0;
        if (step < 0) {
            // Copy data into result and return (negative src step size)
            for (; src_i > stop; src_i += step, dst_i++) {
                std::copy_n(
                    std::begin(_data) + src_i * size,       // src
                    size,                                   // elements to copy
                    std::begin(result._data) + dst_i * size // dst
                );
            }
        } else { /* step >= 0 */
            // Copy data into result and return (positive src step size)
            for (; src_i < stop; src_i += step, dst_i++) {
                std::copy_n(
                    std::begin(_data) + src_i * size,       // src
                    size,                                   // elements to copy
                    std::begin(result._data) + dst_i * size // dst
                );
            }
        }

        return result;
    }

    //! Convert a `nb::tuple` of `nb::int_`, `nb::slice`, and `nb::ellipsis` to
    //! a `std::vector<std::variant<nb::int_, nb::slice>>`, resolving any ellipsis
    //! slicing.
    auto get_item_to_cpp_tuple(const nb::tuple& key) const
    {
        if (key.size() > _ndim) {
            // The key tuple must have fewer elements than this array has number of
            // dimensions
            std::string msg = fmt::format(
                "{}.__getitem__: key tuple size (={}) > ndim (={})",
                ARRAY_TYPE::ARRAY_NAME,
                key.size(),
                _ndim
            );
            throw nb::value_error(msg.c_str());
        }

        nb::int_ integer;
        nb::slice slice;
        nb::ellipsis ellipsis;
        bool ellipsis_found = false;
        std::vector<std::variant<nb::int_, nb::slice>> cpp_tuple;

        for (std::size_t i = 0; i < key.size(); i++) {
            const auto& tuple_element = key[i];
            if (nb::try_cast<nb::slice>(tuple_element, slice)) {
                cpp_tuple.push_back(slice);
            } else if (nb::try_cast<nb::int_>(tuple_element, integer)) {
                cpp_tuple.push_back(integer);
            } else if (nb::try_cast<nb::ellipsis>(tuple_element, ellipsis)) {
                if (ellipsis_found) {
                    // An ellipsis object has already been procecced. Only a single
                    // ellipsis per tuple key is allowed
                    std::string msg = fmt::format(
                        "{}.__getitem__: only one single ellipsis object allowed",
                        ARRAY_TYPE::ARRAY_NAME
                    );
                    throw nb::value_error(msg.c_str());
                } else {
                    // Found first ellipsis, fill missing dimesnsions with full slices
                    ellipsis_found = true;
                    std::size_t n_fill = _ndim - key.size() + 1;
                    for (std::size_t j = 0; j < n_fill; j++) {
                        cpp_tuple.push_back(nb::slice(_shape[i + j]));
                    }
                }
            } else {
                std::string msg = fmt::format(
                    "{}.__getitem__: supported keys are `int`, `slice`, `ellipsis`, or "
                    "a single tuple thereof",
                    ARRAY_TYPE::ARRAY_NAME
                );
                throw nb::value_error(msg.c_str());
            }
        }
        return cpp_tuple;
    }

    //! Helper method for `get_item_tuple`, returns the shape when slicing using `tuple`
    //! and `remaining`
    std::vector<std::size_t> get_item_tuple_shape(
        const std::vector<std::variant<nb::int_, nb::slice>>& tuple,
        const std::vector<std::variant<nb::int_, nb::slice>>& remaining
    ) const
    {
        // Compute the resulting array shape
        std::vector<std::size_t> result_shape;
        for (std::size_t i = 0; i < tuple.size(); i++) {
            auto&& element = tuple[i];
            if (std::holds_alternative<nb::int_>(element)) {
                continue;
            } else { /* std::holds_alternative<nb::slice>(v) */
                auto [_, __, ___, len]
                    = std::get<nb::slice>(element).compute(_shape[i]);
                result_shape.push_back(len);
            }
        }
        for (std::size_t i = remaining.size() + 1; i < _shape.size(); i++) {
            result_shape.push_back(_shape[i]);
        }
        return result_shape;
    }

    //! Helper method for `get_item_tuple`, returns sliced array in `std::vector`
    auto get_item_slice_nested(nb::slice slice) const
    {
        auto [start, stop, step, len] = slice.compute(_shape[0]);

        // The shape of the sliced vectors
        std::vector<std::size_t> new_shape(std::begin(_shape) + 1, std::end(_shape));
        if (!new_shape.size()) {
            new_shape = { 1 };
        }

        // Result array
        std::vector<ARRAY_TYPE> result;

        auto size = fold_shape(new_shape) * _itemsize;
        ARRAY_TYPE tmp = static_cast<const ARRAY_TYPE*>(this)->create_array(new_shape);
        if (step < 0) {
            // Copy data into result and return (negative src step size)
            for (std::ptrdiff_t src_i = start; src_i > stop; src_i += step) {
                std::copy_n(
                    std::begin(_data) + src_i * size, size, std::begin(tmp._data)
                );
                result.push_back(tmp);
            }
        } else {
            // Copy data into result and return (positive src step size)
            for (std::ptrdiff_t src_i = start; src_i < stop; src_i += step) {
                std::copy_n(
                    std::begin(_data) + src_i * size, size, std::begin(tmp._data)
                );
                result.push_back(tmp);
            }
        }
        return result;
    }

    //! Retrieve item(s) from a `std::vector` of `nb::int_` and `nb::slice`
    auto get_item_tuple(std::vector<std::variant<nb::int_, nb::slice>> tuple) const
    {
        using SCALAR_TYPE = typename ARRAY_TYPE::SCALAR_VARIANT;
        using RESULT_TYPE = std::variant<ARRAY_TYPE, SCALAR_TYPE>;

        // Return everything on an empty tuple
        if (tuple.size() == 0) {
            ARRAY_TYPE result = *static_cast<const ARRAY_TYPE*>(this);
            return RESULT_TYPE(result);
        }

        std::variant<nb::int_, nb::slice> current = tuple[0];
        std::vector<std::variant<nb::int_, nb::slice>> remaining(
            std::begin(tuple) + 1, std::end(tuple)
        );

        if (std::holds_alternative<nb::int_>(current)) {

            /*
             * Current tuple element is an integer
             */
            std::ptrdiff_t key
                = static_cast<std::ptrdiff_t>(std::get<nb::int_>(current));
            RESULT_TYPE result = get_item_integer(key);
            if (!remaining.size()) {
                return result; // is always SCALAR_TYPE
            } else {
                return RESULT_TYPE(std::get<ARRAY_TYPE>(result).get_item_tuple(remaining
                ));
            }

        } else { /* std::holds_alternative<nb::slice>(current) */

            /*
             * Current tuple element is a slice
             */
            std::vector<std::size_t> shape = get_item_tuple_shape(tuple, remaining);
            ARRAY_TYPE result
                = static_cast<const ARRAY_TYPE*>(this)->create_array(shape);
            nb::slice slice = std::get<nb::slice>(current);
            auto arrays = get_item_slice_nested(slice);

            for (std::size_t i = 0; i < arrays.size(); i++) {
                auto v = arrays[i].get_item_tuple(remaining);
                if (std::holds_alternative<SCALAR_TYPE>(v)) {
                    auto scalar = std::get<SCALAR_TYPE>(v);
                    scalar.copy_n_to(
                        std::begin(result._data) + i * _itemsize, _itemsize
                    );
                } else {
                    auto array = std::get<ARRAY_TYPE>(v);
                    std::copy_n(
                        std::begin(array._data),
                        array._nitems * _itemsize,
                        std::begin(result._data) + i * array._nitems * _itemsize
                    );
                }
            }

            return RESULT_TYPE(result);
        }
    }

    //! Top-level Python exported `__getitem__` function
    auto get_item(std::variant<nb::int_, nb::slice, nb::ellipsis, nb::tuple> key) const
    {
        using SCALAR_TYPE = typename ARRAY_TYPE::SCALAR_VARIANT;
        using RESULT_TYPE = std::variant<ARRAY_TYPE, SCALAR_TYPE>;

        if (std::holds_alternative<nb::int_>(key)) {

            // Key is of integer type
            return RESULT_TYPE(
                get_item_integer(static_cast<std::ptrdiff_t>(std::get<nb::int_>(key)))
            );

        } else if (std::holds_alternative<nb::slice>(key)) {

            // Key is of slice type
            return RESULT_TYPE(get_item_slice(std::get<nb::slice>(key)));

        } else if (std::holds_alternative<nb::ellipsis>(key)) {

            // Key is a single ellipsis, return a copy of `*this`
            return RESULT_TYPE(*static_cast<const ARRAY_TYPE*>(this));

        } else { /* std::holds_alternative<nb::tuple>(key) */

            // Key is a tuple of slicing instructions
            return RESULT_TYPE(
                get_item_tuple(get_item_to_cpp_tuple(std::get<nb::tuple>(key)))
            );
        }
    }

    /* ****************************************************************************** *
     * *                     `broadcast` family of methods                          * *
     * ****************************************************************************** */

    //! Broadcast `*this` array to `shape`. Throws `nb::value_error` if `*this` shape is
    //! incompatible with `shape`.
    ARRAY_TYPE broadcast_to(const std::vector<std::size_t> shape) const
    {
        if (!is_broadcastable(_shape, shape)) {
            std::string error_msg = fmt::format(
                "Operands could not be broadcast together with shapes: {}, {}",
                tuple_string_from_vec(_shape),
                tuple_string_from_vec(shape)
            );
            throw nb::value_error(error_msg.c_str());
        }

        ARRAY_TYPE result = static_cast<const ARRAY_TYPE*>(this)->create_array(shape);
        broadcast_data_copy(
            _data.begin(), result._data.begin(), _shape, shape, _itemsize
        );
        return result;
    }

    //! Python exported `broadcast_to` method
    ARRAY_TYPE broadcast_to_python(const std::variant<nb::tuple, nb::int_> shape) const
    {
        return broadcast_to(cpp_shape_from_python_shape_like(shape));
    }

    /* ****************************************************************************** *
     * *                     `reshape` family of methods                            * *
     * ****************************************************************************** */

    //! Attempt to reshape `*this` into `new_shape`, resolving any -1 dimension. Returns
    //! the resolved new shape on success.
    std::vector<std::size_t> try_reshape(nb::tuple new_shape) const
    {
        std::vector<std::size_t> new_shape_vec;
        std::size_t index = 0;
        std::size_t reshape_size = 1;
        std::size_t neg_one_count = 0;
        std::ptrdiff_t neg_one_pos = -1;
        for (auto it = new_shape.begin(); it != new_shape.end(); ++it, ++index) {
            std::ptrdiff_t current_value = nb::cast<std::ptrdiff_t>(*it);
            if (current_value == -1) {
                neg_one_count++;
                neg_one_pos = index;        // store the position of -1
                new_shape_vec.push_back(1); // temporarily push 1, replaced later
            } else if (current_value < -1) {
                std::string error_msg = fmt::format(
                    "{}.reshape: array dimensions must be greater than or equal to -1",
                    ARRAY_TYPE::ARRAY_NAME
                );
                throw nb::value_error(error_msg.c_str());
            } else {
                std::size_t c_size_t = nb::cast<std::size_t>(*it);
                new_shape_vec.push_back(c_size_t);
                reshape_size *= c_size_t;
            }
        }

        if (neg_one_count > 1) {
            std::string error_msg = fmt::format(
                "{}.reshape: only one dimension can be -1", ARRAY_TYPE::ARRAY_NAME
            );
            throw nb::value_error(error_msg.c_str());
        }

        if (neg_one_count == 1) {
            if (_nitems % reshape_size != 0) {
                std::string error_msg = fmt::format(
                    "{}.reshape: the size of target array must be unchanged and "
                    "divisible by the known dimensions",
                    ARRAY_TYPE::ARRAY_NAME
                );
                throw nb::value_error(error_msg.c_str());
            }
            new_shape_vec[neg_one_pos] = _nitems / reshape_size;
        }

        std::size_t reshape_elem_count = fold_shape(new_shape_vec);

        // Validate if total elements match the original array
        if (_nitems != reshape_elem_count) {
            std::string error_msg = fmt::format(
                "{}.reshape: target array number of elements does not match the "
                "original array",
                ARRAY_TYPE::ARRAY_NAME
            );
            throw nb::value_error(error_msg.c_str());
        }

        return new_shape_vec;
    }

    //! Python exported `reshape` method
    ARRAY_TYPE reshape(nb::tuple shape) const
    {
        std::vector<std::size_t> cpp_shape = try_reshape(shape);
        ARRAY_TYPE result
            = static_cast<const ARRAY_TYPE*>(this)->create_array(cpp_shape);
        std::copy_n(this->_data.begin(), _data.size(), result._data.begin());

        return result;
    }

    //! Python exported `flatten` method
    ARRAY_TYPE flatten() const
    {
        // Reuse the reshape function to flatten the array
        return reshape(nb::make_tuple(-1));
    }

    //! Python exported `ravel` method
    ARRAY_TYPE ravel() const { return flatten(); }

    //! Python exported `transpose` method.
    ARRAY_TYPE transpose(std::optional<nb::tuple> axes = std::nullopt) const
    {
        switch (_ndim) {
        case 0:
        case 1:
            // Behave like NumPy, simply return `*this` if single dimensional
            return *static_cast<const ARRAY_TYPE*>(this);
        case 2: {
            // Optimized code for `_ndim` == 2
            ARRAY_TYPE result
                = static_cast<const ARRAY_TYPE*>(this)->create_array(_shape);
            std::reverse(result._shape.begin(), result._shape.end());
            for (std::size_t y = 0; y < _shape[0]; y++) {
                for (std::size_t x = 0; x < _shape[1]; x++) {
                    std::copy_n(
                        _data.begin() + (y * _shape[1] + x) * _itemsize,       // src
                        _itemsize,                                             // limbs
                        result._data.begin() + (x * _shape[0] + y) * _itemsize // dst
                    );
                }
            }
            return result;
        }

        default: {
            std::vector<size_t> new_axis(_ndim);

            if (axes.has_value()) {
                std::variant<nb::tuple, nb::int_> axis_variant = axes.value();
                new_axis = get_normalized_axes(axis_variant, _ndim);
            } else {
                // reverse the order of axes by default
                std::iota(new_axis.begin(), new_axis.end(), 0);
                std::reverse(new_axis.begin(), new_axis.end());
            }

            std::vector<size_t> new_shape(_ndim);
            for (size_t i = 0; i < _ndim; ++i) {
                new_shape[i] = _shape[new_axis[i]];
            }

            ARRAY_TYPE result
                = static_cast<const ARRAY_TYPE*>(this)->create_array(new_shape);
            transpose_axes_and_copy_data(
                _data.begin(), result._data.begin(), _shape, new_axis, _itemsize
            );
            return result;
        }
        }
    }

    //! Python exported `squeeze` method
    ARRAY_TYPE squeeze(std::optional<std::variant<nb::int_, nb::tuple>> axis) const
    {
        std::vector<std::size_t> shape = _shape;
        std::set<std::ptrdiff_t> axis_set;
        if (axis.has_value()) {
            // Given an int or tuple of ints, remove the specified dimensions if their
            // size is one. Else throw an error.
            auto ax = *axis;
            nb::tuple axis_tuple;

            if (std::holds_alternative<nb::tuple>(ax)) {
                axis_tuple = std::get<nb::tuple>(ax);
            } else if (std::holds_alternative<nb::int_>(ax)) {
                std::ptrdiff_t axis = std::ptrdiff_t(std::get<nb::int_>(ax));
                axis_tuple = nb::make_tuple(axis);
            }
            for (auto ptr = axis_tuple.begin(); ptr != axis_tuple.end(); ptr++) {
                std::ptrdiff_t axis_n = std::ptrdiff_t(nanobind::cast<nb::int_>(*ptr));
                if (axis_n >= std::ptrdiff_t(shape.size())) {
                    std::string error_msg = fmt::format(
                        "{}.squeeze: specified axis larger than number of array "
                        "dimensions",
                        ARRAY_TYPE::ARRAY_NAME
                    );
                    throw nb::index_error(error_msg.c_str());
                }
                axis_set.insert(axis_n);
            }
            std::ptrdiff_t cnt = 0;
            auto predicate = [&](std::size_t dim) {
                if (axis_set.find(cnt) != axis_set.end() && dim != 1) {
                    std::string error_msg = fmt::format(
                        "{}.squeeze: cannot squeeze non-one dimensions",
                        ARRAY_TYPE::ARRAY_NAME
                    );
                    throw nb::value_error(error_msg.c_str());
                }
                return axis_set.find(cnt++) != axis_set.end() && dim == 1;
            };
            shape.erase(
                std::remove_if(shape.begin(), shape.end(), predicate), shape.end()
            );
        } else {
            // Given no specified axis, remove all dimensions of size one.
            auto predicate = [](std::size_t dim) { return dim == 1; };
            shape.erase(
                std::remove_if(shape.begin(), shape.end(), predicate), shape.end()
            );
        }

        // Don't squeeze all dimensions...
        if (shape.size() == 0) {
            shape = { 1 };
        }

        // Create resulting array and copy the data
        ARRAY_TYPE result = static_cast<const ARRAY_TYPE*>(this)->create_array(shape);
        std::copy_n(_data.begin(), _data.size(), result._data.begin());
        return result;
    }

}; // end class: `APyArray`

#endif
