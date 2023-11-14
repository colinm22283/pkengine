#pragma once

#include <iostream>

namespace PKEngine {
    template<bool override_log_disable>
    class _LoggerConsoleStream {
    protected:
#ifdef PKENGINE_LOGGER_ENABLE
        static constexpr bool logger_enable = true;
#else
        static constexpr bool logger_enable = false;
#endif
    public:
        template<typename T>
        inline auto & operator<<(T & v) const noexcept {
            if constexpr (logger_enable || override_log_disable) std::cout << v;
            return *this;
        }
        template<typename T>
        inline auto & operator<<(T && v) const noexcept {
            if constexpr (logger_enable || override_log_disable) std::cout << std::forward<T>(v);
            return *this;
        }
    };

    struct logger_console_stream {
        static constexpr auto status = _LoggerConsoleStream<false>();
        static constexpr auto success = _LoggerConsoleStream<false>();
        static constexpr auto warning = _LoggerConsoleStream<true>();
        static constexpr auto error = _LoggerConsoleStream<true>();
    };
}