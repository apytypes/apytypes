/*
 * APyTypes buffer array, for using arrays and exposing its Python Buffer Protocal.
 * Buffer Protocol documentation: https://docs.python.org/3/c-api/buffer.html
 */

#ifndef _APYBUFFER_H
#define _APYBUFFER_H

// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h>

#include <algorithm>  // std::reverse
#include <functional> // std::multiplies
#include <memory>     // std::allocator
#include <numeric>    // std::accumulate
#include <vector>     // std::vector

#include "apytypes_util.h"

//! Fold a shape under multiplication
[[maybe_unused]] static APY_INLINE std::size_t
fold_shape(const std::vector<std::size_t>& shape)
{
    return std::accumulate(shape.cbegin(), shape.cend(), 1, std::multiplies {});
}

//! Retrieve the byte-strides from a shape
template <typename T>
[[maybe_unused]] static APY_INLINE std::vector<std::size_t>
strides_from_shape(const std::vector<std::size_t>& shape, std::size_t itemsize = 1)
{
    std::size_t n_bytes = sizeof(T) * itemsize;
    std::vector<std::size_t> strides(shape.size(), 0);
    for (std::size_t i = 0; i < shape.size(); i++) {
        strides[i] = std::accumulate(
            shape.crbegin(), shape.crbegin() + i, n_bytes, std::multiplies {}
        );
    }
    std::reverse(strides.begin(), strides.end());
    return strides;
}

template <typename T, typename Allocator = std::allocator<T>> class APyBuffer {

    // APyBuffers are to be inherited from. All fields and constructors are protected.
protected:
    // Base constructor for creating a new `APyBuffer`
    APyBuffer(const std::vector<std::size_t>& shape, std::size_t itemsize = 1)
        : _itemsize { itemsize }
        , _shape { shape }
        , _strides { strides_from_shape<T>(shape, itemsize) }
        , _data(itemsize * fold_shape(shape))
        , _ndim { shape.size() }
    {
    }

    // Retrieve a Python Buffer structure compatible with the Buffer Protocol
    Py_buffer get_py_buffer()
    {
        return Py_buffer {
            (void*)&_data[0],                           // void       *buf
            nullptr,                                    // PyObject   *obj
            fold_shape(_shape) * _itemsize * sizeof(T), // Py_ssize_t len
            _itemsize * sizeof(T),                      // Py_ssize_t itemsize
            0,                                          // int        readonly
            _ndim,                                      // int        ndim
            nullptr,                                    // char       *format
            (Py_ssize_t*)&_shape[0],                    // Py_ssize_t *shape
            (Py_ssize_t*)&_strides[0],                  // Py_ssize_t *strides
            nullptr,                                    // Py_ssize_t suboffsets
            nullptr                                     // void       *internal
        };
    }

    // Resize the underlying buffer without touching its data. Narrowing the buffer will
    // result in loss of data.
    void buffer_resize(const std::vector<std::size_t> shape, std::size_t itemsize)
    {
        if ((_itemsize != itemsize) || (_shape != shape)) {
            _itemsize = itemsize;
            _shape = shape;
            _strides = strides_from_shape<T>(shape, itemsize);
            _data.resize(itemsize * fold_shape(shape));
            _ndim = shape.size();
        }
    }

    std::size_t _itemsize;             // Size of item (in number of objects `T`)
    std::vector<std::size_t> _shape;   // Shape (in number of objects `T`)
    std::vector<std::size_t> _strides; // Stides (in bytes)
    std::vector<T, Allocator> _data;
    std::size_t _ndim;
};

#endif // _APYBUFFER_H
