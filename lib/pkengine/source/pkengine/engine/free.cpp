#include "pkengine/engine.hpp"

void PKEngine::engine_instance::free() {
    object_tree.free();

    model_allocator.free();

    for (auto & frame : frame_data) frame.free();

    command_pool.free();
    frame_buffers.free();

    vulkan_pipeline.free();
    render_pass.free();

    descriptor_pool.free();
    descriptor_set_layout.free();

    index_allocator.free();
    index_buffer.free();
    vertex_allocator.free();
    vertex_buffer.free();

    Shaders::free<logical_device>();

    image_views.free();
    swap_chain.free();

    present_queue.free();
    graphics_queue.free();

    logical_device.free();
    queue_family_indices.free();
    physical_device.free();
    vulkan_surface.free();

    glfw_window.free();

    vulkan_instance.free();
    glfw_instance.free();
}