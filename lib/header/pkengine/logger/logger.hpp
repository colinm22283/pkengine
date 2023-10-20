#pragma once

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <mutex>

#include "../util/ansi.hpp"
#include "../util/const_string.hpp"

#include "config.hpp"

namespace PKEngine {
    enum _logger_level_t { _LL_STATUS, _LL_SUCCESS, _LL_WARNING, _LL_ERROR };

    extern std::mutex _logger_mutex;

    template<_logger_level_t level, typename base_color, Util::ConstString prefix>
    class _Logger {
    protected:
        static inline void print_header() {
            if constexpr (!logger_config::enable) return;
            else if constexpr (level == _LL_STATUS)  fputs(base_color::string + prefix, stdout);
            else if constexpr (level == _LL_SUCCESS) fputs(logger_config::success_color::string + prefix, stdout);
            else if constexpr (level == _LL_WARNING) fputs(logger_config::warning_color::string + prefix, stdout);
            else if constexpr (level == _LL_ERROR)   fputs(logger_config::error_color::string + prefix, stdout);
        }
        static inline void print_footer() { // I love compiler bugs :)
            fputs(
                Util::ConstString<logger_config::reset_color::string.size() + 1>(
                    logger_config::reset_color::string.data,
                    "\n"
                ),
                stdout
            );
        }

        template<bool is_base>
        class LoggerOutStream {
        public:
            consteval LoggerOutStream() = default;
            inline ~LoggerOutStream() {
                 if constexpr (logger_config::enable && is_base) print_footer();
            }

            template<typename T>
            inline LoggerOutStream<false> log(T & value) const noexcept {
                if constexpr (logger_config::enable) std::cout << value;
                return LoggerOutStream<false>();
            }
            template<typename T>
            inline LoggerOutStream<false> log(T && value) const noexcept {
                if constexpr (logger_config::enable) std::cout << value;
                return LoggerOutStream<false>();
            }
            template<typename T> inline LoggerOutStream<false> operator<<(T & value) const noexcept { return log<T>(value); }
            template<typename T> inline LoggerOutStream<false> operator<<(T && value) const noexcept { return log<T>(std::forward<T>(value)); }
        };
    public:
        consteval _Logger() = default;

        template<typename T>
        inline LoggerOutStream<true> log(T & value) const noexcept {
            if constexpr (!logger_config::enable) return LoggerOutStream<true>();
            std::lock_guard<std::mutex> lock(_logger_mutex);
            print_header();
            std::cout << value;
            return LoggerOutStream<true>();
        }
        template<typename T>
        inline LoggerOutStream<true> log(T && value) const noexcept {
            if constexpr (!logger_config::enable) return LoggerOutStream<true>();
            std::lock_guard<std::mutex> lock(_logger_mutex);
            print_header();
            std::cout << value;
            return LoggerOutStream<true>();
        }
        template<typename T> inline LoggerOutStream<true> operator<<(T & value) const noexcept { return log<T>(value); }
        template<typename T> inline LoggerOutStream<true> operator<<(T && value) const noexcept { return log<T>(std::forward<T>(value)); }

        constexpr _Logger<_LL_SUCCESS, base_color, prefix> success() const noexcept { return _Logger<_LL_SUCCESS, base_color, prefix>(); }
        constexpr _Logger<_LL_WARNING, base_color, prefix> warning() const noexcept { return _Logger<_LL_WARNING, base_color, prefix>(); }
        constexpr _Logger<_LL_ERROR, base_color, prefix>   error()   const noexcept { return _Logger<_LL_ERROR, base_color, prefix>(); }
    };

    template<typename base_color, Util::ConstString name>
    using Logger = _Logger<_LL_STATUS, base_color, "[ " + name + " ] ">;
}