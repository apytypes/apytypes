/*
 * Formatting functions for converting APyTypes internal number representations to
 * base-10 strings.
 */

#ifndef _APYTYPES_FORMAT_H
#define _APYTYPES_FORMAT_H

#include "apytypes_fwd.h"
#include "apytypes_intrinsics.h"
#include "apytypes_mp.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <iterator>
#include <regex>
#include <vector>

#include <fmt/format.h>

//! The code in this header depends on limbs being 64-bits or 32-bits long
static_assert(APY_LIMB_SIZE_BITS == 64 || APY_LIMB_SIZE_BITS == 32);

//! Decimal base for conversion between binary and decimal
static constexpr apy_limb_t DECIMAL_BASE = (APY_LIMB_SIZE_BITS == 64)
    ? 10000000000000000000ULL // 64-bit limb -> base: 10^19
    : 1000000000ULL;          // 32-bit limb -> base: 10^9

//! Number of digits in the decimal base
static constexpr unsigned DECIMAL_BASE_DIGITS = (APY_LIMB_SIZE_BITS == 64) ? 19 : 9;

//! Divide limb-vector by `d` in-place and return the remainder.
template <typename RANDOM_ACCESS_ITERATOR_INOUT>
[[maybe_unused, nodiscard]] static APY_INLINE apy_limb_t divmod_small(
    RANDOM_ACCESS_ITERATOR_INOUT begin, RANDOM_ACCESS_ITERATOR_INOUT end, apy_limb_t d
)
{
    assert(d != 0);

    __uint128_t rem { 0 };
    for (auto it = end; it-- != begin;) {
        __uint128_t cur = (rem << APY_LIMB_SIZE_BITS) | __uint128_t(*it);
        *it = apy_limb_t(cur / d);
        rem = cur % d;
    }
    return static_cast<apy_limb_t>(rem);
}

//! Find all consecutive trailing zero limbs in the iterator pair [ `begin`, `end` ).
//! This function returns the forward iterator `it` for which the range [ `it`, `end` )
//! are all zero. Returns `begin` if all limbs in the range are zero. Returns `end` if
//! no limbs in the range are zero.
template <typename RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused, nodiscard]] static APY_INLINE RANDOM_ACCESS_ITERATOR_IN
find_trailing_zero_limbs(RANDOM_ACCESS_ITERATOR_IN begin, RANDOM_ACCESS_ITERATOR_IN end)
{
    auto is_zero = [](auto limb) { return limb == 0; };
    auto it = std::find_if_not(
        std::make_reverse_iterator(end), std::make_reverse_iterator(begin), is_zero
    );
    return it.base();
}

// Convert a limb vector to a decimal vector in base-'DECIMAL_BASE'
template <typename RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused, nodiscard]] static APY_INLINE std::vector<apy_limb_t>
to_decimal(RANDOM_ACCESS_ITERATOR_IN begin, RANDOM_ACCESS_ITERATOR_IN end)
{
    assert(begin >= end);

    // Find the first zero limb among all ranges of zero limbs
    auto first_zero_limb = find_trailing_zero_limbs(begin, end);
    if (std::distance(begin, first_zero_limb) <= 0) {
        // No significant bits in the input, return early
        return { 0 };
    }

    // Resulting decimal vector and a working limb vector
    std::vector<apy_limb_t> result {};
    std::vector<apy_limb_t> work(begin, first_zero_limb);

    // Compute the decimal expansion
    while (work.size()) {
        result.push_back(divmod_small(std::begin(work), std::end(work), DECIMAL_BASE));

        // Remove any new trailing zero limbs
        auto zero_it = find_trailing_zero_limbs(std::begin(work), std::end(work));
        work.erase(zero_it, std::end(work));
    }

    return result;
}

template <typename RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused, nodiscard]] static std::string
to_decimal_string(RANDOM_ACCESS_ITERATOR_IN begin, RANDOM_ACCESS_ITERATOR_IN end)
{
    std::string res {};
    std::vector<apy_limb_t> dec = to_decimal(begin, end);

    // Most significant limb has no zero padding. All other limbs are padded with zeros
    res += fmt::format("{}", dec.back());
    for (auto it = std::crbegin(dec) + 1; it != std::crend(dec); ++it) {
        res += fmt::format("{:0>{}}", *it, DECIMAL_BASE_DIGITS);
    }

    return res;
}

