#pragma once

#include <thread>

#include <pkengine/glfw/glfw_instance.hpp>
#include <pkengine/glfw/window.hpp>

#include <pkengine/vulkan/vulkan_instance.hpp>
#include <pkengine/vulkan/surface.hpp>
#include <pkengine/vulkan/physical_device.hpp>

namespace PKEngine {
    class Context {
        friend class Engine;
    protected:
        GLFW::Window window;

        Vulkan::VulkanInstance vulkan_instance;
        Vulkan::Surface surface = Vulkan::Surface(vulkan_instance, window);
        Vulkan::PhysicalDevice physical_device = Vulkan::PhysicalDevice(vulkan_instance, surface);

    public:
        explicit inline Context() = default;

        inline Context(const Context &) = delete;
        inline Context(Context &&) noexcept = default;

        inline void update() {

        }

        inline void exit() noexcept {
            window.close();
        }
    };
}