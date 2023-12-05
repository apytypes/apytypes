/*
 * Allocator for type `T` which uses `_STACK_ELEMENT_SIZE` elements on the stack as a
 * first priority placement for the data, and falls back to the heap (placement new)
 * if allocation is greater than `_STACK_ELEMENT_SIZE` elements. As many allocations in
 * APyTypes use very small vectors (elements often smaller than say 128 bits), this
 * can many times help save the overhead of performing heap allocations.
 *
 * The idea is to use this allocator with `std::vector<mp_limb_t>`
 *
 * Author: Mikael Henriksson (2023)
 */

#ifndef _APY_DYNAMIC_STACK_ALLOCATOR_H
#define _APY_DYNAMIC_STACK_ALLOCATOR_H

#include <type_traits>
#include <memory>

template <typename T, std::size_t _STACK_ELEMENT_SIZE>
class DynamicStackAllocator
{
public:
    using value_type = T;
    DynamicStackAllocator() noexcept {};

    /*
     * Allocation function
     */
    typename std::allocator<T>::pointer allocate(
        typename std::allocator<T>::size_type n
    ) {
        if (n <= _STACK_ELEMENT_SIZE) {
            return stack_data;
        } else {
            return new T[n];
        }
    }

    /*
     * Deallocation function
     */
    void deallocate(
        typename std::allocator<T>::pointer ptr,
        typename std::allocator<T>::size_type n
    ) {
        if (n <= _STACK_ELEMENT_SIZE) {
            return;
        } else {
            delete[] ptr;
        }
    }

    // Member function `rebind` is non-optional for allocators where template parameters
    // are non-template *type* parameters:
    // https://en.cppreference.com/w/cpp/named_req/Allocator
    template <class U>
    struct rebind {
        using other = DynamicStackAllocator<T, _STACK_ELEMENT_SIZE>;
    };

private:

    // Underlying stack data for small allocations
    T stack_data[_STACK_ELEMENT_SIZE];
};


template <
    typename T, std::size_t _T_STACK_ELEMENT_SIZE,
    typename U, std::size_t _U_STACK_ELEMENT_SIZE
>
bool operator==(
    const DynamicStackAllocator<T, _T_STACK_ELEMENT_SIZE> &,
    const DynamicStackAllocator<U, _U_STACK_ELEMENT_SIZE> &) 
    noexcept
{ 
    // Storage allocated with one `DynamicAllocator` can be deallocated through
    // another `DynamicAllocator`
    return true;
}


template <
    typename T, std::size_t _T_STACK_ELEMENT_SIZE,
    typename U, std::size_t _U_STACK_ELEMENT_SIZE
>
bool operator!=(
    const DynamicStackAllocator<T, _T_STACK_ELEMENT_SIZE> &,
    const DynamicStackAllocator<U, _U_STACK_ELEMENT_SIZE> &) 
    noexcept
{ 
    // Storage allocated with one `DynamicAllocator` can be deallocated through
    // another `DynamicAllocator`
    return false;
}

#endif  // _APY_DYNAMIC_STACK_ALLOCATOR_H
