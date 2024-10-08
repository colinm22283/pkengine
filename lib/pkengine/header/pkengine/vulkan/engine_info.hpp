#pragma once

#include <pkengine/util/version.hpp>

namespace PKEngine::Vulkan {
    static constexpr struct {
        const char * name;
        Version version;
    } engine_info = {
        .name = "PKEngine",
        .version = 0.1,
    };
}