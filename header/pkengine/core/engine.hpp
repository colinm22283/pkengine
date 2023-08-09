#pragma once

#include "../glfw/glfw_instance.hpp"
#include "../glfw/window.hpp"

#include "../vulkan/vulkan_instance.hpp"
#include "../vulkan/surface.hpp"
#include "../vulkan/physical_device.hpp"
#include "../vulkan/logical_device.hpp"
#include "../vulkan/vulkan_queue.hpp"
#include "../vulkan/swap_chain.hpp"
#include "../vulkan/image_views.hpp"
#include "../vulkan/vulkan_pipeline.hpp"

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
        static Vulkan::SwapChain<physical_device, logical_device, vulkan_surface, window, queue_family_indices> swap_chain;
        static Vulkan::ImageViews<logical_device, swap_chain> image_views;
        static Vulkan::VulkanPipeline vulkan_pipeline;

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

            swap_chain.init();
            image_views.init();
            vulkan_pipeline.init();
        }

        static inline void free() {
            vulkan_pipeline.free();
            image_views.free();
            swap_chain.free();

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

        static inline void start() {
            while (!glfwWindowShouldClose(window.handle())) {
                glfwPollEvents();
            }
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

            start();

            free();

            logger.success() << "PKEngine exited with no internal errors";
        }
    };
}