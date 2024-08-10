#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine {
    struct render_config_t {
        const std::size_t max_frames_in_flight;
        const std::uint32_t descriptor_pool_initial_size;
    };
}