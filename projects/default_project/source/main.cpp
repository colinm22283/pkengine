#include <pkengine/object_tree.hpp>

#include <pkengine/component/mesh_renderer.hpp>
#include <pkengine/component/relative_transform.hpp>
#include <block_controller.hpp>

void init() {
    using namespace PKEngine;

    auto & block_parent_node = object_tree.add_node();
    auto & block_parent_transform = block_parent_node.add_component<Transform>();

    auto & block_node = block_parent_node.add_node();
    auto & block_transform = block_node.add_component<RelativeTransform>(block_parent_transform);
    block_node.add_component<MeshRenderer>(block_transform);
    block_node.add_component<BlockController>(block_transform);
}