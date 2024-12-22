#pragma once

#include <span>

#include <pkengine/vulkan/mesh_buffer.hpp>

namespace PKEngine::Vulkan {
    class Mesh {
    public:
        struct SubmeshRange { uint32_t start_index, count; };

    protected:
        std::vector<SubmeshRange> _submesh_ranges;
        MeshBuffer _mesh_buffer;

    public:
        inline Mesh(
            Wrapper::LogicalDevice & logical_device,
            Wrapper::CommandQueue & graphics_queue,
            Wrapper::CommandBuffer & imm_command_buffer,
            Wrapper::Sync::Fence & imm_fence,
            Alloc::VulkanAllocator & allocator,
            std::span<uint32_t> indexes,
            std::span<ShaderStruct::Vertex> vertices,
            std::span<SubmeshRange> _submesh_ranges_
        ):
            _submesh_ranges(_submesh_ranges_.begin(), _submesh_ranges_.end()),
            _mesh_buffer(
                logical_device,
                graphics_queue,
                imm_command_buffer,
                imm_fence,
                allocator,
                indexes,
                vertices
            ) { }

        [[nodiscard]] inline const std::vector<SubmeshRange> & submesh_ranges() const noexcept { return _submesh_ranges; }
        [[nodiscard]] inline const MeshBuffer & mesh_buffer() const noexcept { return _mesh_buffer; }
    };
}