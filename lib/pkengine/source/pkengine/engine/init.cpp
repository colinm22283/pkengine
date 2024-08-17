#include "pkengine/engine.hpp"

void PKEngine::engine_instance::init() {
    current_frame = 0;

    glfw_instance.init();
    vulkan_instance.init();

    glfw_window.init();

    vulkan_surface.init();
    physical_device.init();
    queue_family_indices.init();
    logical_device.init();

    graphics_queue.init(queue_family_indices.graphics_family.value());
    present_queue.init(queue_family_indices.present_family.value());

    swap_chain.init();
    image_views.init();

    Shaders::init<logical_device>();

    vertex_buffer.init(memory_config.vertex_buffer_allocation);
    vertex_allocator.init();
    index_buffer.init(memory_config.index_buffer_allocation);
    index_allocator.init();

    descriptor_set_layout.init();

    std::array<VkDescriptorPoolSize, 2> pool_sizes {
        VkDescriptorPoolSize {
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = static_cast<uint32_t>(render_config.max_frames_in_flight),
        },
        VkDescriptorPoolSize {
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = static_cast<uint32_t>(render_config.max_frames_in_flight),
        },
    };
    descriptor_pool.init(pool_sizes);

    render_pass.init();
    vulkan_pipeline.init();

    frame_buffers.init();
    command_pool.init();

    for (auto & frame : frame_data) frame.init();

    model_allocator.init();

    object_tree.init();
}