#pragma once

#include <pkengine/config/engine_config_type.hpp>

namespace PKEngine {
    constexpr engine_config_t engine_config = {
        .application = {
            .name = "PKEngine Template Project",
            .version = 0.1,
        },
        .window = {
            .title = "PKEngine Template Project",
            .width = 800,
            .height = 800,
        }
    };
}