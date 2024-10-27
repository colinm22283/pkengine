#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/util/exception.hpp>

#include <pkengine/vulkan/wrapper/vulkan_instance.hpp>
#include <pkengine/vulkan/wrapper/surface.hpp>
#include <pkengine/vulkan/util/vulkan_exception.hpp>
#include <pkengine/vulkan/util/checks/is_device_suitable.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class PhysicalDevice {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_EXCEPTION(UnableToFindCapableGPU, "Unable to find vulkan capable GPU");

            PKENGINE_DEFINE_VULKAN_EXCEPTION(UnableToEnumerateDevices, "Unable to enumerate physical devices");
        };
    protected:
        static constexpr auto logger = Logger<"Physical Device">();

        VulkanInstance & vulkan_instance;

        VkPhysicalDevice physical_device = VK_NULL_HANDLE;

    public:
        inline PhysicalDevice(VulkanInstance & _vulkan_instance, Surface & surface):
            vulkan_instance(_vulkan_instance) {
            logger.debug() << "Initializing physical device...";

            uint32_t device_count = 0;
            Util::throw_on_fail<Exceptions::UnableToEnumerateDevices>(
                vkEnumeratePhysicalDevices(vulkan_instance.handle(), &device_count, nullptr)
            );
            if (device_count == 0) throw Exceptions::UnableToFindCapableGPU();
            VkPhysicalDevice devices[device_count];
            Util::throw_on_fail<Exceptions::UnableToEnumerateDevices>(
                vkEnumeratePhysicalDevices(vulkan_instance.handle(), &device_count, devices)
            );

            logger.debug() << "Checking device suitability:";
            auto indented_logger = logger.debug().indent();

            for (const auto & device : devices) {
                VkPhysicalDeviceProperties props;

                vkGetPhysicalDeviceProperties(device, &props);

                indented_logger << "Device \"" << props.deviceName << "\":";
                if (Util::Checks::is_device_suitable(surface, device)) {
                    indented_logger.indent() << "Suitable";

                    physical_device = device;
                    break;
                }
                else {
                    indented_logger.indent() << "Not suitable";
                }
            }

            if (physical_device == VK_NULL_HANDLE) throw Exceptions::UnableToFindCapableGPU();

            logger.success() << "Physical device initialized";
        }

        [[nodiscard]] constexpr const VkPhysicalDevice & handle() const noexcept { return physical_device; }
    };
}