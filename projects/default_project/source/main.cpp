#include <vector>
#include <thread>
#include <cmath>

#include <pkengine/engine.hpp>
#include <pkengine/scene.hpp>

#include <pkengine/component/absolute_transform.hpp>
#include <pkengine/component/relative_transform.hpp>
#include <pkengine/component/mesh_renderer.hpp>

#include <pkengine/logger/logger.hpp>

static constexpr auto logger = PKEngine::Logger<"User Main">();

void init(PKEngine::Engine & engine, PKEngine::Context & context) {
    using namespace PKEngine;

    context.resize_window(800, 600);

    Scene & scene = context.get_scene();

    SceneNode & top_node = scene.get_top_node();
    top_node.add_component<Component::AbsoluteTransform>();

    SceneNode & object = top_node.add_child();

    auto & object_transform = object.add_component<Component::RelativeTransform>();
}
