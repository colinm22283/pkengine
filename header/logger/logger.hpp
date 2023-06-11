#pragma once

#include <cstdio>
#include <iostream>

#include <util/const_string.hpp>
#include <util/ansi.hpp>

#include <logger/config.hpp>
#include <logger/logfile.hpp>

namespace PKEngine {
    namespace _LoggerPrivate {
        static constexpr auto postfix = ANSI::Reset::string + "\n";

        template<typename ANSIColor, ConstString prefix, bool has_file, typename logfile>
        class logger_t { };

        template<typename ANSIColor, ConstString prefix, typename logfile>
        class logger_t<ANSIColor, prefix, false, logfile> {
        protected:
            static constexpr auto full_prefix = ANSIColor::string + "[ " + prefix + " ] ";

            template<bool is_root>
            struct sub_logger_t {
                inline ~sub_logger_t() noexcept { if (is_root) fputs(postfix, stdout); }

                template<typename T>
                inline sub_logger_t<false> log(T & value) const noexcept {
                    std::cout << value;
                    return sub_logger_t<false>();
                }
                template<typename T>
                inline sub_logger_t<false> log(T && value) const noexcept {
                    std::cout << value;
                    return sub_logger_t<false>();
                }
                template<ConstString str>
                inline sub_logger_t<false> log() const noexcept {
                    fputs(str, stdout);
                    return sub_logger_t<false>();
                }
                template<typename T> inline sub_logger_t<false> operator<<(T & value) const noexcept { return log(value); }
                template<typename T> inline sub_logger_t<false> operator<<(T && value) const noexcept { return log(value); }
            };

        public:
            consteval logger_t() = default;

            template<typename T>
            inline sub_logger_t<true> log(T & value) const noexcept {
                fputs(full_prefix, stdout);
                std::cout << value;
                return sub_logger_t<true>();
            }
            template<typename T>
            inline sub_logger_t<true> log(T && value) const noexcept {
                fputs(full_prefix, stdout);
                std::cout << value;
                return sub_logger_t<true>();
            }
            template<ConstString str>
            inline sub_logger_t<true> log() const noexcept {
                constexpr auto temp = full_prefix + str;
                fputs(temp, stdout);
                return sub_logger_t<true>();
            }
            template<typename T> inline sub_logger_t<true> operator<<(T & value) const noexcept { return log(value); }
            template<typename T> inline sub_logger_t<true> operator<<(T && value) const noexcept { return log(value); }

            consteval auto success() const noexcept { return logger_t<ANSI::GreenFg::Bold, prefix, false, logfile>(); }
            consteval auto warn() const noexcept { return logger_t<ANSI::YellowFg::Bold, prefix + " WARNING", false, logfile>(); }
            consteval auto error() const noexcept { return logger_t<ANSI::RedFg::Bold, prefix + " ERROR", false, logfile>(); }
        };

        template<typename ANSIColor, ConstString prefix, typename logfile> requires(logfile::is_logfile)
        class logger_t<ANSIColor, prefix, true, logfile> {
        protected:
            static constexpr auto semi_prefix = "[ " + prefix + " ] ";
            static constexpr auto full_prefix = ANSIColor::string + semi_prefix;

            template<bool is_root>
            struct sub_logger_t {
                inline ~sub_logger_t() noexcept {
                    if (is_root) {
                        fputs(postfix, stdout);
                        logfile::file << '\n';
                    }
                }

                template<typename T>
                inline sub_logger_t<false> log(T & value) const noexcept {
                    std::cout << value;
                    logfile::file << value;
                    return sub_logger_t<false>();
                }
                template<typename T>
                inline sub_logger_t<false> log(T && value) const noexcept {
                    std::cout << value;
                    logfile::file << value;
                    return sub_logger_t<false>();
                }
                template<ConstString str>
                inline sub_logger_t<false> log() const noexcept {
                    fputs(str, stdout);
                    logfile::file << str;
                    return sub_logger_t<false>();
                }
                template<typename T> inline sub_logger_t<false> operator<<(T & value) const noexcept { return log(value); }
                template<typename T> inline sub_logger_t<false> operator<<(T && value) const noexcept { return log(value); }
            };

        public:
            consteval logger_t() noexcept = default;

            template<typename T>
            inline sub_logger_t<true> log(T & value) const noexcept {
                fputs(full_prefix, stdout);
                std::cout << value;
                logfile::file << semi_prefix << value;
                return sub_logger_t<true>();
            }
            template<typename T>
            inline sub_logger_t<true> log(T && value) const noexcept {
                fputs(full_prefix, stdout);
                std::cout << value;
                logfile::file << semi_prefix << value;
                return sub_logger_t<true>();
            }
            template<ConstString str>
            inline sub_logger_t<true> log() const noexcept {
                constexpr auto temp = full_prefix + str;
                constexpr auto file_temp = semi_prefix + str;
                fputs(temp, stdout);
                logfile::file << file_temp;
                return sub_logger_t<true>();
            }
            template<typename T> inline sub_logger_t<true> operator<<(T & value) const noexcept { return log(value); }
            template<typename T> inline sub_logger_t<true> operator<<(T && value) const noexcept { return log(value); }

            consteval auto success() const noexcept { return logger_t<ANSI::GreenFg::Bold, prefix, true, logfile>(); }
            consteval auto warn() const noexcept { return logger_t<ANSI::YellowFg::Bold, prefix + " WARNING", true, logfile>(); }
            consteval auto error() const noexcept { return logger_t<ANSI::RedFg::Bold, prefix + " ERROR", true, logfile>(); }
        };

        struct NoneType { };
    }

    template<typename ANSIColor, Util::ConstString name>
    using Logger = _LoggerPrivate::logger_t<ANSIColor, name, false, _LoggerPrivate::NoneType>;
    template<typename ANSIColor, Util::ConstString name, Util::ConstString log_path>
    using FileLogger = _LoggerPrivate::logger_t<ANSIColor, name, true, _LoggerPrivate::LogFile<log_path>>;
}