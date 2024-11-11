#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include <pkengine/vulkan/util/vulkan_exception.hpp>

#include <pkengine/vulkan/alloc/vulkan_allocator.hpp>

#include <pkengine/vulkan/struct/image_create_info.hpp>
#include <pkengine/vulkan/struct/image_view_create_info.hpp>

namespace PKEngine::Vulkan::Alloc {
    class AllocatedImage {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(ImageInitError, "Unable to allocate vulkan image");
            PKENGINE_DEFINE_VULKAN_EXCEPTION(ImageViewInitError, "Unable to initialize vulkan image view");
        };

    protected:
        static constexpr auto logger = Logger<"Allocated Image">();

        Wrapper::LogicalDevice & logical_device;
        VulkanAllocator & allocator;

        VkFormat image_format;
        VkExtent3D image_extent;
        VkImage image = VK_NULL_HANDLE;
        VkImageView image_view = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;

    public:
        inline AllocatedImage(
            Wrapper::LogicalDevice & _logical_device,
            VulkanAllocator & _allocator,
            VkFormat _image_format,
            VkExtent3D _image_extent,
            VkImageUsageFlags image_usage_flags,
            VmaMemoryUsage memory_usage,
            VkMemoryPropertyFlags memory_properties
        ):
            logical_device(_logical_device),
            allocator(_allocator),
            image_format(_image_format),
            image_extent(_image_extent) {
            VkImageCreateInfo image_info = Struct::image_create_info(
                image_format,
                image_usage_flags,
                image_extent
            );

            VmaAllocationCreateInfo alloc_info = {
                .usage = memory_usage,
                .requiredFlags = memory_properties,
            };

            vmaCreateImage(allocator.handle(), &image_info, &alloc_info, &image, &allocation, nullptr);

            VkImageViewCreateInfo image_view_info = Struct::image_view_create_info(
                image_format,
                image,
                VK_IMAGE_ASPECT_COLOR_BIT
            );

            vkCreateImageView(logical_device.handle(), &image_view_info, nullptr, &image_view);

            logger.debug() << "Allocated vulkan image";
        }

        inline ~AllocatedImage() {
            if (image_view != VK_NULL_HANDLE) vkDestroyImageView(logical_device.handle(), image_view, nullptr);
            if (image != VK_NULL_HANDLE) {
                vmaDestroyImage(allocator.handle(), image, allocation);

                logger.debug() << "Deallocated vulkan image";
            }
        }

        [[nodiscard]] inline const VkImage & handle() const noexcept { return image; }
        [[nodiscard]] inline const VkExtent3D & extent() const noexcept { return image_extent; }
        [[nodiscard]] inline const VkImageView & view() const noexcept { return image_view; }
    };
}
