#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan::Struct {
    inline VkSubmitInfo2 submit_info(
        VkCommandBufferSubmitInfo & command_buffer_submit_info,
        VkSemaphoreSubmitInfo & signal_semaphore_info,
        VkSemaphoreSubmitInfo & wait_semaphore_info
    ) {
        return VkSubmitInfo2 {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
            .pNext = nullptr,

            .waitSemaphoreInfoCount = (uint32_t) 1,
            .pWaitSemaphoreInfos = &wait_semaphore_info,

            .commandBufferInfoCount = 1,
            .pCommandBufferInfos = &command_buffer_submit_info,

            .signalSemaphoreInfoCount = (uint32_t) 1,
            .pSignalSemaphoreInfos = &signal_semaphore_info,

        };
    }
}