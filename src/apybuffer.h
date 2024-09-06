/*
 * APyTypes buffer array, for using arrays and exposing its Python Buffer Protocol.
 * Buffer Protocol documentation: https://docs.python.org/3/c-api/buffer.html
 */

#ifndef _APYBUFFER_H
#define _APYBUFFER_H

// Python details. These should be included before standard header files:
// https://docs.python.org/3/c-api/intro.html#include-files
#include <Python.h>

#include <cstdlib>     // std::malloc
#include <memory>      // std::allocator
#include <type_traits> // std::true_type
#include <vector>      // std::vector

#include "apytypes_util.h"

template <typename T> struct NoDefaultConstructAllocator {
    using value_type = T;
    using size_type = typename std::allocator<T>::size_type;
    using difference_type = typename std::allocator<T>::difference_type;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;
    template <typename U> struct rebind {
        typedef NoDefaultConstructAllocator<U> other;
    };

    NoDefaultConstructAllocator() noexcept = default;
    ~NoDefaultConstructAllocator() noexcept = default;

    T* allocate(std::size_t n)
    {
        return reinterpret_cast<T*>(std::malloc(sizeof(T) * n));
    }
    T* allocate(std::size_t n, const void* hint)
    {
        (void)hint;
        return allocate(n);
    }
    void deallocate(T* p, std::size_t n)
    {
        (void)n;
        std::free(p);
    }

    template <typename U>
    bool operator==(const NoDefaultConstructAllocator<U>& _) noexcept
    {
        return true;
    }
    template <typename U>
    bool operator!=(const NoDefaultConstructAllocator<U>& _) noexcept
    {
        return false;
    }
};

/*
 * (1) APyBuffer that does *not* default allocate its data
 */
// template <typename T, typename Allocator = NoDefaultConstructAllocator<T>> class
// APyBuffer {

/*
 * (2) APyBuffer the does default allocate its data
 */
template <typename T, typename Allocator = std::allocator<T>> class APyBuffer {

public:
    // The underlying vector type
    using vector_type = std::vector<T, Allocator>;

    //! APyBuffers are to be inherited from. All fields and constructors are protected.
protected:
    //! Base constructor for creating a new `APyBuffer`
    APyBuffer(const std::vector<std::size_t>& shape, std::size_t itemsize = 1)
        : _itemsize { itemsize }
        , _shape { shape }
        , _nitems { fold_shape(shape) }
        , _data(itemsize * _nitems)
        , _ndim { shape.size() }
        , _strides {} // uninitialized on construction (is initialized on demand)
    {
    }

    //! Return a Python Buffer structure compatible with the Buffer Protocol
    Py_buffer get_py_buffer()
    {
        _strides = strides_from_shape(_shape, _itemsize * sizeof(T));
        return Py_buffer {
            (void*)&_data[0],                // void       *buf
            nullptr,                         // PyObject   *obj
            _nitems * _itemsize * sizeof(T), // Py_ssize_t len
            _itemsize * sizeof(T),           // Py_ssize_t itemsize
            0,                               // int        readonly
            _ndim,                           // int        ndim
            nullptr,                         // char       *format
            (Py_ssize_t*)&_shape[0],         // Py_ssize_t *shape
            (Py_ssize_t*)&_strides[0],       // Py_ssize_t *strides
            nullptr,                         // Py_ssize_t suboffsets
            nullptr                          // void       *internal
        };
    }

    //! Resize the underlying buffer without touching its data. Narrowing the buffer
    //! will result in loss of data.
    void buffer_resize(const std::vector<std::size_t> shape, std::size_t itemsize)
    {
        if ((_itemsize != itemsize) || (_shape != shape)) {
            _itemsize = itemsize;
            _shape = shape;
            _nitems = fold_shape(shape);
            _data.resize(itemsize * _nitems);
            _ndim = shape.size();
            /* `_strides` are updated on demand */
        }
    }

    std::size_t _itemsize;             // Size of a single item (in number of `T`)
    std::vector<std::size_t> _shape;   // Shape, number of items along each dimension
    std::size_t _nitems;               // Total number of items in buffer
    vector_type _data;                 // Underlying data vector
    std::size_t _ndim;                 // Number of dimensions
    std::vector<std::size_t> _strides; // Byte-strides (uninitialized until the member
                                       // function `get_py_buffer()` is called)

public:
    // Getters for important field
    const std::vector<std::size_t>& get_shape() const noexcept { return _shape; }
};

#endif // _APYBUFFER_H
