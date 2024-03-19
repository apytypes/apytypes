/*
 * Scratch vector drop-in replacement for C++17 `std::vector` using small scratch
 * memory. Same functionality as `std::vector`, but a compile-time selected number of
 * elements are stored in the `scratch_vector`, not externally on the heap. If the
 * internal scratch vector cache memory overflows, data is moved and stored on the heap
 * instead.
 *
 * Author: Mikael Henriksson (2024)
 */

#ifndef _APYTYPES_SCRATCH_VECTOR_H
#define _APYTYPES_SCRATCH_VECTOR_H

#include <fmt/format.h>

#include <algorithm>        // std::copy, std::copy_n
#include <array>            // std::array
#include <cstddef>          // std::size_t, std::ptrdiff_t
#include <initializer_list> // std::initializer_list
#include <iterator>         // std::begin, std::end, std::make_reverse_iterator
#include <memory>           // std::allocator_traits
#include <stdexcept>        // std::out_of_range
#include <vector>           // std::vector

template <typename T, typename PTR_T> class ScratchVectorIteratorBase {
public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = PTR_T;

    /* Iterator category */
    using iterator_category = std::random_access_iterator_tag;

    /* Random access iterator */
    ScratchVectorIteratorBase(pointer ptr)
        : _ptr { ptr }
    {
    }
    ScratchVectorIteratorBase& operator++() { return ++_ptr, *this; }
    ScratchVectorIteratorBase& operator--() { return --_ptr, *this; }
    ScratchVectorIteratorBase operator++(int)
    {
        auto res = *this;
        ++_ptr;
        return res;
    }
    ScratchVectorIteratorBase operator--(int)
    {
        auto res = *this;
        --_ptr;
        return res;
    }
    ScratchVectorIteratorBase operator+(difference_type n) const
    {
        return ScratchVectorIteratorBase(_ptr + n);
    }
    ScratchVectorIteratorBase& operator+=(difference_type n)
    {
        _ptr += n;
        return *this;
    }
    difference_type operator-(ScratchVectorIteratorBase other) const
    {
        return _ptr - other._ptr;
    }
    ScratchVectorIteratorBase operator-(difference_type n) const
    {
        return ScratchVectorIteratorBase(_ptr - n);
    }
    ScratchVectorIteratorBase& operator-=(difference_type n)
    {
        _ptr -= n;
        return *this;
    }
    reference operator[](std::size_t n) { return _ptr[n]; }
    const_reference operator[](std::size_t n) const { return _ptr[n]; }
    reference operator*() { return *_ptr; }
    const_reference operator*() const { return *_ptr; }

    bool operator==(ScratchVectorIteratorBase other) const
    {
        return _ptr == other._ptr;
    }
    bool operator!=(ScratchVectorIteratorBase other) const
    {
        return _ptr != other._ptr;
    }
    bool operator<(ScratchVectorIteratorBase other) const { return _ptr < other._ptr; }
    bool operator>(ScratchVectorIteratorBase other) const { return _ptr > other._ptr; }
    bool operator<=(ScratchVectorIteratorBase other) const
    {
        return _ptr <= other._ptr;
    }
    bool operator>=(ScratchVectorIteratorBase other) const
    {
        return _ptr >= other._ptr;
    }

private:
    pointer _ptr;
};

