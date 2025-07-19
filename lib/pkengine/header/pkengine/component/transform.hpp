#pragma once

#include <pkengine/component/component.hpp>

#include <pkengine/math/vector.hpp>

namespace PKEngine::Component {
    class Transform : public Component {
    public:
        Math::Vector3 position;
        Math::Vector3 rotation;
        Math::Vector3 scale;

        explicit inline Transform(SceneNode & _parent): Component(_parent) { }
        inline ~Transform() override = default;

        [[nodiscard]] inline const std::type_info & type_id() const noexcept override { return typeid(Transform); }

        [[nodiscard]] virtual inline Math::Vector3 absolute_position() const noexcept = 0;
        [[nodiscard]] virtual inline Math::Vector3 absolute_rotation() const noexcept = 0;
        [[nodiscard]] virtual inline Math::Vector3 absolute_scale() const noexcept = 0;
    };
}