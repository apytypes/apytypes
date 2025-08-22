/*
 * Utility functions
 */

#ifndef _APYTYPES_UTIL_H
#define _APYTYPES_UTIL_H

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/variant.h>

#include <fmt/format.h>

#include <algorithm>        // std::find, std::unique, etc...
#include <cstddef>          // std::size_t
#include <cstdint>          // std::int64_t
#include <functional>       // std::bit_not
#include <initializer_list> // std::initializer_list
#include <iterator>         // std::distance
#include <numeric>          // std::accumulate, std::multiplies
#include <optional>         // std::optional, std::nullopt
#include <regex>            // std::regex, std::regex_replace
#include <sstream>          // std::stringstream
#include <string>           // std::string
#include <tuple>            // std::tuple
#include <type_traits>      // std::is_same_v
#include <vector>           // std::vector

#include "apytypes_fwd.h"
#include "apytypes_mp.h"

/*!
 * Type trait removing const-volatile qualifiers and reference of `T`. Equivalent to the
 * C++20 trait `std::remove_cvref`.
 */
template <typename T> struct remove_cvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

/*!
 * Type trait removing const-volatile qualifiers and reference of `T`. Equivalent to the
 * C++20 trait `std::remove_cvref_t`.
 */
template <typename T> using remove_cvref_t = typename remove_cvref<T>::type;

/*!
 * Macro for splitting a single `std::uint64_t` into comma-separated limbs
 */
#if COMPILER_LIMB_SIZE == 64
#define UINT64_TO_LIMB(x) apy_limb_t(x)
#else
#define UINT64_TO_LIMB(x)                                                              \
    apy_limb_t(std::uint64_t(x)), apy_limb_t(std::uint64_t(x) >> 32)
#endif

/*
 * Not implemented exception
 */
class NotImplementedException : public std::domain_error {
public:
    NotImplementedException(std::optional<std::string> msg = std::nullopt)
        : std::domain_error(msg.value_or("Not implemented yet")) { };
};

//! Quickly evaluate how many limbs are required to store a `bits` bit word.
//! Undefined behaviour when `bits` is equal to zero.
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t
bits_to_limbs(std::size_t bits)
{
    static_assert(APY_LIMB_SIZE_BITS == 64 || APY_LIMB_SIZE_BITS == 32);
    if constexpr (APY_LIMB_SIZE_BITS == 64) {
        return ((bits - 1) >> 6) + 1;
    } else { /* APY_LIMB_SIZE_BITS == 32 */
        return ((bits - 1) >> 5) + 1;
    }
}

//! Count the number of significant limbs in unsigned limb vector
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t
significant_limbs(RANDOM_ACCESS_ITERATOR begin, RANDOM_ACCESS_ITERATOR end)
{
    auto is_non_zero = [](auto n) { return n != 0; };
    auto back_non_zero_it = std::find_if(
        std::reverse_iterator(end), std::reverse_iterator(begin), is_non_zero
    );
    return std::distance(begin, back_non_zero_it.base());
}

//! Return the number of leading zeros of a limb vector
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t
limb_vector_leading_zeros(RANDOM_ACCESS_ITERATOR begin, RANDOM_ACCESS_ITERATOR end)
{
    auto is_non_zero = [](auto n) { return n != 0; };
    auto rev_non_zero_it = std::find_if(
        std::reverse_iterator(end), std::reverse_iterator(begin), is_non_zero
    );
    std::size_t zero_limbs = std::distance(rev_non_zero_it.base(), end);
    if (rev_non_zero_it.base() == begin) {
        // All limbs are zero limbs
        return APY_LIMB_SIZE_BITS * zero_limbs;
    } else {
        // Some of the limbs are non-zero
        return APY_LIMB_SIZE_BITS * zero_limbs + leading_zeros(*rev_non_zero_it);
    }
}

