#pragma once

#ifndef TEST
#define TEST

#include <cstdint>
#include <algorithm>
#include <exception>

#include "pkengine/util/const_string.hpp"

#define PKENGINE_INTERNAL_EXCEPTION(_NAME_, _MESSAGE_)                                       \
    struct _NAME_ : public PKEngine::Exception::InternalException {                          \
        [[nodiscard]] virtual const char * what() const noexcept final { return _MESSAGE_; } \
        [[nodiscard]] virtual bool is_glfw_error() const noexcept final { return false; }    \
        [[nodiscard]] virtual bool is_vulkan_error() const noexcept final { return false; }  \
    }

#define PKENGINE_INTERNAL_GLFW_EXCEPTION(_NAME_, _MESSAGE_)                                  \
    struct _NAME_ : public PKEngine::Exception::InternalException {                          \
        [[nodiscard]] virtual const char * what() const noexcept final { return _MESSAGE_; } \
        [[nodiscard]] virtual bool is_glfw_error() const noexcept final { return true; }     \
        [[nodiscard]] virtual bool is_vulkan_error() const noexcept final { return false; }  \
    }

#define PKENGINE_INTERNAL_VULKAN_EXCEPTION(_NAME_, _MESSAGE_)                                \
    struct _NAME_ : public PKEngine::Exception::InternalException {                          \
        [[nodiscard]] virtual const char * what() const noexcept final { return _MESSAGE_; } \
        [[nodiscard]] virtual bool is_glfw_error() const noexcept final { return false; }    \
        [[nodiscard]] virtual bool is_vulkan_error() const noexcept final { return true; }   \
    }

namespace PKEngine::Exception {
    struct InternalException : public std::exception {
        [[nodiscard]] virtual bool is_glfw_error() const noexcept = 0;
        [[nodiscard]] virtual bool is_vulkan_error() const noexcept = 0;
    };

    namespace Internal {
        // standard exceptions
        PKENGINE_INTERNAL_EXCEPTION(cant_open_log_file, "Unable to open log file");
        PKENGINE_INTERNAL_EXCEPTION(cant_find_vulkan_capable_gpu, "Unable to locate vulkan capable GPU");

        PKENGINE_INTERNAL_EXCEPTION(unable_to_get_suitable_memory_type, "Unable to get suitable memory type");

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
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_shader_module, "Unable to create vulkan shader module");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_render_pass, "Unable to create vulkan render pass");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_pipeline_layout, "Unable to create vulkan pipeline layout");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_pipeline, "Unable to create vulkan pipeline");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_vulkan_frame_buffer, "Unable to create vulkan frame buffer");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_vulkan_command_pool, "Unable to create vulkan command pool");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_vulkan_command_buffer, "Unable to create vulkan command buffer");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_record_vulkan_command_buffer, "Unable to enter_main_loop recording vulkan command buffer");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_vulkan_device_buffer, "Unable to create vulkan vertex buffer");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_vulkan_descriptor_set_layout, "Unable to create vulkan descriptor set layout");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_vulkan_descriptor_pool, "Unable to create vulkan descriptor pool");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_vulkan_descriptor_sets, "Unable to create vulkan descriptor sets");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_vulkan_image, "Unable to create vulkan image");

        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_semaphore, "Unable to create vulkan semaphore");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_create_fence, "Unable to create vulkan fence");

        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_allocate_device_buffer, "Unable to allocate device buffer");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_bind_device_buffer, "Unable to bind device buffer");

        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_allocate_image_buffer, "Unable to allocate image buffer");
        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_bind_image_buffer, "Unable to bind image buffer");

        PKENGINE_INTERNAL_VULKAN_EXCEPTION(vulkan_unable_to_submit_draw_buffer, "Unable to submit draw command buffer");
    }
}

#endif