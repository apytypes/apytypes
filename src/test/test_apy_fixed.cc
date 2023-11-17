#include "../apy_fixed.h"
#include "catch.hpp"

#include <iostream>
#include <stdexcept>


TEST_CASE("APyFixed must have a positive non-zero size")
{
    // Zero bit fixed-point types does not exist
    REQUIRE_THROWS_AS( APyFixed(0, 1), std::domain_error );

    // One bit fixed-point types does not throw on creation
    APyFixed(1, 0);
}


TEST_CASE("Underlying data vector sizing")
{
    REQUIRE(APyFixed(128, 12).bits()        == 128);
    REQUIRE(APyFixed(128, 12).int_bits()    == 12);
    REQUIRE(APyFixed(128, 0).vector_size()  == 2);
    REQUIRE(APyFixed(129, 0).vector_size()  == 3);
    REQUIRE(APyFixed(1, 0).vector_size()    == 1);
}

TEST_CASE("Test equality operator")
{
    // Two default zero initialized APyFixed are equal no matter their bit widths
    REQUIRE(APyFixed(3, 2) == APyFixed(200, 2));
    REQUIRE(APyFixed(64, 5) == APyFixed(6, -20000));
}

TEST_CASE("Test inequality operator")
{
}

TEST_CASE("Addition")
{
    //REQUIRE(APyInt(4,3) + APyInt(4, 2) == APyInt(5, 5));
}

TEST_CASE("to_str_dec()")
{
     using int64_vec = std::vector<int64_t>;

    // Single to_str_dec() point test
    const auto POINT_TEST = [](
        unsigned bits,
        int int_bits,
        const std::vector<int64_t> &vec_in,
        const char *expected_result
    ) {
        APyFixed fixed(bits, int_bits, vec_in);
        REQUIRE(fixed.to_string_dec() == expected_result);
    };

    // Simple integer tests
    POINT_TEST( 400,      0, int64_vec{ 0, 0, 0, 0, 0, 0, 0 }, "0");
    POINT_TEST( 400,    400, int64_vec{ 0, 0, 0, 0, 0, 0, 0 }, "0");
    POINT_TEST( 400,  -1400, int64_vec{ 0, 0, 0, 0, 0, 0, 0 }, "0");
    POINT_TEST( 400,  12000, int64_vec{ 0, 0, 0, 0, 0, 0, 0 }, "0");
    POINT_TEST(   8,      8, int64_vec{ 0 }, "0" );
    POINT_TEST(  64,     64, int64_vec{  1234000876300021324 },  "1234000876300021324");
    POINT_TEST(  64,     64, int64_vec{ -4321000867300021394 }, "-4321000867300021394");
    POINT_TEST(  32,    200,
        int64_vec{ 0x976541 },
        "3712207424220948591436712392519649713293914084621881966592"
    );
    POINT_TEST( 128, 128,
        int64_vec{ 0x1234, 0xABCD },
        "811306251105819789627956"
    );
    POINT_TEST( 128, 128,
        int64_vec{ 0x1234000067ABC, int64_t(0xfff123ull << (64-4*6) ) },
        "-77174568541894605962799058381276484"
    );

    // Fractional numbers test
    POINT_TEST(32,  28, int64_vec{ 0x976541 }, "620116.0625");
    POINT_TEST(32,  23, int64_vec{ 0x976541 }, "19378.626953125");
    POINT_TEST(32,   1, int64_vec{ 0x976541 }, "0.0046202247031033039093017578125");
    POINT_TEST(32,   0, int64_vec{ 0x976541 }, "0.00231011235155165195465087890625");
    POINT_TEST(32,  -1, int64_vec{ 0x976541 }, "0.001155056175775825977325439453125");
    POINT_TEST(26,  -1, int64_vec{ 0x976541 }, "0.073923595249652862548828125");
    POINT_TEST(26,  -9, int64_vec{ 0x976541 }, "0.00028876404394395649433135986328125");

    // Special case:
    // The aboslute value during negation in to_str_dec() increases the underlying
    // vector size by one.
    POINT_TEST( 64,  64, int64_vec{ int64_t(0x0000000000000013) }, "19");
    POINT_TEST( 64,  64, int64_vec{ int64_t(0xFFFFFFFFFFFFFFFF) }, "-1");
    POINT_TEST( 64,  64,
        int64_vec{ int64_t(0x8000000000000000) },
        "-9223372036854775808");
    POINT_TEST(128, 128,
        int64_vec{ 0, int64_t(0x8000000000000000) },
        "-170141183460469231731687303715884105728");

}
