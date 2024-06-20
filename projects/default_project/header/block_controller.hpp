#pragma once

#include <pkengine/component/component_base.hpp>
#include <pkengine/component/transform.hpp>

class BlockController : public PKEngine::ComponentBase {
protected:
    float time = 0;
    PKEngine::Transform & pos;

public:
    inline BlockController(PKEngine::EngineNode & _parent, PKEngine::Transform & _pos): PKEngine::ComponentBase(_parent), pos(_pos) { }
    
    inline void update() override {
        using namespace PKEngine;

        pos.position = Vector3(0.3f * std::cos(time), 0.3f * std::sin(time), 0);
        pos.rotation = Vector3(time, time, time);

        time += (float) std::chrono::duration_cast<std::chrono::microseconds>(Time::delta_time()).count() * 0.000001f;
    }
};