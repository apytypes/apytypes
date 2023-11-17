#include "../apy_fixed.h"
#include "catch.hpp"

#include <iostream>
#include <stdexcept>


TEST_CASE("Underlying data vector sizing")
{
    REQUIRE(APyFixed(128, 12).bits()        == 128);
    REQUIRE(APyFixed(128, 12).int_bits()    == 12);
    REQUIRE(APyFixed(128, 0).vector_size()  == 2);
    REQUIRE(APyFixed(129, 0).vector_size()  == 3);
    REQUIRE(APyFixed(1, 0).vector_size()    == 1);
}

TEST_CASE("APyFixed must have a positive non-zero size")
{
    // Zero-bit fixed-point types does not exist
    REQUIRE_THROWS_AS( APyFixed(0, 1), std::domain_error );

    // One bit fixed-point types does not throw on creation
    APyFixed(1, 1);
}

TEST_CASE("Test equality operator")
{
    // Two default zero initialized APyFixed are equal no matter their bit widths
    REQUIRE(APyFixed(1, 4) == APyFixed(3, -2));
    REQUIRE(APyFixed(10, 10) == APyFixed(100, 10));

    // APyFixed equality does not depend on their lengith, but of their value
    //REQUIRE(APyFixed(3, 2) == APyFixed(200, 2));
    //REQUIRE(APyFixed(64, 5) == APyFixed(6, 5));
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

    // Point test
    const auto POINT_TEST = [](
        unsigned bits,
        int int_bits,
        const std::vector<int64_t> &vec_in,
        const char *expected_result
    ) {
        APyFixed fixed(bits, int_bits);
        fixed.from_vector(vec_in);
        REQUIRE(fixed.to_string_dec() == expected_result);
    };

    // Simple integer tests
    POINT_TEST(   400,      0, int64_vec{ 0, 0, 0, 0, 0, 0, 0 }, "0" );
    POINT_TEST(   400,    400, int64_vec{ 0, 0, 0, 0, 0, 0, 0 }, "0" );
    POINT_TEST(   400,  -1400, int64_vec{ 0, 0, 0, 0, 0, 0, 0 }, "0" );
    //POINT_TEST(   400,  12000, int64_vec{ 0, 0, 0, 0, 0, 0, 0 }, "0" );
    POINT_TEST(   8,   8, int64_vec{ 0 }, "0" );
    POINT_TEST(  64,  64, int64_vec{  1234000876300021324 },  "1234000876300021324" );
    POINT_TEST(  64,  64, int64_vec{ -4321000867300021394 }, "-4321000867300021394" );
    POINT_TEST( 128, 128,
        int64_vec{ 0x1234, 0xABCD },
        "811306251105819789627956"
    );
    POINT_TEST( 128, 128,
        int64_vec{ 0x1234000067ABC, int64_t(0xfff123ull << (64-4*6) ) },
        "-77174568541894605962799058381276484"
    );

    // Fractional numbers test
    //POINT_TEST(   8, 13, int64_vec{ 0x18 }, "1.5" );
    POINT_TEST(   8,  4, int64_vec{ 0x18 }, "1.5" );
    POINT_TEST(   8,  1, int64_vec{ 0x18 }, "0.1875" );
    POINT_TEST(   8,  0, int64_vec{ 0x18 }, "0.09375" );
    POINT_TEST(   8, -1, int64_vec{ 0x18 }, "0.046875" );
    POINT_TEST(   8, -7, int64_vec{ 0x18 }, "0.000732421875" );
    POINT_TEST(   8, -9, int64_vec{ 0x18 }, "0.00018310546875" );

    // Good example for some profiling of the printing mechanism?
    //POINT_TEST(   8, -123456, int64_vec{ 0x18 }, "0.046875" );

}
