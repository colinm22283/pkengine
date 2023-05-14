#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace PKEngine {
    inline VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR> & available_present_modes) {
        for (const auto& availablePresentMode : available_present_modes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }
}