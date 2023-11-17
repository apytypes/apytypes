/*
 * Dynamic arbitrary precision integer data type.
 */

#include "apy_fixed.h"
#include "apy_util.h"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string>

/*
 * Constructors
 */
 APyFixed::APyFixed(unsigned bits, int int_bits) :
    _bits{bits},
    _int_bits{int_bits},
    _data(idiv64_ceil_fast(bits), int64_t(0))
{
    if (bits <= 0) {
        throw std::domain_error("APyInt needs a size of atleast 1 bit");
    }
}

APyFixed::APyFixed(unsigned bits, int int_bits, int value) :
    _bits{bits},
    _int_bits{int_bits},
    _data(idiv64_ceil_fast(bits), int64_t(0))
{
    if (bits <= 0) {
        throw std::domain_error("APyInt needs a size of atleast 1 bit");
    }
    _data[0] = int64_t(value);
}

APyFixed::APyFixed(unsigned bits, int int_bits, const std::vector<int64_t> &vec) :
    _bits{bits},
    _int_bits{int_bits},
    _data(vec.begin(), vec.end())
{
    if (bits <= 0) {
        throw std::domain_error("APyInt needs a size of atleast 1 bit");
    }
}

/*
 * Binary operators
 */
bool APyFixed::operator==(const APyFixed &rhs) const
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
 * Unariy operators
 */
APyFixed APyFixed::operator-() const {
    APyFixed result(bits()+1, int_bits()+1);
    for (std::size_t i=0; i<result.vector_size(); i++) {
        result._data[i] = ~_data[i];
    }
    if (result.vector_size() > vector_size()) {
        // Pad bits in the new vector data with sign bit
        bool not_sign = bool(_data[vector_size()-1] & 0x8000000000000000);
        result._data[result.vector_size()-1] = not_sign ? int64_t(0) : int64_t(-1);
    }
    result.increment_lsb();
    return result;
}

// Increment the LSB without making the fixed-point number wider
void APyFixed::increment_lsb()
{
    bool carry = true;
    for (auto &word : _data) {
        word += carry;
        carry = uint64_t(word) < uint64_t(carry);
    }
}

/*
 * Utility functions
 */
std::string APyFixed::to_string_dec() const {

    // Print absolute value of number, and conditionally append a minus sign
    APyFixed abs_val(bits()+1, int_bits()+1);
    if (is_negative()) {
        abs_val = -(*this);
    } else {
        abs_val = *this;
    }

    // Conversion to BCD
    std::vector<uint8_t> bcd_list = double_dabble(abs_val._data);
    auto bcd_binary_point = 0;
    auto num_prev_bcds = bcd_list.size();

    if (int_bits() < bits()) {

        // Reverse order of BCD list to MSB first
        std::reverse(bcd_list.begin(), bcd_list.end());

        for (int i=0; i<bits()-int_bits(); i++) {
            bcd_div2(bcd_list);
            if (bcd_list.size() > num_prev_bcds) {
                bcd_binary_point++;
                num_prev_bcds = bcd_list.size();
            }
        }

        // Reverse order BCD list (back) to LSB first
        std::reverse(bcd_list.begin(), bcd_list.end());

        // Trim unnecessary zeros at start of BCD number
        while (int(bcd_list.size()) > bcd_binary_point+1) {
            if (bcd_list.back() != 0) {
                // Nothing more to trim, break out of while-loop
                break;
            }
            bcd_list.pop_back();
        }

        // Convert BCD number to string and return
        std::string result = is_negative() ? "-" : "";
        for (int i=bcd_list.size()-1; i >= 0; i--) {
            result.push_back( static_cast<char>(bcd_list[i] + 0x30) );
            if (bcd_binary_point && i == bcd_binary_point) {
                result.append(".");
            }
        }
        return result;

    } else if (int_bits() > bits()) {

        for (int i=0; i<int_bits()-bits(); i++) {
            bcd_mul2(bcd_list);
            if (bcd_list.size() > num_prev_bcds) {
                bcd_binary_point++;
                num_prev_bcds = bcd_list.size();
            }
        }

        // Convert BCD number to string and return
        std::string result = is_negative() ? "-" : "";
        for (int i=bcd_list.size()-1; i >= 0; i--) {
            result.push_back( static_cast<char>(bcd_list[i] + 0x30) );
        }
        return result;

    } else {

        std::string result = is_negative() ? "-" : "";
        for (int i=bcd_list.size()-1; i >= 0; i--) {
            result.push_back( static_cast<char>(bcd_list[i] + 0x30) );
            if (bcd_binary_point && i == bcd_binary_point) {
                result.append(".");
            }
        }
        return result;

    }
}

// APyFixed &APyFixed::operator=(const APyFixed &rhs) const
// {
//     throw NotImplementedException();
// }

void APyFixed::from_bitstring(const std::string &str)
{
    (void) str;
    throw NotImplementedException();
}

void APyFixed::from_vector(const std::vector<int64_t> &new_vector) {
    if (new_vector.size() != this->vector_size()) {
        throw std::domain_error("Vector size miss-match");
    }
    _data = new_vector;
}

std::string APyFixed::to_string_hex() const {
    throw NotImplementedException();
}

std::string APyFixed::to_string_oct() const {
    throw NotImplementedException();
}
