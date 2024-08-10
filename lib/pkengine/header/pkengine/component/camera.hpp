#pragma once

#include <pkengine/component/component_base.hpp>
#include <pkengine/component/transform.hpp>

namespace PKEngine {
    class Camera : public ComponentBase {
    protected:
        Transform & transform;

        Matrix4 proj { };

        float fov = M_PI / 2.0f;
        float near = 0.1f;
        float far = 10.0f;

        inline void update_proj_matrix() {
            proj = perspective_matrix(
                fov,
                (float) engine_instance::swap_chain.extent.width / (float) engine_instance::swap_chain.extent.height,
                near,
                far
            );
        }

    public:
        inline Camera(EngineNode & _parent, Transform & _transform): ComponentBase(_parent), transform(_transform) {
            update_proj_matrix();
        }

        inline void update() override {
            Vulkan::SceneData * ubo = engine_instance::scene_data_buffers[engine_instance::current_frame].buffer();

            ubo->proj = proj;
            ubo->view =
                translation_matrix(-transform.position.x, -transform.position.y, -transform.position.z) *
                rotation_matrix(transform.rotation.x, transform.rotation.y, transform.rotation.z) *
                scale_matrix(1, 1, -1);
        }
    };
}