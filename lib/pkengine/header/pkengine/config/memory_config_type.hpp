#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine {
    struct memory_config_t {
        /// @brief Vertex buffer allocation in elements
        VkDeviceSize vertex_buffer_allocation;
        /// @brief Index buffer allocation in elements
        VkDeviceSize index_buffer_allocation;
    };
}