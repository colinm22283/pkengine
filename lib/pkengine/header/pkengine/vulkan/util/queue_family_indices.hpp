#pragma once

#include <optional>

#include <vulkan/vulkan.h>

#include <pkengine/vulkan/surface.hpp>
#include <pkengine/vulkan/util/vulkan_exception.hpp>

namespace PKEngine::Vulkan::Util {
    class QueueFamilyIndices {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(CantGetDevSupport, "Can't get physical device surface support");
        };

        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        [[nodiscard]] inline bool is_complete() const noexcept { return graphics_family.has_value() && present_family.has_value(); }

        inline QueueFamilyIndices(Surface & surface, const VkPhysicalDevice & device) {
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
                Util::throw_on_fail<Exceptions::CantGetDevSupport>(
                    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface.handle(), &presentSupport)
                );
                if (presentSupport) {
                    present_family = i;
                }

                if (is_complete()) break;

                i++;
            }
        }
    };
}