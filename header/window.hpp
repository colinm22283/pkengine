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
        static constexpr auto logger = FileLogger<Util::ANSI::CyanFg, "WINDOW", "pkengine.log">();

        GLFWwindow * _handle;

    public:
        inline explicit Window(instance_config_t & config) {
            logger.log<"Creating window...">();

            _handle = glfwCreateWindow(
                config.window.width, config.window.height,
                config.window.title,
                nullptr,
                nullptr
            );

            if (_handle == nullptr) throw Exceptions::UnableToCreateGLFWWindow();

            logger.success().log<"Done!">();
        }
        Window(Window &) = delete;
        Window(Window &&) = delete;

        inline ~Window() {
            glfwDestroyWindow(_handle);

            logger.log<"Window Destroyed">();
        }

        inline GLFWwindow * handle() { return _handle; }
        inline operator GLFWwindow *() { return handle(); }
    };
}