#pragma once

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <mutex>

#include "pkengine/util/ansi.hpp"
#include "pkengine/util/const_string.hpp"

#include "config.hpp"
#include "file_stream.hpp"
#include "console_stream.hpp"
#include "logger_mutex.hpp"

namespace PKEngine {
    enum _logger_level_t { _LL_STATUS, _LL_SUCCESS, _LL_WARNING, _LL_ERROR };

    template<_logger_level_t level, typename base_color, Util::ConstString prefix>
    class _Logger {
    protected:
        static inline void print_header() {
            logger_file_stream << prefix;

            if constexpr (level == _LL_STATUS) {
                logger_console_stream::status << (base_color::string + prefix);
            }
            else if constexpr (level == _LL_SUCCESS) {
                logger_console_stream::success << (LoggerConfig::success_color::string + prefix);
            }
            else if constexpr (level == _LL_WARNING) {
                logger_console_stream::warning << (LoggerConfig::warning_color::string + prefix);
            }
            else if constexpr (level == _LL_ERROR) {
                logger_console_stream::error << (LoggerConfig::error_color::string + prefix);
            }
        }
        static inline void print_footer() { // I love compiler bugs :)
            // TODO: submit a bug report to gcc

            logger_file_stream << "\n";
            if constexpr (level == _LL_STATUS) logger_console_stream::status << (LoggerConfig::reset_color::string + Util::ConstString("\n")).c_str();
            else if constexpr (level == _LL_SUCCESS) logger_console_stream::success << (LoggerConfig::reset_color::string + Util::ConstString("\n")).c_str();
            else if constexpr (level == _LL_WARNING) logger_console_stream::warning << (LoggerConfig::reset_color::string + Util::ConstString("\n")).c_str();
            else if constexpr (level == _LL_ERROR) logger_console_stream::error << (LoggerConfig::reset_color::string + Util::ConstString("\n")).c_str();

        }

        template<typename T>
        static inline void print(T & v) {
            if constexpr (level == _LL_STATUS) logger_console_stream::status << v;
            else if constexpr (level == _LL_SUCCESS) logger_console_stream::success << v;
            else if constexpr (level == _LL_WARNING) logger_console_stream::warning << v;
            else if constexpr (level == _LL_ERROR) logger_console_stream::error << v;
        }
        template<typename T>
        static inline void print(T && v) {
            if constexpr (level == _LL_STATUS) logger_console_stream::status << std::forward<T>(v);
            else if constexpr (level == _LL_SUCCESS) logger_console_stream::success << std::forward<T>(v);
            else if constexpr (level == _LL_WARNING) logger_console_stream::warning << std::forward<T>(v);
            else if constexpr (level == _LL_ERROR) logger_console_stream::error << std::forward<T>(v);
        }

        template<bool is_base>
        class LoggerOutStream {
        public:
            consteval LoggerOutStream() = default;
            inline ~LoggerOutStream() {
                 if constexpr (is_base) print_footer();
            }

            template<typename T>
            inline LoggerOutStream<false> log(T & value) const noexcept {
                logger_file_stream << value;
                print(value);
                return LoggerOutStream<false>();
            }
            template<typename T>
            inline LoggerOutStream<false> log(T && value) const noexcept {
                logger_file_stream << std::forward<T>(value);
                print(std::forward<T>(value));
                return LoggerOutStream<false>();
            }
            template<typename T> inline LoggerOutStream<false> operator<<(T & value) const noexcept { return log<T>(value); }
            template<typename T> inline LoggerOutStream<false> operator<<(T && value) const noexcept { return log<T>(std::forward<T>(value)); }
        };
    public:
        consteval _Logger() = default;

        template<typename T>
        inline LoggerOutStream<true> log(T & value) const noexcept {
            std::lock_guard<std::mutex> lock(_logger_mutex);
            print_header();
            logger_file_stream << value;
            print(value);
            return LoggerOutStream<true>();
        }
        template<typename T>
        inline LoggerOutStream<true> log(T && value) const noexcept {
            std::lock_guard<std::mutex> lock(_logger_mutex);
            print_header();
            logger_file_stream << std::forward<T>(value);
            print(std::forward<T>(value));
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