//! Return the number of leading signs of a limb vector, minus one.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE std::size_t
limb_vector_leading_ones(RANDOM_ACCESS_ITERATOR begin, RANDOM_ACCESS_ITERATOR end)
{
    auto is_not_all_ones = [](auto n) { return n != apy_limb_t(-1); };
    auto rev_not_all_ones_it = std::find_if(
        std::reverse_iterator(end), std::reverse_iterator(begin), is_not_all_ones
    );
    std::size_t all_ones_limbs = std::distance(rev_not_all_ones_it.base(), end);
    if (rev_not_all_ones_it.base() == begin) {
        // All limbs are ones limbs
        return APY_LIMB_SIZE_BITS * all_ones_limbs;
    } else {
        // Some of the limbs are not all ones
        return APY_LIMB_SIZE_BITS * all_ones_limbs + leading_ones(*rev_not_all_ones_it);
    }
}

//! Quickly count the number of nibbles in an unsigned `apy_limb_t`
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

//! Convert a positive arbitrary size integer array (`std::vector<apy_limb_t>`) to a
//! nibble list. The nibble list contains least significant nibble first. Argument `len`
//! indicates the intended bcd length of the output. When set, no more than
//! `result.rend() - len` zeros will be removed.
[[maybe_unused, nodiscard]] static APY_INLINE std::vector<std::uint8_t>
to_nibble_list(const std::vector<apy_limb_t>& data_array, std::size_t len = 0)
{
    std::vector<std::uint8_t> result {};
    for (apy_limb_t data : data_array) {
        for (unsigned i = 0; i < NIBBLES_PER_LIMB; i++) {
            result.push_back(std::uint8_t((data >> (BITS_PER_NIBBLE * i)) & 0x0F));
        }
    }

    // Remove zero-elements *from the end*, but no more than `keep` elements
    auto is_non_zero = [](auto n) { return n != 0; };
    auto non_zero_it = std::find_if(result.rbegin(), result.rend() - len, is_non_zero);
    result.erase(non_zero_it.base(), result.end());

    // Return result
    return result.size() > 0 ? result : std::vector<std::uint8_t> { 0 };
}

//! Convert a nibble list into a positive integer array
//! (`std::vector<apy_limb_t>`). The nibble list is assumed to have least
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

//! Double-Dabble helper class with appropriate methods for performing the
//! double-dabble and reverse double-dabble algorithm.
struct DoubleDabbleList {

    //! Mask with a bit in every position where a nibble starts
    static constexpr apy_limb_t _NIBBLE_MASK = APY_LIMB_SIZE_BITS == 64
        ? 0x1111111111111111 // 64-bit architecture
        : 0x11111111;        // 32-bit architecture

    std::vector<apy_limb_t> data { 0 };

