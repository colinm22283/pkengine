#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <pkengine/glfw/glfw_exception.hpp>
#include <pkengine/glfw/glfw.hpp>

#include <pkengine/logger/logger.hpp>

namespace PKEngine::GLFW {
    class GLFWInstance {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_GLFW_EXCEPTION(InitFailed, "Unable to initialize GLFW");
            PKENGINE_DEFINE_GLFW_EXCEPTION(VulkanUnsupported, "Vulkan is not supported in your system GLFW");
        };

    protected:
        static constexpr auto logger = Logger<"GLFW Instance">();

        bool init_success = false;

    public:
        inline GLFWInstance() {
            logger.debug() << "Initializing GLFW instance...";

            glfwSetErrorCallback(error_callback);

            if (glfwInit() == GLFW_FALSE) throw Exceptions::InitFailed();

            init_success = true;

            if (glfwVulkanSupported() == GLFW_FALSE) throw Exceptions::VulkanUnsupported();

            logger.debug() << "GLFW instance initialized";
        }
        inline ~GLFWInstance() {
            if (init_success) {
                logger.debug() << "Terminating GLFW...";

                glfwTerminate();

                logger.debug() << "GLFW terminated";
            }
        }

        inline void poll_events() noexcept {
            glfwPollEvents();
        }
    };
}