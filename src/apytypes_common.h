#ifndef _APYTYPES_COMMON_H
#define _APYTYPES_COMMON_H

#include "apytypes_fwd.h"
#include "apytypes_intrinsics.h"
#include "apytypes_thread_pool.h"

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/function.h>

#include <cstdint>  // std::uint32_t, uint64_t
#include <optional> // std::optional
#include <random>   // std::mt19937_64, std::random_device
#include <utility>  // std::in_place_t
#include <variant>

/* ********************************************************************************** *
 * *            Quantization modes, overflow modes and convolution modes            * *
 * ********************************************************************************** */

enum class QuantizationMode {
    TRN,            // !< Truncation, quantization toward minus infinity
    TRN_INF,        // !< Truncation, quantization toward plus infinity
    TRN_ZERO,       // !< Unbiased magnitude truncation, quantization toward zero
    TRN_AWAY,       // !< Truncation away from zero
    TRN_MAG,        // !< Magnitude truncation, quantization toward zero
    RND,            // !< Round to nearest, ties toward plus inf
    RND_ZERO,       // !< Round to nearest, ties toward zero
    RND_INF,        // !< Round to nearest, ties away from zero
    RND_MIN_INF,    // !< Round to nearest, ties toward minus infinity
    RND_CONV,       // !< Round to nearest, ties toward even quantization steps
    RND_CONV_ODD,   // !< Round to nearest, ties toward odd quantization steps
    JAM,            // !< Jamming
    JAM_UNBIASED,   // !< Unbiased jamming
    STOCH_WEIGHTED, // !< Weighted stochastic quantization
    STOCH_EQUAL     // !< Stochastic quantization with equal probability
};

enum class OverflowMode {
    WRAP,        // !< Drop bits left of the MSB (two's complement overflowing)
    SAT,         // !< Saturate on overflow
    NUMERIC_STD, // !< Drop bits left of the MSB, but keep the most significant bit
};

enum class ConvolutionMode { FULL, SAME, VALID };

/* ********************************************************************************** *
 * *           Random number engines for APyTypes stochastic quantization           * *
 * ********************************************************************************** */

//! Uniform 64-bit random number generator function type
using Rnd64Func_t = std::uint64_t();

//! Uniform 64-bit random number generator function pointer type
using Rnd64FuncPtr_t = Rnd64Func_t*;

//! Draw a 64-bit uniform random number from the RNG
std::uint64_t rnd64_fx();
std::uint64_t rnd64_fp();

//! Retrieve the seed that was used to initialize the active RNG
std::uint64_t get_rnd64_fx_seed();
std::uint64_t get_rnd64_fp_seed();

//! Retrieve a *copy* of the current mt19937 RNG
std::mt19937_64 get_rnd64_fx_engine();
std::mt19937_64 get_rnd64_fp_engine();

//! Set the current mt19937 RNG engine
void set_rnd64_fx_engine(const std::mt19937_64& engine);
void set_rnd64_fp_engine(const std::mt19937_64& engine);

//! Overwrite which seed value that was used to initialize the current RNG
void set_rnd64_fx_seed(std::uint64_t seed);
void set_rnd64_fp_seed(std::uint64_t seed);

//! Reset the default stochastic quantization RNG
void rst_default_rnd64_fx(std::uint64_t seed);
void rst_default_rnd64_fp(std::uint64_t seed);

/* ********************************************************************************** *
 * *            Global (`thread_local`) floating-point quantization mode            * *
 * ********************************************************************************** */

//! Return the global quantization mode for APyFloat
QuantizationMode get_float_quantization_mode();

//! Set the global quantization mode for APyFloat
void set_float_quantization_mode(QuantizationMode mode);

/* ********************************************************************************** *
 * *                Global (`thread_local`) casting mode for APyFixed               * *
 * ********************************************************************************** */

struct APyFixedCastOption {
    QuantizationMode quantization; //! Quantization mode to use for cast operations
    OverflowMode overflow;         //! Overflow mode to use for cast operations
};

//! Return the global cast mode for APyFixed
APyFixedCastOption get_fixed_cast_mode();

//! Set the global cast mode for APyFixed
void set_fixed_cast_mode(const APyFixedCastOption& mode);

/* ********************************************************************************** *
 * *              Global (`thread_local`) accumulator mode for APyFixed             * *
 * ********************************************************************************** */

// Accumulator type for APyFixed
struct APyFixedAccumulatorOption {
    int bits;                      //! Total number of bits to use for accumulator
    int int_bits;                  //! Number of integer bits to use for accumulator
    QuantizationMode quantization; //! Quantization mode to use for multiplication
    OverflowMode overflow;         //! Overflow mode to apply after each accumulation
};

//! Return the global accumulator mode for APyFixed
std::optional<APyFixedAccumulatorOption> get_accumulator_mode_fixed();

//! Return the global accumulator mode for APyFixed
void set_accumulator_mode_fixed(const std::optional<APyFixedAccumulatorOption>& mode);

