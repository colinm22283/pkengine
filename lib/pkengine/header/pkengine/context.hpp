#pragma once

#include <thread>
#include <vector>

#include <pkengine/glfw/glfw_instance.hpp>
#include <pkengine/glfw/window.hpp>

#include <pkengine/vulkan/frame_data.hpp>

#include <pkengine/vulkan/wrapper/vulkan_instance.hpp>
#include <pkengine/vulkan/wrapper/surface.hpp>
#include <pkengine/vulkan/wrapper/physical_device.hpp>
#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/command_queue.hpp>
#include <pkengine/vulkan/wrapper/swap_chain.hpp>
#include <pkengine/vulkan/wrapper/image_views.hpp>
#include <pkengine/vulkan/wrapper/descriptor_pool.hpp>
#include <pkengine/vulkan/wrapper/descriptor_set_layout.hpp>
#include <pkengine/vulkan/wrapper/descriptor_set.hpp>
#include <pkengine/vulkan/wrapper/shader_module.hpp>
#include <pkengine/vulkan/wrapper/compute_pipeline.hpp>

#include <pkengine/vulkan/alloc/allocated_image.hpp>

#include <pkengine/util/make_array.hpp>

#include <render_config.hpp>

namespace PKEngine {
    using namespace Util;
    using namespace Vulkan;
    using namespace Vulkan::Wrapper;
    using namespace Vulkan::Util;

    class Context {
        friend class Engine;

    protected:
//    public:
        GLFW::Window window;

        VulkanInstance vulkan_instance;
        Surface surface = Surface(vulkan_instance, window);
        PhysicalDevice physical_device = PhysicalDevice(vulkan_instance, surface);
        QueueFamilyIndices queue_family_indices = QueueFamilyIndices(surface, physical_device.handle());
        LogicalDevice logical_device = LogicalDevice(physical_device, queue_family_indices);

        CommandQueue graphics_queue = CommandQueue(
            logical_device,
            queue_family_indices.graphics_family.value()
        );
        CommandQueue present_queue = CommandQueue(
            logical_device,
            queue_family_indices.present_family.value()
        );

        SwapChain swap_chain = SwapChain(window, physical_device, logical_device, surface, queue_family_indices);
        ImageViews image_views = ImageViews(logical_device, swap_chain);

        Alloc::VulkanAllocator allocator = Alloc::VulkanAllocator(vulkan_instance, physical_device, logical_device);

        Alloc::AllocatedImage draw_image = Alloc::AllocatedImage(
            logical_device,
            allocator,
            VK_FORMAT_R16G16B16A16_SFLOAT,
            VkExtent3D {
                .width = (uint32_t) window.width(),
                .height = (uint32_t) window.height(),
                .depth = 1,
            },
            (
                VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                VK_IMAGE_USAGE_STORAGE_BIT |
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
            ),
            VMA_MEMORY_USAGE_GPU_ONLY,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        DescriptorPool global_descriptor_pool = DescriptorPool(
            logical_device,
            100,
            std::vector<DescriptorPool::PoolRatio>({
                DescriptorPool::PoolRatio {
                    .type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                    .ratio = 1.0f,
                },
            })
        );
        DescriptorSetLayout draw_image_descriptor_set_layout = DescriptorSetLayout(
            logical_device,
            std::vector<VkDescriptorSetLayoutBinding>({
                VkDescriptorSetLayoutBinding {
                    .binding = 0,
                    .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                    .descriptorCount = 1,
                },
            }),
            VK_SHADER_STAGE_COMPUTE_BIT,
            0
        );

        DescriptorSet draw_image_descriptor_set = DescriptorSet(
            logical_device,
            global_descriptor_pool,
            draw_image_descriptor_set_layout
        );

        ShaderModule draw_image_shader_module = ShaderModule(
            logical_device,
            "shaders/gradient.comp.spv"
        );

        ComputePipeline draw_image_pipeline = ComputePipeline(
            logical_device,
            std::vector<VkDescriptorSetLayout>({
                draw_image_descriptor_set_layout.handle()
            }),
            draw_image_shader_module
        );

        std::size_t current_frame = 0;
        std::array<FrameData, Config::render_config.max_frames_in_flight> frames =
            make_array<FrameData, Config::render_config.max_frames_in_flight>(
                swap_chain,
                graphics_queue,
                draw_image,
                draw_image_pipeline,
                logical_device,
                queue_family_indices
            );

        [[nodiscard]] FrameData & next_frame() noexcept {
            FrameData & frame = frames[current_frame];

            current_frame = (current_frame + 1) % frames.size();

            return frame;
        }

    public:
        inline Context() {
            VkDescriptorImageInfo image_info {
                .imageView = draw_image.view(),
                .imageLayout = VK_IMAGE_LAYOUT_GENERAL,
            };

            VkWriteDescriptorSet draw_image_write = {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = nullptr,

                .dstSet = draw_image_descriptor_set.handle(),
                .dstBinding = 0,
                .descriptorCount = 1,
                .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                .pImageInfo = &image_info,
            };

            vkUpdateDescriptorSets(logical_device.handle(), 1, &draw_image_write, 0, nullptr);
        }

        inline Context(const Context &) = delete;
        inline Context(Context &&) noexcept = default;

        inline void update() {
            FrameData & frame = next_frame();

            frame.draw(draw_image_descriptor_set);
        }

        inline void exit() noexcept {
            window.close();
        }

        [[nodiscard]] inline bool should_exit() const noexcept {
            return window.should_close();
        }
    };
}