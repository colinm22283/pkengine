#pragma once

#include "pkengine-internal/logger/logger.hpp"

namespace PKEngine {
    class ComponentBase {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::CyanFg, "Debug">();

    public:
        virtual inline ~ComponentBase() = default;

        virtual inline void start() { }
        virtual inline void update() { }
    };
}