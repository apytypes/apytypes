#include "../apy_fixed.h"
#include "catch.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>

using uint64_vec = std::vector<uint64_t>;


TEST_CASE("APyFixed must have a positive non-zero size")
{
    // Zero bit fixed-point types does not exist
    REQUIRE_THROWS_AS( APyFixed(0, 1), std::domain_error );

    // One bit fixed-point types does not throw on creation
    APyFixed(1, 0);
}

TEST_CASE("Vector initialization must be consistent with APyFixed word-length")
{
    // Vector-initialization fails on size missmatch
    REQUIRE_THROWS_AS( APyFixed(65, 0, uint64_vec{0}), std::domain_error );

    // Vector-initalization succeeds on correct sizes
    for (int bits=1; bits<64; bits++) {
        APyFixed(bits, 0, uint64_vec{0});
    }

}

TEST_CASE("APyFixed::bits(), APyFixed::int_bits() and APyFixed::vector_size()")
{
    REQUIRE(APyFixed(128, 12).bits()        == 128);
    REQUIRE(APyFixed(128, 12).int_bits()    == 12);
    REQUIRE(APyFixed(128, 0).vector_size()  == 2);
    REQUIRE(APyFixed(129, 0).vector_size()  == 3);
    REQUIRE(APyFixed(1, 0).vector_size()    == 1);
}

TEST_CASE("APyFixed::twos_complement_overflow()")
{
    REQUIRE(
        APyFixed(128, 1, 
        uint64_vec{0x0, 0x8000000000000000}).to_string_dec() 
        == "-1"
    );
    REQUIRE(
        APyFixed(128, 1, 
        uint64_vec{0x0, 0x4000000000000000}).to_string_dec() 
        == "0.5"
    );
    REQUIRE(
        APyFixed(96, 1, 
        uint64_vec{0x0, 0xFFFFFFFF00000000}).to_string_dec() 
        == "0"
    );
    REQUIRE(
        APyFixed(96, 1, 
        uint64_vec{0x0, 0xFFFFFFFF80000000}).to_string_dec() 
        == "-1"
    );
    REQUIRE(
        APyFixed(96, 1, 
        uint64_vec{0x0, 0xFFFFFFFF40000000}).to_string_dec() 
        == "0.5"
    );

}


TEST_CASE("APyFixed::_data_asl()")
{
    {  /* Test #1 */
        APyFixed fix(32, 32, uint64_vec{ uint64_t(-1) });
        REQUIRE(fix._data_asl(0) == fix._data);
        REQUIRE(fix._data_asl(32) == uint64_vec{ 0xFFFFFFFF00000000              });
        REQUIRE(fix._data_asl(33) == uint64_vec{ 0xFFFFFFFE00000000, uint64_t(-1)});
    }

    {  /* Test #2 */
        APyFixed fix(
            128, 128,
            uint64_vec{ 0xDEADBEEFDEADBEEF, 0x7FFFFFFBADBADBAD }
        );
        REQUIRE(fix._data_asl(0) == fix._data);
        REQUIRE(
            fix._data_asl(256 + 4*10) ==
            uint64_vec{
                0, 0, 0, 0,
                0xADBEEF0000000000,
                0xBADBADDEADBEEFDE,
                0x7FFFFFFBAD
            }
        );
    }

    {  /* Test #3 */
        APyFixed fix(
            128, 128,
            uint64_vec{ 0xDEADBEEFDEADBEEF, 0x8FFFFFFBADBADBAD }
        );
        REQUIRE(fix._data_asl(0) == fix._data);
        REQUIRE(
            fix._data_asl(256 + 4*10) ==
            uint64_vec{
                0, 0, 0, 0,
                0xADBEEF0000000000,
                0xBADBADDEADBEEFDE,
                0xFFFFFF8FFFFFFBAD
            }
        );
    }
}
