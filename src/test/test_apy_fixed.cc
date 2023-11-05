#include "../apy_fixed.h"
#include "catch.hpp"

#include <stdexcept>

TEST_CASE("APyFixed must have a positive non-zero size")
{
    REQUIRE_THROWS_AS( APyFixed(0, 1), std::domain_error );

    // Should not throw
    APyFixed(1, 1);
}

TEST_CASE("APyTypes underlying data vector correct size")
{
    REQUIRE(APyFixed(128, 12).bits()        == 128);
    REQUIRE(APyFixed(128, 12).int_bits()    == 12);
    REQUIRE(APyFixed(128, 0).vector_size()  == 2);
    REQUIRE(APyFixed(129, 0).vector_size()  == 3);
}

TEST_CASE("Test equality operator")
{
    // Two default zero initialized APyFixed are equal no matter their bit widths
    REQUIRE(APyFixed(1, 4) == APyFixed(3, -2));

    // APyFixed equality does not depend on their lengith, but of their value
    //REQUIRE(APyFixed(3, 2) == APyFixed(200, 2));
    //REQUIRE(APyFixed(64, 5) == APyFixed(6, 5));
}

TEST_CASE("Test inequality operator")
{
}

TEST_CASE("APyFixeds addition with and without overflow")
{
    //REQUIRE(APyInt(4,3) + APyInt(4, 2) == APyInt(5, 5));
}
