#include "../apy_util.h"
#include "catch.hpp"

#include <iostream>
#include <iterator>
#include <stdexcept>

using uint8_vec = std::vector<uint8_t>;


TEST_CASE("to_nibble_list() and from_nibble_list()")
{

    // Zero-vector results in nibble-list with one element
    REQUIRE(to_nibble_list({ 0 }) == uint8_vec{ 0 });

    // Simple tests
    REQUIRE(to_nibble_list({ 1 }) == uint8_vec{ 1 });
    REQUIRE(to_nibble_list({ 15 }) == uint8_vec{ 15 });
    REQUIRE(to_nibble_list({ 16 }) == uint8_vec{ 0, 1 });
    REQUIRE(to_nibble_list({ 0xff }) == uint8_vec{ 0x0f, 0x0f });
    REQUIRE(to_nibble_list({ 0xfca }) == uint8_vec{ 0x0a, 0x0c, 0x0f });

    REQUIRE(from_nibble_list({ 0 }) == to_limb_vec({0x0}));
    REQUIRE(from_nibble_list({ 1 }) == to_limb_vec({0x1}));
    REQUIRE(from_nibble_list({ 1, 0 }) == to_limb_vec({0x10}));
    REQUIRE(from_nibble_list({ 9, 3, 0, 7, 3, 9  }) == to_limb_vec({0x930739}));

    /*
     * TODO: Longer tests are actually dependant on the target architecture. These tests
     * would fail on machines where `mp_limb_t` are 32-bit.
     */
    REQUIRE(
        to_nibble_list({
            0xabcd0012,
            0x773300,
            0x7E00AD0000BE00EF,
        }) == uint8_vec{
            0x2, 0x1, 0, 0, 0xd, 0xc, 0xb, 0xa, 0, 0, 0, 0, 0, 0, 0, 0,
            0x0, 0x0, 0x3, 0x3, 0x7, 0x7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0xF, 0xE, 0, 0, 0xE, 0xB, 0, 0, 0, 0, 0xD, 0xA, 0, 0, 0xE, 0x7
        }
    );
}

TEST_CASE("nibble_shift_left_once()")
{
    /*
     * `nibble_shift_left_once()` assumes a list with *least* significant nibble first
     */
    { /* Zero-element */
        std::vector<uint8_t> nibble_list{ 0 };
        bool out_bit = nibble_list_shift_left_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec{ 0 });
        REQUIRE(out_bit == 0);
    }
    { /* Shift with `out_bit` = 1  */
        std::vector<uint8_t> nibble_list = { 0x7, 0xF, 0x8, 0x8 };
        bool out_bit = nibble_list_shift_left_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec{ 0xE, 0xE, 0x1, 0x1 });
        REQUIRE(out_bit == 1);
    }
    { /* Shift with `out_bit = 0 */
        std::vector<uint8_t> nibble_list = { 0x7, 0xF, 0x8, 0x3 };
        bool out_bit = nibble_list_shift_left_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec{ 0xE, 0xE, 0x1, 0x7 });
        REQUIRE(out_bit == 0);
    }
}

TEST_CASE("nibble_shift_right_once()")
{
    /*
     * `nibble_shift_left_once()` assumes a list with *most* significant nibble first
     */
    { /* Zero-element */
        std::vector<uint8_t> nibble_list{ 0 };
        bool out_bit = nibble_list_shift_right_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec{ 0 });
        REQUIRE(out_bit == 0);
    }
    { /* Shift with `out_bit` = 0  */
        std::vector<uint8_t> nibble_list = { 0x7, 0xF, 0x8, 0x8 };
        bool out_bit = nibble_list_shift_right_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec{ 0x3, 0xF, 0xC, 0x4 });
        REQUIRE(out_bit == 0);
    }
    { /* Shift with `out_bit = 1 */
        std::vector<uint8_t> nibble_list = { 0x7, 0xF, 0x8, 0x3 };
        bool out_bit = nibble_list_shift_right_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec{ 0x3, 0xF, 0xC, 0x1 });
        REQUIRE(out_bit == 1);
    }
}

