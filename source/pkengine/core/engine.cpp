#include <pkengine/core/engine.hpp>

using namespace PKEngine;

GLFW::GLFWInstance engine_instance::glfw_instance;
Vulkan::VulkanInstance engine_instance::vulkan_instance;
GLFW::Window engine_instance::window;
Vulkan::Surface<engine_instance::vulkan_instance, engine_instance::window> engine_instance::vulkan_surface;
Vulkan::PhysicalDevice<engine_instance::vulkan_instance, engine_instance::vulkan_surface> engine_instance::physical_device;
Vulkan::ConstQueueFamilyIndices<engine_instance::vulkan_surface, engine_instance::physical_device> engine_instance::queue_family_indices;
Vulkan::LogicalDevice<engine_instance::physical_device, engine_instance::queue_family_indices> engine_instance::logical_device;
Vulkan::VulkanQueue<engine_instance::logical_device> engine_instance::graphics_queue, engine_instance::present_queue;
Vulkan::SwapChain<engine_instance::physical_device, engine_instance::logical_device, engine_instance::vulkan_surface, engine_instance::window, engine_instance::queue_family_indices> engine_instance::swap_chain;
Vulkan::ImageViews<engine_instance::logical_device, engine_instance::swap_chain> engine_instance::image_views;