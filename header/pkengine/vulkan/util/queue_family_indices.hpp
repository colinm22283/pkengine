#pragma once

#include <optional>

#include <vulkan/vulkan.h>

#include "../surface.hpp"

namespace PKEngine::Vulkan {
    template<const auto & surface>
    class QueueFamilyIndices {
    public:
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        [[nodiscard]] inline bool is_complete() const noexcept { return graphics_family.has_value() && present_family.has_value(); }

        inline QueueFamilyIndices(const VkPhysicalDevice & device) {
            uint32_t family_count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, nullptr);

            std::vector<VkQueueFamilyProperties> queue_families(family_count);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, queue_families.data());

            int i = 0;
            for (const auto & queueFamily : queue_families) {
                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    graphics_family = i;
                }

                VkBool32 presentSupport = false;
                if (vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface.handle(), &presentSupport) != VK_SUCCESS) {
                    throw Exception::Internal::vulkan_cant_get_surface_support();
                }
                if (presentSupport) {
                    present_family = i;
                }

                if (is_complete()) break;

                i++;
            }
        }
    };
}