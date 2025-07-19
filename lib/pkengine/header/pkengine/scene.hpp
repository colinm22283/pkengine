#pragma once

#include <pkengine/scene_node.hpp>

#include <pkengine/logger/logger.hpp>

namespace PKEngine {
    class Scene {
    protected:
        static constexpr auto logger = Logger<"Scene">();

        SceneNode top_node;

    public:
        inline Scene() {
            logger.debug() << "Initializing scene...";

            logger.debug() << "Scene initialized";
        }

        inline ~Scene() {
            logger.debug() << "Destroying scene...";

            logger.debug() << "Scene destroyed";
        }

        [[nodiscard]] inline SceneNode & get_top_node() noexcept { return top_node; }
    };
}