#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine {
    struct memory_config_t {
        VkDeviceSize vertex_buffer_allocation;
        VkDeviceSize index_buffer_allocation;
    };
}