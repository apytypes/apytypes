#include "../apy_util.h"
#include "catch.hpp"

#include <iomanip>
#include <iostream>
#include <iterator>

using uint8_vec = std::vector<uint8_t>;

TEST_CASE("to_nibble_list() and from_nibble_list()")
{

    // Zero-vector results in nibble-list with one element
    REQUIRE(to_nibble_list({ 0 }) == uint8_vec { 0 });

    // Simple tests
    REQUIRE(to_nibble_list({ 1 }) == uint8_vec { 1 });
    REQUIRE(to_nibble_list({ 15 }) == uint8_vec { 15 });
    REQUIRE(to_nibble_list({ 16 }) == uint8_vec { 0, 1 });
    REQUIRE(to_nibble_list({ 0xff }) == uint8_vec { 0x0f, 0x0f });
    REQUIRE(to_nibble_list({ 0xfca }) == uint8_vec { 0x0a, 0x0c, 0x0f });

    REQUIRE(from_nibble_list({ 0 }) == to_limb_vec({ 0x0 }));
    REQUIRE(from_nibble_list({ 1 }) == to_limb_vec({ 0x1 }));
    REQUIRE(from_nibble_list({ 1, 0 }) == to_limb_vec({ 0x01 }));
    REQUIRE(from_nibble_list({ 9, 3, 0, 7, 3, 9 }) == to_limb_vec({ 0x937039 }));

    /*
     * TODO: Longer tests are actually dependant on the target architecture. These
     * tests would fail on machines where `mp_limb_t` are 32-bit.
     */
    REQUIRE(
        to_nibble_list({
            0xabcd0012,
            0x773300,
            0x7E00AD0000BE00EF,
        })
        == uint8_vec { 0x2, 0x1, 0, 0, 0xd, 0xc, 0xb, 0xa, 0,   0,   0,   0,
                       0,   0,   0, 0, 0x0, 0x0, 0x3, 0x3, 0x7, 0x7, 0,   0,
                       0,   0,   0, 0, 0,   0,   0,   0,   0xF, 0xE, 0,   0,
                       0xE, 0xB, 0, 0, 0,   0,   0xD, 0xA, 0,   0,   0xE, 0x7 }
    );
    REQUIRE(
        from_nibble_list({ 0x2, 0x1, 0, 0, 0xd, 0xc, 0xb, 0xa, 0,   0,   0,   0,
                           0,   0,   0, 0, 0x0, 0x0, 0x3, 0x3, 0x7, 0x7, 0,   0,
                           0,   0,   0, 0, 0,   0,   0,   0,   0xF, 0xE, 0,   0,
                           0xE, 0xB, 0, 0, 0,   0,   0xD, 0xA, 0,   0,   0xE, 0x7 })
        == to_limb_vec({
            0xabcd0012,
            0x773300,
            0x7E00AD0000BE00EF,
        })
    );
}

TEST_CASE("nibble_shift_left_once()")
{
    /*
     * `nibble_shift_left_once()` assumes a list with *least* significant nibble
     * first
     */
    { /* Zero-element */
        std::vector<uint8_t> nibble_list { 0 };
        bool out_bit = nibble_list_shift_left_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec { 0 });
        REQUIRE(out_bit == 0);
    }
    { /* Shift with `out_bit` = 1  */
        std::vector<uint8_t> nibble_list = { 0x7, 0xF, 0x8, 0x8 };
        bool out_bit = nibble_list_shift_left_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec { 0xE, 0xE, 0x1, 0x1 });
        REQUIRE(out_bit == 1);
    }
    { /* Shift with `out_bit = 0 */
        std::vector<uint8_t> nibble_list = { 0x7, 0xF, 0x8, 0x3 };
        bool out_bit = nibble_list_shift_left_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec { 0xE, 0xE, 0x1, 0x7 });
        REQUIRE(out_bit == 0);
    }
}

TEST_CASE("nibble_shift_right_once()")
{
    /*
     * `nibble_shift_left_once()` assumes a list with *most* significant nibble
     * first
     */
    { /* Zero-element */
        std::vector<uint8_t> nibble_list { 0 };
        bool out_bit = nibble_list_shift_right_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec { 0 });
        REQUIRE(out_bit == 0);
    }
    { /* Shift with `out_bit` = 0  */
        std::vector<uint8_t> nibble_list = { 0x7, 0xF, 0x8, 0x8 };
        bool out_bit = nibble_list_shift_right_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec { 0x3, 0xF, 0xC, 0x4 });
        REQUIRE(out_bit == 0);
    }
    { /* Shift with `out_bit = 1 */
        std::vector<uint8_t> nibble_list = { 0x7, 0xF, 0x8, 0x3 };
        bool out_bit = nibble_list_shift_right_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec { 0x3, 0xF, 0xC, 0x1 });
        REQUIRE(out_bit == 1);
    }
}

