#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan {
    template<const auto & physical_device, const auto & queue_family_indices>
    class LogicalDevice {
    protected:
        static constexpr auto logger = Logger<"Logical Device">();

        VkDevice logical_device = VK_NULL_HANDLE;

    public:
        inline void init() {
            logger << "Creating logical device...";

            float queuePriority = 1.0f;
            VkDeviceQueueCreateInfo queue_create_info = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = queue_family_indices.graphics_family.value(),
                .queueCount = 1,
                .pQueuePriorities = &queuePriority,
            };

            VkPhysicalDeviceFeatures device_features = { };

            VkDeviceCreateInfo create_info = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .queueCreateInfoCount = 1,
                .pQueueCreateInfos = &queue_create_info,
                .enabledLayerCount = 0,
                .enabledExtensionCount = (uint32_t) device_extensions.size(),
                .ppEnabledExtensionNames = device_extensions.data(),
                .pEnabledFeatures = &device_features,
            };

            if (vkCreateDevice(physical_device.handle(), &create_info, nullptr, &logical_device) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_logical_device();
            }

            logger.success() << "Logical device created";
        }
        inline void free() {
            if (logical_device != VK_NULL_HANDLE) {
                logger << "Destroying logical device...";
                vkDestroyDevice(logical_device, nullptr);
                logger.success() << "Logical device destroyed";
            }
        }

        [[nodiscard]] inline VkDevice handle() const noexcept { return logical_device; }
    };
}