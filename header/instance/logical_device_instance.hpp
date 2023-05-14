#pragma once

#include <vulkan/vulkan.h>

#include <util/engine_exception.hpp>

#include <query/query_queue_families.hpp>

#include <config/required_extensions.hpp>

namespace PKEngine {
    class LogicalDeviceInstance {
    protected:
        VkDevice _handle;

    public:
        inline explicit LogicalDeviceInstance(VkPhysicalDevice physical_device, Query::queue_family_indices_t & indices) {
            VkDeviceQueueCreateInfo queue_create_info{};
            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = indices.graphics_family.value();
            queue_create_info.queueCount = 1;
            float queuePriority = 1.0f;
            queue_create_info.pQueuePriorities = &queuePriority;

            VkPhysicalDeviceFeatures deviceFeatures{};

            VkDeviceCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            create_info.pQueueCreateInfos = &queue_create_info;
            create_info.queueCreateInfoCount = 1;
            create_info.pEnabledFeatures = &deviceFeatures;
            create_info.enabledExtensionCount = (uint32_t) device_extensions.size();
            create_info.ppEnabledExtensionNames = device_extensions.data();
            create_info.enabledLayerCount = 0;

            if (vkCreateDevice(physical_device, &create_info, nullptr, &_handle) != VK_SUCCESS) {
                throw Exceptions::UnableToCreateVulkanLogicalDevice();
            }
        }
        inline ~LogicalDeviceInstance() {
            if (_handle) vkDestroyDevice(_handle, nullptr);
        };

        inline LogicalDeviceInstance(LogicalDeviceInstance && other):
          _handle(other._handle) { other._handle = nullptr; }

        [[nodiscard]] inline VkDevice const handle() const { return _handle; }
        [[nodiscard]] inline operator VkDevice const() const { return handle(); }
    };
}