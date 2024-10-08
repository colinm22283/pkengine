#pragma once

#include <pkengine/config/logger_config_type.hpp>

namespace PKEngine::Config {
    constexpr logger_config_t logger_config {
        .log_file_path = "log.txt",
    };
}