#pragma once

#include <cstdint>
#include <algorithm>

#include "../../util/const_string.hpp"

#include "exception_base.hpp"

namespace PKEngine::Exception {
    class InternalException : public ExceptionBase {
    public:
        [[nodiscard]] _exception_type_t exception_type() const noexcept final { return ET_INTERNAL; }
        [[nodiscard]] virtual bool is_glfw_error() const noexcept { return false; }
        [[nodiscard]] virtual bool is_vulkan_error() const noexcept { return false; }
    };

#define PKENGINE_INTERNAL_EXCEPTION(name, message)                                 \
    class name : public InternalException {                                        \
        [[nodiscard]] const char * what() const noexcept final { return message; } \
    }
#define PKENGINE_INTERNAL_GLFW_EXCEPTION(name, message)                                  \
    class name : public InternalException {                                              \
        [[nodiscard]] const char * what() const noexcept final { return message; }       \
        [[nodiscard]] virtual bool is_glfw_error() const noexcept final { return true; } \
    }
#define PKENGINE_INTERNAL_VULKAN_EXCEPTION(name, message)                                  \
    class name : public InternalException {                                                \
        [[nodiscard]] const char * what() const noexcept final { return message; }         \
        [[nodiscard]] virtual bool is_vulkan_error() const noexcept final { return true; } \
    }

    namespace Internal {
        // standard exceptions
        PKENGINE_INTERNAL_EXCEPTION(cant_find_vulkan_capable_gpu, "Unable to locate vulkan capable GPU");

        // glfw exceptions
        PKENGINE_INTERNAL_GLFW_EXCEPTION(glfw_init_error, "Unable to initialize GLFW");
        PKENGINE_INTERNAL_GLFW_EXCEPTION(glfw_vulkan_unsupported, "GLFW instance does not support vulkan");
        PKENGINE_INTERNAL_GLFW_EXCEPTION(glfw_window_init, "Unable to create GLFW window");

        // vulkan exceptions
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_instance_creation, "Unable to create vulkan instance");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_surface_creation, "Unable to create vulkan surface");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_get_surface_support, "Unable to get vulkan surface support");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_logical_device, "Unable to create vulkan logical device");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_swap_chain, "Unable to create vulkan swap chain");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_image_view, "Unable to create vulkan image view");
    }
}