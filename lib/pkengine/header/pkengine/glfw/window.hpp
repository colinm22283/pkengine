#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/glfw/glfw_exception.hpp>

#include <engine_config.hpp>

namespace PKEngine::GLFW {
    class Window {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_GLFW_EXCEPTION(InitFailed, "Unable to initialize GLFW window");
        };

    protected:
        static constexpr auto logger = Logger<"GLFW Window">();

        GLFWwindow * window;

        int _width, _height;
        std::string _title;

    public:
        inline Window():
            window(nullptr),
            _width(Config::engine_config.window.width),
            _height(Config::engine_config.window.height),
            _title(Config::engine_config.window.title) {
            logger.debug() << "Initializing window...";

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);

            if (window == nullptr) throw Exceptions::InitFailed();

            logger.debug() << "Window initialized";
        }
        inline ~Window() {
            if (window != nullptr) {
                logger.debug() << "Destroying window...";

                glfwDestroyWindow(window);

                logger.debug() << "Window destroyed";
            }
        }

        inline Window(const Window &) = delete;
        inline Window(Window && w) noexcept:
            window(w.window),
            _width(w._width), _height(w._height),
            _title(std::move(w._title)) {
            w.window = nullptr;
        }

        [[nodiscard]] constexpr GLFWwindow * const & handle() const noexcept { return window; }
        [[nodiscard]] constexpr const int & width() const noexcept { return _width; }
        [[nodiscard]] constexpr const int & height() const noexcept { return _height; }
        [[nodiscard]] constexpr const std::string & title() const noexcept { return _title; }

        inline void set_title(std::string_view value) noexcept {
            if (window != nullptr) {
                _title = value;
                glfwSetWindowTitle(window, _title.c_str());
            }
        }
        inline void resize(int __width, int __height) noexcept {
            if (window != nullptr) {
                _width = __width;
                _height = __height;
                glfwSetWindowSize(window, _width, _height);
            }
        }

        inline bool should_close() const noexcept {
            return glfwWindowShouldClose(window);
        }
        inline void close() noexcept {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    };
}