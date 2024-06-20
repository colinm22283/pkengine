#pragma once

#include "component/component_base.hpp"

namespace PKEngine {
    class EngineNode {
        friend class ObjectTree;
    protected:
        std::forward_list<std::unique_ptr<ComponentBase>> _components;
        std::forward_list<EngineNode> branches;

        inline void start() {
            for (auto & ele : _components) ele->pre_start();
            for (auto & ele : _components) ele->start();
            for (auto & ele : branches) ele.start();
        }
        inline void update() {
            for (auto & ele : _components) ele->pre_update();
            for (auto & ele : _components) ele->update();
            for (auto & ele : branches) ele.update();
        }
        inline void record_buffer() {
            for (auto & ele : _components) ele->record_buffer();
            for (auto & ele : branches) ele.record_buffer();
        }
        inline void key_down() {
            for (auto & ele : _components) ele->key_down();
            for (auto & ele : branches) ele.key_down();
        }

    public:
        inline EngineNode() = default;
        inline EngineNode(EngineNode &) = delete;
        inline EngineNode(EngineNode && n) noexcept = default;

        [[nodiscard]] inline std::forward_list<std::unique_ptr<ComponentBase>> & components() noexcept { return _components; }
        [[nodiscard]] inline std::forward_list<EngineNode> & children() noexcept { return branches; }

        inline EngineNode & add_node() { return branches.emplace_front(); }

        template<typename T, typename... Args>
        inline T & add_component(Args &&... args) {
            return (T &) *(_components.emplace_front(std::move(std::make_unique<T>(*this, std::forward<Args>(args)...))));
        }
    };
}