////////////////

//! Count the number of significant nibbles in a single unsigned `apy_limb_t`
template <typename INT_TYPE>
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t nibble_width(INT_TYPE x)
{
    std::size_t bits = bit_width(x);
    if (bits % BITS_PER_NIBBLE == 0) {
        return bits / BITS_PER_NIBBLE;
    } else {
        return bits / BITS_PER_NIBBLE + 1;
    }
}

//! Convert an integer array `std::vector<apy_limb_t>` into a nibble list. The nibble
//! list contains least significant nibble first. Any zero nibbles at the end of the
//! result are trimmed. Parameter `len` can be used to indicate the intended length of
//! the nibble list. Zero nibbles within the intended length are not trimmed.
[[maybe_unused, nodiscard]] static APY_INLINE std::vector<std::uint8_t>
to_nibble_list(const std::vector<apy_limb_t>& data_array, std::size_t len = 0)
{
    std::vector<std::uint8_t> result {};
    for (apy_limb_t data : data_array) {
        for (unsigned i = 0; i < NIBBLES_PER_LIMB; i++) {
            result.push_back(std::uint8_t((data >> (BITS_PER_NIBBLE * i)) & 0x0F));
        }
    }

    // Remove zero-elements from the end
    auto is_non_zero = [](auto n) { return n != 0; };
    auto non_zero_it = std::find_if(result.rbegin(), result.rend() - len, is_non_zero);
    result.erase(non_zero_it.base(), result.end());

    // Return result
    return result.size() > 0 ? result : std::vector<std::uint8_t> { 0 };
}

//! Convert a nibble list into limb vector. The nibble list is assumed to have least
//! significant nibble first.
[[maybe_unused, nodiscard]] static APY_INLINE std::vector<apy_limb_t>
from_nibble_list(const std::vector<std::uint8_t>& nibble_list)
{
    // Compute the total number of limbs in the result vector
    std::size_t limbs = nibble_list.size() / NIBBLES_PER_LIMB;
    limbs += nibble_list.size() % NIBBLES_PER_LIMB != 0 ? 1 : 0;

    // Insert one nibble to the limb vector at a time
    std::vector<apy_limb_t> result(limbs, 0);
    for (std::size_t limb_i = 0; limb_i < result.size(); limb_i++) {
        apy_limb_t limb = 0;
        for (std::size_t nbl_i = 0; nbl_i < NIBBLES_PER_LIMB; nbl_i++) {
            auto i = limb_i * NIBBLES_PER_LIMB + nbl_i;
            if (i >= nibble_list.size()) {
                break;
            }
            limb |= (apy_limb_t(nibble_list[i]) & 0xF) << (nbl_i * BITS_PER_NIBBLE);
        }
        result[limb_i] = limb;
    }
    return result;
}

//! Double-Dabble helper class with appropriate methods for performing double-dabble and
//! the reverse double-dabble algorithm.
struct DoubleDabbleList {

    //! Mask with a bit in every position where a nibble starts
    static constexpr apy_limb_t _NIBBLE_MASK = APY_LIMB_SIZE_BITS == 64
        ? apy_limb_t(0x1111111111111111) // 64-bit architecture
        : apy_limb_t(0x11111111);        // 32-bit architecture

    std::vector<apy_limb_t> data { 0 };

    //! Do one iteration of double (double-dabble)
    void do_double(apy_limb_t new_bit)
    {
        // Perform a single bit left shift (double)
        if (apy_inplace_left_shift(data.begin(), data.end(), 1)) {
            data.push_back(1);
        }
        if (new_bit) {
            data[0] |= 0x1;
        }
    }

    //! Do one iteration of dabble (double-dabble)
    void do_dabble()
    {
        for (auto& l : data) {
            // Add 3 to each nibble GEQ 5
            apy_limb_t dabble_mask
                = (((l | l >> 1) & (l >> 2)) | (l >> 3)) & _NIBBLE_MASK;
            l += (dabble_mask << 1) | dabble_mask;
        }
    }

    //! Do one iteration of reverse double (reverse double-dabble)
    void do_reverse_double(apy_limb_t& limb_out)
    {
        limb_out |= apy_inplace_right_shift(data.begin(), data.end(), 1);
    }

