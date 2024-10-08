#pragma once

#include <exception>

#include <pkengine/util/exception.hpp>

#include <pkengine/glfw/glfw.hpp>

namespace PKEngine::GLFW {
    class GLFWException : public PKEngine::Util::Exception {
    public:
        [[nodiscard]] inline const char * what() const noexcept override = 0;
        [[nodiscard]] inline const char * glfw_error() const noexcept { return glfw_error_string; }
    };
}

#define PKENGINE_DEFINE_GLFW_EXCEPTION(name, message) \
    class name : public PKEngine::GLFW::GLFWException { \
    public: \
        [[nodiscard]] inline const char * what() const noexcept override { return message; } \
    }
