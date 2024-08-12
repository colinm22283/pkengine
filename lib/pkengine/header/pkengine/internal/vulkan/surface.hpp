#pragma once

#include <vulkan/vulkan.h>

#include "pkengine/logger/logger.hpp"

#include "pkengine/internal/glfw/window.hpp"

#include "vulkan_instance.hpp"

namespace PKEngine::Vulkan {
    template<const Vulkan::VulkanInstance & inst, const GLFW::Window & window>
    class Surface {
    protected:
        static constexpr auto logger = Logger<"Vulkan Surface">();

        VkSurfaceKHR surface = VK_NULL_HANDLE;

    public:
        inline void init() {
            logger << "Creating vulkan surface...";

            if (glfwCreateWindowSurface(inst.handle(), window.handle(), nullptr, &surface) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_surface_creation();
            }

            logger.success() << "Vulkan surface created";
        }
        inline void free() {
            if (surface != VK_NULL_HANDLE) {
                logger << "Destroying vulkan surface...";

                vkDestroySurfaceKHR(inst.handle(), surface, nullptr);

                logger.success() << "Vulkan surface destroyed";
            }
        }

        [[nodiscard]] inline VkSurfaceKHR handle() const noexcept { return surface; }
    };
}