#include "apytypes_common.h"
#include "apytypes_intrinsics.h"
#include "apytypes_thread_pool.h"

// Python object access through Pybind
#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
namespace nb = nanobind;

#include <cstdlib> // std::getenv
#include <random>  // std::random_devive, std::mt19937_64

#include <fmt/format.h>

/* ********************************************************************************** *
 * *            Random number engines for APyTypes stochastic quantization          * *
 * ********************************************************************************** */

// Seeds used to initialize the stochastic rounding engines
thread_local static std::uint64_t rnd64_fx_seed = std::random_device {}();
thread_local static std::uint64_t rnd64_fp_seed = std::random_device {}();

// Current thread-local RNGs for stochastic quantization: uniform on interval [0, 2^64)
thread_local static std::mt19937_64 current_mt19937_fx { rnd64_fx_seed };
thread_local static std::mt19937_64 current_mt19937_fp { rnd64_fp_seed };

// Draw a 64-bit uniform random number from the RNG
std::uint64_t rnd64_fx() { return current_mt19937_fx(); }
std::uint64_t rnd64_fp() { return current_mt19937_fp(); }

// Retrieve the seed that was used to initialize the active RNG
std::uint64_t get_rnd64_fx_seed() { return rnd64_fx_seed; }
std::uint64_t get_rnd64_fp_seed() { return rnd64_fp_seed; }

// Retrieve a *copy* of the current mt19937 RNG
std::mt19937_64 get_rnd64_fx_engine() { return current_mt19937_fx; }
std::mt19937_64 get_rnd64_fp_engine() { return current_mt19937_fp; }

// Set the current mt19937 RNG engine
void set_rnd64_fx_engine(const std::mt19937_64& engine) { current_mt19937_fx = engine; }
void set_rnd64_fp_engine(const std::mt19937_64& engine) { current_mt19937_fp = engine; }

// Overwrite which seed value that was used to initialize the current RNG
void set_rnd64_fx_seed(std::uint64_t seed) { rnd64_fx_seed = seed; }
void set_rnd64_fp_seed(std::uint64_t seed) { rnd64_fp_seed = seed; }

// Reset the default stochastic quantization random number generators
void rst_default_rnd64_fx(std::uint64_t seed)
{
    rnd64_fx_seed = seed;
    current_mt19937_fx.seed(seed);
}
void rst_default_rnd64_fp(std::uint64_t seed)
{
    rnd64_fp_seed = seed;
    current_mt19937_fp.seed(seed);
}

/* ********************************************************************************** *
 * *             Global (`thread_local`) quantization mode for APyFloat             * *
 * ********************************************************************************** */

// Quantization mode for floating-point arithmetic
thread_local static QuantizationMode qntz_mode_fl = QuantizationMode::RND_CONV;

// Get the thread-local floating-point quantization mode
QuantizationMode get_float_quantization_mode() { return qntz_mode_fl; }

// Set the thread-local floating-point quantization mode
void set_float_quantization_mode(QuantizationMode mode) { qntz_mode_fl = mode; }

/* ********************************************************************************** *
 * *                Global (`thread_local`) casting mode for APyFixed               * *
 * ********************************************************************************** */

//! Default casting mode: { `TRN`, `WRAP` }
thread_local static APyFixedCastOption global_cast_option_fixed
    = { QuantizationMode::TRN, OverflowMode::WRAP };

APyFixedCastOption get_fixed_cast_mode() { return global_cast_option_fixed; }

void set_fixed_cast_mode(const APyFixedCastOption& mode)
{
    global_cast_option_fixed = mode;
}

/* ********************************************************************************** *
 * *            Global (`thread_local`) floating-point accumulator mode             * *
 * ********************************************************************************** */

//! Global accumulator option, default value: `std::nullopt`
thread_local static std::optional<APyFloatAccumulatorOption>
    global_accumulator_option_float = std::nullopt;

std::optional<APyFloatAccumulatorOption> get_accumulator_mode_float()
{
    return global_accumulator_option_float;
}

void set_accumulator_mode_float(const std::optional<APyFloatAccumulatorOption>& mode)
{
    global_accumulator_option_float = mode;
}

/* ********************************************************************************** *
 * *            Global (`thread_local`) fixed-point accumulator mode                * *
 * ********************************************************************************** */

//! Global accumulator option (default value: `std::nullopt`)
thread_local static std::optional<APyFixedAccumulatorOption>
    global_accumulator_option_fixed = std::nullopt;

//! Return the global accumulator mode for APyFixed
std::optional<APyFixedAccumulatorOption> get_accumulator_mode_fixed()
{
    return global_accumulator_option_fixed;
}

//! Return the global accumulator mode for APyFixed
void set_accumulator_mode_fixed(const std::optional<APyFixedAccumulatorOption>& mode)
{
    global_accumulator_option_fixed = mode;
}

/* ********************************************************************************** *
 * *                      Preferred third-party array library                       * *
 * ********************************************************************************** */

static ThirdPartyArrayLibrary preferred_array_lib { ThirdPartyArrayLibrary::NUMPY };

void set_array_library(ThirdPartyArrayLibrary array_lib)
{
    preferred_array_lib = array_lib;
}

void set_array_library_from_str(const std::string& array_lib)
{
    if (array_lib == "numpy") {
        set_array_library(ThirdPartyArrayLibrary::NUMPY);
        return;
    } else if (array_lib == "pytorch") {
        set_array_library(ThirdPartyArrayLibrary::PYTORCH);
        return;
    } else if (array_lib == "tensorflow") {
        set_array_library(ThirdPartyArrayLibrary::TENSORFLOW);
        return;
    } else if (array_lib == "jax") {
        set_array_library(ThirdPartyArrayLibrary::JAX);
    } else if (array_lib == "cupy") {
        set_array_library(ThirdPartyArrayLibrary::CUPY);
    } else {
        auto err_msg = fmt::format(
            "set_array_library: library '{}' not exactly in ('numpy', 'pytorch', "
            "'tensorflow', 'jax', 'cupy')",
            array_lib
        );
        throw nb::value_error(err_msg.c_str());
    }
}

ThirdPartyArrayLibrary get_array_library() { return preferred_array_lib; }

std::string get_array_library_as_str()
{
    switch (preferred_array_lib) {
    case ThirdPartyArrayLibrary::NUMPY:
        return "numpy";
    case ThirdPartyArrayLibrary::PYTORCH:
        return "pytorch";
    case ThirdPartyArrayLibrary::TENSORFLOW:
        return "tensorflow";
    case ThirdPartyArrayLibrary::JAX:
        return "jax";
    case ThirdPartyArrayLibrary::CUPY:
        return "cupy";

    default:
        APYTYPES_UNREACHABLE();
    }
}

/* ********************************************************************************** *
 * *                    Threadpool for submitting parallel tasks                    * *
 * ********************************************************************************** */

//! Thread count environment variable
static const char* ENV_THREAD_COUNT = std::getenv("APYTYPES_THREAD_COUNT");

//! The global APyTypes threadpool
ThreadPool thread_pool(
    APYTYPES_THREADPOOL_ENABLED ? (ENV_THREAD_COUNT ? std::atoi(ENV_THREAD_COUNT) : 0)
                                : (0)
);

//! The global thread pool settings
ThreadPoolSettings thread_pool_settings {};
