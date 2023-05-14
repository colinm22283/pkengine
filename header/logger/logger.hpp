#pragma once

#include <iostream>
#include <stdexcept>
#include <fstream>

#include <logger/config.hpp>

enum logger_level_t { INFO, WARNING, ERROR };

extern std::ofstream _logger_logfile;

template<logger_level_t level, bool is_root>
class _Logger {
public:
    inline ~_Logger() {
        if (is_root) {
            switch (level) {
                case INFO: if (ENABLE_INFO_LOGGING && ENABLE_LOGGING) {
                    std::cout << "\033[0m\n";
                    if (_logger_logfile.is_open()) _logger_logfile << '\n';
                } break;
                case WARNING: if (ENABLE_LOGGING) {
                    std::cout << "\033[0m\n";
                    if (_logger_logfile.is_open()) _logger_logfile << '\n';
                } break;
                case ERROR: if (ENABLE_LOGGING) {
                    std::cerr << "\033[0m\n";
                    if (_logger_logfile.is_open()) _logger_logfile << '\n';
                } break;
            }
        }
    }

    template<typename T>
    inline _Logger<level, false> print(T value) {
        switch (level) {
            case INFO:    if (ENABLE_INFO_LOGGING && ENABLE_LOGGING) {
                std::cout << "\033[34m" << value;
                if (_logger_logfile.is_open()) _logger_logfile << value;
            } break;
            case WARNING: if (ENABLE_LOGGING) {
                std::cout << "\033[93;1m" << value;
                if (_logger_logfile.is_open()) _logger_logfile << value;
            } break;
            case ERROR:   if (ENABLE_LOGGING) {
                std::cerr << "\033[91;1m" << value;
                if (_logger_logfile.is_open()) _logger_logfile << value;
            } break;
        }

        return _Logger<level, false>();
    }

    template<typename T>
    inline _Logger<level, false> operator<<(T value) { return print(value); }
};

namespace Log {
    using info    = _Logger<INFO, true>;
    using warning = _Logger<WARNING, true>;
    using error   = _Logger<ERROR, true>;


    inline void open_logfile(const char * path) {
        _logger_logfile.open(path);
        if (!_logger_logfile.is_open()) throw std::runtime_error("Unable to open logfile");
    }
}