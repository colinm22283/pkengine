#pragma once

#include <pkengine/component/component_base.hpp>
#include <pkengine/component/transform.hpp>

namespace PKEngine {
    class Camera : public ComponentBase {
    protected:
        Transform & transform;

    public:
        Camera(EngineNode & _parent, Transform & _transform): transform(_transform) { }
    };
}