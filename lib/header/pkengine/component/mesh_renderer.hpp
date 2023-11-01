#pragma once

#include "pkengine-internal/engine.hpp"

#include "component_base.hpp"
#include "transform.hpp"

namespace PKEngine {
    class MeshRenderer : public ComponentBase {
    protected:
        float iter = 0;
        Transform & origin;

    public:
        explicit inline MeshRenderer(EngineNode & _parent, Transform & _origin): ComponentBase(_parent), origin(_origin) { }

        inline void update() override { ComponentBase::update();
            PKEngine::Vulkan::Vertex vertexes[] = {
                PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(-0.2, 0, 0)),
                PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(0, -0.2, 0)),
                PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(0, 0.2, 0)),
                PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(0, -0.2, 0)),
                PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(0.2, 0, 0)),
                PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(0, 0.2, 0))
            };

            engine_instance::vertex_buffer.send_buffer<engine_instance::command_pool, engine_instance::graphics_queue>(vertexes, 6);
        }
    };
}