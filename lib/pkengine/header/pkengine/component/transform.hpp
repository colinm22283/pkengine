#pragma once

#include "component_base.hpp"
#include <pkengine/math/vector3.hpp>

namespace PKEngine {
    class Transform : public ComponentBase {
    public:
        Vector3 position, rotation, scale;

        using ComponentBase::ComponentBase;

        Vector3 & absolute_position = position;
        Vector3 & absolute_rotation = rotation;
        Vector3 & absolute_scale = scale;
    };
}