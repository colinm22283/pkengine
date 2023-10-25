#pragma once

#include <memory>
#include <forward_list>

#include <pkengine/component/component_base.hpp>

namespace PKEngine {
    class _ObjectTree {
    protected:
        class Node {
            friend class _ObjectTree;
        protected:
            std::forward_list<std::unique_ptr<ComponentBase>> _components;
            std::forward_list<Node> branches;

            inline void start() {
                for (auto & ele : _components) ele->start();
                for (auto & ele : branches) ele.start();
            }
            inline void update() {
                for (auto & ele : _components) ele->update();
                for (auto & ele : branches) ele.update();
            }

        public:
            [[nodiscard]] inline std::forward_list<std::unique_ptr<ComponentBase>> & components() noexcept { return _components; }
            [[nodiscard]] inline std::forward_list<Node> & children() noexcept { return branches; }

            inline Node & add_node() { return branches.emplace_front(); }

            template<typename T, typename... Args>
            inline T & add_component(Args &&... args) {
                _components.push_front(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
                return (T &) _components.front();
            }
        };

        std::forward_list<Node> branches;

    public:
        inline Node & add_node() { return branches.emplace_front(); }

        inline void start() {
            for (auto & ele : branches) ele.start();
        }
        inline void update() {
            for (auto & ele : branches) ele.update();
        }
    };

    extern _ObjectTree object_tree;
}