#pragma once

#include <pkengine-internal/logger/logger.hpp>

namespace PKEngine {
    class BaseNode {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::CyanFg, "Node">();

    public:
        virtual inline ~BaseNode() = default;

        virtual inline void start() { };
        virtual inline void update() { };
    };
}