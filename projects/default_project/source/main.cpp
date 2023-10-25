#include <pkengine/object_tree.hpp>

#include <pkengine/component/mesh_renderer.hpp>
#include <block_controller.hpp>

void init() {
    using namespace PKEngine;

    auto & block_object = object_tree.add_node();
    auto & block_transform = block_object.add_component<Transform>();
    block_object.add_component<MeshRenderer>(block_transform);
    block_object.add_component<BlockController>(block_transform);
}