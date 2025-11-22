#ifndef APYTYPES_THREAD_POOL_H
#define APYTYPES_THREAD_POOL_H

#ifdef BS_THREAD_POOL_HPP
#error "Please do not include BS_thread_pool.h directly, instead include thread_pool.h"
#endif

#ifdef __EMSCRIPTEN__

#include "apytypes_util.h"

#include <fmt/format.h>

#include <optional>

struct [[nodiscard]] ThreadPool {

#define MAKE_THREADPOOL_EXCEPTION()                                                    \
    NotImplementedException(                                                           \
        fmt::format("ThreadPool::{}: thread pool unusable with Emscripten", __func__)  \
    )

    ThreadPool(std::size_t) { }

    template <typename T1, typename T2, typename F>
    void detach_loop(const T1, const T2, F&&, const std::size_t = 0, const int = 0)
    {
        throw MAKE_THREADPOOL_EXCEPTION();
    }

    std::size_t get_thread_count() const noexcept { return 1; }
    void reset(const std::size_t) { throw MAKE_THREADPOOL_EXCEPTION(); };
    void wait() const { throw MAKE_THREADPOOL_EXCEPTION(); }

#undef THREADPOOL_EXCEPTION
};

struct ThisThread {
    static std::optional<std::size_t> get_index() noexcept { return std::nullopt; }
    static std::optional<void*> get_pool() noexcept { return std::nullopt; }
};

constexpr bool APYTYPES_THREADPOOL_ENABLED = false;

#else /* !(__EMSCRIPTEN__) */

#include "BS_thread_pool.hpp"

using ThreadPool = BS::light_thread_pool;
using ThisThread = BS::this_thread;

constexpr bool APYTYPES_THREADPOOL_ENABLED = true;

#endif // #ifdef __EMSCRIPTEN__
#endif // #ifndef APYTYPES_THREAD_POOL_H
