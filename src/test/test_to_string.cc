#include "../apy_fixed.h"
#include "catch.hpp"
#include <iostream>

using uint64_vec = std::vector<uint64_t>;
#define INT64_VEC(...) int64_vec{ ... };


TEST_CASE("APyFixed::to_str_dec()")
{
    // Single to_str_dec() point test
    const auto POINT_TEST = [](
        unsigned bits,
        int int_bits,
        const uint64_vec &vec_in,
        const char *expected_result
    ) {
        APyFixed fixed(bits, int_bits, vec_in);
        REQUIRE(fixed.to_string_dec() == expected_result);
    };

    // Simple integer tests
    POINT_TEST( 400,      0, uint64_vec{ 0, 0, 0, 0, 0, 0, 0 }, "0");
    POINT_TEST( 400,    400, uint64_vec{ 0, 0, 0, 0, 0, 0, 0 }, "0");
    POINT_TEST( 400,  -1400, uint64_vec{ 0, 0, 0, 0, 0, 0, 0 }, "0");
    POINT_TEST( 400,  12000, uint64_vec{ 0, 0, 0, 0, 0, 0, 0 }, "0");
    POINT_TEST(   8,      8, uint64_vec{ 0 }, "0" );
    POINT_TEST(  64,     64,
        uint64_vec{  1234000876300021324 },
        "1234000876300021324"
    );
    POINT_TEST(  64,     64,
        uint64_vec{ static_cast<unsigned long>(-4321000867300021394) },
        "-4321000867300021394"
    );
    POINT_TEST(  32,    200,
        uint64_vec{ 0x976541 },
        "3712207424220948591436712392519649713293914084621881966592"
    );
    POINT_TEST( 128, 128,
        uint64_vec{ 0x1234, 0xABCD },
        "811306251105819789627956"
    );
    POINT_TEST( 128, 128,
        uint64_vec{ 0x1234000067ABC, uint64_t(0xfff123ull << (64-4*6) ) },
        "-77174568541894605962799058381276484"
    );

    // Fractional numbers test
    POINT_TEST(32,  28, uint64_vec{ 0x976541 }, "620116.0625");
    POINT_TEST(32,  23, uint64_vec{ 0x976541 }, "19378.626953125");
    POINT_TEST(32,   1, uint64_vec{ 0x976541 }, "0.0046202247031033039093017578125");
    POINT_TEST(32,   0, uint64_vec{ 0x976541 }, "0.00231011235155165195465087890625");
    POINT_TEST(32,  -1, uint64_vec{ 0x976541 }, "0.001155056175775825977325439453125");
    POINT_TEST(26,  -1, uint64_vec{ 0x976541 }, "0.073923595249652862548828125");
    POINT_TEST(26,  -9, uint64_vec{ 0x976541 }, "0.00028876404394395649433135986328125");

    // Special case:
    // The aboslute value during negation in to_str_dec() increases the underlying
    // vector size by one.
    POINT_TEST( 64,  64, uint64_vec{ 0x0000000000000013 }, "19");
    POINT_TEST( 64,  64, uint64_vec{ 0xFFFFFFFFFFFFFFFF }, "-1");
    POINT_TEST( 64,  64,
        uint64_vec{ 0x8000000000000000 },
        "-9223372036854775808");
    POINT_TEST(128, 128,
        uint64_vec{ 0, 0x8000000000000000 },
        "-170141183460469231731687303715884105728");

}
