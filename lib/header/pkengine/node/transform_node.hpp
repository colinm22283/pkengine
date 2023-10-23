#pragma once

#include <pkengine-internal/internal/vulkan/shader/vertex.hpp>

#include "base_node.hpp"

namespace PKEngine {
    class TransformNode : public BaseNode {
    public:
        Vulkan::Vertex position;
        Vulkan::Vertex rotation;
        Vulkan::Vertex scale;
    };
}