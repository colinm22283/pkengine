#pragma once

#include <typeinfo>

#include <pkengine/scene_node.hpp>

namespace PKEngine { class SceneNode; }

namespace PKEngine::Component {
    class Component {
    protected:
        SceneNode & parent;
    public:
        inline Component() = delete;
        explicit inline Component(SceneNode & _parent): parent(_parent) { }
        virtual inline ~Component() = default;

        virtual inline void parent_changed() { }

        virtual inline void start() { }
        virtual inline void update() { }

        [[nodiscard]] virtual inline const std::type_info & type_id() const noexcept = 0;
    };
}