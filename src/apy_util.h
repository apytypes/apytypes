/*
 * Utility functions
 */

#ifndef _APY_UTIL_H
#define _APY_UTIL_H

#include <algorithm>  // std::find
#include <cstddef>    // std::size_t
#include <iostream>
#include <iterator>
#include <regex>      // std::regex, std::regex_replace
#include <stdexcept>  // std::logic_error, std::domain_error
#include <vector>     // std::vector

// GMP should be included after all other includes
#include <gmp.h>


/*
 * Sizes of GMP limbs (underlying words)
 */
static constexpr std::size_t _LIMB_SIZE_BYTES = sizeof(mp_limb_t);
static constexpr std::size_t _LIMB_SIZE_BITS  = 8 * _LIMB_SIZE_BYTES;

/*
 * As the underlying GMP integer vector data type `mp_limb_t` can be either 32-bit or
 * 64-bit, depending on the target architecture, we here define a unified way to go from
 * a vector of `uint64_t` to a vector of `mp_limb_t`, irrespective of the `mp_limb_t`
 * size.
 */
static inline std::vector<mp_limb_t> to_limb_vec(std::vector<uint64_t> vec)
{
    static_assert(
           sizeof(mp_limb_t) == 4  // 32-bit target architecture
        || sizeof(mp_limb_t) == 8  // 64-bit target architecture
    );

    if ( sizeof(mp_limb_t) == 4 ) {
        std::vector<mp_limb_t> result( 2*vec.size(), 0 );
        for (std::size_t i=0; i<vec.size(); i++) {
            result[2*i]     = mp_limb_t(vec[i] & 0xFFFFFFFF);
            result[2*i + 1] = mp_limb_t(vec[i] >> 32);
        }
        return result;
    } else {  // sizeof(mp_limb_t) == 8
        return vec;
    }
}

// Not implemented exception
class NotImplementedException : public std::logic_error
{
public:
    NotImplementedException() : std::logic_error("Not implemeted yet") {};
};

// Avaiable string converstion types
enum class STRING_TYPE {
    HEX,
    OCT,
    DEC,
};

// Quickly evaluate how many limbs are requiered to to store an n-bit word
static inline std::size_t bits_to_limbs(std::size_t bits) {
    if (bits%_LIMB_SIZE_BITS == 0) {
        return bits/_LIMB_SIZE_BITS;
    } else {
        return bits/_LIMB_SIZE_BITS + 1;
    }
}

// Convert a positive arbitrary size integer array (array mp_limb_t) to a nibble list.
// The nibble list contains least significant nibble first.
static inline std::vector<uint8_t> to_nibble_list(
    const std::vector<mp_limb_t> &data_array
) {
    constexpr std::size_t NIBBLES_PER_LIMB = 2*_LIMB_SIZE_BYTES;
    constexpr std::size_t BITS_PER_NIBBLE = 4;
    std::vector<uint8_t> result{};
    for (mp_limb_t data : data_array) {
        for (unsigned i=0; i<NIBBLES_PER_LIMB; i++) {
            result.push_back(uint8_t((data >> (BITS_PER_NIBBLE*i)) & 0x0F));
        }
    }

    // Remove zero-elements *from the end*
    auto is_non_zero = [](auto n) { return n != 0; };
    auto non_zero_it = std::find_if(result.rbegin(), result.rend(), is_non_zero);
    result.resize(std::distance(result.begin(), non_zero_it.base()));

    // Return result
    return result.size() > 0 ? result : std::vector<uint8_t>{ 0 };
}

// Convert a nibble list into a positive integer array (mp_limb_t). The nibble list is
// assumed to have most significant nibble first.
static inline std::vector<mp_limb_t> from_nibble_list(
    const std::vector<uint8_t> &nibble_list
) {
    constexpr std::size_t NIBBLES_PER_LIMB = 2*_LIMB_SIZE_BYTES;
    constexpr std::size_t BITS_PER_NIBBLE = 4;

    unsigned limbs = nibble_list.size()/NIBBLES_PER_LIMB;
    limbs += nibble_list.size()%NIBBLES_PER_LIMB != 0 ? 1 : 0;

    std::vector<mp_limb_t> result( limbs, 0 );
    for (std::size_t limb_i=0; limb_i<result.size(); limb_i++) {
        mp_limb_t limb = 0;
        for (std::size_t nbl_i=0; nbl_i<NIBBLES_PER_LIMB; nbl_i++) {
            int i = nibble_list.size() - 1 -(limb_i*NIBBLES_PER_LIMB + nbl_i);
            if (i < 0) {
                // Done reading all nibbles
                break;
            }
            limb |= (mp_limb_t(nibble_list[i]) & 0xF) << (nbl_i*BITS_PER_NIBBLE);
        }
        result[limb_i] = limb;
    }
    return result;
}

// Shift a nibble list left by one stage. Modifies the content of input `nibble_list`
// Assumes that the `back()` element of the input `nibble_list` is the most significant
// nibble.
static inline bool nibble_list_shift_left_once(std::vector<uint8_t> &nibble_list)
{
    if (nibble_list.size() == 0) {
        return false;
    }

    bool output_bit = nibble_list.back() >= 8;
    for (int i=nibble_list.size()-1; i>0; i--) {
        nibble_list[i] = (nibble_list[i] << 1) & 0xF;
        nibble_list[i] += nibble_list[i-1] >= 8 ? 1 : 0;
    }
    nibble_list[0] = (nibble_list[0] << 1) & 0xF;
    return output_bit;
}

