#pragma once

#include <memory>
#include <forward_list>

#include "component/component_base.hpp"
#include "engine_node.hpp"

namespace PKEngine {
    class _ObjectTree {
    protected:
        std::forward_list<EngineNode> branches;

    public:
        inline EngineNode & add_node() { return branches.emplace_front(); }

        inline void start() {
            for (auto & ele : branches) ele.start();
        }
        inline void update() {
            for (auto & ele : branches) ele.update();
        }
    };

    extern _ObjectTree object_tree;
}