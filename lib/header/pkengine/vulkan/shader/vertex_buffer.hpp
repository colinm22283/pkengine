#pragma once

#include "device_buffer.hpp"
#include "vertex.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & physical_device>
    using VertexBuffer = DeviceBuffer<logical_device, physical_device, Vertex, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT>;
}