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

            VkPhysicalDeviceVulkan13Features device_13_features = {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,

                .synchronization2 = true,
                .dynamicRendering = true,
            };
            VkPhysicalDeviceVulkan12Features device_12_features = {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
                .pNext = &device_13_features,

                .descriptorIndexing = true,
                .bufferDeviceAddress = true,
            };
            VkPhysicalDeviceFeatures device_features = { };

            VkDeviceCreateInfo create_info = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .pNext = &device_12_features,

                .queueCreateInfoCount = 1,
                .pQueueCreateInfos = &queue_create_info,
                .enabledLayerCount = 0,
                .enabledExtensionCount = (uint32_t) required_extensions.size(),
                .ppEnabledExtensionNames = required_extensions.data(),
                .pEnabledFeatures = &device_features,
            };

            logger.debug() << "Enabled extensions:";
            auto indented_logger = logger.indent();
            for (const auto & extension : required_extensions) {
                indented_logger.debug() << extension;
            }

            Util::throw_on_fail<Exceptions::InitError>(
                vkCreateDevice(physical_device.handle(), &create_info, nullptr, &logical_device)
            );

            logger.debug() << "Logical device initialized";
        }
        inline ~LogicalDevice() {
            if (logical_device != VK_NULL_HANDLE) {
                logger.debug() << "Destroying logical device...";

                vkDestroyDevice(logical_device, nullptr);

                logger.debug() << "Logical device destroyed";
            }
        }

        inline void wait_idle() {
            vkDeviceWaitIdle(logical_device);
        }

        inline LogicalDevice(const LogicalDevice &) = delete;
        inline LogicalDevice(LogicalDevice && other) noexcept:
            logical_device(other.logical_device) {
            other.logical_device = nullptr;
        }

        [[nodiscard]] constexpr const VkDevice & handle() const noexcept { return logical_device; }
    };
}