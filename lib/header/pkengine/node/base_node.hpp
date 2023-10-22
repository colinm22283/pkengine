#pragma once

namespace PKEngine {
    class BaseNode {
    public:
        virtual inline ~BaseNode() { }

        virtual inline void start() { };
        virtual inline void update() { };
    };
}