    //! Do one iteration of reverse dabble (reverse double-dabble)
    void do_reverse_dabble()
    {
        for (auto& l : data) {
            // Subtract 3 from each nibble GEQ 8
            apy_limb_t dabble_mask = (l >> 3) & _NIBBLE_MASK;
            l -= (dabble_mask << 1) | dabble_mask;
        }
    }
};

//! Double-dabble algorithm for binary->BCD conversion
[[maybe_unused, nodiscard]] static APY_INLINE std::vector<apy_limb_t>
double_dabble(std::vector<apy_limb_t> nibble_data)
{
    if (!nibble_data.size()) {
        return {};
    }

    // Remove zero elements from the back until first non-zero element is found
    // (keep at least one zero at the start)
    auto is_non_zero = [](auto n) { return n != 0; };
    nibble_data.erase(
        std::find_if(nibble_data.rbegin(), nibble_data.rend() - 1, is_non_zero).base(),
        nibble_data.end()
    );

    // Double-dabble algorithm begin
    DoubleDabbleList bcd_list {};
    const auto nibbles_last_limb = nibble_width(nibble_data.back());
    const auto nibbles = nibbles_last_limb
        + APY_LIMB_SIZE_BITS / BITS_PER_NIBBLE * (nibble_data.size() - 1);
    const apy_limb_t new_bit_mask = nibbles_last_limb == 0
        ? apy_limb_t(1) << (APY_LIMB_SIZE_BITS - 1)
        : apy_limb_t(1) << (BITS_PER_NIBBLE * nibbles_last_limb - 1);
    for (std::size_t i = 0; i < BITS_PER_NIBBLE * nibbles; i++) {
        // Shift input data left once
        apy_limb_t new_bit = nibble_data.back() & new_bit_mask;
        apy_inplace_left_shift(nibble_data.begin(), nibble_data.end(), 1);

        // Do the double-dabble (dabble-double)
        bcd_list.do_dabble();
        bcd_list.do_double(new_bit);
    }
    return bcd_list.data;
}

//! Convert a BCD limb vector into a `std::string`.
[[maybe_unused, nodiscard]] static APY_INLINE std::string
bcds_to_string(const std::vector<apy_limb_t>& bcds)
{
    if (bcds.size() == 0) {
        return "";
    }

    // The first limb *should not* be padded with zeros
    std::string result = fmt::format("{:X}", *bcds.crbegin());

    // Any remaining limbs *should* must be zero padded
    for (auto limb_it = bcds.crbegin() + 1; limb_it != bcds.crend(); ++limb_it) {
        result += fmt::format("{:0{}X}", *limb_it, 2 * APY_LIMB_SIZE_BYTES);
    }
    return result;
}

//! Reverse double-dabble algorithm for BCD->binary conversion
[[maybe_unused, nodiscard]] static APY_INLINE std::vector<apy_limb_t>
reverse_double_dabble(const std::vector<std::uint8_t>& bcd_list)
{
    if (bcd_list.size() == 0) {
        return {};
    }

    std::size_t iteration = 0;
    std::vector<apy_limb_t> nibble_data {};
    DoubleDabbleList bcd { from_nibble_list(bcd_list) };
    apy_limb_t new_limb = 0;
    while (
        // As long as there are elements remaining in the BCD list, and we haven't
        // completed a multiple-of-four iterations
        std::any_of(bcd.data.begin(), bcd.data.end(), [](auto n) { return n != 0; })
        || iteration % BITS_PER_NIBBLE != 0) {
        // Right shift the nibble binary data
        if (iteration) {
            new_limb
                = apy_inplace_right_shift(nibble_data.begin(), nibble_data.end(), 1);
        }

        // Insert a new limb to the nibble data vector
        if (iteration % APY_LIMB_SIZE_BITS == 0) {
            nibble_data.insert(nibble_data.begin(), new_limb);
        }

        // Do the (reverse) double-dabble
        bcd.do_reverse_double(nibble_data.back());
        bcd.do_reverse_dabble();

        // Increment iteration counter
        iteration++;
    }

    // Right-adjust the data and return
    auto shft_val
        = (APY_LIMB_SIZE_BITS - (iteration % APY_LIMB_SIZE_BITS)) % APY_LIMB_SIZE_BITS;
    if (iteration && shft_val) {
        apy_inplace_right_shift(nibble_data.begin(), nibble_data.end(), shft_val);
    }

    return nibble_data.size() ? nibble_data : std::vector<apy_limb_t> { 0 };
}