    //! Do one iteration of double (double-dabble)
    void do_double(apy_limb_t new_bit)
    {
        // Perform a single bit left shift (double)
        if (apy_inplace_left_shift(&data[0], data.size(), 1)) {
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
        limb_out |= apy_inplace_right_shift(&data[0], data.size(), 1);
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
        apy_inplace_left_shift(&nibble_data[0], nibble_data.size(), 1);

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
        || iteration % BITS_PER_NIBBLE != 0
    ) {
        // Right shift the nibble binary data
        if (iteration) {
            new_limb = apy_inplace_right_shift(&nibble_data[0], nibble_data.size(), 1);
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
        apy_inplace_right_shift(&nibble_data[0], nibble_data.size(), shft_val);
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
    auto shift_out = apy_inplace_right_shift(&bcd_list[0], bcd_list.size(), 1);
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
    auto shift_out = apy_inplace_left_shift(&bcd_list[0], bcd_list.size(), 1);
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

//! Perform arithmetic right shift on a limb vector.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE void limb_vector_asr(
    RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end, unsigned shift_amnt
)
{
    // Return early if no shift or no vector
    if (!shift_amnt) {
        return;
    }

    apy_limb_t sign_limb = apy_limb_signed_t(*(it_end - 1)) >> (APY_LIMB_SIZE_BITS - 1);
    std::size_t vec_size = std::distance(it_begin, it_end);
    unsigned limb_skip = shift_amnt / APY_LIMB_SIZE_BITS;
    if (limb_skip >= vec_size) {
        std::fill(it_begin, it_end, sign_limb);
        return; // early return
    } else if (limb_skip) {
        for (auto it = it_begin; it < it_end - limb_skip; ++it) {
            *it = *(it + limb_skip);
        }
        for (auto it = it_end - limb_skip; it < it_end; ++it) {
            *it = sign_limb;
        }
    }

    // Perform the in-limb shifting
    unsigned limb_shift = shift_amnt % APY_LIMB_SIZE_BITS;
    if (limb_shift) {
        apy_inplace_right_shift(
            &*it_begin, // dst/src
            vec_size,   // limb vector size
            limb_shift  // shift amount
        );

        // Sign extend the most significant bits
        if (sign_limb) {
            *(it_end - 1)
                |= ~((apy_limb_t(1) << (APY_LIMB_SIZE_BITS - limb_shift)) - 1);
        }
    }
}

//! Perform logical right shift on a limb vector.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE void limb_vector_lsr(
    RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end, unsigned shift_amnt
)
{
    // Return early if no shift or no vector
    if (!shift_amnt) {
        return;
    }

    std::size_t vec_size = std::distance(it_begin, it_end);
    unsigned limb_skip = shift_amnt / APY_LIMB_SIZE_BITS;
    if (limb_skip >= vec_size) {
        std::fill(it_begin, it_end, 0);
        return; // early return
    } else if (limb_skip) {
        for (auto it = it_begin; it < it_end - limb_skip; ++it) {
            *it = *(it + limb_skip);
        }
        for (auto it = it_end - limb_skip; it < it_end; ++it) {
            *it = 0;
        }
    }

    unsigned limb_shift = shift_amnt % APY_LIMB_SIZE_BITS;
    // Perform the in-limb shifting
    if (limb_shift) {
        apy_inplace_right_shift(
            &*it_begin, // dst/src
            vec_size,   // limb vector size
            limb_shift  // shift amount
        );
    }
}

//! Perform logical left shift on a limb vector.
template <class RANDOM_ACCESS_ITERATOR>
static APY_INLINE void limb_vector_lsl_inner(
    RANDOM_ACCESS_ITERATOR it_begin,
    RANDOM_ACCESS_ITERATOR it_end,
    unsigned int limb_skip,
    unsigned int limb_shift,
    unsigned int vec_size
)
{
    if (limb_skip) {
        for (auto it = it_end - 1; it != it_begin + limb_skip - 1; --it) {
            *it = *(it - limb_skip);
        }
        for (auto it = it_begin; it != it_begin + limb_skip; ++it) {
            *it = 0;
        }
    }

    // Perform the in-limb shifting
    if (limb_shift) {
        apy_inplace_left_shift(
            &*it_begin, // src/dst
            vec_size,   // limb vector size
            limb_shift  // shift amount
        );
    }
}

//! Perform logical left shift on a limb vector.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE void limb_vector_lsl(
    RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end, unsigned shift_amnt
)
{
    // Return early if no shift or no vector
    if (!shift_amnt) {
        return;
    }

    std::size_t vec_size = std::distance(it_begin, it_end);
    unsigned limb_skip = shift_amnt / APY_LIMB_SIZE_BITS;
    if (limb_skip >= vec_size) {
        std::fill(it_begin, it_end, 0);
        return; // early return
    }
    unsigned limb_shift = shift_amnt % APY_LIMB_SIZE_BITS;
    limb_vector_lsl_inner(it_begin, it_end, limb_skip, limb_shift, vec_size);
}

//! Test if positive integer value in limb vector is greater than or equal to a
//! non-negative power-of-two (>= 2 ^ `n` for unsigned `n`)
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE bool limb_vector_gte_pow2(
    RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end, unsigned n
)
{
    unsigned bit_idx = n % APY_LIMB_SIZE_BITS;
    unsigned limb_idx = n / APY_LIMB_SIZE_BITS;
    std::size_t n_limbs = std::distance(it_begin, it_end);

    if (limb_idx >= n_limbs) {
        // The power-of-two is outside of the limb vector range
        return false;
    }

    // In the first limb, test for `bit_idx` up-to `APY_LIMB_SIZE_BITS - 1`
    apy_limb_t mask = ~((apy_limb_t(1) << bit_idx) - 1);
    if (mask & it_begin[limb_idx]) {
        return true;
    }

    // In the remaining limbs, test if any bit at all is set
    for (std::size_t i = limb_idx + 1; i < n_limbs; i++) {
        if (it_begin[i]) {
            return true;
        }
    }

    return false;
}

//! Test if the two's complement value of `src1` is smaller than that of `src2`
template <class RANDOM_ACCESS_ITERATOR1, class RANDOM_ACCESS_ITERATOR2>
[[maybe_unused, nodiscard]] static APY_INLINE bool limb_vector_signed_less_than(
    RANDOM_ACCESS_ITERATOR1 src1, RANDOM_ACCESS_ITERATOR2 src2, std::size_t limbs
)
{
    static_assert(std::is_same_v<remove_cvref_t<decltype(*src1)>, apy_limb_t>);
    static_assert(std::is_same_v<remove_cvref_t<decltype(*src2)>, apy_limb_t>);
    assert(limbs > 0);

    if (src1[limbs - 1] != src2[limbs - 1]) {
        return apy_limb_signed_t(src1[limbs - 1]) < apy_limb_signed_t(src2[limbs - 1]);
    } else {
        for (std::size_t i = limbs - 1; i--;) {
            if (src1[i] != src2[i]) {
                bool is_negative = apy_limb_signed_t(src1[limbs - 1]) < 0;
                return is_negative
                    ? apy_limb_signed_t(src1[i]) < apy_limb_signed_t(src2[i])
                    : apy_limb_t(src1[i]) < apy_limb_t(src2[i]);
            }
        }
    }
    return false;
}

//! Add a power-of-two (2 ^ `n`) onto a limb vector. Return carry out.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE apy_limb_t limb_vector_add_pow2(
    RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end, unsigned n
)
{
    unsigned limb_idx = n / APY_LIMB_SIZE_BITS;
    auto limbs = std::distance(it_begin, it_end);
    if (limb_idx < limbs) {
        apy_limb_t term_limb = apy_limb_t(1) << (n % APY_LIMB_SIZE_BITS);
        return apy_inplace_addition_single_limb(
            &*(it_begin + limb_idx), // dst
            limbs - limb_idx,        // src1 limb length
            term_limb                // src2
        );
    }
    return 0;
}

