/*
 * APyFixed: Dynamic arbitrary fixed-point data type.
 */

#include "apy_fixed.h"
#include "apy_util.h"

#include <algorithm>   // std::copy, std::max, std::transform, std::fill, std::fill_n
                       // std::remove, std::copy, std::for_each
#include <cstddef>     // std::size_t
#include <functional>  // std::bit_not
#include <regex>       // std::regex
#include <iterator>    // std::back_inserter
#include <stdexcept>   // std::domain_error
#include <string>      // std::string
#include <vector>      // std::vector, std::swap

// GMP should be included after all other includes
#include <gmp.h>


/* ********************************************************************************** *
 *                                Constructors                                        *
 * ********************************************************************************** */

// Zero-initializing constructor
APyFixed::APyFixed(int bits, int int_bits) :
    _bits{bits},
    _int_bits{int_bits},
    _data(bits_to_limbs(bits))
{
    _constructor_sanitize_bits();
}

// Construct from a decimal string
APyFixed::APyFixed(int bits, int int_bits, const char *str, int base) :
    APyFixed(bits, int_bits)
{
    switch (base) {
        case 8:  from_string_oct(str); break;
        case 10: from_string_dec(str); break;
        case 16: from_string_hex(str); break;
        default: throw std::domain_error("Unknown numberic base");
    }
    twos_complement_overflow();
}

// Underlying vector iterator-based constructor
template <typename _ITER>
APyFixed::APyFixed(int bits, int int_bits, _ITER begin, _ITER end) :
    _bits{bits},
    _int_bits{int_bits},
    _data(begin, end)
{
    _constructor_sanitize_bits();

    if (std::distance(begin, end) <= 0) {
        throw std::domain_error(
            "APyInt vector initialization needs propriate vector size"
        );
    } else if (std::size_t(std::distance(begin, end)) != bits_to_limbs(bits)) {
        throw std::domain_error(
            "APyInt vector initialization needs propriate vector size"
        );
    }

    // Two's-complements overflow bits outside of the range
    twos_complement_overflow();
}


// Construction from std::vector<mp_limb_t>
APyFixed::APyFixed(int bits, int int_bits, const std::vector<mp_limb_t> &vec) :
    APyFixed(bits, int_bits, vec.begin(), vec.end()) {}

// Construction from std::vector<mp_limb_signed_t>
APyFixed::APyFixed(int bits, int int_bits, const std::vector<mp_limb_signed_t> &vec) :
    APyFixed(bits, int_bits, vec.begin(), vec.end()) {}


/* ********************************************************************************** *
 *                         Public member functions (methods)                          *
 * ********************************************************************************** */

void APyFixed::twos_complement_overflow() noexcept
{
    unsigned bits_last_word = _bits & (_LIMB_SIZE_BITS-1);
    if (bits_last_word) {
        // Exploit signed arithmetic right-shift to perform two's complement overflow
        unsigned shft_amnt = _LIMB_SIZE_BITS - bits_last_word;
        _data.back() = mp_limb_signed_t(_data.back() << (shft_amnt)) >> (shft_amnt);
    }
}

/*
 * Binary operators
 */

APyFixed APyFixed::operator+(const APyFixed &rhs) const
{
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());

    APyFixed result(res_int_bits+res_frac_bits, res_int_bits);
    std::vector<mp_limb_t> operand_shifted;

    if (frac_bits() < rhs.frac_bits()) {
        // Right-hand side (rhs) has more fractional bits.
        _normalize_binary_points(result, operand_shifted, rhs, *this);
    } else {  // frac_bits() >= rhs.frac_bits() 
        // Left-hand side (*this) has more fractional bits.
        _normalize_binary_points(result, operand_shifted, *this, rhs);
    }

    // Add with carry and return
    mpn_add_n(
        &result._data[0],     // dst
        &result._data[0],     // src1
        &operand_shifted[0],  // src2
        result.vector_size()  // limb vector length
    );
    return result;
}

