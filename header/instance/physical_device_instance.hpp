#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include <util/engine_exception.hpp>

#include <instance/vulkan_instance.hpp>

#include <checks/is_device_suitable.hpp>

namespace PKEngine {
    class PhysicalDeviceInstance {
    protected:
        VkPhysicalDevice _handle = VK_NULL_HANDLE;

    public:
        inline explicit PhysicalDeviceInstance(VkInstance vulkan_instance, VkSurfaceKHR surface) {
            uint32_t device_count = 0;
            vkEnumeratePhysicalDevices(vulkan_instance, &device_count, nullptr);
            if (device_count == 0) throw Exceptions::UnableToFindGPUsWithVulkanSupport();
            std::vector<VkPhysicalDevice> devices(device_count);
            vkEnumeratePhysicalDevices(vulkan_instance, &device_count, devices.data());

            for (const auto& device : devices) {
                if (Checks::is_device_suitable(device, surface)) {
                    _handle = device;
                    break;
                }
            }

            if (_handle == VK_NULL_HANDLE) throw Exceptions::SuitableGPUNotFound();
        }

        inline PhysicalDeviceInstance(PhysicalDeviceInstance && other): _handle(other._handle) { }
        inline PhysicalDeviceInstance(PhysicalDeviceInstance & other): _handle(other._handle) { }

        [[nodiscard]] inline VkPhysicalDevice const handle() const { return _handle; }
        [[nodiscard]] inline operator VkPhysicalDevice const() const { return handle(); }
    };
}