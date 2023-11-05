#pragma once

#include <vulkan/vulkan.h>

#include "device_buffer.hpp"
#include "../command_buffer.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & physical_device, typename T, VkBufferUsageFlags usage = 0, VkMemoryPropertyFlags properties = 0>
    class StagedBuffer {
        static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");

    protected:
        DeviceBuffer<
            logical_device, physical_device,
            T,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        > staging_buffer;
        DeviceBuffer<
            logical_device, physical_device,
            T,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | properties
        > vertex_buffer;

        VkDeviceSize _capacity = 0; // count

        template<const auto & command_pool, const auto & vulkan_queue>
        inline void commit_staging_buffer(VkDeviceSize offset, VkDeviceSize size) {
            CommandBuffer<logical_device, command_pool> command_buffer;
            command_buffer.init();

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(command_buffer.handle(), &beginInfo);
            VkBufferCopy copyRegion{};
            copyRegion.srcOffset = 0;
            copyRegion.dstOffset = offset;
            copyRegion.size = size;
            vkCmdCopyBuffer(command_buffer.handle(), staging_buffer.buffer_handle(), vertex_buffer.buffer_handle(), 1, &copyRegion);

            vkEndCommandBuffer(command_buffer.handle());

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            VkCommandBuffer cmd_buf = command_buffer.handle();
            submitInfo.pCommandBuffers = &cmd_buf;

            vkQueueSubmit(vulkan_queue.handle(), 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(vulkan_queue.handle());

            command_buffer.free();
        }

    public:
        inline void init(VkDeviceSize __capacity) {
            _capacity = __capacity;
            staging_buffer.init(__capacity);
            vertex_buffer.init(__capacity);
        }
        inline void free() {
            _capacity = 0;
            vertex_buffer.free();
            staging_buffer.free();
        }

        [[nodiscard]] inline VkBuffer buffer_handle() const noexcept { return vertex_buffer.buffer_handle(); }

        [[nodiscard]] inline VkDeviceSize capacity() const noexcept { return _capacity; }

        template<const auto & command_pool, const auto & vulkan_queue>
        inline void send_buffer(T * data, VkDeviceSize count, VkDeviceSize offset = 0) {
            VkDeviceSize size = count * sizeof(T);

            void * temp;
            vkMapMemory(logical_device.handle(), staging_buffer.memory_handle(), 0, size, 0, &temp);
            std::memcpy(temp, data, (std::size_t) size);
            vkUnmapMemory(logical_device.handle(), staging_buffer.memory_handle());

            commit_staging_buffer<command_pool, vulkan_queue>(offset, size);
        }
        template<const auto & command_pool, const auto & vulkan_queue, VkDeviceSize n>
        inline void send_buffer(T (&& vertexes)[n], VkDeviceSize offset = 0) { send_buffer<command_pool, vulkan_queue>(vertexes, n, offset); }
        template<const auto & command_pool, const auto & vulkan_queue, VkDeviceSize n>
        inline void send_buffer(T (& vertexes)[n], VkDeviceSize offset = 0) { send_buffer<command_pool, vulkan_queue>(vertexes, n, offset); }
    };
}