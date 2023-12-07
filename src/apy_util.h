/*
 * Utility functions
 */

#ifndef _APY_UTIL_H
#define _APY_UTIL_H

#include <algorithm>  // std::find
#include <cstddef>    // std::size_t
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
        return std::vector<mp_limb_t>(vec.begin(), vec.end());
    }
}

// Not implemented exception
class NotImplementedException : public std::logic_error
{
public:
    NotImplementedException() : std::logic_error("Not implemeted yet") {};
};

// Quickly evaluate how many limbs are requiered to to store an n-bit word
static inline std::size_t bits_to_limbs(std::size_t bits) {
    if (bits%_LIMB_SIZE_BITS == 0) {
        return bits/_LIMB_SIZE_BITS;
    } else {
        return bits/_LIMB_SIZE_BITS + 1;
    }
}

// Count the number of significant limbs in limb vector
static inline std::size_t significant_limbs(const std::vector<mp_limb_t> &vector)
{
    auto is_non_zero = [](auto n) { return n != 0; };
    auto back_non_zero_it = std::find_if(vector.crbegin(), vector.crend(), is_non_zero);
    return std::distance(vector.begin(), back_non_zero_it.base());
}

// Quickly perform `1 + ceil(log2(x))` for unsigned integer (`mp_limb_t`) `x` if `x` is
// non-zero and return the value. If `x` is zero, return zero.
static inline std::size_t bit_width(mp_limb_t x)
{
    // Optimized on x86-64 using single `bsr` instruction since GCC-13.1
    std::size_t result = 0;
    while (x) {
        x >>= 1;
        result++;
    }
    return result;
}

// Quickly count the number of nibbles in an unsigned `mp_limb_t`
static inline std::size_t nibble_width(mp_limb_t x)
{
    std::size_t bits = bit_width(x);
    if (bits % 4 == 0) {
        return bits/4;
    } else {
        return bits/4 + 1;
    }
}

// Convert a positive arbitrary size integer array (`std::vector<mp_limb_t>`) to a
// nibble list. The nibble list contains least significant nibble first.
// Argument `len` indicates the intended bcd length of the output. When set, no more
// than `result.rend() - len` zeros will be removed.
static inline std::vector<uint8_t> to_nibble_list(
    const std::vector<mp_limb_t> &data_array,
    std::size_t len = 0
) {
    constexpr std::size_t NIBBLES_PER_LIMB = 2*_LIMB_SIZE_BYTES;
    constexpr std::size_t BITS_PER_NIBBLE = 4;
    std::vector<uint8_t> result{};
    for (mp_limb_t data : data_array) {
        for (unsigned i=0; i<NIBBLES_PER_LIMB; i++) {
            result.push_back(uint8_t((data >> (BITS_PER_NIBBLE*i)) & 0x0F));
        }
    }

    // Remove zero-elements *from the end*, but no more than `keep` elements
    auto is_non_zero = [](auto n) { return n != 0; };
    auto non_zero_it = std::find_if(result.rbegin(), result.rend()-len, is_non_zero);
    result.erase(non_zero_it.base(), result.end());

    // Return result
    return result.size() > 0 ? result : std::vector<uint8_t>{ 0 };
}

