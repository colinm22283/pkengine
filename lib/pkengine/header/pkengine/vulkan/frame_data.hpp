#pragma once

#include <cstdint>
#include <cmath>

#include <pkengine/vulkan/mesh.hpp>
#include <pkengine/vulkan/camera_data.hpp>

#include <pkengine/vulkan/wrapper/swap_chain.hpp>
#include <pkengine/vulkan/wrapper/command_pool.hpp>
#include <pkengine/vulkan/wrapper/command_buffer.hpp>
#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/graphics_pipeline.hpp>
#include <pkengine/vulkan/wrapper/descriptor_set.hpp>

#include <pkengine/vulkan/struct/rendering_info.hpp>
#include <pkengine/vulkan/struct/rendering_attachment_info.hpp>
#include <pkengine/vulkan/struct/image_subresource_range.hpp>
#include <pkengine/vulkan/struct/depth_attachment_info.hpp>

#include <pkengine/vulkan/wrapper/sync/semaphore.hpp>
#include <pkengine/vulkan/wrapper/sync/fence.hpp>

#include <pkengine/vulkan/alloc/allocated_image.hpp>

#include <pkengine/vulkan/util/queue_family_indices.hpp>
#include <pkengine/vulkan/util/transition_image.hpp>
#include <pkengine/vulkan/util/copy_image.hpp>

#include <pkengine/util/erasable_list.hpp>

#include <render_config.hpp>

namespace PKEngine::Vulkan {
    struct FrameData {
        Wrapper::SwapChain & swap_chain;
        Wrapper::CommandQueue & graphics_queue;
        float & render_scale;
        Alloc::AllocatedImage & draw_image;
        Alloc::AllocatedImage & depth_image;
        Wrapper::GraphicsPipeline & draw_pipeline;
        PKEngine::Util::ErasableList<Mesh> & meshes;
        CameraData & camera_data;

        Wrapper::CommandPool command_pool;
        Wrapper::CommandBuffer command_buffer;

        Wrapper::Sync::Semaphore swapchain_semaphore, render_semaphore;
        Wrapper::Sync::Fence render_fence;

        inline FrameData(
            Wrapper::SwapChain & _swap_chain,
            Wrapper::CommandQueue & _graphics_queue,
            float & _render_scale,
            Alloc::AllocatedImage & _draw_image,
            Alloc::AllocatedImage & _depth_image,
            Wrapper::GraphicsPipeline & _draw_pipeline,
            Wrapper::LogicalDevice & logical_device,
            Util::QueueFamilyIndices & queue_family_indices,
            PKEngine::Util::ErasableList<Mesh> & _meshes,
            CameraData & _camera_data
        ):
            swap_chain(_swap_chain),
            graphics_queue(_graphics_queue),
            render_scale(_render_scale),
            draw_image(_draw_image),
            depth_image(_depth_image),
            draw_pipeline(_draw_pipeline),
            meshes(_meshes),
            camera_data(_camera_data),
            command_pool(logical_device, queue_family_indices.graphics_family.value()),
            command_buffer(logical_device, command_pool),
            swapchain_semaphore(logical_device),
            render_semaphore(logical_device),
            render_fence(logical_device, true) { }

        inline FrameData(FrameData && other) noexcept:
            swap_chain(other.swap_chain),
            graphics_queue(other.graphics_queue),
            render_scale(other.render_scale),
            draw_image(other.draw_image),
            depth_image(other.depth_image),
            draw_pipeline(other.draw_pipeline),
            meshes(other.meshes),
            camera_data(other.camera_data),
            command_pool(std::move(other.command_pool)),
            command_buffer(std::move(other.command_buffer)),
            swapchain_semaphore(std::move(other.swapchain_semaphore)),
            render_semaphore(std::move(other.render_semaphore)),
            render_fence(std::move(other.render_fence)) { }

        inline FrameData & operator=(FrameData && other) noexcept {
            command_pool = std::move(other.command_pool);
            command_buffer = std::move(other.command_buffer);
            swapchain_semaphore = std::move(other.swapchain_semaphore);
            render_semaphore = std::move(other.render_semaphore);
            render_fence = std::move(other.render_fence);

            return *this;
        }

        inline void clear_background() const {
            VkClearColorValue clear_value = { {
                Config::render_config.background_color.x,
                Config::render_config.background_color.y,
                Config::render_config.background_color.z,
                Config::render_config.background_color.w
            } };
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
                .width = (uint32_t) ((float) std::min(swap_chain.image_extent().width, draw_image.extent().width) * render_scale),
                .height = (uint32_t) ((float) std::min(swap_chain.image_extent().height, draw_image.extent().height) * render_scale),
            };
            VkRenderingAttachmentInfo color_attachment = Struct::rendering_attachment_info(
                draw_image.view(),
                nullptr,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
            );
            VkRenderingAttachmentInfo depth_attachment = Struct::depth_attachment_info(
                depth_image.view(),
                VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
            );

            VkRenderingInfo renderInfo = Struct::rendering_info(
                draw_extent,
                &color_attachment,
                &depth_attachment
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

            VkViewport viewport = {
                .x = 0,
                .y = 0,
                .width = (float) draw_extent.width,
                .height = (float) draw_extent.height,
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
                    .width = draw_extent.width,
                    .height = draw_extent.height,
                },
            };

            vkCmdSetScissor(command_buffer.handle(), 0, 1, &scissor);

            for (Mesh & mesh : meshes) {
                ShaderStruct::GraphicalPushConstants push_constants = {
                    .world_matrix = camera_data.world_mat(),
                    .vertex_buffer = mesh.mesh_buffer().vertex_buffer_address(),
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
                    mesh.mesh_buffer().index_buffer().handle(),
                    0,
                    VK_INDEX_TYPE_UINT32
                );

                for (const Mesh::SubmeshRange & submesh_range : mesh.submesh_ranges()) {
                    vkCmdDrawIndexed(
                        command_buffer.handle(),
                        submesh_range.count,
                        1,
                        submesh_range.start_index,
                        0,
                        0
                    );
                }
            }

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
            Util::transition_image(command_buffer, depth_image.handle(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);

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

        inline void await_complete() {
            render_fence.await();
        }
    };
}
