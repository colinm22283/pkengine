#pragma once

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <util/engine_exception.hpp>

#include <logger/logger.hpp>

namespace PKEngine {
    class Surface {
    protected:
        static constexpr auto logger = FileLogger<Util::ANSI::CyanFg, "SURFACE", "pkengine.log">();

        VkSurfaceKHR _handle = VK_NULL_HANDLE;
        VkInstance_T * const _instance;

    public:
        inline Surface(VkInstance instance, GLFWwindow * window):
          _instance(instance) {
            logger.log<"Creating surface...">();

            if (glfwCreateWindowSurface(_instance, window, nullptr, &_handle) != VK_SUCCESS) {
                throw Exceptions::UnableToCreateWindowSurface();
            }

            logger.success().log<"Done!">();
        }
        inline ~Surface() {
            if (_handle != VK_NULL_HANDLE) vkDestroySurfaceKHR(_instance, _handle, nullptr);

            logger.log<"Surface Destroyed">();
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