// Convert a nibble list into a positive integer array (`std::vector<mp_limb_t>`). The
// nibble list is assumed to have least significant nibble first.
static inline std::vector<mp_limb_t> from_nibble_list(
    const std::vector<uint8_t> &nibble_list
) {
    constexpr std::size_t NIBBLES_PER_LIMB = 2*_LIMB_SIZE_BYTES;
    constexpr std::size_t BITS_PER_NIBBLE = 4;

    // Compute the total number of limbs in the result vector
    std::size_t limbs = nibble_list.size()/NIBBLES_PER_LIMB;
    limbs += nibble_list.size()%NIBBLES_PER_LIMB != 0 ? 1 : 0;

    // Insert one nibble to the limb vector at a time
    std::vector<mp_limb_t> result( limbs, 0 );
    for (std::size_t limb_i=0; limb_i<result.size(); limb_i++) {
        mp_limb_t limb = 0;
        for (std::size_t nbl_i=0; nbl_i<NIBBLES_PER_LIMB; nbl_i++) {
            auto i = limb_i * NIBBLES_PER_LIMB + nbl_i;
            if (i >= nibble_list.size()) {
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


// Double-Dabble helper class with proporate methods for performing the double-dabble
// and reverse double-dable algorithm.
struct DoubleDabbleList {

    // Mask with a bit in every possition where a nibble starts
    static constexpr mp_limb_t _NIBBLE_MASK = 
        _LIMB_SIZE_BITS == 64
        ? 0x1111111111111111  // 64-bit architecture
        : 0x11111111;         // 32-bit architecture

    std::vector<mp_limb_t> data{ 0 };

    // Do one iteration of double (double-dabble)
    void do_double(mp_limb_t new_bit)
    {
        // Perform a single bit left shift (double)
        if (mpn_lshift(&data[0], &data[0], data.size(), 1)) {
            data.push_back(1);
        }
        if (new_bit) {
            data[0] |= 0x1;
        }
    }

    // Do one iteration of dabble (double-dabble)
    void do_dabble()
    {
        for (auto &l : data) {
            // Add 3 to each nibble GEQ 5
            mp_limb_t dabble_mask = (((l | l>>1) & (l>>2)) | (l>>3)) & _NIBBLE_MASK;
            l += (dabble_mask << 1) | dabble_mask;
        }
    }

    // Do one iteration of reverse double (reverse double-dabble)
    void do_reverse_double(mp_limb_t &limb_out)
    {
        limb_out |= mpn_rshift(&data[0], &data[0], data.size(), 1);
    }

    // Do one iteration of reverse dabble (reverse double-dabble)
    void do_reverse_dabble()
    {
        for (auto &l : data) {
            // Subtract 3 from each nibble GEQ 8
            mp_limb_t dabble_mask = (l >> 3) & _NIBBLE_MASK;
            l -= (dabble_mask << 1) | dabble_mask;
        }
    }
};

// Double-dabble algorithm for binary->BCD conversion
static inline std::vector<mp_limb_t> double_dabble(std::vector<mp_limb_t> nibble_data)
{
    if (!nibble_data.size()) {
        return {};
    }

    // Remove zero elements from the back until first non-zero element is found (keep
    // atleast one zero at the start)
    auto is_non_zero = [](auto n) { return n != 0; };
    nibble_data.erase(
        std::find_if(nibble_data.rbegin(), nibble_data.rend()-1, is_non_zero).base(),
        nibble_data.end()
    );

    // Double-dabble algorithm begin
    DoubleDabbleList bcd_list{};
    const auto nibbles_last_limb = nibble_width(nibble_data.back());
    const auto nibbles = nibbles_last_limb + _LIMB_SIZE_BITS/4 * (nibble_data.size()-1);
    const mp_limb_t new_bit_mask = nibbles_last_limb == 0
        ? mp_limb_t(1) << (_LIMB_SIZE_BITS - 1)
        : mp_limb_t(1) << (4*nibbles_last_limb - 1);
    for (std::size_t i=0; i<4*nibbles; i++) {
        // Shift input data left once
        mp_limb_t new_bit = nibble_data.back() & new_bit_mask;
        mpn_lshift(&nibble_data[0], &nibble_data[0], nibble_data.size(), 1);

        // Do the double-dabble (dabble-double)
        bcd_list.do_dabble();
        bcd_list.do_double(new_bit);
    }
    return bcd_list.data;
}

// Reverse double-dabble algorithm for BCD->binary conversion
static inline std::vector<mp_limb_t> reverse_double_dabble(
    const std::vector<uint8_t> &bcd_list
) {
    if (bcd_list.size() == 0) {
        return {};
    }

    std::size_t iteration=0;
    std::vector<mp_limb_t> nibble_data{};
    DoubleDabbleList bcd{ from_nibble_list(bcd_list) };
    mp_limb_t new_limb = 0;
    while (
        // As long as there are elements remaining in the BCD list, and we haven't
        // completed a multiple-of-four iterations
        std::any_of(bcd.data.begin(), bcd.data.end(), [](auto n){ return n!=0; })
        || iteration % 4 != 0
    ) {
        // Insert a new limb to the nibble data vector
        if (iteration % _LIMB_SIZE_BITS == 0) {
            nibble_data.insert(nibble_data.begin(), new_limb);
        }

        // Right shift the nibble binary data
        new_limb = mpn_rshift(&nibble_data[0], &nibble_data[0], nibble_data.size(), 1);

        // Do the (reverse) double-dabble
        bcd.do_reverse_double(nibble_data.back());
        bcd.do_reverse_dabble();

        // Increment iteration counter
        iteration++;
    }

    // Right-adjust the data and return
    std::size_t shft_val = _LIMB_SIZE_BITS - (iteration % _LIMB_SIZE_BITS);
    if (iteration && shft_val) {
        mpn_rshift(&nibble_data[0], &nibble_data[0], nibble_data.size(), shft_val);
    }
    return nibble_data.size() ? nibble_data : std::vector<mp_limb_t>{ 0 };
}

// Divide the number in a BCD limb vector by two.
static inline void bcd_limb_vec_div2(std::vector<mp_limb_t> &bcd_list)
{
    if (bcd_list.size() == 0) {
        return;
    }

    // Do a single vector right-shift and possibly prepend the new data
    auto shift_out = mpn_rshift(&bcd_list[0], &bcd_list[0], bcd_list.size(), 1);
    if (shift_out) {
        bcd_list.insert(bcd_list.begin(), shift_out);
    }

    // Subtract 3 from each nibble greater than or equal to 8
    for (auto &l : bcd_list) {
        mp_limb_t dabble_mask = (l >> 3) & DoubleDabbleList::_NIBBLE_MASK;
        l -= (dabble_mask << 1) | dabble_mask;
    }
}

// Multiply the number in a BCD limb vector by two.
static inline void bcd_limb_vec_mul2(std::vector<mp_limb_t> &bcd_list)
{
    if (bcd_list.size() == 0) {
        return;
    }

    // Add 3 to each nibble greater than or equal to 5
    for (auto &l : bcd_list) {
        mp_limb_t dabble_mask =
            (((l | l>>1) & (l>>2)) | (l>>3)) & DoubleDabbleList::_NIBBLE_MASK;
        l += (dabble_mask << 1) | dabble_mask;
    }

    // Multiply by two
    auto shift_out = mpn_lshift(&bcd_list[0], &bcd_list[0], bcd_list.size(), 1);
    if (shift_out) {
        bcd_list.push_back(shift_out);
    }
}

// Multiply BCD vector (`std::vector<uint8_t>`) by two. The first element (`front()`)
// in the vector is considered LSB.
static inline void bcd_mul2(std::vector<uint8_t> &bcd_list)
{
    if (bcd_list.size() == 0) {
        return;
    }

    // Multiply each BCD by two
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
    static const auto regex = std::regex("^\\s+");
    return std::regex_replace(str, regex, "");
}

// Trim a string from trailing whitespace
static inline std::string string_trim_trailing_whitespace(const std::string &str)
{
    static const auto regex = std::regex("\\s+$");
    return std::regex_replace(str, regex, "");
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
    static const auto regex = std::regex(validity_regex);
    return std::regex_match(str, regex);
}

// Trim a string from unnecessary leading and trailing zeros, that don't affect numeric
// value of the string. This function also attaches a zero to the string if it starts
// with a decimal dot, and it removes the decimal dot if no digit after it affects it's
// value (e.g., 0.00 == 0).
static inline std::string string_trim_zeros(const std::string &str)
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


// Perform arithmetic right shift on a limb vector. Accelerated using GMP.
static inline void limb_vector_asr(std::vector<mp_limb_t> &vec, unsigned shift_amnt)
{
    if (!vec.size() || !shift_amnt) {
        return;
    }

    mp_limb_t sign_limb = mp_limb_signed_t(vec.back()) >> (_LIMB_SIZE_BITS-1);
    unsigned limb_shift = shift_amnt%_LIMB_SIZE_BITS;
    unsigned limb_skip  = shift_amnt/_LIMB_SIZE_BITS;
    if (limb_skip >= vec.size()) {
        std::fill(vec.begin(), vec.end(), sign_limb);
        return;  // early return
    } else if (limb_skip) {
        for (unsigned i=0; i<vec.size()-limb_skip; i++) {
            vec[i] = vec[i+limb_skip];
        }
        for (unsigned i=vec.size()-limb_skip; i<vec.size(); i++) {
            vec[i] = sign_limb;
        }
    }

    // Perform the in-limb shifting
    if (limb_shift) {
        mpn_rshift(
            &vec[0],     // dst
            &vec[0],     // src
            vec.size(),  // limb vector size
            limb_shift   // shift amount
        );

        // Sign extend the most significant bits
        if (sign_limb) {
            vec.back() |= ~((mp_limb_t(1) << (_LIMB_SIZE_BITS - limb_shift)) - 1);
        }
    }
}

// Perform logical right shift on a limb vector. Accelerated using GMP.
static inline void limb_vector_lsr(std::vector<mp_limb_t> &vec, unsigned shift_amnt)
{
    if (!vec.size() || !shift_amnt) {
        return;
    }

    unsigned limb_shift = shift_amnt%_LIMB_SIZE_BITS;
    unsigned limb_skip  = shift_amnt/_LIMB_SIZE_BITS;
    if (limb_skip >= vec.size()) {
        std::fill(vec.begin(), vec.end(), 0);
        return;  // early return
    } else if (limb_skip) {
        for (unsigned i=0; i<vec.size()-limb_skip; i++) {
            vec[i] = vec[i+limb_skip];
        }
        for (unsigned i=vec.size()-limb_skip; i<vec.size(); i++) {
            vec[i] = 0;
        }
    }

    // Perform the in-limb shifting
    if (limb_shift) {
        mpn_rshift(
            &vec[0],     // dst
            &vec[0],     // src
            vec.size(),  // limb vector size
            limb_shift   // shift amount
        );
    }
}

// Perform logical left shift on a limb vector. Accelerated using GMP.
static inline void limb_vector_lsl(std::vector<mp_limb_t> &vec, unsigned shift_amnt)
{
    if (!vec.size() || !shift_amnt) {
        return;
    }

    unsigned limb_shift = shift_amnt%_LIMB_SIZE_BITS;
    unsigned limb_skip  = shift_amnt/_LIMB_SIZE_BITS;
    if (limb_skip >= vec.size()) {
        std::fill(vec.begin(), vec.end(), 0);
        return;  // early return
    } else if (limb_skip) {
        for (unsigned i = vec.size()-1; i>=limb_skip; i--) {
            vec[i] = vec[i-limb_skip];
        }
        for (unsigned i=0; i<limb_skip; i++) {
            vec[i] = 0;
        }
    }

    // Perform the in-limb shifting
    if (limb_shift) {
        mpn_lshift(
            &vec[0],     // dst
            &vec[0],     // src
            vec.size(),  // limb vector size
            limb_shift   // shift amount
        );
    }
}

#endif
