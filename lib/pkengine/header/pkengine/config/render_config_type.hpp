#pragma once

#include <cstdint>

namespace PKEngine::Config {
    struct render_config_t {
        uint32_t max_frames_in_flight;
    };
}