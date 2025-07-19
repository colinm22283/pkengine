#pragma once

#include <pkengine/component/transform.hpp>

#include <pkengine/math/vector.hpp>

namespace PKEngine::Component {
    class RelativeTransform : public Transform {
    protected:
        std::reference_wrapper<Transform> parent_transform;

    public:
        explicit inline RelativeTransform(SceneNode & _parent):
            Transform(_parent),
            parent_transform([this]() -> Transform & {
                auto & transform = parent.get_component_of_type<Transform>();

                return transform;
            }()) { }
        inline ~RelativeTransform() override = default;

        inline void update() override {

        }

        [[nodiscard]] inline const std::type_info & type_id() const noexcept override { return typeid(Transform); }

        [[nodiscard]] inline Math::Vector3 absolute_position() const noexcept final {
            return parent_transform.get().position + position;
        }
        [[nodiscard]] inline Math::Vector3 absolute_rotation() const noexcept final { return rotation; }
        [[nodiscard]] inline Math::Vector3 absolute_scale() const noexcept final { return scale; }
    };
}