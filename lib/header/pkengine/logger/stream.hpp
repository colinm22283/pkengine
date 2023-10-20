#pragma once

#include <fstream>
#include <iostream>

#include "../exception/internal.hpp"

#include <logger_config.hpp>

namespace PKEngine {
    class _LoggerStream {
    protected:
#ifdef PKENGINE_LOG_FILE_ENABLE
        std::ofstream log_file;
#endif

    public:
        inline void init() {
            log_file.open(logger_config.log_file_path);
            if (!log_file.is_open()) throw Exception::Internal::cant_open_log_file();
        }
        inline void free() { }

        template<typename T>
        inline auto & operator<<(T & v) const noexcept {
#ifdef PKENGINE_LOGGER_ENABLE
            std::cout << v;
#endif
#ifdef PKENGINE_LOG_FILE_ENABLE
//            log_file << v;
#endif
            return *this;
        }
        template<typename T>
        inline auto & operator<<(T && v) const noexcept {
#ifdef PKENGINE_LOGGER_ENABLE
            std::cout << v;
#endif
#ifdef PKENGINE_LOG_FILE_ENABLE
//            log_file << std::forward(v);
#endif
            return *this;
        }
    };

    extern _LoggerStream logger_stream_instance;
}