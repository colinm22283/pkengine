#include "pkengine-internal/engine.hpp"

using namespace PKEngine;

GLFW::GLFWInstance engine_instance::glfw_instance;
Vulkan::VulkanInstance engine_instance::vulkan_instance;
GLFW::Window engine_instance::window;
Vulkan::Surface<engine_instance::vulkan_instance, engine_instance::window> engine_instance::vulkan_surface;
Vulkan::PhysicalDevice<engine_instance::vulkan_instance, engine_instance::vulkan_surface> engine_instance::physical_device;
Vulkan::ConstQueueFamilyIndices<engine_instance::vulkan_surface, engine_instance::physical_device> engine_instance::queue_family_indices;
Vulkan::LogicalDevice<engine_instance::physical_device, engine_instance::queue_family_indices> engine_instance::logical_device;
Vulkan::VulkanQueue<engine_instance::logical_device> engine_instance::graphics_queue, engine_instance::present_queue;
Vulkan::Semaphore<engine_instance::logical_device> engine_instance::image_available_semaphore;
Vulkan::SwapChain<engine_instance::physical_device, engine_instance::logical_device, engine_instance::vulkan_surface, engine_instance::window, engine_instance::queue_family_indices, engine_instance::image_available_semaphore> engine_instance::swap_chain;
Vulkan::ImageViews<engine_instance::logical_device, engine_instance::swap_chain> engine_instance::image_views;

Vulkan::VertexBuffer<engine_instance::logical_device, engine_instance::physical_device> engine_instance::vertex_buffer;
engine_instance::vertex_allocator_t engine_instance::vertex_allocator;
Vulkan::IndexBuffer<engine_instance::logical_device, engine_instance::physical_device> engine_instance::index_buffer;
engine_instance::index_allocator_t engine_instance::index_allocator;

Vulkan::RenderPass<engine_instance::logical_device, engine_instance::swap_chain> engine_instance::render_pass;
Vulkan::Pipeline::VulkanPipeline<engine_instance::logical_device, engine_instance::swap_chain, engine_instance::ShaderSequence, engine_instance::render_pass, engine_instance::vertex_buffer> engine_instance::vulkan_pipeline;

Vulkan::FrameBuffers<engine_instance::logical_device, engine_instance::swap_chain, engine_instance::render_pass, engine_instance::image_views> engine_instance::frame_buffers;
Vulkan::CommandPool<engine_instance::logical_device, engine_instance::queue_family_indices> engine_instance::command_pool;
Vulkan::RenderCommandBuffer<engine_instance::logical_device, engine_instance::command_pool> engine_instance::command_buffer;

Vulkan::Semaphore<engine_instance::logical_device> engine_instance::render_complete_semaphore;
Vulkan::Fence<engine_instance::logical_device, true> engine_instance::in_flight_fence;

engine_instance::model_allocator_t engine_instance::model_allocator;

PKEngine::ObjectTree engine_instance::object_tree;