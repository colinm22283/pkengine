#pragma once

#include <cstdint>
#include <cmath>

#include <pkengine/vulkan/wrapper/swap_chain.hpp>
#include <pkengine/vulkan/wrapper/command_pool.hpp>
#include <pkengine/vulkan/wrapper/command_buffer.hpp>
#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/compute_pipeline.hpp>
#include <pkengine/vulkan/wrapper/descriptor_set.hpp>

#include <pkengine/vulkan/wrapper/sync/semaphore.hpp>
#include <pkengine/vulkan/wrapper/sync/fence.hpp>

#include <pkengine/vulkan/alloc/allocated_image.hpp>

#include <pkengine/vulkan/util/queue_family_indices.hpp>
#include <pkengine/vulkan/util/transition_image.hpp>
#include <pkengine/vulkan/util/copy_image.hpp>

namespace PKEngine::Vulkan {
    struct FrameData {
        Wrapper::SwapChain & swap_chain;
        Wrapper::CommandQueue & graphics_queue;
        Alloc::AllocatedImage & draw_image;
        Wrapper::ComputePipeline & draw_pipeline;

        Wrapper::CommandPool command_pool;
        Wrapper::CommandBuffer command_buffer;

        Wrapper::Sync::Semaphore swapchain_semaphore, render_semaphore;
        Wrapper::Sync::Fence render_fence;

        inline FrameData(
            Wrapper::SwapChain & _swap_chain,
            Wrapper::CommandQueue & _graphics_queue,
            Alloc::AllocatedImage & _draw_image,
            Wrapper::ComputePipeline & _draw_pipeline,
            Wrapper::LogicalDevice & logical_device,
            Util::QueueFamilyIndices & queue_family_indices
        ):
            swap_chain(_swap_chain),
            graphics_queue(_graphics_queue),
            draw_image(_draw_image),
            draw_pipeline(_draw_pipeline),
            command_pool(logical_device, queue_family_indices.graphics_family.value()),
            command_buffer(logical_device, command_pool),
            swapchain_semaphore(logical_device),
            render_semaphore(logical_device),
            render_fence(logical_device) { }

        inline void draw(
            Wrapper::DescriptorSet & draw_descriptor_set
        ) {
            render_fence.await();
            render_fence.reset();

            command_buffer.reset();
            command_buffer.begin_one_time();

            uint32_t swap_chain_image_index = swap_chain.next_image_index(swapchain_semaphore);

            Util::transition_image(command_buffer, draw_image.handle(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

//            VkClearColorValue clear_value = { { 0.0f, 0.0f, 1.0f, 1.0f } };
//
//            VkImageSubresourceRange clear_range = {
//                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
//                .baseMipLevel = 0,
//                .levelCount = VK_REMAINING_MIP_LEVELS,
//                .baseArrayLayer = 0,
//                .layerCount = VK_REMAINING_ARRAY_LAYERS,
//            };
//            vkCmdClearColorImage(
//                command_buffer.handle(),
//                draw_image.handle(),
//                VK_IMAGE_LAYOUT_GENERAL,
//                &clear_value,
//                1,
//                &clear_range
//            );

            vkCmdBindPipeline(
                command_buffer.handle(),
                VK_PIPELINE_BIND_POINT_COMPUTE,
                draw_pipeline.handle()
            );

            vkCmdBindDescriptorSets(
                command_buffer.handle(),
                VK_PIPELINE_BIND_POINT_COMPUTE,
                draw_pipeline.layout(),
                0,
                1,
                &draw_descriptor_set.handle(),
                0,
                nullptr
            );

            vkCmdDispatch(
                command_buffer.handle(),
                std::ceil(draw_image.extent().width / 16.0),
                std::ceil(draw_image.extent().height / 16.0),
                1
            );

            Util::transition_image(command_buffer, draw_image.handle(), VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
            Util::transition_image(command_buffer, swap_chain.images()[swap_chain_image_index], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

            VkExtent2D draw_extent = {
                .width = draw_image.extent().width,
                .height = draw_image.extent().height,
            };
            Util::copy_image(command_buffer, draw_image.handle(), swap_chain.images()[swap_chain_image_index], draw_extent, swap_chain.image_extent());

            Util::transition_image(command_buffer, swap_chain.images()[swap_chain_image_index], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);;

            command_buffer.end();

            graphics_queue.submit(command_buffer, swapchain_semaphore, render_semaphore, render_fence);

            swap_chain.present(graphics_queue, render_semaphore, swap_chain_image_index);
        }
    };
}
