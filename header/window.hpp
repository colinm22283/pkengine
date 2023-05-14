#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include <logger/logger.hpp>

#include <types/engine_config.hpp>

#include <util/engine_exception.hpp>

namespace PKEngine {
    class Window {
    protected:
        GLFWwindow * _handle;

        const char * glfw_error = "";

        void error_callback(int, const char * error_string) {
            glfw_error = error_string;
        }

    public:
        inline explicit Window(instance_config_t & config) {
            Log::info() << "Window Created";

            _handle = glfwCreateWindow(
                config.window.width, config.window.height,
                config.window.title,
                nullptr,
                nullptr
            );

            if (_handle == nullptr) throw Exceptions::UnableToCreateGLFWWindow();
        }
        Window(Window &) = delete;
        Window(Window &&) = delete;

        inline ~Window() {
            Log::info() << "Window Destroyed";

            glfwDestroyWindow(_handle);
        }

        inline GLFWwindow * handle() { return _handle; }
        inline operator GLFWwindow *() { return handle(); }

        inline const char * get_error() { return glfw_error; }
    };
}