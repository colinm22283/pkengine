#pragma once

#include <fstream>
#include <iostream>

#include "pkengine/exception/internal.hpp"

#include "logger_config.hpp"

namespace PKEngine {
    class _LoggerFileStream {
    protected:
#ifdef PKENGINE_LOG_FILE_ENABLE
        static std::ofstream log_file;
#endif

    public:
        inline void init() const {
            log_file.open(logger_config.log_file_path);
            if (!log_file.is_open()) throw Exception::Internal::cant_open_log_file();
        }
        inline void free() const noexcept { }

        template<typename T>
        inline auto & operator<<(T & v) const noexcept {
#ifdef PKENGINE_LOG_FILE_ENABLE
            log_file << v;
            log_file.flush();
#endif
            return *this;
        }
        template<typename T>
        inline auto & operator<<(T && v) const noexcept {
#ifdef PKENGINE_LOG_FILE_ENABLE
            log_file << std::forward<T>(v);
            log_file.flush();
#endif
            return *this;
        }
    };

    constexpr _LoggerFileStream logger_file_stream;
}