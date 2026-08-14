// simd_hints.h
#pragma once

#if defined(__EMSCRIPTEN__)
#define VECTORIZE_LOOP
#elif defined(__clang__)
#define VECTORIZE_LOOP _Pragma("clang loop vectorize(enable) interleave(enable)")
#elif defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER)
#define VECTORIZE_LOOP _Pragma("vector always")
#elif defined(__GNUC__)
#define VECTORIZE_LOOP _Pragma("GCC ivdep")
#elif defined(_MSC_VER)
#define VECTORIZE_LOOP __pragma(loop(ivdep))
#else
#define VECTORIZE_LOOP
#endif