TEST_CASE("double_dabble()")
{
    // Simple tests for double-dabble
    REQUIRE(double_dabble({}) == to_limb_vec({}));
    REQUIRE(double_dabble({ 0 }) == to_limb_vec({ 0x0 }));
    REQUIRE(double_dabble({ 1 }) == to_limb_vec({ 0x1 }));
    REQUIRE(double_dabble({ 9 }) == to_limb_vec({ 0x9 }));
    REQUIRE(double_dabble({ 10412 }) == to_limb_vec({ 0x10412 }));

    // Problematique test #1
    REQUIRE(double_dabble({ 0x0ff }) == to_limb_vec({ 0x255 }));
    REQUIRE(double_dabble({ 0x100 }) == to_limb_vec({ 0x256 }));
    REQUIRE(double_dabble({ 0x0ffff }) == to_limb_vec({ 0x65535 }));
    REQUIRE(double_dabble({ 0x10000 }) == to_limb_vec({ 0x65536 }));

    // Problematique test #2
    REQUIRE(
        double_dabble({ 0xFFFFFFFFFFFA70CE, 0xFFFFFAAAFFFFFFFF, 0xFFFFFAAAFFFFFFFF })
        == to_limb_vec(
            { 0x2798928620777678, 0x6703142792640521, 0x4369458071533059, 0x6277099740 }
        )
    );

    // Problematique test #3
    REQUIRE(
        double_dabble({ 0, 0x8000000000000000, 0 })
        == to_limb_vec({ 0x7303715884105728, 0x8346046923173168, 0x1701411 })
    );
}

TEST_CASE("reverse_double_dabble()")
{
    REQUIRE(reverse_double_dabble({}) == to_limb_vec({}));
    REQUIRE(reverse_double_dabble({ 0 }) == to_limb_vec({ 0 }));
    REQUIRE(reverse_double_dabble({ 1 }) == to_limb_vec({ 1 }));
    REQUIRE(reverse_double_dabble({ 9 }) == to_limb_vec({ 9 }));
    REQUIRE(reverse_double_dabble({ 0, 1 }) == to_limb_vec({ 0xA }));
    REQUIRE(reverse_double_dabble({ 5, 1 }) == to_limb_vec({ 0xF }));
    REQUIRE(reverse_double_dabble({ 5, 5, 2 }) == to_limb_vec({ 0xFF }));
    REQUIRE(reverse_double_dabble({ 6, 5, 2 }) == to_limb_vec({ 0x100 }));
    REQUIRE(reverse_double_dabble({ 6, 0, 2 }) == to_limb_vec({ 0xCE }));

    REQUIRE(
        reverse_double_dabble({
            0, 0, 0, 0, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 5, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 8, 7, 6, 5, 4, 3, 2, 1,
        })
        == to_limb_vec({ 0x0b0290d4c7eba390, 0x00baf96937ffc2a8, 0x159ffe72 })
    );
}

TEST_CASE("bcd_limb_vec_div2()")
{
    {
        // Test 1: Continues division by two of 1
        std::vector<mp_limb_t> num { 1 };
        for (int i = 0; i < 13; i++) {
            bcd_limb_vec_div2(num);
        }

        REQUIRE(num.size() == 2);
        REQUIRE(num[0] == 0x00001220703125000);
    }

    {
        // Test 2: Divide 45 by 2 13 times
        std::vector<mp_limb_t> num { 0x45 };
        for (int i = 0; i < 13; i++) {
            bcd_limb_vec_div2(num);
        }

        REQUIRE(num.size() == 2);
        REQUIRE(num[0] == 0x54931640625000);
    }
}

