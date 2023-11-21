/*
 * APyFixed: Dynamic arbitrary fixed-point data type.
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
 APyFixed::APyFixed(int bits, int int_bits) :
    _bits{bits},
    _int_bits{int_bits},
    _data(idiv64_ceil_fast(bits), int64_t(0))  // Zero initialize data
{
    if (bits <= 0) {
        throw std::domain_error(
            "APyInt needs a positive integer bit-size of atleast 1 bit"
        );
    }
}

APyFixed::APyFixed(int bits, int int_bits, int value) :
    _bits{bits},
    _int_bits{int_bits},
    _data(idiv64_ceil_fast(bits), int64_t(0))  // Zero initialize data
{
    if (bits <= 0) {
        throw std::domain_error(
            "APyInt needs a positive integer bit-size of atleast 1 bit"
        );
    }
    _data[0] = int64_t(value);
    twos_complement_overflow();
}

APyFixed::APyFixed(int bits, int int_bits, const std::vector<int64_t> &vec) :
    _bits{bits},
    _int_bits{int_bits},
    _data(vec.begin(), vec.end())
{
    if (bits <= 0) {
        throw std::domain_error(
            "APyInt needs a positive integer bit-size of atleast 1 bit"
        );
    }
    if (vec.size() != idiv64_ceil_fast(bits)) {
        throw std::domain_error(
            "APyInt vector initialization needs propriate vector size"
        );
    }
    twos_complement_overflow();
}

/*
 * Methods
 */

void APyFixed::twos_complement_overflow() noexcept
{
    unsigned bits_last_word = _bits & 0x3F;
    if (bits_last_word) {
        // Exploit signed arithmetic right-shift to perform two's complement overflow
        unsigned shft_amnt = 64-bits_last_word;
        _data.back() = int64_t(uint64_t(_data.back()) << (shft_amnt)) >> (shft_amnt);
    }
}

/*
 * Binary operators
 */
bool APyFixed::operator==(const APyFixed &rhs) const
{


    
    // The integers are equal
    return true;
}

/*
 * Unariy operators
 */
APyFixed APyFixed::operator-() const {
    APyFixed result(_bits+1, _int_bits+1);
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
void APyFixed::increment_lsb() noexcept
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

// APyFixed &APyFixed::operator=(const APyFixed &rhs) const
// {
//     throw NotImplementedException();
// }

void APyFixed::from_bitstring(const std::string &str)
{
    (void) str;
    throw NotImplementedException();
}

void APyFixed::from_vector(const std::vector<int64_t> &new_vector)
{
    if (new_vector.size() != this->vector_size()) {
        throw std::domain_error("Vector size miss-match");
    }
    _data = new_vector;
    twos_complement_overflow();
}

/*
 * Conversion to string functions
 */

std::string APyFixed::to_string_dec() const {

    // Print absolute value of number, and conditionally append a minus sign
    APyFixed abs_val(_bits+1, _int_bits+1);
    abs_val = is_negative() ? -(*this) : *this;

    // Conversion of data to BCD
    std::vector<uint8_t> bcd_list = double_dabble(abs_val._data);
    auto bcd_binary_point = 0;
    auto num_prev_bcds = bcd_list.size();

    if (int_bits() < bits()) {
        // Reverse order of BCD list to most-significant-bcd first, so that bcd_div2()
        // can append to bcd_list from the back.
        std::reverse(bcd_list.begin(), bcd_list.end());

        for (int i=0; i<bits()-int_bits(); i++) {
            bcd_div2(bcd_list);
            if (bcd_list.size() > num_prev_bcds) {
                bcd_binary_point++;
                num_prev_bcds = bcd_list.size();
            }
        }

        // Reverse order of BCD list (back) to least-significant-bcd first
        std::reverse(bcd_list.begin(), bcd_list.end());

        // Trim unnecessary zeros at start of BCD number that might be occur during
        // bcd_div2(). The first zero after the decimal point (of value 10^0) is never
        // trimmed.
        while (bcd_list.size() > std::size_t(bcd_binary_point+1)) {
            if (bcd_list.back() != 0) {
                // Nothing more to trim, break out of loop
                break;
            }
            bcd_list.pop_back();
        }
    }

    // Multiply BCD number by two
    for (int i=0; i<int_bits()-bits(); i++) {
        bcd_mul2(bcd_list);
    }

    // Convert BCD number to characters in a string and return
    std::string result = is_negative() ? "-" : "";
    for (int i=bcd_list.size()-1; i >= 0; i--) {
        result.push_back( static_cast<char>(bcd_list[i] + 0x30) );
        if (bcd_binary_point && i == bcd_binary_point) {
            result.append(".");
        }
    }
    return result;
}

std::string APyFixed::to_string_hex() const 
{
    throw NotImplementedException();
}

std::string APyFixed::to_string_oct() const 
{
    throw NotImplementedException();
}

std::string APyFixed::repr() const {
    return "fx<" + std::to_string(_bits) + ", " + std::to_string(_int_bits) + ">(" + to_string() + ")";
}