template <typename T, std::size_t _N_SCRATCH_ELEMENTS = 1> class ScratchVector {
public:
    /* ****************************************************************************** *
     * *                          Vector name aliasing                              * *
     * ****************************************************************************** */

    using value_type = T;
    using allocator_type = std::allocator<T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
    using iterator = ScratchVectorIteratorBase<T, T*>;
    using const_iterator = ScratchVectorIteratorBase<const T, const T*>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /* ****************************************************************************** *
     * *                             Constructors                                   * *
     * ****************************************************************************** */

    ScratchVector() noexcept
        : _size {}
        , _scratch_data {}
        , _vector_data {}
    {
        /* Default constructor */
    }

    ScratchVector(size_type count, const T& value)
        : ScratchVector()
    {
        _size = count;
        if (count <= _N_SCRATCH_ELEMENTS) {
            std::fill_n(std::begin(_scratch_data), count, value);
        } else {
            _vector_data = std::vector<T>(count, value);
        }
    }

    explicit ScratchVector(size_type count)
        : ScratchVector(count, T())
    {
        /* Zero-initialization */
    }

    template <class InputIt>
    ScratchVector(InputIt first, InputIt last)
        : ScratchVector()
    {
        auto distance = std::distance(first, last);
        if (distance < 0) {
            throw std::out_of_range(
                "ScratchVector::ScratchVector(_IT first, _IT last): "
                "iterator last before iterator first"
            );
        } else if (std::distance(first, last) < std::ptrdiff_t(_N_SCRATCH_ELEMENTS)) {
            std::copy(first, last, std::begin(_scratch_data));
        } else {
            _vector_data = std::vector<T>(first, last);
        }
        _size = distance;
    }

    ScratchVector(const ScratchVector& other)
        : ScratchVector()
    {
        _size = other._size;
        std::copy(
            std::begin(other._scratch_data),
            std::end(other._scratch_data),
            std::begin(_scratch_data)
        );
        _vector_data = other._vector_data;
    }

    ScratchVector(std::initializer_list<T> init)
        : ScratchVector()
    {
        _size = init.size();
        if (init.size() <= _N_SCRATCH_ELEMENTS) {
            std::copy(init.begin(), init.end(), std::begin(_scratch_data));
        } else {
            _vector_data = std::vector<T>(init);
        }
    }

    /* ****************************************************************************** *
     * *                                Iterators                                   * *
     * ****************************************************************************** */

    iterator begin() noexcept
    {
        if (_vector_data.size()) {
            return iterator(&*std::begin(_vector_data));
        } else {
            return iterator(&*std::begin(_scratch_data));
        }
    }

    const_iterator begin() const noexcept
    {
        if (_vector_data.size()) {
            return const_iterator(&*std::cbegin(_vector_data));
        } else {
            return const_iterator(&*std::cbegin(_scratch_data));
        }
    };

    const_iterator cbegin() const noexcept { return begin(); }

    iterator end() noexcept
    {
        if (_vector_data.size()) {
            return iterator(&*std::end(_vector_data));
        } else {
            return iterator(&*(std::begin(_scratch_data) + _size));
        }
    }

    const_iterator end() const noexcept
    {
        if (_vector_data.size()) {
            return const_iterator(&*std::cend(_vector_data));
        } else {
            return const_iterator(&*(std::cbegin(_scratch_data) + _size));
        }
    }

    const_iterator cend() const noexcept { return end(); }

    reverse_iterator rbegin() noexcept
    {
        return std::make_reverse_iterator(begin() + _size);
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return std::make_reverse_iterator(begin() + _size);
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return std::make_reverse_iterator(begin() + _size);
    }

    reverse_iterator rend() noexcept { return std::make_reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept
    {
        return std::make_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const noexcept
    {
        return std::make_reverse_iterator(begin());
    }

    /* ****************************************************************************** *
     * *                                Methods                                     * *
     * ****************************************************************************** */

    size_type size() const noexcept { return _size; }

    reference operator[](size_type pos)
    {
        if (_vector_data.size()) {
            return _vector_data[pos];
        } else {
            return _scratch_data[pos];
        }
    }

    const_reference operator[](size_type pos) const
    {
        if (_vector_data.size()) {
            return _vector_data[pos];
        } else {
            return _scratch_data[pos];
        }
    }

    reference at(size_type pos)
    {
        if (pos < _size) {
            return this[pos];
        } else {
            throw std::out_of_range(fmt::format(
                "ScratchVector::at(size_type pos): out of range, "
                "_size=={}, pos=={}",
                _size,
                pos
            ));
        }
    }

    const_reference at(size_type pos) const
    {
        if (pos < _size) {
            return this[pos];
        } else {
            throw std::out_of_range(fmt::format(
                "ScratchVector::at(size_type pos): out of range, "
                "_size=={}, pos=={}",
                _size,
                pos
            ));
        }
    }

    void resize(size_type count)
    {
        if (_vector_data.size() > 0) {
            // Heap vector already in use. Just keep on using it, it is free at this
            // point
            _vector_data.resize(count);
            _size = count;
        } else if (count > _N_SCRATCH_ELEMENTS) {
            // Upsizing to the heap vector. Initialize heap vector and copy data
            _vector_data = std::vector<T>(count);
            std::copy(
                std::begin(_scratch_data),
                std::end(_scratch_data),
                std::begin(_vector_data)
            );
            _size = count;
        } else {
            // Resizing withing the scratch array
            _size = count;
        }
    }

    ScratchVector& operator=(const ScratchVector& other)
    {
        _size = other._size;
        std::copy(
            std::begin(other._scratch_data),
            std::end(other._scratch_data),
            std::begin(_scratch_data)
        );
        _vector_data = other._vector_data;
        return *this;
    }

    ScratchVector& operator=(const std::vector<T>& other)
    {
        _size = other.size();
        if (_size <= _N_SCRATCH_ELEMENTS) {
            std::copy(std::begin(other), std::end(other), std::begin(_scratch_data));
        } else {
            _vector_data = other;
        }
        return *this;
    }

    reference back() { return *--std::end(*this); }

    const_reference back() const { return *--std::end(*this); }

    explicit operator std::vector<T>() const
    {
        if (_vector_data.size() > 0) {
            return _vector_data;
        } else {
            std::vector<T> result(_size);
            std::copy_n(std::begin(_scratch_data), _size, std::begin(result));
            return result;
        }
    }

private:
    std::size_t _size;                                // Elements in this ScratchVector
    std::array<T, _N_SCRATCH_ELEMENTS> _scratch_data; // Underlying scratch
    std::vector<T> _vector_data;                      // Underlying vector
};

#endif // _APYTYPES_SCRATCH_VECTOR_H
