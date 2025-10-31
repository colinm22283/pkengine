#pragma once

#include <list>

#include <pkengine/component/component.hpp>

#include "scene.hpp"

namespace PKEngine::Component { class Component; }

namespace PKEngine {
    class SceneNode {
        friend class Scene;

    public:
        struct Exceptions {
            PKENGINE_DEFINE_EXCEPTION(ComponentNotFoundWithType, "Unable to find component with requested type");
            PKENGINE_DEFINE_EXCEPTION(NoParent, "Component is the top level component");
        };

    protected:
        std::optional<std::reference_wrapper<SceneNode>> parent;

        inline SceneNode() = default;

    public:
        inline SceneNode(SceneNode & _parent) : parent(std::ref(_parent)) {}

        std::list<SceneNode> children;

        std::list<std::unique_ptr<Component::Component>> components;

        inline SceneNode & add_child() {
            return children.emplace_back(*this);
        }

        template<typename T, typename... Args>
        inline T & add_component(Args &&... args) {
            components.push_back(std::make_unique<T>(*this, std::forward<Args>(args)...));

            return (T &) *components.back();
        }

        template<typename T>
        inline T & get_component_of_type() {
            for (auto & component : components) {
                if (dynamic_cast<T *>(&*component) != nullptr) {
                    return (T &) *component;
                }
            }

            throw Exceptions::ComponentNotFoundWithType();
        }

        [[nodiscard]] inline SceneNode & parent_node() noexcept {
            if (parent.has_value()) return *parent;
            else throw Exceptions::NoParent();
        }
    };
}