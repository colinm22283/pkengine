#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan::Struct {
    inline VkCommandBufferSubmitInfo command_buffer_submit_info(VkCommandBuffer command_buffer) {
        return VkCommandBufferSubmitInfo {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
            .pNext = nullptr,
            .commandBuffer = command_buffer,
            .deviceMask = 0,
        };
    }
}