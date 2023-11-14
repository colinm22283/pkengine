#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan {
    template<const auto & surface>
    class SwapChainSupport {
    public:
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;

        inline SwapChainSupport(const VkPhysicalDevice & device) {
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
                device,
                surface.handle(),
                &capabilities
            );

            uint32_t format_count;
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                device,
                surface.handle(),
                &format_count,
                nullptr
            );

            if (format_count != 0) {
                formats.resize(format_count);
                vkGetPhysicalDeviceSurfaceFormatsKHR(
                    device,
                    surface.handle(),
                    &format_count,
                    formats.data()
                );
            }

            uint32_t present_mode_count;
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device,
                surface.handle(),
                &present_mode_count,
                nullptr
            );

            if (present_mode_count != 0) {
                present_modes.resize(present_mode_count);
                vkGetPhysicalDeviceSurfacePresentModesKHR(
                    device,
                    surface.handle(),
                    &present_mode_count,
                    present_modes.data()
                );
            }
        }
    };
}