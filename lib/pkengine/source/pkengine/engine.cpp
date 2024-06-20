#include "pkengine/engine.hpp"

using namespace PKEngine;

GLFW::GLFWInstance engine_instance::glfw_instance;
Vulkan::VulkanInstance engine_instance::vulkan_instance;
GLFW::Window engine_instance::glfw_window;
Vulkan::Surface<engine_instance::vulkan_instance, engine_instance::glfw_window> engine_instance::vulkan_surface;
Vulkan::PhysicalDevice<engine_instance::vulkan_instance, engine_instance::vulkan_surface> engine_instance::physical_device;
Vulkan::ConstQueueFamilyIndices<engine_instance::vulkan_surface, engine_instance::physical_device> engine_instance::queue_family_indices;
Vulkan::LogicalDevice<engine_instance::physical_device, engine_instance::queue_family_indices> engine_instance::logical_device;
Vulkan::VulkanQueue<engine_instance::logical_device> engine_instance::graphics_queue, engine_instance::present_queue;
std::array<Vulkan::Semaphore<engine_instance::logical_device>, render_config.max_frames_in_flight> engine_instance::image_available_semaphores;
Vulkan::SwapChain<engine_instance::physical_device, engine_instance::logical_device, engine_instance::vulkan_surface, engine_instance::glfw_window, engine_instance::queue_family_indices> engine_instance::swap_chain;
Vulkan::ImageViews<engine_instance::logical_device, engine_instance::swap_chain> engine_instance::image_views;

Vulkan::VertexBuffer<engine_instance::logical_device, engine_instance::physical_device> engine_instance::vertex_buffer;
engine_instance::vertex_allocator_t engine_instance::vertex_allocator;
Vulkan::IndexBuffer<engine_instance::logical_device, engine_instance::physical_device> engine_instance::index_buffer;
engine_instance::index_allocator_t engine_instance::index_allocator;

UniformBuffers engine_instance::uniform_buffers;

Vulkan::DescriptorSetLayout<engine_instance::logical_device> engine_instance::descriptor_set_layout;
Vulkan::RenderPass<engine_instance::logical_device, engine_instance::swap_chain> engine_instance::render_pass;
Vulkan::Pipeline::VulkanPipeline<engine_instance::logical_device, engine_instance::swap_chain, engine_instance::ShaderSequence, engine_instance::render_pass, engine_instance::vertex_buffer, engine_instance::descriptor_set_layout> engine_instance::vulkan_pipeline;

Vulkan::FrameBuffers<engine_instance::logical_device, engine_instance::swap_chain, engine_instance::render_pass, engine_instance::image_views> engine_instance::frame_buffers;
Vulkan::CommandPool<engine_instance::logical_device, engine_instance::queue_family_indices> engine_instance::command_pool;
std::array<Vulkan::RenderCommandBuffer<engine_instance::logical_device, engine_instance::command_pool>, render_config.max_frames_in_flight> engine_instance::command_buffers;

std::array<Vulkan::Semaphore<engine_instance::logical_device>, render_config.max_frames_in_flight> engine_instance::render_complete_semaphores;
std::array<Vulkan::Fence<engine_instance::logical_device, true>, render_config.max_frames_in_flight> engine_instance::in_flight_fences;

engine_instance::model_allocator_t engine_instance::model_allocator;

PKEngine::ObjectTree engine_instance::object_tree;

unsigned int engine_instance::current_frame;