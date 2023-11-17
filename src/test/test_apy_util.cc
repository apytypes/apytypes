#include "../apy_util.h"
#include "catch.hpp"

#include <iterator>
#include <stdexcept>

TEST_CASE("to_nibble_list()")
{
    using int64_vec = std::vector<int64_t>;
    using uint8_vec = std::vector<uint8_t>;

    // Zero-vector results in nibble-list with one element
    REQUIRE(to_nibble_list(int64_vec{ 0 }) == uint8_vec{ 0 });

    // Simple tests
    REQUIRE(to_nibble_list(int64_vec{ 1 }) == uint8_vec{ 1 });
    REQUIRE(to_nibble_list(int64_vec{ 15 }) == uint8_vec{ 15 });
    REQUIRE(to_nibble_list(int64_vec{ 16 }) == uint8_vec{ 0, 1 });
    REQUIRE(to_nibble_list(int64_vec{ 0xff }) == uint8_vec{ 0x0f, 0x0f });
    REQUIRE(to_nibble_list(int64_vec{ 0xfca }) == uint8_vec{ 0x0a, 0x0c, 0x0f });

    // Longer test
    REQUIRE(
        to_nibble_list(int64_vec{
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
    using int64_vec = std::vector<int64_t>;
    using uint8_vec = std::vector<uint8_t>;

    { /* Zero-element */
        auto nibble_list = to_nibble_list(int64_vec{ 0x00 });
        bool out_bit = nibble_list_shift_left_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec{ 0 });
        REQUIRE(out_bit == 0);
    }
    { /* Simple shift */
        auto nibble_list = to_nibble_list(int64_vec{ 0x88F7 });
        bool out_bit = nibble_list_shift_left_once(nibble_list);
        REQUIRE(nibble_list == uint8_vec{ 0x0E, 0x0E, 0x01, 0x01 });
        REQUIRE(out_bit == 1);
    }
    { /* More complex shift */
        auto nibble_list = to_nibble_list(int64_vec{
            int64_t(0xF000070F0056789A),
            0x13373,
            18,
            0xFFFFF00
        });
        bool out_bit = nibble_list_shift_left_once(nibble_list);
        REQUIRE(nibble_list == 
            uint8_vec{
                0x4, 0x3, 0x1, 0xF, 0xC, 0xA, 0, 0, 0xE, 0x1, 0xE, 0, 0, 0, 0, 0xE,
                0x7, 0xE, 0x6, 0x6, 0x2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0x4, 0x2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0x0, 0x0, 0xE, 0xF, 0xF, 0xF, 0xF
            }
        );
        REQUIRE(out_bit == 1);
    }
}

TEST_CASE("double_dabble()")
{
    using int64_vec = std::vector<int64_t>;
    using uint8_vec = std::vector<uint8_t>;

    // Double-dabble only works for positive integers
    REQUIRE_THROWS_AS(double_dabble(int64_vec{ -1 }), std::domain_error);

    // Simple tests for double-dabble
    REQUIRE(double_dabble(int64_vec{ 0 }) == uint8_vec{ 0 });
    REQUIRE(double_dabble(int64_vec{ 1 }) == uint8_vec{ 1 });
    REQUIRE(double_dabble(int64_vec{ 9 }) == uint8_vec{ 9 });
    REQUIRE(double_dabble(int64_vec{ 10 }) == uint8_vec{ 0, 1 });

    // Problematique test #1
    REQUIRE(double_dabble(int64_vec{ 0x0ff }) == uint8_vec{ 5, 5, 2 });
    REQUIRE(double_dabble(int64_vec{ 0x100 }) == uint8_vec{ 6, 5, 2 });
    REQUIRE(double_dabble(int64_vec{ 0x0ffff }) == uint8_vec{ 5, 3, 5, 5, 6 });
    REQUIRE(double_dabble(int64_vec{ 0x10000 }) == uint8_vec{ 6, 3, 5, 5, 6 });

    // Larger tests
    REQUIRE(
        double_dabble(int64_vec{ 0x7ff003011fff2fef })
        == uint8_vec{
            3, 8, 5, 3, 7, 0, 4, 9, 5, 0, 4, 7, 1, 7, 8, 8, 1, 2, 9
        }
    );
    REQUIRE(
        double_dabble(int64_vec{ 0x00FFFFFFFFFA70CE, 0x1234500 })
        == uint8_vec{
            8, 3, 8, 5, 0, 8, 6, 9, 3, 9, 8, 6, 2, 3, 2, 7, 6, 8, 6, 5,
            2, 3, 2, 1, 2, 5, 3
        }
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
