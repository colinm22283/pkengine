#pragma once

#include <pkengine/config/render_config_type.hpp>

namespace PKEngine::Config {
    constexpr render_config_t render_config {
       .max_frames_in_flight = 3,
    };
}