/* ********************************************************************************** *
 * *            Global (`thread_local`) floating-point accumulator mode             * *
 * ********************************************************************************** */

//! Accumulator mode type for APyFloat
struct APyFloatAccumulatorOption {
    //! Number of exponent bits
    std::uint8_t exp_bits;
    //! Number of mantissa bits
    std::uint8_t man_bits;
    //! Exponent bias
    std::optional<exp_t> bias;
    //! Quantization mode
    QuantizationMode quantization;
    //! Psueod RNG seed
    std::uint64_t seed;
    //! RNG engine
    std::mt19937_64 rng_engine;

    APyFloatSpec get_spec(exp_t backup_bias) const noexcept
    {
        return APyFloatSpec { exp_bits, man_bits, bias.value_or(backup_bias) };
    }
};

//! Return the global accumulator mode for APyFloat
std::optional<APyFloatAccumulatorOption> get_accumulator_mode_float();

//! Set the global accumulator mode for APyFloat
void set_accumulator_mode_float(const std::optional<APyFloatAccumulatorOption>& mode);

/* ********************************************************************************** *
 * *                     Preferred third-party array library                        * *
 * ********************************************************************************** */

//! Enum for the supported third-party array libraries
enum class ThirdPartyArrayLibrary {
    NUMPY,
    PYTORCH,
    TENSORFLOW,
    JAX,
    CUPY,
};

//! Type of a third-party array library. Returning this to Python will
template <typename... NB_ARGS>
using ThirdPartyArray = std::variant<
    nanobind::ndarray<nanobind::numpy, NB_ARGS...>,
    nanobind::ndarray<nanobind::pytorch, NB_ARGS...>,
    nanobind::ndarray<nanobind::tensorflow, NB_ARGS...>,
    nanobind::ndarray<nanobind::jax, NB_ARGS...>,
    nanobind::ndarray<nanobind::cupy, NB_ARGS...>>;

//! Python-exported set preferred array library function
void set_array_library_from_str(const std::string& array_lib);

//! Set the preferred third-party array library.
void set_array_library(ThirdPartyArrayLibrary array_lib);

//! Return the currently set preferred third-party array library.
ThirdPartyArrayLibrary get_array_library();

//! Return the currently set preferred third-party array library as a string.
std::string get_array_library_as_str();

//! Convert an un-tagged `nb::ndarray` (Python capsule around `void *`) to a third-party
//! array-library specific array.
template <typename... NB_ARGS>
ThirdPartyArray<NB_ARGS...> make_third_party_ndarray(
    const nanobind::ndarray<NB_ARGS...>& array_ref, ThirdPartyArrayLibrary array_lib
)
{
    namespace nb = nanobind;
    switch (array_lib) {
    case ThirdPartyArrayLibrary::NUMPY: {
        using NDARRAY_T = nb::ndarray<nb::numpy, NB_ARGS...>;
        return ThirdPartyArray<NB_ARGS...>(std::in_place_type<NDARRAY_T>, array_ref);
    }
    case ThirdPartyArrayLibrary::PYTORCH: {
        using NDARRAY_T = nb::ndarray<nb::pytorch, NB_ARGS...>;
        return ThirdPartyArray<NB_ARGS...>(std::in_place_type<NDARRAY_T>, array_ref);
    }
    case ThirdPartyArrayLibrary::TENSORFLOW: {
        using NDARRAY_T = nb::ndarray<nb::tensorflow, NB_ARGS...>;
        return ThirdPartyArray<NB_ARGS...>(std::in_place_type<NDARRAY_T>, array_ref);
    }
    case ThirdPartyArrayLibrary::JAX: {
        using NDARRAY_T = nb::ndarray<nb::jax, NB_ARGS...>;
        return ThirdPartyArray<NB_ARGS...>(std::in_place_type<NDARRAY_T>, array_ref);
    }
    case ThirdPartyArrayLibrary::CUPY: {
        using NDARRAY_T = nb::ndarray<nb::cupy, NB_ARGS...>;
        return ThirdPartyArray<NB_ARGS...>(std::in_place_type<NDARRAY_T>, array_ref);
    }
    default:
        APYTYPES_UNREACHABLE();
    }
}

/* ********************************************************************************** *
 * *                Threadpool for submitting parallel job tasks to                 * *
 * ********************************************************************************** */

//! The global APyTypes threadpool
extern ThreadPool thread_pool;

//! Array-specific thread settings class
struct ArrayThreadSetting {
    std::size_t n_mac_threshold;
};

//! Threadpool settings class
struct ThreadPoolSettings {
    ArrayThreadSetting apyfixedarray { /* n_mac_threshold = */ 10'000 };
    ArrayThreadSetting apycfixedarray { /* n_mac_threshold = */ 2'500 };
    ArrayThreadSetting apyfloatarray { /* n_mac_threshold = */ 5'000 };
    ArrayThreadSetting apycfloatarray { /* n_mac_threshold = */ 1'000 };
};

//! Threadpool settings
extern ThreadPoolSettings thread_pool_settings;

#endif // _APYTYPES_COMMON_H
