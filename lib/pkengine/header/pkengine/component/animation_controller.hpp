#pragma once

#include "component_base.hpp"
#include "transform.hpp"

namespace PKEngine {
    class AnimationController : public ComponentBase {
    protected:
        Transform & origin;

        unsigned int current_index = 0;

        std::vector<Vector3> positions;

    public:
        inline AnimationController(
            EngineNode & _parent,
            Transform & _origin,
            std::vector<Vector3> && _positions
        ):
            ComponentBase(_parent),
            origin(_origin),
            positions(std::move(_positions)) { }

        inline void start() override { ComponentBase::start();
            origin.position = positions[0];
        }

        inline void update() override { ComponentBase::update();
            if ((positions[current_index] - origin.position).magnitude() < 0.001f) {
                current_index++;
                if (current_index == positions.size()) current_index = 0;
            }
            else {
                Vector3 dir = (positions[current_index] - origin.position);
                float mag = dir.magnitude();
                Vector3 velocity = dir * (1 / std::min(mag, 1.0f));
                origin.position += velocity * (float) (Time::delta_time<std::chrono::microseconds>()).count() / 1000000.0f;
            }
        }
    };
}