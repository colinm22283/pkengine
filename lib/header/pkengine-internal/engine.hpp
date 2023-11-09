#pragma once

#include <cmath>

#include "internal/glfw/glfw_instance.hpp"
#include "internal/glfw/window.hpp"

#include "internal/vulkan/vulkan_instance.hpp"
#include "internal/vulkan/surface.hpp"
#include "internal/vulkan/physical_device.hpp"
#include "internal/vulkan/logical_device.hpp"
#include "internal/vulkan/vulkan_queue.hpp"
#include "internal/vulkan/swap_chain.hpp"
#include "internal/vulkan/image_views.hpp"
#include "internal/vulkan/render_pass.hpp"
#include "internal/vulkan/pipeline/vulkan_pipeline.hpp"
#include "internal/vulkan/frame_buffer.hpp"
#include "internal/vulkan/frame_buffers.hpp"
#include "internal/vulkan/command_pool.hpp"
#include "internal/vulkan/render_command_buffer.hpp"
#include "internal/vulkan/sync/semaphore.hpp"
#include "internal/vulkan/sync/fence.hpp"
#include "internal/vulkan/shader/vertex_buffer.hpp"
#include "internal/vulkan/shader/index_buffer.hpp"
#include "internal/vulkan/shader/device_allocator.hpp"

#include "manifest/shader_manifest.hpp"

#include "model/model_allocator.hpp"

#include "project.hpp"
#include "object_tree.hpp"

#include <memory_config.hpp>

#include "object_tree.hpp"
#include <pkengine/time.hpp>

namespace PKEngine {
    class engine_instance {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::CyanFg, "Engine Instance">();

        // graphics api init
    protected:
        static GLFW::GLFWInstance glfw_instance;
        static Vulkan::VulkanInstance vulkan_instance;

        // window
    public:
        static GLFW::Window window;

        // vulkan objects
    public:
        static Vulkan::Surface<vulkan_instance, window> vulkan_surface;
        static Vulkan::PhysicalDevice<vulkan_instance, vulkan_surface> physical_device;
        static Vulkan::ConstQueueFamilyIndices<vulkan_surface, physical_device> queue_family_indices;
        static Vulkan::LogicalDevice<physical_device, queue_family_indices> logical_device;
        static Vulkan::VulkanQueue<logical_device> graphics_queue, present_queue;
        static Vulkan::Semaphore<logical_device> image_available_semaphore;
        static Vulkan::SwapChain<physical_device, logical_device, vulkan_surface, window, queue_family_indices, image_available_semaphore> swap_chain;
        static Vulkan::ImageViews<logical_device, swap_chain> image_views;

        using Shaders = Manifest::ShaderManifest;
        using ShaderSequence = Shaders::DefaultShaderSequence;

        static Vulkan::VertexBuffer<logical_device, physical_device> vertex_buffer;
        using vertex_allocator_t = Vulkan::DeviceAllocator<Vulkan::Vertex, vertex_buffer>;
        static vertex_allocator_t vertex_allocator;
        static Vulkan::IndexBuffer<logical_device, physical_device> index_buffer;
        using index_allocator_t = Vulkan::DeviceAllocator<uint32_t, index_buffer>;
        static index_allocator_t index_allocator;

        static Vulkan::RenderPass<logical_device, swap_chain> render_pass;
        static Vulkan::Pipeline::VulkanPipeline<logical_device, swap_chain, ShaderSequence, render_pass, vertex_buffer> vulkan_pipeline;

        static Vulkan::FrameBuffers<logical_device, swap_chain, render_pass, image_views> frame_buffers;
        static Vulkan::CommandPool<logical_device, queue_family_indices> command_pool;
        static Vulkan::RenderCommandBuffer<logical_device, command_pool> command_buffer;

        static Vulkan::Semaphore<logical_device> render_complete_semaphore;
        static Vulkan::Fence<logical_device, true> in_flight_fence;

        using model_allocator_t = ModelAllocator<vertex_allocator, index_allocator, vertex_allocator_t::Allocation, index_allocator_t::Allocation>;
        static model_allocator_t model_allocator;

        static ObjectTree object_tree;

    protected:
        static inline void init() {
            logger_file_stream.init();

            glfw_instance.init();
            vulkan_instance.init();

            window.init();

            vulkan_surface.init();
            physical_device.init();
            queue_family_indices.init();
            logical_device.init();

            graphics_queue.init(queue_family_indices.graphics_family.value());
            present_queue.init(queue_family_indices.present_family.value());

            image_available_semaphore.init();
            swap_chain.init();
            image_views.init();

            Shaders::init<logical_device>();

            vertex_buffer.init(memory_config.vertex_buffer_allocation);
            vertex_allocator.init();
            index_buffer.init(memory_config.index_buffer_allocation);
            index_allocator.init();

            render_pass.init();
            vulkan_pipeline.init();

            frame_buffers.init();
            command_pool.init();
            command_buffer.init();

            render_complete_semaphore.init();
            in_flight_fence.init();

            model_allocator.init();

            object_tree.init();
        }

        static inline void free() {
            object_tree.free();

            model_allocator.free();

            in_flight_fence.free();
            render_complete_semaphore.free();

            command_buffer.free();
            command_pool.free();
            frame_buffers.free();

            vulkan_pipeline.free();
            render_pass.free();

            index_allocator.free();
            index_buffer.free();
            vertex_allocator.free();
            vertex_buffer.free();

            Shaders::free<logical_device>();

            image_views.free();
            swap_chain.free();
            image_available_semaphore.free();

            present_queue.free();
            graphics_queue.free();

            logical_device.free();
            queue_family_indices.free();
            physical_device.free();
            vulkan_surface.free();

            window.free();

            vulkan_instance.free();
            glfw_instance.free();

            logger_file_stream.free();
        }

        static inline void enter_main_loop() noexcept {
            try {
                while (!glfwWindowShouldClose(window.handle())) {
                    glfwPollEvents();

                    Time::update();

                    object_tree.update();

                    draw_frame();

                    vkDeviceWaitIdle(logical_device.handle());
                }
            }
            catch (std::exception & ex) {
                logger.error() << "Exception occurred during user code update";
                logger.error() << "what(): " << ex.what();
            }
        }

        static inline void draw_frame() {
            in_flight_fence.wait();

            uint32_t image_index = swap_chain.get_next_image_index();

            command_buffer.reset();
            command_buffer.record<
                render_pass,
                swap_chain,
                frame_buffers,
                vulkan_pipeline,
                vertex_buffer,
                index_buffer,
                object_tree
            >(image_index);

            graphics_queue.submit<
                image_available_semaphore,
                render_complete_semaphore,
                in_flight_fence,
                command_buffer
            >();

            swap_chain.present<render_complete_semaphore, present_queue>(image_index);
        }

    public:
        static inline void run() {
            logger << "Starting PKEngine...";

            try {
                init();

                Time::start();
            }
            catch (const Exception::InternalException & ex) {
                logger.error() << "Exception occurred during PKEngine initialization:";
                logger.error() << "\tWhat: " << ex.what();
                if (ex.is_glfw_error()) {
                    logger.error() << "\tGLFW Error String: " << GLFW::glfw_error_string;
                }

                free();

                return;
            }

            try {
                ::init();

                object_tree.start();

                enter_main_loop();
            }
            catch (std::exception & ex) {
                logger.error() << "Exception occurred during user code initialization";
                logger.error() << "What(): " << ex.what();

                free();

                return;
            }

            free();

            logger.success() << "PKEngine exited with no internal errors";
        }
    };
}