//! Subtract a power-of-two (2 ^ `n`) from a limb vector. Return borrow.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE apy_limb_t limb_vector_sub_pow2(
    RANDOM_ACCESS_ITERATOR it_begin, RANDOM_ACCESS_ITERATOR it_end, unsigned n
)
{
    unsigned limb_idx = n / APY_LIMB_SIZE_BITS;
    auto limbs = std::distance(it_begin, it_end);
    if (limb_idx < limbs) {
        apy_limb_t term_limb = apy_limb_t(1) << (n % APY_LIMB_SIZE_BITS);
        return apy_inplace_subtraction_single_limb(
            &*(it_begin + limb_idx), // dst
            limbs - limb_idx,        // dst/src1 limb length
            term_limb                // src2
        );
    }
    return 0;
}

//! Return the `bits` specifier from user provided optional bit specifiers.
//! Throws `nb::value_error` if the resulting number of bits is less than or equal to
//! zero, or if not exactly two of three bit specifiers are set.
[[maybe_unused, nodiscard]] static APY_INLINE int bits_from_optional(
    std::optional<int> bits, std::optional<int> int_bits, std::optional<int> frac_bits
)
{
    int n_bit_spec = bits.has_value() + int_bits.has_value() + frac_bits.has_value();
    if (n_bit_spec != 2) {
        throw nanobind::value_error(
            "Fixed-point bit specification needs exactly two of three bit specifiers "
            "(`bits`, `int_bits`, `frac_bits`) set"
        );
    }

    int result = bits.has_value() ? *bits : *int_bits + *frac_bits;
    if (result <= 0) {
        throw nanobind::value_error(
            "Fixed-point bit specification needs a positive integer bit-size (>= 1 bit)"
        );
    }

    return result;
}

