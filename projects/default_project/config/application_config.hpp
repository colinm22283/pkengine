#pragma once

#include <pkengine/config/application_config_type.hpp>

namespace PKEngine {
    constexpr engine_config_t engine_config = {
        .application = {
            .name = "PKEngine Test App",
            .version = 0.1,
        },
        .window = {
            .title = "PKEngine Test App",
            .width = 800,
            .height = 800,
        }
    };
}