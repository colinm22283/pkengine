#pragma once

#include <cstdio>
#include <iostream>
#include <mutex>

#include <util/const_string.hpp>
#include <util/ansi.hpp>

#include <logger/config.hpp>
#include <logger/logfile.hpp>

namespace PKEngine {
    namespace _LoggerPrivate {
        static constexpr auto postfix = ANSI::Reset::string + "\n";

        using lock_t = std::unique_lock<std::mutex>;
        extern std::mutex mutex;

        template<typename ANSIColor, ConstString prefix, bool has_file, typename logfile>
        class logger_t { };

        template<typename ANSIColor, ConstString prefix, typename logfile>
        class logger_t<ANSIColor, prefix, false, logfile> {
        protected:
            static constexpr auto full_prefix = ANSIColor::string + "[ " + prefix + " ] ";

            struct sub_logger_t {
                template<typename T>
                inline sub_logger_t log(T & value) const noexcept {
                    std::cout << value;
                    return sub_logger_t();
                }
                template<typename T>
                inline sub_logger_t log(T && value) const noexcept {
                    std::cout << value;
                    return sub_logger_t();
                }
                template<ConstString str>
                inline sub_logger_t log() const noexcept {
                    fputs(str, stdout);
                    return sub_logger_t();
                }
                template<typename T> inline sub_logger_t operator<<(T & value) const noexcept { return log(value); }
                template<typename T> inline sub_logger_t operator<<(T && value) const noexcept { return log(value); }
            };
            struct root_logger_t {
                lock_t lock;

                explicit inline root_logger_t(lock_t && _lock): lock(std::move(_lock)) { }
                inline ~root_logger_t() noexcept {
                    fputs(postfix, stdout);
                }

                template<typename T>
                inline sub_logger_t log(T & value) const noexcept {
                    std::cout << value;
                    return sub_logger_t();
                }
                template<typename T>
                inline sub_logger_t log(T && value) const noexcept {
                    std::cout << value;
                    return sub_logger_t();
                }
                template<ConstString str>
                inline sub_logger_t log() const noexcept {
                    fputs(str, stdout);
                    return sub_logger_t();
                }
                template<typename T> inline sub_logger_t operator<<(T & value) const noexcept { return log(value); }
                template<typename T> inline sub_logger_t operator<<(T && value) const noexcept { return log(value); }
            };

        public:
            consteval logger_t() = default;

            template<typename T>
            inline root_logger_t log(T & value) const noexcept {
                fputs(full_prefix, stdout);
                std::cout << value;
                return sub_logger_t();
            }
            template<typename T>
            inline root_logger_t log(T && value) const noexcept {
                fputs(full_prefix, stdout);
                std::cout << value;
                return root_logger_t();
            }
            template<ConstString str>
            inline root_logger_t log() const noexcept {
                constexpr auto temp = full_prefix + str;
                fputs(temp, stdout);
                return sub_logger_t();
            }
            template<typename T> inline root_logger_t operator<<(T & value) const noexcept { return log(value); }
            template<typename T> inline root_logger_t operator<<(T && value) const noexcept { return log(value); }

            consteval auto success() const noexcept { return logger_t<ANSI::GreenFg::Bold, prefix, false, logfile>(); }
            consteval auto warn() const noexcept { return logger_t<ANSI::YellowFg::Bold, prefix + " WARNING", false, logfile>(); }
            consteval auto error() const noexcept { return logger_t<ANSI::RedFg::Bold, prefix + " ERROR", false, logfile>(); }
        };

        template<typename ANSIColor, ConstString prefix, typename logfile> requires(logfile::is_logfile)
        class logger_t<ANSIColor, prefix, true, logfile> {
        protected:
            static constexpr auto semi_prefix = "[ " + prefix + " ] ";
            static constexpr auto full_prefix = ANSIColor::string + semi_prefix;

            struct sub_logger_t {
                template<typename T>
                inline sub_logger_t log(T & value) const noexcept {
                    std::cout << value;
                    logfile::file << value;
                    return sub_logger_t();
                }
                template<typename T>
                inline sub_logger_t log(T && value) const noexcept {
                    std::cout << value;
                    logfile::file << value;
                    return sub_logger_t();
                }
                template<ConstString str>
                inline sub_logger_t log() const noexcept {
                    fputs(str, stdout);
                    logfile::file << str;
                    return sub_logger_t();
                }
                template<typename T> inline sub_logger_t operator<<(T & value) const noexcept { return log(value); }
                template<typename T> inline sub_logger_t operator<<(T && value) const noexcept { return log(value); }
            };
            struct root_logger_t {
                lock_t lock;

                explicit inline root_logger_t(lock_t && _lock): lock(std::move(_lock)) { }
                inline ~root_logger_t() noexcept {
                    fputs(postfix, stdout);
                }

                template<typename T>
                inline sub_logger_t log(T & value) const noexcept {
                    std::cout << value;
                    logfile::file << value;
                    return sub_logger_t();
                }
                template<typename T>
                inline sub_logger_t log(T && value) const noexcept {
                    std::cout << value;
                    logfile::file << value;
                    return sub_logger_t();
                }
                template<ConstString str>
                inline sub_logger_t log() const noexcept {
                    fputs(str, stdout);
                    logfile::file << str;
                    return sub_logger_t();
                }
                template<typename T> inline sub_logger_t operator<<(T & value) const noexcept { return log(value); }
                template<typename T> inline sub_logger_t operator<<(T && value) const noexcept { return log(value); }
            };

        public:
            consteval logger_t() noexcept = default;

            template<typename T>
            inline root_logger_t log(T & value) const noexcept {
                lock_t lock(_LoggerPrivate::mutex);
                fputs(full_prefix, stdout);
                std::cout << value;
                logfile::file << semi_prefix << value;
                return root_logger_t(std::move(lock));
            }
            template<typename T>
            inline root_logger_t log(T && value) const noexcept {
                lock_t lock(_LoggerPrivate::mutex);
                fputs(full_prefix, stdout);
                std::cout << value;
                logfile::file << semi_prefix << value;
                return root_logger_t(std::move(lock));
            }
            template<ConstString str>
            inline root_logger_t log() const noexcept {
                lock_t lock(_LoggerPrivate::mutex);
                constexpr auto temp = full_prefix + str;
                constexpr auto file_temp = semi_prefix + str;
                fputs(temp, stdout);
                logfile::file << file_temp;
                return root_logger_t(std::move(lock));
            }
            template<typename T> inline root_logger_t operator<<(T & value) const noexcept {
                lock_t lock(_LoggerPrivate::mutex);
                fputs(full_prefix, stdout);
                std::cout << value;
                logfile::file << semi_prefix << value;
                return root_logger_t(std::move(lock));
            }
            template<typename T> inline root_logger_t operator<<(T && value) const noexcept {
                lock_t lock(_LoggerPrivate::mutex);
                fputs(full_prefix, stdout);
                std::cout << value;
                logfile::file << semi_prefix << value;
                return root_logger_t(std::move(lock));
            }

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