//! Return a two-tuple of ints (`bits`, `int_bits`) for a set of optional
//! bit-specifiers used in the casting context. Throws `nanobind::value_error` if
//! parameters do not add up, or if resulting `bits` is smaller than or equal to zero.
[[maybe_unused, nodiscard]] static APY_INLINE std::tuple<int, int>
bits_from_optional_cast(
    std::optional<int> bits,
    std::optional<int> int_bits,
    std::optional<int> frac_bits,
    int current_bits,
    int current_int_bits
)
{
    std::optional<std::tuple<int, int>> result = std::nullopt;
    int n_bit_spec = bits.has_value() + int_bits.has_value() + frac_bits.has_value();
    if (n_bit_spec == 1 && !bits.has_value()) {
        if (int_bits.has_value()) {
            result = { current_bits - current_int_bits + *int_bits, *int_bits };
        } else { /* frac_bits.has_value() */
            result = { current_int_bits + *frac_bits, current_int_bits };
        }
    } else if (n_bit_spec == 2) {
        if (bits.has_value()) {
            result = { *bits, int_bits.has_value() ? *int_bits : *bits - *frac_bits };
        } else { /* int_bits.has_value() && frac_bits.has_value() */
            result = { *int_bits + *frac_bits, *int_bits };
        }
    }

    if (!result.has_value() || std::get<0>(*result) <= 0) {
        throw nanobind::value_error(
            "Fixed-point casting bit specification needs exactly one of `int_bits` or "
            "`frac_bits` set, or exactly two of three specifiers (`bits`, `int_bits`, "
            "`frac_bits`) set"
        );
    }

    return *result;
}

//! Test if two's complement value in a limb vector is negative
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE bool limb_vector_is_negative(
    RANDOM_ACCESS_ITERATOR cbegin_it, RANDOM_ACCESS_ITERATOR cend_it
)
{
    (void)cbegin_it;
    return apy_limb_signed_t(*std::prev(cend_it)) < 0;
}

//! Test if the stored value of a limb vector is zero (all limbs are zero)
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE bool
limb_vector_is_zero(RANDOM_ACCESS_ITERATOR cbegin_it, RANDOM_ACCESS_ITERATOR cend_it)
{
    return std::all_of(cbegin_it, cend_it, [](auto n) { return n == 0; });
}

//! Reduce the first `n` bits in a limb vector over bitwise `or`. Return bool.
//! Unconditionally Return `false` when `n == 0`. Undefined behaviour if `n` is
//! greater than the number of bits in the limb vector.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE bool limb_vector_or_reduce(
    RANDOM_ACCESS_ITERATOR cbegin_it, RANDOM_ACCESS_ITERATOR cend_it, unsigned n
)
{
    (void)cend_it;
    const unsigned full_limbs = n / APY_LIMB_SIZE_BITS;

    // The full limbs can be reduced as full integers
    for (auto limb_it = cbegin_it; limb_it != cbegin_it + full_limbs; ++limb_it) {
        if (*limb_it != 0) {
            return true;
        }
    }

    const unsigned last_limb_bits = n % APY_LIMB_SIZE_BITS;
    // The last limb must be masked
    if (last_limb_bits) {
        apy_limb_t last_limb = *(cbegin_it + full_limbs);
        apy_limb_t limb_mask = (apy_limb_t(1) << last_limb_bits) - 1;
        if (last_limb & limb_mask) {
            return true;
        }
    }

    return false;
}

//! Test if the `n` -th bit (zero indexed) is set in a limb vector. Undefined behaviour
//! if bit `n` is located outside the limb vector.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused, nodiscard]] static APY_INLINE bool limb_vector_test_bit(
    RANDOM_ACCESS_ITERATOR cbegin_it, RANDOM_ACCESS_ITERATOR cend_it, unsigned n
)
{
    (void)cend_it;
    unsigned bit_idx = n % APY_LIMB_SIZE_BITS;
    unsigned limb_idx = n / APY_LIMB_SIZE_BITS;
    apy_limb_t mask = apy_limb_t(1) << bit_idx;
    apy_limb_t limb = cbegin_it[limb_idx];
    return mask & limb;
}

