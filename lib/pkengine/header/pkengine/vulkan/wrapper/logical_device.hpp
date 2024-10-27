#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/required_extensions.hpp>
#include <pkengine/vulkan/wrapper/physical_device.hpp>
#include <pkengine/vulkan/util/queue_family_indices.hpp>
#include <pkengine/vulkan/util/vulkan_exception.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class LogicalDevice {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize logical device");
        };

    protected:
        static constexpr auto logger = Logger<"Physical Device">();

        VkDevice logical_device;

    public:
        inline LogicalDevice(PhysicalDevice & physical_device, Util::QueueFamilyIndices & queue_family_indices) {
            logger.debug() << "Initializing logical device...";

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
                .enabledExtensionCount = (uint32_t) required_extensions.size(),
                .ppEnabledExtensionNames = required_extensions.data(),
                .pEnabledFeatures = &device_features,
            };

            Util::throw_on_fail<Exceptions::InitError>(
                vkCreateDevice(physical_device.handle(), &create_info, nullptr, &logical_device)
            );

            logger.success() << "Logical device initialized";
        }
        inline ~LogicalDevice() {
            if (logical_device != VK_NULL_HANDLE) {
                logger.debug() << "Destroying logical device...";

                vkDestroyDevice(logical_device, nullptr);

                logger.success() << "Logical device destroyed";
            }
        }

        inline LogicalDevice(const LogicalDevice &) = delete;
        inline LogicalDevice(LogicalDevice && other) noexcept:
            logical_device(other.logical_device) {
            other.logical_device = nullptr;
        }

        [[nodiscard]] constexpr const VkDevice & handle() const noexcept { return logical_device; }
    };
}