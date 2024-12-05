#pragma once

#include <pkengine/vulkan/shader_struct/vertex.hpp>

#include <pkengine/vulkan/alloc/allocated_buffer.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/command_queue.hpp>
#include <pkengine/vulkan/wrapper/command_buffer.hpp>
#include <pkengine/vulkan/wrapper/sync/fence.hpp>

namespace PKEngine::Vulkan {
    class Mesh {
    protected:
        Alloc::AllocatedBuffer<uint32_t> _index_buffer;
        Alloc::AllocatedBuffer<ShaderStruct::Vertex> _vertex_buffer;
        VkDeviceAddress _vertex_buffer_address;

    public:
        inline Mesh(
            Wrapper::LogicalDevice & logical_device,
            Wrapper::CommandQueue & graphics_queue,
            Wrapper::CommandBuffer & imm_command_buffer,
            Wrapper::Sync::Fence & imm_fence,
            Alloc::VulkanAllocator & allocator,
            std::span<uint32_t> indexes,
            std::span<ShaderStruct::Vertex> vertices
        ):
            _index_buffer(
                allocator,
                indexes.size(),
                VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                VMA_MEMORY_USAGE_GPU_ONLY
            ),
            _vertex_buffer(
                allocator,
                vertices.size(),
                VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
                VMA_MEMORY_USAGE_GPU_ONLY
            ) {
            VkBufferDeviceAddressInfo device_address_info = {
                .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
                .buffer = _vertex_buffer.handle(),
            };
            _vertex_buffer_address = vkGetBufferDeviceAddress(
                logical_device.handle(),
                &device_address_info
            );

            Alloc::AllocatedBuffer<char> staging_buffer(
                allocator,
                indexes.size_bytes() + vertices.size_bytes(),
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VMA_MEMORY_USAGE_CPU_ONLY
            );

            void * data = staging_buffer.get_mapped_data();

            std::memcpy(data, vertices.data(), vertices.size_bytes());
            std::memcpy((char *) data + vertices.size_bytes(), indexes.data(), indexes.size_bytes());

            imm_command_buffer.immediate_record(
                imm_fence,
                [&]() {
                    VkBufferCopy vertex_copy {
                        .srcOffset = 0,
                        .dstOffset = 0,
                        .size = vertices.size_bytes(),
                    };
                    vkCmdCopyBuffer(
                        imm_command_buffer.handle(),
                        staging_buffer.handle(),
                        _vertex_buffer.handle(),
                        1,
                        &vertex_copy
                    );

                    VkBufferCopy index_copy {
                        .srcOffset = vertices.size_bytes(),
                        .dstOffset = 0,
                        .size = indexes.size_bytes(),
                    };

                    vkCmdCopyBuffer(
                        imm_command_buffer.handle(),
                        staging_buffer.handle(),
                        _index_buffer.handle(),
                        1,
                        &index_copy
                    );
                }
            );

            graphics_queue.submit(imm_command_buffer, imm_fence);

            imm_fence.await();
        }

        [[nodiscard]] constexpr const Alloc::AllocatedBuffer<uint32_t> & index_buffer() const noexcept { return _index_buffer; }
        [[nodiscard]] constexpr const Alloc::AllocatedBuffer<ShaderStruct::Vertex> & vertex_buffer() const noexcept { return _vertex_buffer; }
        [[nodiscard]] constexpr const VkDeviceAddress & vertex_buffer_address() const noexcept { return _vertex_buffer_address; }
    };
}