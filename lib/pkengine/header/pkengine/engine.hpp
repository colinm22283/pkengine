#pragma once

#include <cmath>

#include "pkengine/internal/glfw/glfw_instance.hpp"
#include "pkengine/internal/glfw/window.hpp"

#include "pkengine/internal/vulkan/vulkan_instance.hpp"
#include "pkengine/internal/vulkan/surface.hpp"
#include "pkengine/internal/vulkan/physical_device.hpp"
#include "pkengine/internal/vulkan/logical_device.hpp"
#include "pkengine/internal/vulkan/vulkan_queue.hpp"
#include "pkengine/internal/vulkan/swap_chain.hpp"
#include "pkengine/internal/vulkan/image_views.hpp"
#include "pkengine/internal/vulkan/render_pass.hpp"
#include "pkengine/internal/vulkan/pipeline/vulkan_pipeline.hpp"
#include "pkengine/internal/vulkan/frame_buffer.hpp"
#include "pkengine/internal/vulkan/frame_buffers.hpp"
#include "pkengine/internal/vulkan/command_pool.hpp"
#include "pkengine/internal/vulkan/render_command_buffer.hpp"
#include "pkengine/internal/vulkan/sync/semaphore.hpp"
#include "pkengine/internal/vulkan/sync/fence.hpp"
#include "pkengine/internal/vulkan/buffer/vertex_buffer.hpp"
#include "pkengine/internal/vulkan/buffer/index_buffer.hpp"
#include "pkengine/internal/vulkan/buffer/device_allocator.hpp"
#include "pkengine/internal/vulkan/descriptor_set_layout.hpp"
#include "pkengine/internal/vulkan/descriptor_pool.hpp"
#include "pkengine/internal/vulkan/descriptor_set.hpp"
#include "pkengine/internal/vulkan/buffer/descriptor_buffer.hpp"

#include <pkengine/frame_data.hpp>

#include "pkengine/manifest/shader_manifest.hpp"

#include "pkengine/model/model_allocator.hpp"

#include "project.hpp"
#include "object_tree.hpp"

#include "memory_config.hpp"

#include "object_tree.hpp"
#include "time.hpp"

#include <render_config.hpp>

namespace PKEngine {
    class engine_instance {
    protected:
        static constexpr auto logger = Logger<"Engine Instance">();

        // graphics api init
    protected:
        static GLFW::GLFWInstance glfw_instance;
        static Vulkan::VulkanInstance vulkan_instance;

        // window
        static GLFW::Window glfw_window;

        // vulkan objects
    public:
        static Vulkan::Surface<vulkan_instance, glfw_window> vulkan_surface;
        static Vulkan::PhysicalDevice<vulkan_instance, vulkan_surface> physical_device;
        static Vulkan::ConstQueueFamilyIndices<vulkan_surface, physical_device> queue_family_indices;
        static Vulkan::LogicalDevice<physical_device, queue_family_indices> logical_device;
        static Vulkan::VulkanQueue<logical_device> graphics_queue, present_queue;
//        static std::array<Vulkan::Semaphore<logical_device>, render_config.max_frames_in_flight> image_available_semaphores;
        static Vulkan::SwapChain<physical_device, logical_device, vulkan_surface, glfw_window, queue_family_indices> swap_chain;
        static Vulkan::ImageViews<logical_device, swap_chain> image_views;

        using Shaders = Manifest::ShaderManifest;
        using ShaderSequence = Shaders::DefaultShaderSequence;

        static Vulkan::VertexBuffer<logical_device, physical_device> vertex_buffer;
        using vertex_allocator_t = Vulkan::DeviceAllocator<Vulkan::Vertex, vertex_buffer>;
        static vertex_allocator_t vertex_allocator;
        static Vulkan::IndexBuffer<logical_device, physical_device> index_buffer;
        using index_allocator_t = Vulkan::DeviceAllocator<uint32_t, index_buffer>;
        static index_allocator_t index_allocator;

//        static std::array<Vulkan::DescriptorBuffer<logical_device, physical_device, Vulkan::SceneData>, render_config.max_frames_in_flight> scene_data_buffers;
//        static std::array<Vulkan::DescriptorBuffer<logical_device, physical_device, Vulkan::ModelData>, render_config.max_frames_in_flight> model_data_buffers;

        static Vulkan::DescriptorSetLayout<logical_device> descriptor_set_layout;
        static Vulkan::DescriptorPool<logical_device> descriptor_pool;
//        static std::array<Vulkan::DescriptorSet<logical_device, descriptor_set_layout, descriptor_pool>, render_config.max_frames_in_flight> descriptor_sets;

        static Vulkan::RenderPass<logical_device, swap_chain> render_pass;
        static Vulkan::Pipeline::VulkanPipeline<logical_device, swap_chain, ShaderSequence, render_pass, vertex_buffer, descriptor_set_layout> vulkan_pipeline;

        static Vulkan::FrameBuffers<logical_device, swap_chain, render_pass, image_views> frame_buffers;
        static Vulkan::CommandPool<logical_device, queue_family_indices> command_pool;
//        static std::array<Vulkan::RenderCommandBuffer<logical_device, command_pool>, render_config.max_frames_in_flight> command_buffers;

//        static std::array<Vulkan::Semaphore<logical_device>, render_config.max_frames_in_flight> render_complete_semaphores;
//        static std::array<Vulkan::Fence<logical_device, true>, render_config.max_frames_in_flight> in_flight_fences;

        static std::array<
            FrameData<
                logical_device,
                physical_device,
                descriptor_set_layout,
                descriptor_pool,
                command_pool
            >,
            render_config.max_frames_in_flight
        > frame_data;
        static inline auto & get_current_frame_data() noexcept { return frame_data[current_frame]; }

        using model_allocator_t = ModelAllocator<vertex_allocator, index_allocator, vertex_allocator_t::Allocation, index_allocator_t::Allocation>;
        static model_allocator_t model_allocator;

        static ObjectTree object_tree;

        static unsigned int current_frame;

    protected:
        static void init();
        static void free();
        static void enter_main_loop();

        static void draw_frame() {
            auto & frame = get_current_frame_data();
            frame.in_flight_fence.wait();

            uint32_t image_index = swap_chain.get_next_image_index(frame.image_available_semaphore);

            frame.command_buffer.reset();
            frame.command_buffer.record<
                render_pass,
                swap_chain,
                frame_buffers,
                vulkan_pipeline,
                vertex_buffer,
                index_buffer,
                object_tree
            >(frame.descriptor_set, image_index);

            graphics_queue.submit(
                frame.image_available_semaphore,
                frame.render_complete_semaphore,
                frame.in_flight_fence,
                frame.command_buffer
            );

            swap_chain.present<present_queue>(image_index, frame.render_complete_semaphore);

            current_frame = (current_frame + 1) % render_config.max_frames_in_flight;
        }

        static void recreate_swap_chain();

    public:
        static bool run() noexcept;

        struct window {
            static inline void resize(int width, int height) {
                glfw_window.set_dimensions(width, height);

                recreate_swap_chain();

                // TODO: complete this, will probably have to like refresh every vulkan object :(
            }
        };
    };
}