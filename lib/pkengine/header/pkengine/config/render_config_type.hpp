#pragma once

#include <cstdint>

#include <pkengine/vulkan/shader_struct/vec4.hpp>

namespace PKEngine::Config {
    struct render_config_t {
        uint32_t max_frames_in_flight = 3;
        Vulkan::ShaderStruct::Vec4 background_color = {
            .x = 0.2f,
            .y = 0.2f,
            .z = 0.2f,
            .w = 1.0f,
        };
    };
}