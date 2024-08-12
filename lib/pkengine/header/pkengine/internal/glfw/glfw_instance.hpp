#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "pkengine/exception/internal.hpp"

#include "pkengine/logger/logger.hpp"

#include "glfw.hpp"

namespace PKEngine::GLFW {
    class GLFWInstance {
    protected:
        static constexpr auto logger = Logger<"GLFW Instance">();

        bool init_success = false;

    public:
        inline void init() {
            logger << "Initializing GLFW instance...";

            glfwSetErrorCallback(error_callback);

            if (glfwInit() == GLFW_FALSE) throw Exception::Internal::glfw_init_error();

            init_success = true;

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            if (glfwVulkanSupported() == GLFW_FALSE) throw Exception::Internal::glfw_vulkan_unsupported();

            logger.success() << "GLFW instance initialized";
        }
        inline void free() const {
            if (init_success) {
                logger << "Terminating GLFW...";

                glfwTerminate();

                logger.success() << "GLFW terminated";
            }
        }
    };
}