#pragma once

#include <iostream>

#include <logger/logger.hpp>

#include <types/engine_config.hpp>

#include <util/engine_exception.hpp>

namespace PKEngine {
    extern const char * glfw_error_string;

    class GLFWInstance {
    protected:
        static constexpr auto logger = FileLogger<Util::ANSI::CyanFg, "GLFW INST", "pkengine.log">();

        static void error_callback(int, const char * str) { PKEngine::glfw_error_string = str; }

    public:
        inline explicit GLFWInstance(engine_config_t & config) {
            logger.log<"Creating GLFW instance...">();

            glfwSetErrorCallback(error_callback);

            if (glfwInit() == GLFW_FALSE) throw Exceptions::GLFWInitError();

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            if (glfwVulkanSupported() == GLFW_FALSE) throw Exceptions::VulkanNotSupported();

            logger.success().log<"Done!">();
        }
        inline ~GLFWInstance() {
            glfwTerminate();

            logger.log<"GLFW Instance Destroyed">();
        }
    };
}