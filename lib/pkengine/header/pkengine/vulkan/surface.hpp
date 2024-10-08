#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/glfw/window.hpp>

#include <pkengine/vulkan/util/vulkan_exception.hpp>
#include <pkengine/vulkan/vulkan_instance.hpp>

namespace PKEngine::Vulkan {
    class Surface {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize vulkan surface");
        };

    protected:
        static constexpr auto logger = Logger<"Vulkan Surface">();

        VulkanInstance & vulkan_instance;

        VkSurfaceKHR surface = VK_NULL_HANDLE;

    public:
        inline Surface(VulkanInstance & _vulkan_instance, GLFW::Window & window):
            vulkan_instance(_vulkan_instance) {
            logger.debug() << "Initializing vulkan surface...";

            Util::throw_on_fail<Exceptions::InitError>(
                glfwCreateWindowSurface(vulkan_instance.handle(), window.handle(), nullptr, &surface)
            );

            logger.success() << "Vulkan surface initialized";
        }
        inline ~Surface() {
            if (surface != VK_NULL_HANDLE) {
                logger.debug() << "Destroying vulkan surface...";

                vkDestroySurfaceKHR(vulkan_instance.handle(), surface, nullptr);

                logger.success() << "Vulkan surface destroyed";
            }
        }

        inline Surface(const Surface &) = delete;
        inline Surface(Surface && other) noexcept:
            vulkan_instance(other.vulkan_instance),
            surface(other.surface) {
            other.surface = VK_NULL_HANDLE;
        }

        [[nodiscard]] constexpr const VkSurfaceKHR & handle() noexcept { return surface; }
    };
}