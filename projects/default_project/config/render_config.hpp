#pragma once

#include <pkengine/config/render_config_type.hpp>

namespace PKEngine {
    constexpr render_config_t render_config = {
        .max_frames_in_flight = 3,
        .descriptor_pool_initial_size = 1024,
    };
}