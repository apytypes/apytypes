#include "../apy_int.h"
#include "catch.hpp"

#include <stdexcept>

TEST_CASE("APyInts must have a positive non-zero size")
{
    REQUIRE_THROWS_AS( APyInt(0), std::domain_error );

    // Should not throw
    APyInt(1);
}

TEST_CASE("APyTypes underlying data vector correct size")
{
    REQUIRE(APyInt(128).bits() == 128);
    REQUIRE(APyInt(128).vector_size() == 2);
    REQUIRE(APyInt(129).vector_size() == 3);
}

TEST_CASE("Test equality operator")
{
    // Two default zero initialized APyInts are equal
    REQUIRE(APyInt(1) == APyInt(100));

    // APyInts equality does not depend on their lengith, but of their value
    REQUIRE(APyInt(3, 2) == APyInt(200, 2));
    REQUIRE(APyInt(64, 5) == APyInt(6, 5));
}

TEST_CASE("Test inequality operator")
{
}

TEST_CASE("APyInts addition with and without overflow")
{
    //REQUIRE(APyInt(4,3) + APyInt(4, 2) == APyInt(5, 5));
}
