#include <pkengine/core/engine.hpp>

using namespace PKEngine;

GLFW::GLFWInstance engine_instance::glfw_instance;
Vulkan::VulkanInstance engine_instance::vulkan_instance;
GLFW::Window engine_instance::window;
Vulkan::Surface<engine_instance::vulkan_instance, engine_instance::window> engine_instance::vulkan_surface;
Vulkan::PhysicalDevice<engine_instance::vulkan_instance, engine_instance::vulkan_surface> engine_instance::physical_device;