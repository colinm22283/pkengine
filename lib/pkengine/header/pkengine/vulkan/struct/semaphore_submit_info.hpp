#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan::Struct {
    inline VkSemaphoreSubmitInfo semaphore_submit_info(VkPipelineStageFlags2 stage_mask, VkSemaphore semaphore) {
        return VkSemaphoreSubmitInfo {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
            .pNext = nullptr,
            .semaphore = semaphore,
            .value = 1,
            .stageMask = stage_mask,
            .deviceIndex = 0,
        };
    }
}