//! Set the `n` -th bit (zero indexed) of a limb vector to `bit`. Undefined behaviour if
//! bit `n` is located outside the limb vector.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE void limb_vector_set_bit(
    RANDOM_ACCESS_ITERATOR begin_it, RANDOM_ACCESS_ITERATOR end_it, unsigned n, bool bit
)
{
    (void)end_it;
    unsigned bit_idx = n % APY_LIMB_SIZE_BITS;
    unsigned limb_idx = n / APY_LIMB_SIZE_BITS;
    apy_limb_t bit_mask = apy_limb_t(1) << bit_idx;
    apy_limb_t bit_unmask = ~bit_mask;
    begin_it[limb_idx]
        = (begin_it[limb_idx] & bit_unmask) | (apy_limb_t(bit) << bit_idx);
}

//! Take the two's complement negative value of a limb vector and place onto `res_out`
template <class RANDOM_ACCESS_ITERATOR_IN, class RANDOM_ACCESS_ITERATOR_OUT>
[[maybe_unused]] static APY_INLINE apy_limb_t limb_vector_negate(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it,
    RANDOM_ACCESS_ITERATOR_IN cend_it,
    RANDOM_ACCESS_ITERATOR_OUT res_it
)
{
    return apy_negate(&*res_it, &*cbegin_it, std::distance(cbegin_it, cend_it));
}

//! Take the two's complement negative value of a limb vector inplace
template <class RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused]] static APY_INLINE apy_limb_t limb_vector_negate_inplace(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it, RANDOM_ACCESS_ITERATOR_IN cend_it
)
{
    return apy_inplace_negate(&*cbegin_it, std::distance(cbegin_it, cend_it));
}

//! Add an LSB to a limb vector inplace
template <class RANDOM_ACCESS_ITERATOR_IN>
[[maybe_unused]] static APY_INLINE apy_limb_t limb_vector_add_one_lsb_inplace(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it, RANDOM_ACCESS_ITERATOR_IN cend_it
)
{
    return apy_inplace_add_one_lsb(&*cbegin_it, std::distance(cbegin_it, cend_it));
}

//! Take the two's complement absolute value of a limb vector and place onto `res_out`.
//! Return `true` if the argument is negative, and `false` otherwise.
template <class RANDOM_ACCESS_ITERATOR_IN, class RANDOM_ACCESS_ITERATOR_OUT>
[[maybe_unused]] static APY_INLINE bool limb_vector_abs(
    RANDOM_ACCESS_ITERATOR_IN cbegin_it,
    RANDOM_ACCESS_ITERATOR_IN cend_it,
    RANDOM_ACCESS_ITERATOR_OUT res_it
)
{
    bool is_negative = apy_limb_signed_t(*std::prev(cend_it)) < 0;
    if (is_negative) {
        limb_vector_negate(cbegin_it, cend_it, res_it);
    } else {
        std::copy(cbegin_it, cend_it, res_it);
    }
    return is_negative;
}

//! Test if the bits in a limb vector are all zeros starting from bit `n` (zero
//! indexed). Bit `n` is assumed to be located in the first limb.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE bool limb_vector_all_zeros(
    RANDOM_ACCESS_ITERATOR cbegin_it, RANDOM_ACCESS_ITERATOR cend_it, unsigned n = 0
)
{
    apy_limb_t mask = ~((apy_limb_t(1) << n) - 1);
    if (*cbegin_it & mask) {
        // One or more bits in the masked first limb are non-zero
        return false;
    } else {
        // Test if remaining limbs are all full zero limbs
        for (auto it = cbegin_it + 1; it != cend_it; ++it) {
            if (*it != 0) {
                return false;
            }
        }
        return true; // All zeros
    }
}

//! Test if the bits in a limb vector are all ones starting from bit `n` (zero indexed).
//! Bit `n` is assumed to be located in the first limb.
template <class RANDOM_ACCESS_ITERATOR>
[[maybe_unused]] static APY_INLINE bool limb_vector_all_ones(
    RANDOM_ACCESS_ITERATOR cbegin_it, RANDOM_ACCESS_ITERATOR cend_it, unsigned n = 0
)
{
    apy_limb_t mask = ~((apy_limb_t(1) << n) - 1);
    if (~*cbegin_it & mask) {
        // One or more bits in the masked first limb are non-ones
        return false;
    } else {
        // Test if remaining limbs are all full zero limbs
        for (auto it = cbegin_it + 1; it != cend_it; ++it) {
            if (*it != apy_limb_t(-1)) {
                return false;
            }
        }
        return true; // All ones
    }
}