APyFixed APyFixed::operator-(const APyFixed &rhs) const
{
    const int res_int_bits = std::max(rhs.int_bits(), int_bits()) + 1;
    const int res_frac_bits = std::max(rhs.frac_bits(), frac_bits());

    APyFixed result(res_int_bits+res_frac_bits, res_int_bits);
    std::vector<mp_limb_t> operand_shifted;

    bool swap_operand;
    if (frac_bits() < rhs.frac_bits()) {
        // Right-hand side (rhs) has more fractional bits.
        _normalize_binary_points(result, operand_shifted, rhs, *this);
        swap_operand = false;
    } else {  // frac_bits() >= rhs.frac_bits() 
        // Left-hand side (*this) has more fractional bits.
        _normalize_binary_points(result, operand_shifted, *this, rhs);
        swap_operand = true;
    }

    // Add with carry and return
    mpn_sub_n(
        &result._data[0],                                       // dst
        swap_operand ? &result._data[0] : &operand_shifted[0],  // src1
        swap_operand ? &operand_shifted[0] : &result._data[0],  // src2
        result.vector_size()                                    // limb vector length
    );
    return result;
}

APyFixed APyFixed::operator*(const APyFixed &rhs) const
{
    const int res_int_bits = int_bits() + rhs.int_bits();
    const int res_frac_bits = frac_bits() + rhs.frac_bits();
    std::vector<mp_limb_t> abs_operand1 = _unsigned_abs();
    std::vector<mp_limb_t> abs_operand2 = rhs._unsigned_abs();
    APyFixed result(res_int_bits+res_frac_bits, res_int_bits);
    bool sign_product = is_negative() ^ rhs.is_negative();

    // mpn_mul requires the limb vector length of the first operand to be greater than
    // or equally long as the limb vector length of the second operand. Simply swap the
    // operands (essentially a free operation in C++) if this is not the case
    if (abs_operand1.size() < abs_operand2.size()) {
        std::swap(abs_operand1, abs_operand2);
    }

    mpn_mul(
        &result._data[0],     // dst
        &abs_operand1[0],     // src1
        abs_operand1.size(),  // src1 limb vector length
        &abs_operand2[0],     // src2
        abs_operand2.size()   // src2 limb vector length
    );

    // Handle sign
    if (sign_product) {
        result._data = result._non_extending_negate();
    }
    return result;
}


/*
 * Unariy operators
 */
 APyFixed APyFixed::operator-() const
{
    // Invert all bits of *this, possibly append sign to new limb, and increment lsb
    APyFixed result(_bits+1, _int_bits+1);
    std::transform(_data.cbegin(), _data.cend(), result._data.begin(), std::bit_not{});
    if (result.vector_size() > vector_size()) {
        mp_limb_t sign = _data.back() & (mp_limb_t(1) << (_LIMB_SIZE_BITS - 1));
        result._data.back() = sign ? 0 : mp_limb_signed_t(-1);
    }
    result.increment_lsb();
    return result;
}

// Increment the LSB without making the fixed-point number wider. Returns carry out
mp_limb_t APyFixed::increment_lsb() noexcept
{
    return mpn_add_1(
        &_data[0],      // dst
        &_data[0],      // src1
        vector_size(),  // limb vector length
        1               // src2
    );
}

/*
 * Utility functions
 */