TEST_CASE("limb_vector_asr()")
{
    { /* Test #1 */
        std::vector<mp_limb_t> operand = { mp_limb_t(-1) };

        limb_vector_asr(operand, 0);
        REQUIRE(operand == std::vector<mp_limb_t> { mp_limb_t(-1) });

        limb_vector_asr(operand, 12345);
        REQUIRE(operand == std::vector<mp_limb_t> { mp_limb_t(-1) });
    }

    { /* Test #2 */
        std::vector<mp_limb_t> operand = { 8 };

        limb_vector_asr(operand, 0);
        REQUIRE(operand == std::vector<mp_limb_t> { 8 });

        limb_vector_asr(operand, 2);
        REQUIRE(operand == std::vector<mp_limb_t> { 2 });
    }

    { /* Test #4 */
        std::vector<mp_limb_t> operand = { 0, 0, 0x12300000 };

        limb_vector_asr(operand, 0);
        REQUIRE(operand == std::vector<mp_limb_t> { 0, 0, 0x12300000 });

        limb_vector_asr(operand, 4);
        REQUIRE(operand == std::vector<mp_limb_t> { 0, 0, 0x01230000 });

        limb_vector_asr(operand, 4 * 6 + _LIMB_SIZE_BITS);
        REQUIRE(operand == std::vector<mp_limb_t> { 0x2300000000000000, 0x1, 0 });
    }

    { /* Test #4 */
        std::vector<mp_limb_t> operand = { 0, 0, mp_limb_t(-0x12300000) };

        limb_vector_asr(operand, 0);
        REQUIRE(operand == std::vector<mp_limb_t> { 0, 0, mp_limb_t(-0x12300000) });

        limb_vector_asr(operand, 4);
        REQUIRE(operand == std::vector<mp_limb_t> { 0, 0, mp_limb_t(-0x01230000) });

        limb_vector_asr(operand, 4 * 6 + _LIMB_SIZE_BITS);
        REQUIRE(
            operand
            == std::vector<mp_limb_t> { mp_limb_t(-0x2300000000000000 - 0),
                                        mp_limb_t(-0x0000000000000001 - 1),
                                        mp_limb_t(-0x0000000000000000 - 1) }
        );
    }
}

TEST_CASE("limb_vector_lsl() and limb_vector_lsr()")
{
    { /* Test #1 */
        std::vector<mp_limb_t> operand = { mp_limb_t(-1) };

        limb_vector_lsl(operand, 0);
        REQUIRE(operand == std::vector<mp_limb_t> { mp_limb_t(-1) });

        limb_vector_lsr(operand, 0);
        REQUIRE(operand == std::vector<mp_limb_t> { mp_limb_t(-1) });
    }

    { /* Test #2 */
        std::vector<mp_limb_t> operand = { 0, 0, mp_limb_t(0x0123abc0) };

        limb_vector_lsr(operand, 0);
        REQUIRE(operand == std::vector<mp_limb_t> { 0, 0, mp_limb_t(0x0123abc0) });

        limb_vector_lsr(operand, 4);
        REQUIRE(operand == std::vector<mp_limb_t> { 0, 0, mp_limb_t(0x00123abc) });

        limb_vector_lsr(operand, 4 * 4 + _LIMB_SIZE_BITS);
        REQUIRE(
            operand
            == std::vector<mp_limb_t> { mp_limb_t(0x3abc000000000000),
                                        mp_limb_t(0x12),
                                        mp_limb_t(0) }
        );

        limb_vector_lsl(operand, 4 * 4 + _LIMB_SIZE_BITS);
        REQUIRE(operand == std::vector<mp_limb_t> { 0, 0, mp_limb_t(0x00123abc) });

        limb_vector_lsl(operand, 4);
        REQUIRE(operand == std::vector<mp_limb_t> { 0, 0, mp_limb_t(0x0123abc0) });

        limb_vector_lsl(operand, 0);
        REQUIRE(operand == std::vector<mp_limb_t> { 0, 0, mp_limb_t(0x0123abc0) });
    }
}

TEST_CASE("limb_vector_bit_width()")
{
    { /* Test #1 */
        std::vector<mp_limb_t> v { 0, 0, 0 };
        REQUIRE(limb_vector_leading_zeros(v) == 3 * _LIMB_SIZE_BITS);
    }

    { /* Test #2 */
        std::vector<mp_limb_t> v { mp_limb_t(-1), mp_limb_t(-1) };
        REQUIRE(limb_vector_leading_zeros(v) == 0);
    }

    { /* Test #3 */
        std::vector<mp_limb_t> v { 0, 0x800, 0, 0, 0, 0 };
        REQUIRE(
            limb_vector_leading_zeros(v) == 4 * _LIMB_SIZE_BITS + _LIMB_SIZE_BITS - 12
        );
    }

    { /* Test #4 */
        std::vector<mp_limb_t> v { 0, 0x001, 0, 0 };
        REQUIRE(
            limb_vector_leading_zeros(v) == 2 * _LIMB_SIZE_BITS + _LIMB_SIZE_BITS - 1
        );
    }
}
