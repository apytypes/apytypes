#include "apytypes_common.h"
#include <random>
#include <stdexcept>

RoundingMode rounding_mode = RoundingMode::TIES_TO_EVEN;

void set_rounding_mode(RoundingMode mode) { rounding_mode = mode; }

RoundingMode get_rounding_mode() { return rounding_mode; }

// This creates a random seed on every program start.
std::uint64_t rounding_seed = std::random_device {}();

// A random number engine is used instead of purely std::random_device so that runs can
// be reproducible.
std::mt19937_64 gen64(rounding_seed);

void set_rounding_seed(std::uint64_t seed)
{
    rounding_seed = seed;
    gen64.seed(seed);
}

std::uint64_t get_rounding_seed() { return rounding_seed; }

std::uint64_t random_number() { return gen64(); }
