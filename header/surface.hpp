#pragma once

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <util/engine_exception.hpp>

#include <logger/logger.hpp>

namespace PKEngine {
    class Surface {
    protected:
        VkSurfaceKHR _handle = VK_NULL_HANDLE;
        VkInstance_T * const _instance;

    public:
        inline Surface(VkInstance instance, GLFWwindow * window):
          _instance(instance) {
            Log::info() << "Surface Created";

            if (glfwCreateWindowSurface(_instance, window, nullptr, &_handle) != VK_SUCCESS) {
                throw Exceptions::UnableToCreateWindowSurface();
            }
        }
        inline ~Surface() {
            Log::info() << "Surface Destroyed";

            if (_handle != VK_NULL_HANDLE) vkDestroySurfaceKHR(_instance, _handle, nullptr);
        }

        inline Surface(Surface && other) noexcept:
          _handle(other._handle),
          _instance(other._instance) {
            other._handle = VK_NULL_HANDLE;
        }
        inline Surface(Surface &) = delete;

        [[nodiscard]] inline VkSurfaceKHR handle() const { return _handle; }
        [[nodiscard]] inline operator VkSurfaceKHR() const { return handle(); }
    };
}