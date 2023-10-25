#pragma once

#include <pkengine/math/vector3.hpp>

namespace PKEngine {
    class Transform : public ComponentBase {
    public:
        Vector3 position, rotation, scale;

        inline Transform() = default;
    };
}