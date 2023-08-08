#pragma once

#include "types/engine_config_type.hpp"

namespace PKEngine {
    constexpr _engine_config_t engine_config = {
        .application = {
            .name = "PKEngine Test App",
            .version = 0.1,
        },
        .window = {
            .title = "PKEngine Test App",
        }
    };
}