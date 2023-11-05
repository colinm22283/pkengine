#pragma once

#include <vulkan/vulkan.h>

#include "staged_buffer.hpp"
#include "vertex.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & physical_device>
    class VertexBuffer : public StagedBuffer<logical_device, physical_device, Vertex, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT> {
    public:
        [[nodiscard]] consteval auto get_bind_info() const noexcept { return Vertex::get_bind_info(); }
    };
}