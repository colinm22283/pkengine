#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace PKEngine {
    inline VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR> & available_formats) {
        for (const auto & availableFormat : available_formats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return available_formats[0];
    }
}