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
        static inline void init() {
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

        static inline void free() {
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

        static inline void test(GLFWwindow* window, int key, int scancode, int action, int mods) {
            logger << "TEST: " << key << "\n";

            if (action == GLFW_PRESS) object_tree.key_down();
        }

        static inline void enter_main_loop() {
            glfwSetKeyCallback(glfw_window.handle(), test);

            while (!glfwWindowShouldClose(glfw_window.handle())) {
                glfwPollEvents();

                Time::update();

                object_tree.update();

                draw_frame();

                vkDeviceWaitIdle(logical_device.handle());
            }
        }

        static inline void draw_frame() {
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

        static inline void recreate_swap_chain() {
            vkDeviceWaitIdle(logical_device.handle());

            frame_buffers.free();
            image_views.free();
            swap_chain.free();

            swap_chain.init();
            image_views.init();
            frame_buffers.init();
        }

    public:
        static inline bool run() noexcept {
            logger << "Starting PKEngine...";

            static auto do_cleanup = []() {
                try {
                    free();
                }
                catch (const std::exception & ex) {
                    logger.error() << "Exception occurred during PKEngine cleanup";
                    logger.error() << "what(): " << ex.what();

                    return false;
                }

                return true;
            };

            try {
                Time::start();

                init();
            }
            catch (const Exception::InternalException & ex) {
                logger.error() << "Exception occurred during PKEngine initialization";
                logger.error() << "\tWhat: " << ex.what();
                if (ex.is_glfw_error()) {
                    logger.error() << "\tGLFW Error String: " << GLFW::glfw_error_string;
                }

                do_cleanup();

                return false;
            }

            try {
                ::init();
            }
            catch (const std::exception & ex) {
                logger.error() << "Exception occurred during user code initialization";
                logger.error() << "What(): " << ex.what();

                do_cleanup();

                return false;
            }


            try {
                object_tree.start();
            }
            catch (const std::exception & ex) {
                logger.error() << "Exception occurred during user code start";
                logger.error() << "What(): " << ex.what();

                do_cleanup();

                return false;
            }

            try {
                enter_main_loop();
            }
            catch (const std::exception & ex) {
                logger.error() << "Exception occurred during user code update";
                logger.error() << "what(): " << ex.what();

                do_cleanup();

                return false;
            }

            if (!do_cleanup()) return false;

            logger.success() << "PKEngine exited with no internal errors";

            return true;
        }

        struct window {
            static inline void resize(int width, int height) {
                glfw_window.set_dimensions(width, height);

                recreate_swap_chain();

                // TODO: complete this, will probably have to like refresh every vulkan object :(
            }
        };
    };
}