//! Copy limbs from `src` to `dst` and possibly sign extend the data in `dst`
template <typename RANDOM_ACCESS_ITERATOR_IN, typename RANDOM_ACCESS_ITERATOR_OUT>
[[maybe_unused]] static APY_INLINE void limb_vector_copy_sign_extend(
    RANDOM_ACCESS_ITERATOR_IN src_begin,
    RANDOM_ACCESS_ITERATOR_IN src_end,
    RANDOM_ACCESS_ITERATOR_OUT dst_begin,
    RANDOM_ACCESS_ITERATOR_OUT dst_end
)
{
    std::size_t src_vector_size = std::distance(src_begin, src_end);
    std::size_t dst_vector_size = std::distance(dst_begin, dst_end);
    std::copy_n(src_begin, std::min(src_vector_size, dst_vector_size), dst_begin);
    if (src_vector_size < dst_vector_size) {
        std::fill(
            dst_begin + src_vector_size,
            dst_end,
            limb_vector_is_negative(src_begin, src_end) ? -1 : 0
        );
    }
}

//! Read an unsigned 64-bit value from a limb vector. If `APY_LIMB_SIZE_BITS == 64`,
//! this results in a normal vector read without bounds checking. If `APY_LIMB_SIZE_BITS
//! == 32`, the second 32-bits limb is bound-checked and the result upper 32-bits are
//! zeroed if out-of-bounds.
template <typename VECTOR_TYPE>
[[maybe_unused, nodiscard]] static APY_INLINE std::uint64_t
limb_vector_to_uint64(const VECTOR_TYPE& limb_vec, std::size_t n)
{
    static_assert(APY_LIMB_SIZE_BITS == 32 || APY_LIMB_SIZE_BITS == 64);
    if constexpr (APY_LIMB_SIZE_BITS == 64) {
        // No bound-checking for 64-bit limbs
        return std::uint64_t(limb_vec[n]);
    } else { /* APY_LIMB_SIZE_BITS == 32 */
        if (n + 1 < limb_vec.size()) {
            return std::uint64_t(limb_vec[n]) | (std::uint64_t(limb_vec[n + 1]) << 32);
        } else {
            return std::uint64_t(limb_vec[n]);
        }
    }
}

//! Construct a Python tuple-literal-string from a vector of `T`. The type `T` must be
//! convertible to string through both a `std::stringstream` and `fmt::format({})`.
template <typename T> std::string tuple_string_from_vec(const std::vector<T>& vec)
{
    if (vec.size() == 0) {
        return "()";
    } else if (vec.size() == 1) {
        return fmt::format("({},)", vec[0]);
    } else {
        std::stringstream ss;
        ss << "(";
        for (auto& d : vec) {
            ss << d << ", ";
        }
        return ss.str().substr(0, ss.str().length() - 2).append(")");
    }
}

//! Fold a shape under multiplication
[[maybe_unused]] static APY_INLINE std::size_t fold_shape(
    std::vector<std::size_t>::const_iterator cbegin,
    std::vector<std::size_t>::const_iterator cend
)
{
    return std::accumulate(cbegin, cend, 1, std::multiplies {});
}

//! Fold a shape under multiplication
[[maybe_unused]] static APY_INLINE std::size_t
fold_shape(const std::vector<std::size_t>& shape)
{
    return fold_shape(std::cbegin(shape), std::cend(shape));
}

//! Compute the strides from a shape
[[maybe_unused]] static APY_INLINE std::vector<std::size_t>
strides_from_shape(const std::vector<std::size_t>& shape, std::size_t acc_base = 1)
{
    std::vector<std::size_t> strides(shape.size(), 0);
    for (std::size_t i = 0; i < shape.size(); i++) {
        strides[shape.size() - 1 - i] = std::accumulate(
            shape.crbegin(), shape.crbegin() + i, acc_base, std::multiplies {}
        );
    }
    return strides;
}

