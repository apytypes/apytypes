/*
 * Dynamic arbitrary precision integer data type.
 */

#include "apy_int.h"
#include <algorithm>
#include <cstddef>
#include <exception>
#include <iostream>
#include <stdexcept>

/*
 * Constructors
 */
APyInt::APyInt(std::size_t size) : _size{size}, _data(size, int64_t(0))
{
    if (size <= 0) {
        throw std::domain_error("APyInt needs a size of atleast 1 bit");
    }
}

APyInt::APyInt(std::size_t size, int value) : _size{size}, _data(size, int64_t(0))
{
    if (size <= 0) {
        throw std::domain_error("APyInt needs a size of atleast 1 bit");
    }
    _data[0] = int64_t(value);
}

/*
 * Binary operators
 */
bool APyInt::operator==(const APyInt &rhs) const
{
    // Test that data is equal in regions of overlapping data
    for (std::size_t i=0; i<std::min(_data.size(), rhs._data.size()); i++) {
        if (_data.at(i) != rhs._data.at(i)) {
            return false;
        }
    }

    // Test that data is zeroed in regions of the larger integer
    if (_data.size() < rhs._data.size()) {
        bool signbit = _data.at(_data.size()-1) & (int64_t(1) << 63);
        for (std::size_t i=_data.size()+1; i<rhs._data.size(); i++) {
            if (signbit && rhs._data.at(i) != int64_t(0)-1) {
                return false;
            }
            else if (!signbit && rhs._data.at(i) != int64_t(0)) {
                return false;
            }
        }
    } else {
        bool signbit = rhs._data.at(rhs._data.size()-1) & (int64_t(1) << 63);
        for (std::size_t i=rhs._data.size()+1; i<_data.size(); i++) {
            if (signbit && _data.at(i) != int64_t(0)-1) {
                return false;
            } else if (!signbit && _data.at(i) != int64_t(0)) {
                return false;
            }
        }
    }
    
    // The integers are equal
    return true;
}

/*
 * Utility functions
 */

