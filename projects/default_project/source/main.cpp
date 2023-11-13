#include <pkengine-internal/engine.hpp>

#include <pkengine/component/mesh_renderer.hpp>
#include <pkengine/component/relative_transform.hpp>
#include <block_controller.hpp>
#include <pkengine/component/animation_controller.hpp>

void init() {
    using namespace PKEngine;

    auto & block_node = engine_instance::object_tree.add_node();
    auto & block_transform = block_node.add_component<Transform>();
    block_transform.position.x = 0.5;
    block_transform.position.y = 0.5;
    block_node.add_component<MeshRenderer>(block_transform);
    block_node.add_component<AnimationController>(block_transform, std::vector<Vector3> {
        Vector3(0, 0, 0),
        Vector3(1, 1, 0),
        Vector3(1, -1, 0),
        Vector3(-0.8, -0.5, 0)
    });

    auto & block_node_2 = engine_instance::object_tree.add_node();
    auto & block_transform_2 = block_node_2.add_component<Transform>();
    block_transform_2.position.x = -0.5;
    block_transform_2.position.y = -0.5;
    block_node_2.add_component<MeshRenderer>(block_transform_2);
    block_node_2.add_component<BlockController>(block_transform_2);
}
