/*
 * Dynamic arbitrary precision integer data type.
 */

#include "apy_fixed.h"
#include "apy_util.h"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include <iostream>

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
    result = *this;
    for (auto &data : result._data) {
        data = ~data;
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
    std::vector<uint8_t> data_bcd = double_dabble(abs_val._data);
    std::deque<uint8_t> bcd_deque(data_bcd.begin(), data_bcd.end());
    int bcd_binary_point = 0;

    if (int_bits() < bits()) {

        // Step-wise divide BCD number by two
        auto num_bcds_before_div = bcd_deque.size();
        auto num_prev_bcds = num_bcds_before_div;
        for (int i=0; i<bits()-int_bits(); i++) {
            bcd_div2(bcd_deque);
            if (bcd_deque.size() > num_prev_bcds) {
                bcd_binary_point++;
                num_prev_bcds = bcd_deque.size();
            }
        }

        // Trim unnecessary zeros at start of BCD number
        while (int(bcd_deque.size()) > bcd_binary_point+1) {
            if (bcd_deque.back() != 0) {
                // Nothing more to trim, break out of while-loop
                break;
            }
            bcd_deque.pop_back();
        }

    } else if (int_bits() > bits()) {
        // More integer bits that bits, not implemented yet
        throw NotImplementedException();
    }

    // Convert BCD number to string and return
    std::string result = is_negative() ? "-" : "";
    for (int i=bcd_deque.size()-1; i >= 0; i--) {
        result.append( std::to_string(static_cast<int>(bcd_deque[i])) );
        if (bcd_binary_point && i == bcd_binary_point) {
            result.append(".");
        }
    }
    return result;
}

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
