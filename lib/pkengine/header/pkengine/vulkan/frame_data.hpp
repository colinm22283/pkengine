#pragma once

#include <cstdint>
#include <cmath>

#include <pkengine/vulkan/mesh.hpp>

#include <pkengine/vulkan/wrapper/swap_chain.hpp>
#include <pkengine/vulkan/wrapper/command_pool.hpp>
#include <pkengine/vulkan/wrapper/command_buffer.hpp>
#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/graphics_pipeline.hpp>
#include <pkengine/vulkan/wrapper/descriptor_set.hpp>

#include <pkengine/vulkan/struct/rendering_info.hpp>
#include <pkengine/vulkan/struct/rendering_attachment_info.hpp>
#include <pkengine/vulkan/struct/image_subresource_range.hpp>

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
        Wrapper::GraphicsPipeline & draw_pipeline;
        Mesh & triangle_mesh;

        Wrapper::CommandPool command_pool;
        Wrapper::CommandBuffer command_buffer;

        Wrapper::Sync::Semaphore swapchain_semaphore, render_semaphore;
        Wrapper::Sync::Fence render_fence;

        inline FrameData(
            Wrapper::SwapChain & _swap_chain,
            Wrapper::CommandQueue & _graphics_queue,
            Alloc::AllocatedImage & _draw_image,
            Wrapper::GraphicsPipeline & _draw_pipeline,
            Wrapper::LogicalDevice & logical_device,
            Util::QueueFamilyIndices & queue_family_indices,
            Mesh & _triangle_mesh
        ):
            swap_chain(_swap_chain),
            graphics_queue(_graphics_queue),
            draw_image(_draw_image),
            draw_pipeline(_draw_pipeline),
            triangle_mesh(_triangle_mesh),
            command_pool(logical_device, queue_family_indices.graphics_family.value()),
            command_buffer(logical_device, command_pool),
            swapchain_semaphore(logical_device),
            render_semaphore(logical_device),
            render_fence(logical_device) { }

        inline void clear_background() const {
            VkClearColorValue clear_value = { { 0.0f, 0.0f, 1.0f, 1.0f } };
            VkImageSubresourceRange clear_range = Struct::image_subresource_range(VK_IMAGE_ASPECT_COLOR_BIT);

            vkCmdClearColorImage(
                command_buffer.handle(),
                draw_image.handle(),
                VK_IMAGE_LAYOUT_GENERAL,
                &clear_value,
                1,
                &clear_range
            );
        }

        inline void draw_geometry() const {
            VkExtent2D draw_extent = {
                .width = draw_image.extent().width,
                .height = draw_image.extent().height,
            };
            VkRenderingAttachmentInfo color_attachment = Struct::rendering_attachment_info(
                draw_image.view(),
                nullptr,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
            );

            VkRenderingInfo renderInfo = Struct::rendering_info(
                draw_extent,
                &color_attachment,
                nullptr
            );
            vkCmdBeginRendering(
                command_buffer.handle(),
                &renderInfo
            );

            vkCmdBindPipeline(
                command_buffer.handle(),
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                draw_pipeline.handle()
            );

            ShaderStruct::GraphicalPushConstants push_constants = {
                .world_matrix = ShaderStruct::mat4_identity,
                .vertex_buffer = triangle_mesh.vertex_buffer_address(),
            };
            vkCmdPushConstants(
                command_buffer.handle(),
                draw_pipeline.layout(),
                VK_SHADER_STAGE_VERTEX_BIT,
                0,
                sizeof(ShaderStruct::GraphicalPushConstants),
                &push_constants
            );
            vkCmdBindIndexBuffer(
                command_buffer.handle(),
                triangle_mesh.index_buffer().handle(),
                0,
                VK_INDEX_TYPE_UINT32
            );

            VkViewport viewport = {
                .x = 0,
                .y = 0,
                .width = (float) draw_image.extent().width,
                .height = (float) draw_image.extent().height,
                .minDepth = 0.f,
                .maxDepth = 1.f,
            };

            vkCmdSetViewport(command_buffer.handle(), 0, 1, &viewport);

            VkRect2D scissor = {
                .offset = {
                    .x = 0,
                    .y = 0,
                },
                .extent = {
                    .width = draw_image.extent().width,
                    .height = draw_image.extent().height,
                },
            };

            vkCmdSetScissor(command_buffer.handle(), 0, 1, &scissor);

            vkCmdDrawIndexed(
                command_buffer.handle(),
                6,
                1,
                0,
                0,
                0
            );

            vkCmdEndRendering(command_buffer.handle());
        }

        inline void draw(
            Wrapper::DescriptorSet & draw_descriptor_set
        ) {
            render_fence.await();
            render_fence.reset();

            command_buffer.reset();
            command_buffer.begin_one_time();

            Util::transition_image(command_buffer, draw_image.handle(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);
            clear_background();

            Util::transition_image(command_buffer, draw_image.handle(), VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

            draw_geometry();

            uint32_t swap_chain_image_index = swap_chain.next_image_index(swapchain_semaphore);

            Util::transition_image(command_buffer, draw_image.handle(), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
            Util::transition_image(command_buffer, swap_chain.images()[swap_chain_image_index], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

            VkExtent2D draw_extent = {
                .width = draw_image.extent().width,
                .height = draw_image.extent().height,
            };
            Util::copy_image(command_buffer, draw_image.handle(), swap_chain.images()[swap_chain_image_index], draw_extent, swap_chain.image_extent());

            Util::transition_image(command_buffer, swap_chain.images()[swap_chain_image_index], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

            command_buffer.end();

            graphics_queue.submit(command_buffer, swapchain_semaphore, render_semaphore, render_fence);

            swap_chain.present(graphics_queue, render_semaphore, swap_chain_image_index);
        }
    };
}
