#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include <pkengine/vulkan/wrapper/vulkan_instance.hpp>
#include <pkengine/vulkan/wrapper/physical_device.hpp>
#include <pkengine/vulkan/wrapper/logical_device.hpp>

#include <pkengine/vulkan/util/vulkan_exception.hpp>

namespace PKEngine::Vulkan::Alloc {
    class VulkanAllocator {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize VMA Allocator");
        };
    protected:
        static constexpr auto logger = Logger<"Swap Chain">();

        VmaAllocator allocator = VK_NULL_HANDLE;

    public:
        inline VulkanAllocator(
            Wrapper::VulkanInstance & vulkan_instance,
            Wrapper::PhysicalDevice & physical_device,
            Wrapper::LogicalDevice & logical_device
        ) {
            logger.debug() << "Initializing vulkan allocator...";

            VmaAllocatorCreateInfo allocator_info {
                .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,

                .physicalDevice = physical_device.handle(),
                .device = logical_device.handle(),

                .instance = vulkan_instance.handle(),
            };

            vmaCreateAllocator(&allocator_info, &allocator);

            logger.success() << "Vulkan allocator initialized";
        }

        inline ~VulkanAllocator() {
            if (allocator != VK_NULL_HANDLE) {
                logger.debug() << "Destroying vulkan allocator...";

                vmaDestroyAllocator(allocator);

                logger.success() << "Vulkan allocator destroyed";
            }
        }

        [[nodiscard]] const VmaAllocator & handle() const noexcept { return allocator; }
    };
}