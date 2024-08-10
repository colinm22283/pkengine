#pragma once

#include "pkengine/internal/vulkan/buffer/device_buffer.hpp"
#include "pkengine/internal/vulkan/types/pixel.hpp"

namespace PKEngine::Vulkan {
    class Image {
    protected:
        std::size_t _width, _height;

        DeviceBuffer<
            engine_instance::logical_device,
            engine_instance::physical_device,
            Pixel,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        > staging_buffer;

        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkImage image = VK_NULL_HANDLE;

    public:
        inline Image(std::size_t __width, std::size_t __height) : _width(__width), _height(__height) {
            staging_buffer.init(_width * _height);

            VkImageCreateInfo image_info {
                .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .flags = 0,
                .imageType = VK_IMAGE_TYPE_2D,
                .format = VK_FORMAT_R8G8B8A8_SRGB,
                .extent = {
                    .width = static_cast<uint32_t>(_width),
                    .height = static_cast<uint32_t>(_height),
                    .depth = 1,
                },
                .mipLevels = 1,
                .arrayLayers = 1,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            };

            if (vkCreateImage(engine_instance::logical_device.handle(), &image_info, nullptr, &image) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_vulkan_image();
            }

            VkMemoryRequirements memory_requirements;
            vkGetImageMemoryRequirements(engine_instance::logical_device.handle(), image, &memory_requirements);

            VkMemoryAllocateInfo alloc_info{
                .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                .allocationSize = memory_requirements.size,
                .memoryTypeIndex = choose_memory_type<engine_instance::physical_device>(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
            };

            if (vkAllocateMemory(engine_instance::logical_device.handle(), &alloc_info, nullptr, &memory) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_allocate_image_buffer();
            }
            if (vkBindImageMemory(engine_instance::logical_device.handle(), image, memory, 0) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_bind_image_buffer();
            }
        }

        inline ~Image() {
            if (memory != VK_NULL_HANDLE) vkFreeMemory(engine_instance::logical_device.handle(), memory, nullptr);

            if (image != VK_NULL_HANDLE) {
                vkDestroyImage(engine_instance::logical_device.handle(), image, nullptr);
            }

            staging_buffer.free();
        }

        inline Image(Image &) = delete;
        inline Image(Image && other) noexcept: _width(other._width), _height(other._height), staging_buffer(std::move(other.staging_buffer)), memory(other.memory), image(other.image) {
            other.memory = VK_NULL_HANDLE;
            other.image = VK_NULL_HANDLE;
        }

        [[nodiscard]] constexpr std::size_t width() const noexcept { return _width; }
        [[nodiscard]] constexpr std::size_t height() const noexcept { return _height; }

        [[nodiscard]] inline Pixel * bind_staging_buffer() { return (Pixel *) staging_buffer.bind(); }
        inline void unbind_staging_buffer() { staging_buffer.unbind(); }

        inline void commit_staging_buffer() {
            CommandBuffer<engine_instance::logical_device, engine_instance::command_pool> command_buffer;
            command_buffer.init();

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(command_buffer.handle(), &beginInfo);

            VkBufferImageCopy region {
                .bufferOffset = 0,
                .bufferRowLength = 0,
                .bufferImageHeight = 0,

                .imageSubresource = {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .mipLevel = 0,
                    .baseArrayLayer = 0,
                    .layerCount = 1,
                },

                .imageOffset = { 0, 0, 0 },

                .imageExtent = {
                    (uint32_t) _width,
                    (uint32_t) _height,
                    1
                },
            };

            vkCmdCopyBufferToImage(
                command_buffer.handle(),
                staging_buffer.buffer_handle(),
                image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1,
                &region
            );

            vkEndCommandBuffer(command_buffer.handle());

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            VkCommandBuffer cmd_buf = command_buffer.handle();
            submitInfo.pCommandBuffers = &cmd_buf;

            vkQueueSubmit(engine_instance::graphics_queue.handle(), 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(engine_instance::graphics_queue.handle());

            command_buffer.free();
        }
    };
}