//! In-place multi-limb swap function
template <typename RANDOM_ACCESS_ITERATOR_INOUT>
[[maybe_unused]] static APY_INLINE void multi_limb_swap(
    RANDOM_ACCESS_ITERATOR_INOUT a_it,
    RANDOM_ACCESS_ITERATOR_INOUT b_it,
    std::size_t itemsize
)
{
    for (std::size_t i = 0; i < itemsize; i++) {
        std::swap(*(a_it + i), *(b_it + i));
    }
}

//! In-place multi-limb reverse function
template <typename RANDOM_ACCESS_ITERATOR_INOUT>
[[maybe_unused]] static APY_INLINE void multi_limb_reverse(
    RANDOM_ACCESS_ITERATOR_INOUT begin_it,
    RANDOM_ACCESS_ITERATOR_INOUT end_it,
    std::size_t itemsize
)
{
    auto n_items = std::distance(begin_it, end_it) / itemsize;
    for (std::size_t i = 0; i < (n_items + 1) / 2; i++) {
        auto it1 = begin_it + (i * itemsize);
        auto it2 = begin_it + itemsize * (n_items - i - 1);
        multi_limb_swap(it1, it2, itemsize);
    }
}

//! Get the convolutional properties `len`, `n_left`, and `n_right` (in that order) from
//! a string of either "full", "same", or "valid". Throws a `nanobind::value_error` if
//! the string is anything else.
template <typename APY_ARRAY>
[[maybe_unused]] static APY_INLINE std::tuple<std::size_t, std::size_t, std::size_t>
get_conv_lengths(const std::string& mode, const APY_ARRAY& a, const APY_ARRAY& b)
{
    std::size_t len, n_left, n_right;
    if (mode == "full") {
        len = a->shape()[0] + b->shape()[0] - 1;
        n_left = b->shape()[0] - 1;
        n_right = b->shape()[0] - 1;
    } else if (mode == "same") {
        len = a->shape()[0];
        n_left = b->shape()[0] / 2;
        n_right = b->shape()[0] - n_left - 1;
    } else if (mode == "valid") {
        len = a->shape()[0] - b->shape()[0] + 1;
        n_left = 0;
        n_right = 0;
    } else {
        auto msg = fmt::format("mode='{}' not in 'full', 'same', or 'valid'", mode);
        throw nanobind::value_error(msg.c_str());
    }
    return { len, n_left, n_right };
}

//! Macro for creating a void-specialization state-less functor `FUNCTOR_NAME` from a
//! function `FUNC_NAME`. The void-specialization functor allows template argument
//! deduction to be performed once its function is called. Neat!
#define CREATE_FUNCTOR_FROM_FUNC(FUNCTOR_NAME, FUNC_NAME)                              \
    template <typename... T> struct FUNCTOR_NAME { };                                  \
    template <> struct FUNCTOR_NAME<> {                                                \
        template <typename... T> auto operator()(T... t) const                         \
        {                                                                              \
            return FUNC_NAME(t...);                                                    \
        }                                                                              \
    }

CREATE_FUNCTOR_FROM_FUNC(apy_add_n_functor, apy_addition_same_length);
CREATE_FUNCTOR_FROM_FUNC(apy_sub_n_functor, apy_subtraction_same_length);

//! Mark nanobind Python-exposed parameter that does not support implicit conversions
//! from other types.
#define NB_NARG(name) nb::arg(name).noconvert()

//! Mark a special double-underscore nanobind operator (e.g., `__add__`) the implements
//! an arithmetic operation. In Python, when a bound function with this annotation is
//! called with incompatible arguments, it will return `NotImplemented` rather than
//! raising a `TypeError` as is default.
#define NB_OP(args) nb::is_operator(args)

//! Short-hand C++ arithmetic functors
#define STD_ADD std::plus
#define STD_SUB std::minus
#define STD_MUL std::multiplies
#define STD_DIV std::divides

#endif // _APYTYPES_UTIL_H
