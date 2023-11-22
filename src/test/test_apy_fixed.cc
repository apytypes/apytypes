#include "../apy_fixed.h"
#include "catch.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>

using int64_vec = std::vector<int64_t>;
#define INT64_VEC(...) int64_vec{ ... };


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

TEST_CASE("APyFixed::twos_complement_overflow()")
{
    REQUIRE(
        APyFixed(128, 1, 
        int64_vec{0x0, int64_t(0x8000000000000000)}).to_string_dec() 
        == "-1"
    );
    REQUIRE(
        APyFixed(128, 1, 
        int64_vec{0x0, int64_t(0x4000000000000000)}).to_string_dec() 
        == "0.5"
    );
    REQUIRE(
        APyFixed(96, 1, 
        int64_vec{0x0, int64_t(0xFFFFFFFF00000000)}).to_string_dec() 
        == "0"
    );
    REQUIRE(
        APyFixed(96, 1, 
        int64_vec{0x0, int64_t(0xFFFFFFFF80000000)}).to_string_dec() 
        == "-1"
    );
    REQUIRE(
        APyFixed(96, 1, 
        int64_vec{0x0, int64_t(0xFFFFFFFF40000000)}).to_string_dec() 
        == "0.5"
    );

}


TEST_CASE("APyFixed::_data_asl()")
{
    {  /* Test #1 */
        APyFixed fix(32, 32, int64_vec{ -1 });
        REQUIRE(fix._data_asl(0) == fix._data);
        REQUIRE(fix._data_asl(32) == int64_vec{ int64_t(0xFFFFFFFF00000000)    });
        REQUIRE(fix._data_asl(33) == int64_vec{ int64_t(0xFFFFFFFE00000000), -1});
    }

    {  /* Test #2 */
        APyFixed fix(
            128, 128,
            int64_vec{ int64_t(0xDEADBEEFDEADBEEF), int64_t(0x7FFFFFFBADBADBAD) }
        );
        REQUIRE(fix._data_asl(0) == fix._data);
        REQUIRE(
            fix._data_asl(256 + 4*10) ==
            int64_vec{
                0, 0, 0, 0,
                int64_t(0xADBEEF0000000000),
                int64_t(0xBADBADDEADBEEFDE),
                int64_t(0x7FFFFFFBAD)
            }
        );
    }

    {  /* Test #3 */
        APyFixed fix(
            128, 128,
            int64_vec{ int64_t(0xDEADBEEFDEADBEEF), int64_t(0x8FFFFFFBADBADBAD) }
        );
        REQUIRE(fix._data_asl(0) == fix._data);
        REQUIRE(
            fix._data_asl(256 + 4*10) ==
            int64_vec{
                0, 0, 0, 0,
                int64_t(0xADBEEF0000000000),
                int64_t(0xBADBADDEADBEEFDE),
                int64_t(0xFFFFFF8FFFFFFBAD)
            }
        );
    }
}
