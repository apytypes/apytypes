#include "../apy_util.h"
#include "catch.hpp"

#include <stdexcept>

TEST_CASE("to_nibble_list")
{
    using int64_vec = std::vector<int64_t>;
    using uint8_vec = std::vector<uint8_t>;

    // Zero-vector results in nibble-list with one element
    REQUIRE(to_nibble_list(int64_vec{ 0 }) == uint8_vec{ 0 });

    // Simple tests
    REQUIRE(to_nibble_list(int64_vec{ 1 }) == uint8_vec{ 1 });
    REQUIRE(to_nibble_list(int64_vec{ 15 }) == uint8_vec{ 15 });
    REQUIRE(to_nibble_list(int64_vec{ 16 }) == uint8_vec{ 0, 1 });

    // Longer test
    REQUIRE(
        to_nibble_list(int64_vec{ 0xFF, 19, 18, 17 }) ==
        uint8_vec{
            0x0F, 0x0F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0x03, 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0x02, 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0x01, 0x01
        }
    );
}

TEST_CASE("nibble_shift_left_once")
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
        auto nibble_list = to_nibble_list(int64_vec{ 0xFF, 19, 18, 0xF7 });
        bool out_bit = nibble_list_shift_left_once(nibble_list);
        REQUIRE(nibble_list == 
            uint8_vec{
                0x0E, 0x0F, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0x06, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0x04, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0x0E, 0x0E
            }
        );
        REQUIRE(out_bit == 1);
    }
}

TEST_CASE("Test double-dabble algorithm")
{
    using int64_vec = std::vector<int64_t>;
    using uint8_vec = std::vector<uint8_t>;

    // Double-dabble only works for positive integers
    REQUIRE_THROWS_AS(double_dabble(int64_vec{ -1 }), std::domain_error);

    // Tests for double dabble
    REQUIRE(double_dabble(int64_vec{ 0 }) == uint8_vec{ 0 });
    REQUIRE(double_dabble(int64_vec{ 1 }) == uint8_vec{ 1 });
    REQUIRE(double_dabble(int64_vec{ 9 }) == uint8_vec{ 9 });
    REQUIRE(double_dabble(int64_vec{ 10 }) == uint8_vec{ 0, 1 });
    REQUIRE(
        double_dabble(int64_vec{ 0x7FFFFFFFFFFFFFFF })
        == uint8_vec{ 7, 0, 8, 5, 7, 7, 4, 5, 8, 6, 3, 0, 2, 7, 3, 3, 2, 2, 9 }
    );
    REQUIRE(
        double_dabble(int64_vec{ 0x1FFFFFFFFFFFFFFF, 0x12345 })
        == uint8_vec{ 
            1, 9, 9, 0, 4, 9, 9, 2, 9, 1, 6, 1, 9, 9, 6, 7, 7, 7, 3, 8, 4, 5, 7, 3, 1
        }
    );

}
