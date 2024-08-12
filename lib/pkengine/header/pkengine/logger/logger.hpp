#pragma once

#include <mutex>
#include <iostream>

#include <pkengine/util/const_string.hpp>
#include <pkengine/util/ansi_color.hpp>
#include <pkengine/util/spin_lock.hpp>
#include <pkengine/util/temp_file.hpp>

#ifndef LOGGER_VISIBILITY_LEVEL
#define LOGGER_VISIBILITY_LEVEL STATUS
#endif

#ifndef LOGGER_INDENT_LEVEL
#define LOGGER_INDENT_LEVEL 40
#endif

#ifndef LOGGER_INDENT_SIZE
#define LOGGER_INDENT_SIZE 2
#endif

namespace PKEngine {
    enum _LoggerLevel {
        _LoggerLevel_DEBUG, _LoggerLevel_SUCCESS, _LoggerLevel_STATUS, _LoggerLevel_WARNING, _LoggerLevel_ERROR,
    };

    extern Util::SpinLock _Logger_lock;
    extern Util::TempFile _Logger_temp_file;
    constexpr auto _Logger_postfix = Util::ANSIColor::Reset::const_string + "\n";

    template<_LoggerLevel level, Util::ConstString name, std::size_t indent_level = 0>
    class _Logger {
    protected:
        struct LoggerLevels {
            static constexpr _LoggerLevel DEBUG = _LoggerLevel_DEBUG;
            static constexpr _LoggerLevel SUCCESS = _LoggerLevel_SUCCESS;
            static constexpr _LoggerLevel STATUS = _LoggerLevel_STATUS;
            static constexpr _LoggerLevel WARNING = _LoggerLevel_WARNING;
            static constexpr _LoggerLevel ERROR = _LoggerLevel_ERROR;
        };
        static constexpr _LoggerLevel visibility_level = LoggerLevels::LOGGER_VISIBILITY_LEVEL;

        static constexpr std::size_t name_size = LOGGER_INDENT_LEVEL;
        static constexpr std::size_t indent_size = LOGGER_INDENT_SIZE;

        static constexpr auto color = []() {
            if constexpr (level == _LoggerLevel_DEBUG) return Util::ANSIColor::Dimmed::CyanFg::const_string;
            else if constexpr (level == _LoggerLevel_SUCCESS) return Util::ANSIColor::GreenFg::const_string;
            else if constexpr (level == _LoggerLevel_STATUS) return Util::ANSIColor::BlueFg::const_string;
            else if constexpr (level == _LoggerLevel_WARNING) return Util::ANSIColor::Dimmed::Bold::YellowFg::const_string;
            else if constexpr (level == _LoggerLevel_ERROR) return Util::ANSIColor::Bold::RedFg::const_string;
            else return Util::ConstString("");
        }();
        static constexpr auto level_name = []() {
            if constexpr (level == _LoggerLevel_DEBUG) return Util::ConstString("DEBUG  ");
            else if constexpr (level == _LoggerLevel_SUCCESS) return Util::ConstString("SUCCESS");
            else if constexpr (level == _LoggerLevel_STATUS) return Util::ConstString("STATUS ");
            else if constexpr (level == _LoggerLevel_WARNING) return Util::ConstString("WARNING");
            else if constexpr (level == _LoggerLevel_ERROR) return Util::ConstString("ERROR  ");
            else return Util::ConstString("");
        }();

        static_assert(
            level_name.size() + name.size() + 8 <= name_size,
            "Logger name is too long!"
        );
        static constexpr std::size_t prefix_repeat_count = name_size - level_name.size() - name.size() - 8;
        static constexpr auto prefix =
            color +
            "[ " +
            level_name +
            " | " +
            Util::const_string_repeat < " ", prefix_repeat_count / 2> +
            name +
            Util::const_string_repeat<" ", prefix_repeat_count / 2 + prefix_repeat_count % 2> +
            " ] " +
            Util::const_string_repeat<" ", indent_level * indent_size>;

        static constexpr bool logger_enabled = level >= visibility_level;

        template<bool is_base>
        class SubLogger {
        public:
            inline ~SubLogger() {
                if constexpr (logger_enabled && is_base) {
                    std::cout << _Logger_postfix.string_view();
                    std::cout.flush();
                    _Logger_temp_file << _Logger_postfix.string_view();
                    _Logger_temp_file.flush();

                    _Logger_lock.unlock();
                }
            }

            template<typename T>
            inline auto operator<<(T & v) const noexcept {
                if constexpr (logger_enabled) {
                    std::cout << v;
                    _Logger_temp_file << v;
                }
                return SubLogger<false>();
            }

            template<typename T>
            inline auto operator<<(T && v) const noexcept {
                if constexpr (logger_enabled) {
                    std::cout << v;
                    _Logger_temp_file << v;
                }
                return SubLogger<false>();
            }
        };

    public:
        template<typename T>
        inline auto operator<<(T & v) const noexcept {
            if constexpr (logger_enabled) {
                _Logger_lock.lock();
                std::cout << prefix.string_view() << v;
                _Logger_temp_file << prefix.string_view() << v;
            }
            return SubLogger<true>();
        }

        template<typename T>
        inline auto operator<<(T && v) const noexcept {
            if constexpr (logger_enabled) {
                _Logger_lock.lock();
                std::cout << prefix.string_view() << v;
                _Logger_temp_file << prefix.string_view() << v;
            }
            return SubLogger<true>();
        }

        consteval auto debug() const noexcept { return _Logger<_LoggerLevel_DEBUG, name>(); }
        consteval auto success() const noexcept { return _Logger<_LoggerLevel_SUCCESS, name>(); }
        consteval auto status() const noexcept { return _Logger<_LoggerLevel_STATUS, name>(); }
        consteval auto warning() const noexcept { return _Logger<_LoggerLevel_WARNING, name>(); }
        consteval auto error() const noexcept { return _Logger<_LoggerLevel_ERROR, name>(); }

        template<std::size_t indent_count = 1>
        consteval auto indent() const noexcept { return _Logger<level, name, indent_level + indent_count>(); }

        template<std::size_t dedent_count = 1>
        consteval auto dedent() const noexcept {
            static_assert(indent_level > 0, "Cannot indent to a negative value!");
            return _Logger<level, name, indent_level - dedent_count>();
        }
    };

    template<Util::ConstString name>
    using Logger = _Logger<_LoggerLevel_STATUS, name>;

    inline void dump_log_to(const char * path) {
        _Logger_temp_file.seekg(0, std::ios::beg);

        std::ofstream os(path);

        os << _Logger_temp_file.rdbuf();

        _Logger_temp_file.clear();
        _Logger_temp_file.seekg(0, std::ios::end);
    }
}