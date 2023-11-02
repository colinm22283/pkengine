#pragma once

#include "device_buffer.hpp"
#include "vertex.hpp"
#include "../command_buffer.hpp"

namespace PKEngine::Vulkan {
//    template<const auto & logical_device, const auto & physical_device>
//    using VertexBuffer = DeviceBuffer<logical_device, physical_device, Vertex, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT>;

    template<const auto & logical_device, const auto & physical_device>
    class VertexBuffer {
    protected:
        DeviceBuffer<
            logical_device, physical_device,
            Vertex,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        > staging_buffer;
        DeviceBuffer<
            logical_device, physical_device,
            Vertex,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        > vertex_buffer;

        VkDeviceSize _capacity = 0; // count
        VkDeviceSize _size = 0; // bytes

        template<const auto & command_pool, const auto & vulkan_queue>
        inline void commit_staging_buffer() {
            CommandBuffer<logical_device, command_pool> command_buffer;
            command_buffer.init();

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(command_buffer.handle(), &beginInfo);
            VkBufferCopy copyRegion{};
            copyRegion.srcOffset = 0;
            copyRegion.dstOffset = 0;
            copyRegion.size = _size;
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
        inline void init(VkDeviceSize capacity) {
            _capacity = capacity;
            _size = 0;

            staging_buffer.init(capacity);
            vertex_buffer.init(capacity);
        }
        inline void free() {
            _capacity = 0;
            _size = 0;
            vertex_buffer.free();
            staging_buffer.free();
        }

        [[nodiscard]] inline VkBuffer buffer_handle() const noexcept { return vertex_buffer.buffer_handle(); }

        [[nodiscard]] inline VkDeviceSize capacity() const noexcept { return _capacity; }
        [[nodiscard]] inline VkDeviceSize size() const noexcept { return _size; }
        [[nodiscard]] consteval auto get_bind_info() const noexcept { return Vertex::get_bind_info(); }

        template<const auto & command_pool, const auto & vulkan_queue>
        inline void send_buffer(Vertex * data, VkDeviceSize count) {
            PKENGINE_INLINE_RUNTIME_ASSERT(count <= _capacity, "count must be less than capacity in send_buffer call");

            _size = count * sizeof(Vertex);

            void * temp;
            vkMapMemory(logical_device.handle(), staging_buffer.memory_handle(), 0, _size, 0, &temp);
            std::memcpy(temp, data, (std::size_t) _size);
            vkUnmapMemory(logical_device.handle(), staging_buffer.memory_handle());

            commit_staging_buffer<command_pool, vulkan_queue>();
        }
        template<const auto & command_pool, const auto & vulkan_queue, VkDeviceSize n>
        inline void send_buffer(Vertex (&& vertexes)[n]) { send_buffer<command_pool, vulkan_queue>(vertexes, n); }
        template<const auto & command_pool, const auto & vulkan_queue, VkDeviceSize n>
        inline void send_buffer(Vertex (& vertexes)[n]) { send_buffer<command_pool, vulkan_queue>(vertexes, n); }
    };
}