//! Divide the number in a BCD limb vector by two.
[[maybe_unused]] static APY_INLINE void
bcd_limb_vec_div2(std::vector<apy_limb_t>& bcd_list)
{
    if (bcd_list.size() == 0) {
        return;
    }

    // Do a single vector right-shift and possibly prepend the new data
    auto shift_out = apy_inplace_right_shift(bcd_list.begin(), bcd_list.end(), 1);
    if (shift_out) {
        bcd_list.insert(bcd_list.begin(), shift_out);
    }

    // Subtract 3 from each nibble greater than or equal to 8
    for (auto& l : bcd_list) {
        apy_limb_t dabble_mask = (l >> 3) & DoubleDabbleList::_NIBBLE_MASK;
        l -= (dabble_mask << 1) | dabble_mask;
    }
}

//! Multiply the number in a BCD limb vector by two.
[[maybe_unused]] static APY_INLINE void
bcd_limb_vec_mul2(std::vector<apy_limb_t>& bcd_list)
{
    if (bcd_list.size() == 0) {
        return;
    }

    // Add 3 to each nibble greater than or equal to 5
    for (auto& l : bcd_list) {
        apy_limb_t dabble_mask
            = (((l | l >> 1) & (l >> 2)) | (l >> 3)) & DoubleDabbleList::_NIBBLE_MASK;
        l += (dabble_mask << 1) | dabble_mask;
    }

    // Multiply by two
    auto shift_out = apy_inplace_left_shift(bcd_list.begin(), bcd_list.end(), 1);
    if (shift_out) {
        bcd_list.push_back(shift_out);
    }
}

//! Multiply BCD vector (`std::vector<std::uint8_t>`) by two. The first element
//! (`front()`) in the vector is considered LSB.
[[maybe_unused]] static APY_INLINE void bcd_mul2(std::vector<std::uint8_t>& bcd_list)
{
    if (bcd_list.size() == 0) {
        return;
    }

    // Multiply each BCD by two
    bool carry_bit = false;
    for (auto& bcd : bcd_list) {
        if (bcd >= 5) {
            bcd += 3;
        }
        bcd <<= 1;
        bcd += static_cast<std::uint8_t>(carry_bit);
        carry_bit = bcd >= 16;
        bcd &= 0xF;
    }
    if (carry_bit) {
        bcd_list.push_back(1);
    }
}

//! Trim a string from leading whitespace
[[maybe_unused, nodiscard]] static APY_INLINE std::string
string_trim_leading_whitespace(const std::string& str)
{
    static const auto regex = std::regex("^\\s+");
    return std::regex_replace(str, regex, "");
}

//! Trim a string from trailing whitespace
[[maybe_unused, nodiscard]] static APY_INLINE std::string
string_trim_trailing_whitespace(const std::string& str)
{
    static const auto regex = std::regex("\\s+$");
    return std::regex_replace(str, regex, "");
}

//! Trim a string from leading and trailing whitespace
[[maybe_unused, nodiscard]] static APY_INLINE std::string
string_trim_whitespace(const std::string& str)
{
    return string_trim_leading_whitespace(string_trim_trailing_whitespace(str));
}

//! Test if a string is a valid numeric decimal string
[[maybe_unused, nodiscard]] static APY_INLINE bool
is_valid_decimal_numeric_string(const std::string& str)
{
    // Test with validity regex
    const char validity_regex[] = R"((^-?[0-9]+\.?[0-9]*$)|(^-?[0-9]*\.?[0-9]+)$)";
    static const auto regex = std::regex(validity_regex);
    return std::regex_match(str, regex);
}

//! Trim a string from unnecessary leading and trailing zeros, that don't affect
//! numeric value of the string. This function also attaches a zero to the string
//! if it starts with a decimal dot, and it removes the decimal dot if no digit
//! after it affects its value (e.g., 0.00 == 0).
[[maybe_unused, nodiscard]] static APY_INLINE std::string
string_trim_zeros(const std::string& str)
{
    std::string result = str;

    // Remove all leading zeros
    static const auto regex = std::regex("^0*");
    result = std::regex_replace(result, regex, "");

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

#endif // _APYTYPES_FORMAT_H
