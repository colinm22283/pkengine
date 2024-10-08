#pragma once

#include "config.hpp"

#include "pkengine/exception/runtime.hpp"

namespace PKEngine::Assert {
    template<typename ExceptionType>
    constexpr void runtime_assert(auto && condition) {
        if constexpr (runtime_assert_enable) {
            if (!condition()) throw ExceptionType();
        }
    }
    template<typename ExceptionType>
    constexpr void runtime_assert(bool condition) {
        if constexpr (runtime_assert_enable) {
            if (!condition) throw ExceptionType();
        }
    }
}

#define PKENGINE_INLINE_RUNTIME_ASSERT(condition, message)                 \
do {                                                                       \
    PKENGINE_RUNTIME_EXCEPTION(_exception_to_throw_t, message);            \
    PKEngine::Assertion::runtime_assert<_exception_to_throw_t>(condition); \
} while (0);
