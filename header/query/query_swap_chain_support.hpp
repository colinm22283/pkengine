#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace PKEngine {
    struct swap_chain_support_info_t {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    swap_chain_support_info_t query_swap_chain_support(VkPhysicalDevice device, VkSurfaceKHR surface) {
        swap_chain_support_info_t info;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            device,
            surface,
            &info.capabilities
        );

        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device,
            surface,
            &format_count,
            nullptr
        );

        if (format_count != 0) {
            info.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                device,
                surface,
                &format_count,
                info.formats.data()
            );
        }

        uint32_t present_mode_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device,
            surface,
            &present_mode_count,
            nullptr
        );

        if (present_mode_count != 0) {
            info.present_modes.resize(present_mode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device,
                surface,
                &present_mode_count,
                info.present_modes.data()
            );
        }


        return info;
    }
}