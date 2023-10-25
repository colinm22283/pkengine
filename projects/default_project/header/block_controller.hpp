#pragma once

#include <pkengine/component/component_base.hpp>
#include <pkengine/component/transform.hpp>

class BlockController : public PKEngine::ComponentBase {
protected:
    float time = 0;
    PKEngine::Transform & pos;

public:
    explicit inline BlockController(PKEngine::Transform & _pos): pos(_pos) { }

    inline void update() override {
        pos.position = { std::cos(time), std::sin(time), 0 };

        time += 0.001;
    }
};