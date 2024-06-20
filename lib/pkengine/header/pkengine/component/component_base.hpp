#pragma once

#include <chrono>

#include <pkengine/logger/logger.hpp>

namespace PKEngine {
    class EngineNode;

    class ComponentBase {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::MagentaFg, "Debug">();

        EngineNode const & parent;

    public:
        explicit inline ComponentBase() = delete;
        explicit inline ComponentBase(EngineNode & _parent): parent(_parent) { }
        virtual inline ~ComponentBase() = default;

        virtual inline void pre_start() { }
        virtual inline void pre_update() { }
        virtual inline void record_buffer() { }

        virtual inline void start() { }
        virtual inline void update() { }

        virtual inline void key_down() { }
    };
}