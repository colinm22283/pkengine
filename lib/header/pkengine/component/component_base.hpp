#pragma once

#include <chrono>

#include <pkengine-internal/logger/logger.hpp>

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

        virtual inline void sync_start() { }
        virtual inline void sync_update() { }
        virtual inline void sync_record_buffer() { }

        virtual inline void start() { }
        virtual inline void update() { }
    };
}