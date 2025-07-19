#pragma once

#include <pkengine/component/transform.hpp>

#include <pkengine/math/vector.hpp>

namespace PKEngine::Component {
    class AbsoluteTransform : public Transform {
    public:
        explicit inline AbsoluteTransform(SceneNode & _parent): Transform(_parent) { }
        inline ~AbsoluteTransform() override = default;

        [[nodiscard]] inline const std::type_info & type_id() const noexcept override { return typeid(Transform); }

        [[nodiscard]] inline Math::Vector3 absolute_position() const noexcept final { return position; }
        [[nodiscard]] inline Math::Vector3 absolute_rotation() const noexcept final { return rotation; }
        [[nodiscard]] inline Math::Vector3 absolute_scale() const noexcept final { return scale; }
    };
}