// Shift a nibble list right by one stage. Modifies the content of input `nibble_list`.
// Assumes that the `back()` element of the input `nibble_list` is the least significant
// nibble.
static inline bool nibble_list_shift_right_once(std::vector<uint8_t> &nibble_list)
{
    if (nibble_list.size() == 0) {
        return false;
    }

    bool output_bit = nibble_list.back() & 0x1; 
    for (int i=nibble_list.size()-1; i>0; i--) {
        nibble_list[i] >>= 1;
        nibble_list[i] += nibble_list[i-1] & 0x1 ? 0x8 : 0x0;
    }
    nibble_list[0] >>= 1;
    return output_bit;
}

// Double-dabble algorithm for binary->BCD conversion
static inline std::vector<uint8_t> double_dabble(const std::vector<mp_limb_t> &data)
{
    if (data.size() == 0) {
        return {};
    }

    std::vector<uint8_t> nibble_list = to_nibble_list(data);  // LSNibble first
    std::vector<uint8_t> bcd_list{ 0 };
    for (std::size_t i=0; i<4*nibble_list.size(); i++) {
        bool insert_bit = nibble_list_shift_left_once(nibble_list);
        for (auto &bcd : bcd_list) {
            if (bcd >= 5) {
                bcd += 3;
            }
            bcd <<= 1;
            bcd += static_cast<uint8_t>(insert_bit);
            insert_bit = bcd >= 16;
            bcd &= 0xF;
        }
        if (insert_bit) {
            bcd_list.push_back(1);
        }
    }
    return bcd_list;
}

// Reverse double-dabble algorithm for BCD->binary conversion
static inline std::vector<mp_limb_t> reverse_double_dabble(
    std::vector<uint8_t> bcd_list
) {
    if (bcd_list.size() == 0) {
        return {};
    }

    std::vector<uint8_t> nibble_list{};  // MSNibble first
    int iteration=0;
    while (
        // As long as there are elements remaining in the BCD list and we haven't
        // completed a multiple-of-four iterations
        std::any_of(bcd_list.begin(), bcd_list.end(), [](auto n){ return n!=0; })
        || iteration % 4 != 0
    ) {
        // Shift BCD list to the right
        bool carry_bcd_to_nibble = nibble_list_shift_right_once(bcd_list);
        for (auto &bcd : bcd_list) {
            if (bcd >= 8) {
                bcd -= 3;
            }
        }

        // Append a new nibble every fourth iteration
        if (iteration % 4 == 0) {
            nibble_list.push_back(0);
        }
        nibble_list_shift_right_once(nibble_list);
        if (carry_bcd_to_nibble) {
            nibble_list.front() |= 0x8;
        }

        // Increment iteration counter
        iteration++;
    }
    return nibble_list.size() ? from_nibble_list(nibble_list) : std::vector<mp_limb_t>{0};
}

// Divide BCD number by two. First element in input array in considered MSB
static inline void bcd_div2(std::vector<uint8_t> &bcd_list)
{
    if (bcd_list.size() == 0) {
        return;
    }

    // Add a new least significant bcd?
    bool new_lsbcd = bool(bcd_list.back() & 0x1);

    // Division by two
    for (int i=bcd_list.size()-1; i>0; i--) {
        bcd_list[i] >>= 1;
        bcd_list[i] += bcd_list[i-1] & 0x1 ? 5 : 0;
    }
    bcd_list[0] >>= 1;

    // Add the new least significant *bcd*
    if (new_lsbcd) {
        bcd_list.push_back(0x5);
    }
}

// Multiply BCD number by two. First element in input array is considered LSB
static inline void bcd_mul2(std::vector<uint8_t> &bcd_list)
{
    if (bcd_list.size() == 0) {
        return;
    }

    // Multiply by two
    bool carry_bit = false;
    for (auto &bcd : bcd_list) {
        if (bcd >= 5) {
            bcd += 3;
        }
        bcd <<= 1;
        bcd += static_cast<uint8_t>(carry_bit);
        carry_bit = bcd >= 16;
        bcd &= 0xF;
    }
    if (carry_bit) {
        bcd_list.push_back(1);
    }

}

// Trim a string from leading whitespace
static inline std::string string_trim_leading_whitespace(const std::string &str)
{
    return std::regex_replace(str, std::regex("^\\s+"), "");
}

// Trim a string from trailing whitespace
static inline std::string string_trim_trailing_whitespace(const std::string &str)
{
    return std::regex_replace(str, std::regex("\\s+$"), "");
}

// Trim a string from leading and trailing whitespace
static inline std::string string_trim_whitespace(const std::string &str)
{
    return string_trim_leading_whitespace(string_trim_trailing_whitespace(str));
}

// Test if a string is a valid numeric decimal string
static inline bool is_valid_decimal_numeric_string(const std::string &str)
{
    // Test with validity regex
    const char validity_regex[] = R"((^-?[0-9]+\.?[0-9]*$)|(^-?[0-9]*\.?[0-9]+)$)";
    return std::regex_match(str, std::regex(validity_regex));
}

// Trim a string from unnecessary leading and trailing zeros, that don't affect numeric
// value of the string. This function also attaches a zero to the string if it starts
// with a decimal dot, and it removes the decimal dot if no digit after it affects it's
// value (e.g., 0.00 == 0).
static inline std::string string_trim_zeros(const std::string &str)
{
    std::string result = str;

    // Remove all leading zeros
    result = std::regex_replace(result, std::regex("^0*"), "");

    // Remove all trailing zeros, after a decimal dot
    while (result.find('.') != std::string::npos && result.back() == '0') {
        result.pop_back();
    }

    // Decimal point at the end?
    if (result.size() && result.back() == '.') {
        // Erase it
        result.pop_back();
    }

    // Decimal point in the start?
    if (result.size() && result.front() == '.') {
        // Append a zero
        result.insert(0, "0");
    }

    // Return the result
    return result.size() ? result : "0";
}

#endif
