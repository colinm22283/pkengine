#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include <util/engine_exception.hpp>

namespace PKEngine::Query {
    struct queue_family_indices_t {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        [[nodiscard]] inline bool is_complete() const {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    inline queue_family_indices_t query_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface) {
        queue_family_indices_t indices;

        uint32_t family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, queue_families.data());

        int i = 0;
        for (const auto & queueFamily : queue_families) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = i;
            }

            VkBool32 presentSupport = false;
            if (vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport) != VK_SUCCESS) {
                throw Exceptions::UnableToGetDeviceSurfaceSupport();
            }
            if (presentSupport) {
                indices.present_family = i;
            }

            if (indices.is_complete()) break;

            i++;
        }

        return indices;
    }
}