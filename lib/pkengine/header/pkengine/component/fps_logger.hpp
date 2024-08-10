#pragma once

#include <pkengine/component/component_base.hpp>
#include <pkengine/time.hpp>

namespace PKEngine {
    class FPSLogger : public ComponentBase {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::MagentaFg, "FPS Logger">();

        Time::duration interval;
        Time::duration current_time = Time::duration(0);

    public:
        template<typename T>
        inline FPSLogger(EngineNode & _parent, T _interval):
            ComponentBase(_parent),
            interval(std::chrono::duration_cast<Time::duration>(_interval)) { }

        inline void update() override {
            ComponentBase::update();

            if (current_time >= interval) {
                current_time -= interval;

                logger << Time::frames_per_second();
            }

            current_time += Time::delta_time();
        }
    };
}