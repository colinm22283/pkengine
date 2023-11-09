#pragma once

#include <vulkan/vulkan.h>

#include "device_buffer.hpp"
#include "../command_buffer.hpp"
#include "../../../logger/logger.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & physical_device, typename T, VkBufferUsageFlags usage = 0, VkMemoryPropertyFlags properties = 0>
    class StagedBuffer {
        static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");

        using buffer_t = StagedBuffer<logical_device, physical_device, T, usage, properties>;

    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Staged Buffer">();

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
        void * staging_buffer_memory;

        VkDeviceSize _capacity = 0; // count

        template<auto & command_pool, auto & vulkan_queue>
        class Committer {
            using this_t = Committer<command_pool, vulkan_queue>;

        protected:
            buffer_t & buffer;
            CommandBuffer<logical_device, command_pool> command_buffer;
            bool committed = false;
            std::vector<VkBufferCopy> copy_regions;

        public:
            explicit inline Committer(buffer_t & _buffer): buffer(_buffer) {
                command_buffer.init();

                VkCommandBufferBeginInfo beginInfo{};
                beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

                vkBeginCommandBuffer(command_buffer.handle(), &beginInfo);
            }
            inline ~Committer() { command_buffer.free(); }
            inline Committer(Committer &) = delete;
            inline Committer(Committer &&) = delete;

            inline this_t & add(T * data, VkDeviceSize size, VkDeviceSize offset) noexcept {
                VkDeviceSize byte_offset = offset * sizeof(T);
                VkDeviceSize byte_size = size * sizeof(T);

                std::memcpy((void *)(((char *) buffer.staging_buffer_memory) + byte_offset), data, byte_size);

                copy_regions.emplace_back(VkBufferCopy {
                    .srcOffset = byte_offset,
                    .dstOffset = byte_offset,
                    .size = byte_size,
                });

                return *this;
            }
            template<VkDeviceSize n> inline this_t & add(T (& data)[n], VkDeviceSize offset) noexcept { return add(data, n, offset); }
            template<VkDeviceSize n> inline this_t & add(T (&& data)[n], VkDeviceSize offset) noexcept { return add(std::move(data), n, offset); }

            inline void commit() noexcept {
                vkCmdCopyBuffer(
                    command_buffer.handle(),
                    buffer.staging_buffer.buffer_handle(),
                    buffer.vertex_buffer.buffer_handle(),
                    copy_regions.size(),
                    copy_regions.data()
                );

                vkEndCommandBuffer(command_buffer.handle());

                VkSubmitInfo submitInfo{};
                submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                submitInfo.commandBufferCount = 1;
                VkCommandBuffer cmd_buf = command_buffer.handle();
                submitInfo.pCommandBuffers = &cmd_buf;

                vkQueueSubmit(vulkan_queue.handle(), 1, &submitInfo, VK_NULL_HANDLE);
                vkQueueWaitIdle(vulkan_queue.handle());
            }
        };

    public:
        inline void init(VkDeviceSize __capacity) {
            staging_buffer.init(__capacity);
            vertex_buffer.init(__capacity);
            vkMapMemory(logical_device.handle(), staging_buffer.memory_handle(), 0, _capacity, 0, &staging_buffer_memory);
            _capacity = __capacity;
        }
        inline void free() {
            _capacity = 0;
            vkUnmapMemory(logical_device.handle(), staging_buffer.memory_handle());
            staging_buffer.free();
            vertex_buffer.free();
        }

        [[nodiscard]] inline VkBuffer buffer_handle() const noexcept { return vertex_buffer.buffer_handle(); }

        [[nodiscard]] inline VkDeviceSize capacity() const noexcept { return _capacity; }

        template<const auto & command_pool, const auto & vulkan_queue>
        [[nodiscard]] inline auto begin_transfer() noexcept { return Committer<command_pool, vulkan_queue>(*this); }
    };
}