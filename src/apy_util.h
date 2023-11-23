/*
 * Utility functions
 */

#ifndef _APY_UTIL_H
#define _APY_UTIL_H

#include <cstddef>
#include <cstdlib>
#include <stdexcept>
#include <vector>

// Not implemented exception
class NotImplementedException : public std::logic_error
{
public:
    NotImplementedException() : std::logic_error("Not implemeted yet") {};
};

// Perform fast integer ceil( n/64 )
static inline std::size_t idiv64_ceil_fast(std::size_t n) {
    if (n%64 == 0) {
        return n/64;
    } else {
        return n/64+1;
    }
}

// Avaiable string converstion types
enum class STRING_TYPE {
    HEX,
    OCT,
    DEC,
};

// Convert a positive arbitrary size integer array to a nibble list
static inline std::vector<uint8_t> to_nibble_list(
    const std::vector<uint64_t> &data_array
) {
    std::vector<uint8_t> result{};
    for (uint64_t data : data_array) {
        for (int i=0; i<16; i++) {
            result.push_back(static_cast<uint8_t>((data >> (4*i)) & 0x0F));
        }
    }

    // Remove zero-elements from the end
    int zero_elements = 0;
    for (auto nibble = result.crbegin(); nibble != result.crend(); ++nibble) {
        if (*nibble == 0) {
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
        nibble_list[i] &= 0x0F;
        if (i > 0 && nibble_list[i-1] >= 8) {
            nibble_list[i] += 1;
        }
    }
    return output_bit;
}

// Double-dabble algorithm for binary->bcd conversion
static inline std::vector<uint8_t> double_dabble(const std::vector<uint64_t> &data)
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
