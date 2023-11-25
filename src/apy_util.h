/*
 * Utility functions
 */

#ifndef _APY_UTIL_H
#define _APY_UTIL_H

#include <cstddef>    // std::size_t
#include <stdexcept>  // std::logic_error, std::domain_error
#include <vector>     // std::vector

// GMP should be included after all other includes
#include <gmp.h>

// Sizes of GMP limbs (underlying words)
static constexpr std::size_t _LIMB_SIZE_BYTES = sizeof(mp_limb_t);
static constexpr std::size_t _LIMB_SIZE_BITS  = 8 * _LIMB_SIZE_BYTES;

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

// Convert a positive arbitrary size integer array to a nibble list
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

    // Remove zero-elements from the end
    int zero_elements = 0;
    for (auto nibble_it = result.crbegin(); nibble_it != result.crend(); ++nibble_it) {
        if (*nibble_it == 0) {
            zero_elements++;
        } else {
            break;
        }
    }
    result.resize(result.size() - zero_elements);
    return result.size() > 0 ? result : std::vector<uint8_t>{ 0 };
}

// Shift a nibble list left by one stage. Modifies the content of input nibble_list
static inline bool nibble_list_shift_left_once(std::vector<uint8_t> &nibble_list)
{
    bool output_bit = *(--nibble_list.cend()) >= 8;
    for (int i=nibble_list.size()-1; i>=0; i--) {
        nibble_list[i] <<= 1;
        nibble_list[i] &= 0xF;
        if (i > 0 && nibble_list[i-1] >= 8) {
            nibble_list[i] += 1;
        }
    }
    return output_bit;
}

// Double-dabble algorithm for binary->bcd conversion
static inline std::vector<uint8_t> double_dabble(const std::vector<mp_limb_t> &data)
{
    std::vector<uint8_t> nibble_list = to_nibble_list(data);
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

// Divide BCD number by two. First element in input array in considered MSB
static inline void bcd_div2(std::vector<uint8_t> &bcd_list)
{
    if (bcd_list.size() == 0) {
        return;
    }

    // Add a new least significant bcd?
    bool new_lsbcd = bool( *(bcd_list.end()-1) & 0x01);

    // Division by two
    for (int i=bcd_list.size()-1; i>0; i--) {
        bcd_list[i] >>= 1;
        if (bcd_list[i-1] & 0x01){
            bcd_list[i] += 5;
        }
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

#endif
