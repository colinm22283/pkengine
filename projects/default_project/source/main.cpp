#include <pkengine/engine.hpp>

#include <pkengine/component/mesh_renderer.hpp>
#include <pkengine/component/relative_transform.hpp>
#include <pkengine/component/animation_controller.hpp>
#include <pkengine/component/camera.hpp>
#include <pkengine/component/fps_logger.hpp>

#include <block_controller.hpp>

#include <pkengine/image/image_reader.hpp>

void init() {
    using namespace PKEngine;

    auto test_image = ImageReader::read_file("test.png");

    auto & block_node = engine_instance::object_tree.add_node();
    auto & block_transform = block_node.add_component<Transform>();
    block_transform.position.x = 2;
    block_transform.position.y = 2;
    block_node.add_component<MeshRenderer>(block_transform);
    block_node.add_component<AnimationController>(block_transform, std::vector<Vector3> {
        Vector3(-0.8, 0.8, 0),
        Vector3(0.8, 0.8, 0),
        Vector3(-0.8, -0.2, 0),
        Vector3(0.8, -0.8, 0),
        Vector3(-0.8, -0.8, 0)
    });

    *(char *) 0 = 10;

//    auto & block_node_2 = block_node.add_node();
//    auto & block_transform_2 = block_node_2.add_component<RelativeTransform>(block_transform);
//    block_node_2.add_component<MeshRenderer>(block_transform_2);
//    block_node_2.add_component<BlockController>(block_transform_2);

    auto & player = engine_instance::object_tree.add_node();
    auto & player_transform = player.add_component<Transform>();
    player.add_component<Camera>(player_transform);
    player_transform.position.z = -2;
    player_transform.position.x = 0;

    auto & fps_logger_node = engine_instance::object_tree.add_node();
    fps_logger_node.add_component<FPSLogger>(std::chrono::seconds(1));
}
