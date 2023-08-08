#pragma once

#include "../glfw/glfw_instance.hpp"
#include "../glfw/window.hpp"

#include "../vulkan/vulkan_instance.hpp"
#include "../vulkan/surface.hpp"
#include "../vulkan/physical_device.hpp"

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

        static inline void init() {
            glfw_instance.init();
            engine_instance::vulkan_instance.init();

            window.init();

            vulkan_surface.init();
            physical_device.init();
        }

        static inline void free() {
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