void APyFixed::from_vector(const std::vector<mp_limb_t> &new_vector)
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

    if (frac_bits() > 0) {
        // Reverse order of BCD list to most-significant-bcd first, so that bcd_div2()
        // can append to bcd_list from the back.
        std::reverse(bcd_list.begin(), bcd_list.end());

        for (int i=0; i<frac_bits(); i++) {
            bcd_div2(bcd_list);
            if (bcd_list.size() > num_prev_bcds) {
                bcd_binary_point++;
                num_prev_bcds = bcd_list.size();
            }
        }

        // Reverse order of BCD list (back) to least-significant-bcd first
        std::reverse(bcd_list.begin(), bcd_list.end());

        // Trim unnecessary zeros at start of BCD number that might be occur during
        // bcd_div2(). The first zero, left of the decimal point (of value 10^0), is
        // never trimmed.
        while (bcd_list.size() > std::size_t(bcd_binary_point+1)) {
            if (bcd_list.back() != 0) {
                // Nothing more to trim, break out of loop
                break;
            }
            bcd_list.pop_back();
        }
    }

    // Multiply BCD number by two (if it has negative number of fractional bits)
    for (int i=0; i< -frac_bits(); i++) {
        bcd_mul2(bcd_list);
    }

    // Convert BCD number to characters in a string and return
    std::string result = is_negative() ? "-" : "";
    for (int i=bcd_list.size()-1; i >= 0; i--) {
        result.push_back( static_cast<char>(bcd_list[i] + 0x30) );
        if (bcd_binary_point && i == bcd_binary_point) {
            result.push_back('.');
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

void APyFixed::from_string_dec(const std::string &str)
{
    // Trim the string from leading and trailing whitespace
    std::string str_trimmed = string_trim_whitespace(str);

    // Check the validity as a decimal string
    if ( !is_valid_decimal_numeric_string(str_trimmed) ) {
        throw std::domain_error("Not a valid decimal numeric string");
    }

    // Test if negative. If so, remove the negative sign from the string.
    // `is_valid_decimal_numeric_string()` makes sure that str_trimmed[0] is valid.
    bool is_negative = str_trimmed.front() == '-';
    if (is_negative) {
        str_trimmed.erase(0, 1);
    }

    // Trim leading and trailing zeros that don't affect the numeric value of the
    // decimal number
    str_trimmed = string_trim_zeros(str_trimmed);

    // Find the binary point of the trimmed string and remove it from the string
    std::size_t binary_point_dec = str_trimmed.find('.');
    binary_point_dec = (binary_point_dec == std::string::npos) ? 0 : binary_point_dec;
    str_trimmed.erase(
        std::remove(str_trimmed.begin(), str_trimmed.end(), '.'),
        str_trimmed.cend()
    );

    // Copy characters (from back) of the trimmed string into a BCD list
    std::vector<uint8_t> bcd_list;
    std::for_each(str_trimmed.crbegin(), str_trimmed.crend(),
        [&](char c){ bcd_list.push_back(c - 0x30); }
    );

    // Multiply BCD number by 2^(frac_bits() + 1) (extra bit for rounding)
    auto bcd_list_size_prev = bcd_list.size();
    for (int i=0; i<frac_bits() + 1; i++) {
        bcd_mul2(bcd_list);
    }

    // Reverse the order of the BCD list (to MSB first)
    std::reverse(bcd_list.begin(), bcd_list.end());

    // Remove elements after decimal dot
    if (binary_point_dec) {
        bcd_list.erase(
            bcd_list.end() - (bcd_list_size_prev-binary_point_dec), bcd_list.end()
        );
    }

    // Reverse double-dabble algorithm (BCD -> binary)
    std::vector<mp_limb_t> data = reverse_double_dabble(bcd_list);

    // Round the data
    mpn_add_1(
        &data[0],     // dst
        &data[0],     // src1
        data.size(),  // limb vector length
        1             // src2
    );
    mpn_rshift(
        &data[0],     // dst
        &data[0],     // src
        data.size(),  // limb vector length
        1             // shift amount
    );

    // Adjust limb vector if negative fractional bits are present
    if (frac_bits() + 1 < 0) {
        limb_vector_asr(data, -(frac_bits() + 1));
    }

    // Copy the data into the result vector
    _data = data;
    _data.resize(bits_to_limbs(bits()));
    if (is_negative) {
        _data = _non_extending_negate();
    }

    // Two's complement overflow and we're done
    twos_complement_overflow();
}

void APyFixed::from_string_hex(const std::string &str)
{
    (void) str;
    throw NotImplementedException();
}

void APyFixed::from_string_oct(const std::string &str)
{
    (void) str;
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


/* ********************************************************************************** *
 *                           Private member functions                                 *
 * ********************************************************************************** */

// Sanitize the _bits and _int_bits parameters
void APyFixed::_constructor_sanitize_bits() const
{
    if (_bits <= 0) {
        throw std::domain_error(
            "APyInt needs a positive integer bit-size of at-least 1 bit"
        );
    }
}

// Sign preserving automatic size extending arithmetic left shift
std::vector<mp_limb_t> APyFixed::_data_asl(unsigned shift_val) const
{
    if (shift_val == 0) {
        return _data;
    }

    // Perform the left-shift
    unsigned vec_skip_val  = shift_val/_LIMB_SIZE_BITS;
    unsigned bit_shift_val = shift_val%_LIMB_SIZE_BITS;
    std::vector<mp_limb_t> result(bits_to_limbs(bits() + shift_val), 0);
    std::copy(_data.cbegin(), _data.cend(), result.begin()+vec_skip_val);
    mpn_lshift(
        &result[0],     // dst
        &result[0],     // src
        result.size(),  // limb vector length
        bit_shift_val   // shift amount
    );

    // Append sign bits for "arithmetic" shift
    if ( (bits()+shift_val)%_LIMB_SIZE_BITS > 0 ) {
        // Arithmetic right-shift
        mp_limb_t sign_int = mp_limb_signed_t(_data.back()) >> (_LIMB_SIZE_BITS - 1);
        mp_limb_t or_mask = ~((mp_limb_t(1) << (bits()+shift_val)%_LIMB_SIZE_BITS) - 1);
        result.back() |= or_mask & sign_int;
    }
    
    return result;
}


// Prepare for binary arithmetic by aligning the binary points of two limb vectors.
// The limbs of the first operand (`operand1`) are copied into the limb vector of
// `result` and vector sign extended. The limbs of the second operand (`operand2`)
// are vector-shifted by `operand1.frac_bits()` - `operand2.frac_bits()` bits to the
// left and copied into the `operand_shifted` limb vector.
// Assumptions when calling this method:
//   * `result` is already initialized with a propriate vector limb size
//   * `operand_shifted` is *not* initialized, i.e., it's an empty (zero element)
//      vector
//   * the number of fractional bits in `operand1` is greater than that of `operand2`
void APyFixed::_normalize_binary_points(
    APyFixed &result,
    std::vector<mp_limb_t> &operand_shifted,
    const APyFixed &operand1,
    const APyFixed &operand2
) const {
    std::copy(operand1._data.cbegin(), operand1._data.cend(), result._data.begin());
    operand_shifted = operand2._data_asl(operand1.frac_bits() - operand2.frac_bits());

    // Vector sign-extend the `result` limb vector
    mp_limb_t s_ext = mp_limb_signed_t(operand1._data.back()) >> (_LIMB_SIZE_BITS - 1);
    std::fill(result._data.begin() + operand1.vector_size(), result._data.end(), s_ext);

    // Vector sign-extend the `operand_shifted` limb vector
    std::fill_n(
        std::back_inserter(operand_shifted),
        result.vector_size() - operand_shifted.size(),
        mp_limb_signed_t(operand_shifted.back()) >> (_LIMB_SIZE_BITS - 1)
    );
}


// Retrieve a limb vector with the negated value from this APyFixed type. This member 
// function does not extend the size of the result, unlike `APyFixed::operator-()`,
// that extends the result with one bit.
std::vector<mp_limb_t> APyFixed::_non_extending_negate() const
{
    // Invert all bits and increment the lsb
    APyFixed result(bits(), 0);
    std::transform(_data.begin(), _data.end(), result._data.begin(), std::bit_not{});
    result.increment_lsb();
    return result._data;
}


// Get the absolute value of the number in the limb vector. This method does not
// extend the resulting limb vector to make place for an additional bit. Instead, it
// relies on the user knowing that the number in the vector is now unsigned.
std::vector<mp_limb_t> APyFixed::_unsigned_abs() const
{
    return is_negative() ? _non_extending_negate() : _data;
}

