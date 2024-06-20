#pragma once

#include <memory>
#include <forward_list>

#include "pkengine/component/component_base.hpp"
#include "pkengine/engine_node.hpp"

namespace PKEngine {
    class ObjectTree {
        friend class engine_instance;

    protected:
        std::forward_list<EngineNode> branches;

        inline void init() const noexcept { }
        inline void free() {
            branches.clear();
        }

    public:
        inline EngineNode & add_node() { return branches.emplace_front(); }

        inline void start() {
            for (auto & ele : branches) ele.start();
        }
        inline void update() {
            for (auto & ele : branches) ele.update();
        }
        inline void record_buffer() {
            for (auto & ele : branches) ele.record_buffer();
        }
    };
}