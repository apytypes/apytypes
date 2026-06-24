#ifndef _APYTYPES_CONTEXT_H
#define _APYTYPES_CONTEXT_H

#include "apytypes_common.h"

#include <optional>

//! Base class defining the interface for contexts
class ContextManager {
public:
    virtual ~ContextManager() = default;
    virtual void enter_context() = 0;
    virtual void exit_context() = 0;
};

/* ********************************************************************************** *
 * *                      Floating-point quantization context                       * *
 * ********************************************************************************** */

class APyFloatQuantizationContext : public ContextManager {
public:
    APyFloatQuantizationContext(
        const QuantizationMode& new_mode,
        std::optional<std::uint64_t> seed = std::nullopt
    );
    void enter_context() override;
    void exit_context() override;

private:
    QuantizationMode prev_mode, context_mode;
    std::uint64_t prev_seed, context_seed;
    std::mt19937_64 prev_engine, context_engine;
};

/* ********************************************************************************** *
 * *                       Floating-point accumulator context                       * *
 * ********************************************************************************** */

// Accumulator context
class APyFloatAccumulatorContext : public ContextManager {
public:
    APyFloatAccumulatorContext(
        std::optional<int> = std::nullopt,
        std::optional<int> = std::nullopt,
        std::optional<exp_t> = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt,
        std::optional<std::uint64_t> seed = std::nullopt
    );
    void enter_context() override;
    void exit_context() override;

private:
    std::optional<APyFloatAccumulatorOption> previous_mode, context_mode;
    std::uint64_t previous_seed;
    std::mt19937_64 previous_engine;
};

/* ********************************************************************************** *
 * *                            Fixed-point cast context                            * *
 * ********************************************************************************** */

class APyFixedCastContext : public ContextManager {
public:
    APyFixedCastContext(
        std::optional<QuantizationMode> quantization = std::nullopt,
        std::optional<OverflowMode> overflow = std::nullopt
    );
    void enter_context() override;
    void exit_context() override;

private:
    std::optional<QuantizationMode> context_qntz_mode;
    std::optional<OverflowMode> context_overflow_mode;
    APyFixedCastOption previous_mode;
};

/* ********************************************************************************** *
 * *                         Fixed-point accumulator context                        * *
 * ********************************************************************************** */

// Accumulator context
class APyFixedAccumulatorContext : public ContextManager {
public:
    APyFixedAccumulatorContext(
        std::optional<int> int_bits = std::nullopt,
        std::optional<int> frac_bits = std::nullopt,
        std::optional<QuantizationMode> quantization = std::nullopt,
        std::optional<OverflowMode> overflow = std::nullopt,
        std::optional<int> bits = std::nullopt
    );
    void enter_context() override;
    void exit_context() override;

private:
    std::optional<APyFixedAccumulatorOption> previous_mode, context_mode;
};

#endif // _APYTYPES_CONTEXT_H
