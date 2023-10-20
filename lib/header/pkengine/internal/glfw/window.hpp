#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "pkengine/exception/internal.hpp"

#include "pkengine/logger/logger.hpp"

#include "engine_config.hpp"

namespace PKEngine::GLFW {
    class Window {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Window">();

        GLFWwindow * window;

        int _width, _height;
        const char * _title;

    public:
        inline Window() noexcept:
            window(nullptr),
            _width(engine_config.window.width),
            _height(engine_config.window.height),
            _title(engine_config.window.title) { }

        inline void init() {
            logger << "Initializing window...";

            window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);

            if (window == nullptr) throw Exception::Internal::glfw_window_init();

            logger.success() << "Window initialized";
        }
        inline void free() noexcept {
            if (window != nullptr) {
                logger << "Destroying window...";

                glfwDestroyWindow(window);

                logger.success() << "Window destroyed";
            }
        }

        Window(Window &)  = delete;
        Window(Window &&) = delete;

        [[nodiscard]] inline GLFWwindow * handle() const noexcept { return window; }
        [[nodiscard]] inline int width() const noexcept { return _width; }
        [[nodiscard]] inline int height() const noexcept { return _height; }
        [[nodiscard]] inline const char * title() const noexcept { return _title; }

        inline void set_title(const char * value) noexcept { _title = value; glfwSetWindowTitle(window, _title); }
        inline void set_dimensions(int __width, int __height) noexcept {
            _width = __width;
            _height = __height;
            glfwSetWindowSize(window, _width, _height);
        }
    };
}