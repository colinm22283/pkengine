#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/util/vulkan_exception.hpp>
#include <pkengine/vulkan/struct/image_create_info.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class VulkanImage {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize vulkan image");
        };

    protected:
        static constexpr auto logger = Logger<"Vulkan Image">();

        LogicalDevice & logical_device;

        VkImage image = VK_NULL_HANDLE;

    public:
        inline VulkanImage(
            LogicalDevice & _logical_device,
            VkFormat format,
            VkImageUsageFlags usage_flags,
            VkExtent3D extent
        ):
            logical_device(_logical_device) {
            logger.debug() << "Initializing vulkan image";

            VkImageCreateInfo image_info = Struct::image_create_info(
                format, usage_flags, extent
            );

            Util::throw_on_fail<Exceptions::InitError>(
                vkCreateImage(logical_device.handle(), &image_info, nullptr, &image)
            );

            logger.debug() << "Vulkan image initialized";
        }
        inline ~VulkanImage() {
            if (image != VK_NULL_HANDLE) {
                logger.debug() << "Destroying vulkan image";

                vkDestroyImage(logical_device.handle(), image, nullptr);

                logger.debug() << "Vulkan image destroyed";
            }
        }

        [[nodiscard]] constexpr const VkImage & handle() const noexcept { return image; }
    };
}
