#pragma once

#include <cmath>

#include "../glfw/glfw_instance.hpp"
#include "../glfw/window.hpp"

#include "../vulkan/vulkan_instance.hpp"
#include "../vulkan/surface.hpp"
#include "../vulkan/physical_device.hpp"
#include "../vulkan/logical_device.hpp"
#include "../vulkan/vulkan_queue.hpp"
#include "../vulkan/swap_chain.hpp"
#include "../vulkan/image_views.hpp"
#include "../vulkan/render_pass.hpp"
#include "../vulkan/pipeline/vulkan_pipeline.hpp"
#include "../vulkan/frame_buffer.hpp"
#include "../vulkan/frame_buffers.hpp"
#include "../vulkan/command_pool.hpp"
#include "../vulkan/command_buffer.hpp"
#include "../vulkan/sync/semaphore.hpp"
#include "../vulkan/sync/fence.hpp"
#include "../vulkan/shader/vertex_buffer.hpp"
#include "../vulkan/shader/index_buffer.hpp"

#include "../manifest/shader_manifest.hpp"

#include <memory_config.hpp>

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
    protected:
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

        static Vulkan::RenderPass<logical_device, swap_chain> render_pass;
        static Vulkan::Pipeline::VulkanPipeline<logical_device, swap_chain, ShaderSequence, render_pass, vertex_buffer> vulkan_pipeline;

        static Vulkan::FrameBuffers<logical_device, swap_chain, render_pass, image_views> frame_buffers;
        static Vulkan::CommandPool<logical_device, queue_family_indices> command_pool;
        static Vulkan::CommandBuffer<logical_device, command_pool> command_buffer;

        static Vulkan::Semaphore<logical_device> render_complete_semaphore;
        static Vulkan::Fence<logical_device, true> in_flight_fence;

    protected:
        static inline void init() {
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

            vertex_buffer.init(memory_config.vertex_buffer_initial_allocation);

            render_pass.init();
            vulkan_pipeline.init();

            frame_buffers.init();
            command_pool.init();
            command_buffer.init();

            render_complete_semaphore.init();
            in_flight_fence.init();
        }

        static inline void free() {
            in_flight_fence.free();
            render_complete_semaphore.free();

            command_buffer.free();
            command_pool.free();
            frame_buffers.free();

            vulkan_pipeline.free();
            render_pass.free();

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
        }

        static inline void enter_main_loop() {
            float iter = 0;

            while (!glfwWindowShouldClose(window.handle())) {
                glfwPollEvents();

                draw_frame();

                iter += 0.001;

                if (iter > 50) {
                    vertex_buffer.load_buffer({
                                                  Vulkan::Vertex(0.9 * std::cos(iter), 0.9 * std::sin(iter), 0.2),
                                                  Vulkan::Vertex(-0.2 * std::sin(iter), 0.2 * std::cos(iter), 0.7),
                                                  Vulkan::Vertex(-0.2 * std::cos(iter), -0.2 * std::sin(iter), 1),
                                                  Vulkan::Vertex(-0.2 * std::cos(iter), -0.2 * std::sin(iter), 1),
                                                  Vulkan::Vertex(0.2 * std::sin(iter), -0.2 * std::cos(iter), 0.7),
                                                  Vulkan::Vertex(0.9 * std::cos(iter), 0.9 * std::sin(iter), 0.2),
                                                  Vulkan::Vertex(-1, -1, 1),
                                                  Vulkan::Vertex(0, -1, 0.2),
                                                  Vulkan::Vertex(-1, 0, 0.2),
                                              });
                }
                else {
                    vertex_buffer.load_buffer({
                                                  Vulkan::Vertex(0.9 * std::cos(iter), 0.9 * std::sin(iter), 0.2),
                                                  Vulkan::Vertex(-0.2 * std::sin(iter), 0.2 * std::cos(iter), 0.7),
                                                  Vulkan::Vertex(-0.2 * std::cos(iter), -0.2 * std::sin(iter), 1),
                                                  Vulkan::Vertex(-0.2 * std::cos(iter), -0.2 * std::sin(iter), 1),
                                                  Vulkan::Vertex(0.2 * std::sin(iter), -0.2 * std::cos(iter), 0.7),
                                                  Vulkan::Vertex(0.9 * std::cos(iter), 0.9 * std::sin(iter), 0.2),
                                              });
                }


            }

            vkDeviceWaitIdle(logical_device.handle());
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
                vertex_buffer
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
            }
            catch (const Exception::ExceptionBase & ex) {
                logger.error() << "Exception occurred during PKEngine initialization:";
                logger.error() << "\tWhat: " << ex.what();
                if (ex.exception_type() == Exception::ET_INTERNAL) {
                    auto & ie = (Exception::InternalException &) ex;
                    if (ie.is_glfw_error()) {
                        logger.error() << "\tGLFW Error String: " << GLFW::glfw_error_string;
                    }
                }

                free();

                return;
            }

            enter_main_loop();

            free();

            logger.success() << "PKEngine exited with no internal errors";
        }
    };
}