TEST_CASE("double_dabble()")
{
    // Simple tests for double-dabble
    REQUIRE(double_dabble({   }) == uint8_vec{ 0 });
    REQUIRE(double_dabble({ 0 }) == uint8_vec{ 0 });
    REQUIRE(double_dabble({ 1 }) == uint8_vec{ 1 });
    REQUIRE(double_dabble({ 9 }) == uint8_vec{ 9 });
    REQUIRE(double_dabble({ 10412 }) == uint8_vec{ 2, 1, 4, 0, 1 });

    // Problematique test #1
    REQUIRE(double_dabble({ 0x0ff }) == uint8_vec{ 5, 5, 2 });
    REQUIRE(double_dabble({ 0x100 }) == uint8_vec{ 6, 5, 2 });
    REQUIRE(double_dabble({ 0x0ffff }) == uint8_vec{ 5, 3, 5, 5, 6 });
    REQUIRE(double_dabble({ 0x10000 }) == uint8_vec{ 6, 3, 5, 5, 6 });

    // Problematique test #2
    REQUIRE(
        double_dabble({ 0xFFFFFFFFFFFA70CE, 0xFFFFFAAAFFFFFFFF, 0xFFFFFAAAFFFFFFFF})
        == uint8_vec{
            8, 7, 6, 7, 7, 7, 0, 2, 6, 8, 2, 9, 8, 9, 7, 2, 1, 2, 5, 0,
            4, 6, 2, 9, 7, 2, 4, 1, 3, 0, 7, 6, 9, 5, 0, 3, 3, 5, 1, 7,
            0, 8, 5, 4, 9, 6, 3, 4, 0, 4, 7, 9, 9, 0, 7, 7, 2, 6
        }
    );

    // Problematique test #3
    REQUIRE(
        double_dabble({ 0, 0x8000000000000000, 0})
        == uint8_vec{
            8, 2, 7, 5, 0, 1, 4, 8, 8, 5, 1, 7, 3, 0, 3, 7, 8, 6, 1, 3,
            7, 1, 3, 2, 9, 6, 4, 0, 6, 4, 3, 8, 1, 1, 4, 1, 0, 7, 1
        }
    );
}

TEST_CASE("reverse_double_dabble()")
{
    REQUIRE(reverse_double_dabble({}) == to_limb_vec({}));
    REQUIRE(reverse_double_dabble({ 0 }) == to_limb_vec({ 0 }));
    REQUIRE(reverse_double_dabble({ 1 }) == to_limb_vec({ 1 }));
    REQUIRE(reverse_double_dabble({ 9 }) == to_limb_vec({ 9 }));
    REQUIRE(reverse_double_dabble({ 1, 0 }) == to_limb_vec({ 0xA }));
    REQUIRE(reverse_double_dabble({ 1, 5 }) == to_limb_vec({ 0xF }));
    REQUIRE(reverse_double_dabble({ 2, 5, 5 }) == to_limb_vec({ 0xFF }));
    REQUIRE(reverse_double_dabble({ 2, 5, 6 }) == to_limb_vec({ 0x100 }));

    REQUIRE(
        reverse_double_dabble({
            1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 5, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 3, 0, 0, 0, 0,
        })
        == to_limb_vec({0x0b0290d4c7eba390, 0x00baf96937ffc2a8, 0x159ffe72})
    );

}

TEST_CASE("bcd_div2()")
{
    {
        // Test 1: Continues division by two of 1
        std::vector<uint8_t> num{1};
        for (int i=0; i<13; i++) {
            bcd_div2(num);
        }

        char res[] = "00001220703125";
        REQUIRE(std::size(res)-1 == num.size());
        for (std::size_t i=0; i<std::size(res)-1; i++) {
            REQUIRE(res[i] == num.at(i)+0x30);
        }
    }

    {
        // Test 2: Divide 45 by 2 13 times
        std::vector<uint8_t> num{0x4, 0x5};
        for (int i=0; i<13; i++) {
            bcd_div2(num);
        }

        char res[] = "000054931640625";
        REQUIRE(std::size(res)-1 == num.size());
        for (std::size_t i=0; i<std::size(res)-1; i++) {
            REQUIRE(res[i] == num.at(i)+0x30);
        }
    }
}

