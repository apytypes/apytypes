/*
 * APyFixed: Dynamic arbitrary fixed-point data type.
 */

#include "apy_fixed.h"
#include "apy_util.h"

#include <algorithm>
#include <cassert>
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
    _data(idiv64_ceil_fast(bits), uint64_t(0))  // Zero initialize data
{
    if (bits <= 0) {
        throw std::domain_error(
            "APyInt needs a positive integer bit-size of at-least 1 bit"
        );
    }
}

APyFixed::APyFixed(int bits, int int_bits, int value) :
    _bits{bits},
    _int_bits{int_bits},
    _data(idiv64_ceil_fast(bits), uint64_t(0))  // Zero initialize data
{
    if (bits <= 0) {
        throw std::domain_error(
            "APyInt needs a positive integer bit-size of at-least 1 bit"
        );
    }
    _data[0] = uint64_t(value);
    twos_complement_overflow();
}

APyFixed::APyFixed(int bits, int int_bits, const std::vector<uint64_t> &vec) :
    _bits{bits},
    _int_bits{int_bits},
    _data(vec.begin(), vec.end())
{
    if (bits <= 0) {
        throw std::domain_error(
            "APyInt needs a positive integer bit-size of at-least 1 bit"
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
        _data.back() = int64_t(_data.back() << (shft_amnt)) >> (shft_amnt);
    }
}

/*
 * Binary operators
 */
APyFixed APyFixed::operator+(const APyFixed &rhs) const
{
    const int rhs_frac_bits = rhs.bits() - rhs.int_bits();
    const int lhs_frac_bits = bits() - int_bits();
    const int res_frac_bits = std::max(rhs_frac_bits, lhs_frac_bits);
    const int res_int_bits = std::max(int_bits(), rhs.int_bits()) + 1;
    const int res_bits = res_int_bits + res_frac_bits;

    APyFixed result(res_bits, res_int_bits);
    std::vector<uint64_t> other_shifted;

    if (lhs_frac_bits <= rhs_frac_bits) {
        // Right-hand side (rhs) has more fractional bits
        std::copy(rhs._data.cbegin(), rhs._data.cend(), result._data.begin());
        other_shifted = _data_asl(rhs_frac_bits - lhs_frac_bits);
    } else {
        // Left-hand side (*this) has more fractional bits
        std::copy(_data.cbegin(), _data.cend(), result._data.begin());
        other_shifted = rhs._data_asl(lhs_frac_bits - rhs_frac_bits);
    }

    // Add with carry
    bool carry = false;
    for (unsigned i=0; i<result.vector_size(); i++) {
        // TODO: Look at...
        uint64_t term = other_shifted[i] + uint64_t(carry);
        result._data[i] += term;
        carry = result._data[i] < term;
    }
    return result;
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
        result._data[result.vector_size()-1] = not_sign ? 0 : int64_t(-1);
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
        carry = word < uint64_t(carry);
    }
}

/*
 * Utility functions
 */

void APyFixed::from_bitstring(const std::string &str)
{
    (void) str;
    throw NotImplementedException();
}

void APyFixed::from_vector(const std::vector<uint64_t> &new_vector)
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
    return std::string(
        "fx<"
        + std::to_string(_bits)
        + ", "
        + std::to_string(_int_bits)
        + ">("
        + to_string()
        + ")"
    );
}

/*
 * Private helper methods
 */

std::vector<uint64_t> APyFixed::_data_asl(unsigned shift_val) const
{
    int vector_size = idiv64_ceil_fast(bits() + shift_val);
    std::vector<uint64_t> result(vector_size, 0);

    unsigned vec_skip_val  = shift_val/64;
    unsigned bit_shift_val = shift_val%64;
    if (bit_shift_val > 0) {
        for (unsigned i=result.size()-1; i>vec_skip_val; i--) {
            unsigned src_idx = i-vec_skip_val;
            uint64_t a = src_idx   < _data.size() ? _data[src_idx]   : 0;
            uint64_t b = src_idx-1 < _data.size() ? _data[src_idx-1] : 0;
            result.at(i) = (a << bit_shift_val) | (b >> (64-bit_shift_val));
        }
    } else {
        for (unsigned i=result.size()-1; i>vec_skip_val; i--) {
            unsigned src_idx = i-vec_skip_val;
            uint64_t a = src_idx < _data.size() ? _data[src_idx] : 0;
            result[i] = a;
        }
    }
    result[vec_skip_val] = uint64_t(_data[0]) << bit_shift_val;

    // Append sign bits for "arithmetic" shift
    if ( (bits()+shift_val) % 64 > 0 ) {
        int64_t sign_int = int64_t(_data.back()) >> 63;  // Arithmetic right-shift
        int64_t or_mask = ~((uint64_t(1) << (bits()+shift_val)%64) - 1);
        result.back() |= or_mask & sign_int;
    }
    
    return result;
}
