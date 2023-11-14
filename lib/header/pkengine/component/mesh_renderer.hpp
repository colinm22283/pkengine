#pragma once

#include <vulkan/vulkan.h>

#include "pkengine/engine.hpp"

#include <pkengine/time.hpp>

#include "component_base.hpp"
#include "transform.hpp"

namespace PKEngine {
    class MeshRenderer : public ComponentBase {
    protected:
        float iter = 0;
        Transform & origin;

        engine_instance::model_allocator_t::Allocation model = engine_instance::model_allocator.template allocate<
            engine_instance::command_pool, engine_instance::graphics_queue
        >({
            PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(-0.2f, 0, origin.position.x / 2 + 0.5f)),
            PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(0.2, 0, origin.position.x / -2 + 1.5f)),
            PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(0, 0.2, origin.position.x / -2 + 1.5f)),
            PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(0, -0.2, origin.position.x / 2 + 0.5f)),
        }, {
            0, 1, 2, 1, 0, 3
        });

    public:
        explicit inline MeshRenderer(EngineNode & _parent, Transform & _origin): ComponentBase(_parent), origin(_origin) { }

        inline void update() override {
            engine_instance::vertex_buffer.begin_transfer<engine_instance::command_pool, engine_instance::graphics_queue>().add({
                PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(-0.2f, 0, origin.position.x / -2 + 1.5f)),
                PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(0.2, 0, origin.position.x / -2 + 1.5f)),
                PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(0, 0.2, origin.position.x / 2 + 0.5f)),
                PKEngine::Vulkan::Vertex(origin.absolute_position + Vector3(0, -0.2, origin.position.x / 2 + 0.5f)),
            }, model.vertex_allocation.offset()).commit();
        }

        inline void sync_record_buffer() override { ComponentBase::sync_record_buffer();
            vkCmdDrawIndexed(
                engine_instance::command_buffer.handle(),
                model.index_allocation.size(),
                1,
                model.index_allocation.offset(),
                (int32_t) model.vertex_allocation.offset(),
                0
            );
        }
    };
}