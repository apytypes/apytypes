#include "catch.hpp"
#include "../apy_int.h"
#include <stdexcept>

TEST_CASE("APyInts must have a positive non-zero size")
{
    REQUIRE_THROWS_AS( APyInt(0), std::domain_error );

    // Should not throw
    APyInt(1);
}

TEST_CASE("Test equality operator")
{
    REQUIRE(APyInt(1) == APyInt(1));
    REQUIRE(APyInt(3, 2) == APyInt(5, 2));
    REQUIRE(APyInt(7, 5) == APyInt(6, 5));
}

TEST_CASE("APyInts addition with and without overflow")
{
    //REQUIRE(APyInt(4,3) + APyInt(4, 2) == APyInt(5, 5));
}
