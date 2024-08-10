#pragma once

#include "component_base.hpp"
#include <pkengine/math/vector3.hpp>

namespace PKEngine {
    class Transform : public ComponentBase {
    protected:
        explicit inline Transform(
            EngineNode & _parent,
            const Vector3 & _absolute_position,
            const Vector3 & _absolute_rotation,
            const Vector3 & _absolute_scale
        ):
            ComponentBase(_parent),
            absolute_position(_absolute_position),
            absolute_rotation(_absolute_rotation),
            absolute_scale(_absolute_scale) { }

    public:
        Vector3 position = Vector3(0, 0, 0);
        Vector3 rotation = Vector3(0, 0, 0);
        Vector3 scale = Vector3(1, 1, 1);

        using ComponentBase::ComponentBase;

        const Vector3 & absolute_position;
        const Vector3 & absolute_rotation;
        const Vector3 & absolute_scale;

        explicit inline Transform(EngineNode & _parent):
            ComponentBase(_parent),
            absolute_position(position),
            absolute_rotation(rotation),
            absolute_scale(scale) { }
    };
}
