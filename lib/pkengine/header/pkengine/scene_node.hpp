#pragma once

#include <list>

#include <pkengine/component/component.hpp>

namespace PKEngine::Component { class Component; }

namespace PKEngine {
    class SceneNode {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_EXCEPTION(ComponentNotFoundWithType, "Unable to find component with requested type");
        };

    public:
        std::list<SceneNode> children;

        std::list<std::unique_ptr<Component::Component>> components;

        inline SceneNode & add_child() {
            return children.emplace_back();
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
    };
}