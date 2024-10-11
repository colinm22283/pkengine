#pragma once

#include <thread>

#include <pkengine/glfw/glfw_instance.hpp>
#include <pkengine/glfw/window.hpp>

#include <pkengine/vulkan/vulkan_instance.hpp>
#include <pkengine/vulkan/surface.hpp>
#include <pkengine/vulkan/physical_device.hpp>
#include <pkengine/vulkan/logical_device.hpp>
#include <pkengine/vulkan/device_queue.hpp>

namespace PKEngine {
    class Context {
        friend class Engine;
    protected:
    public:
        GLFW::Window window;

        Vulkan::VulkanInstance vulkan_instance;
        Vulkan::Surface surface = Vulkan::Surface(vulkan_instance, window);
        Vulkan::PhysicalDevice physical_device = Vulkan::PhysicalDevice(vulkan_instance, surface);
        Vulkan::Util::QueueFamilyIndices queue_family_indices = Vulkan::Util::QueueFamilyIndices(surface, physical_device.handle());
        Vulkan::LogicalDevice logical_device = Vulkan::LogicalDevice(physical_device, queue_family_indices);

        Vulkan::Sync::Fence in_flight_fence = Vulkan::Sync::Fence(logical_device, true);

        Vulkan::DeviceQueue device_queue = Vulkan::DeviceQueue(logical_device, in_flight_fence, queue_family_indices.graphics_family.value());

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