#pragma once

#include <vulkan/vulkan.h>

#include "pkengine/engine.hpp"

#include "pkengine/exception/internal.hpp"
#include "command_buffer.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & command_pool>
    class RenderCommandBuffer : public CommandBuffer<logical_device, command_pool> {
    protected:
        using CommandBuffer<logical_device, command_pool>::buffer;

    public:
        template<
            const auto & render_pass,
            const auto & swap_chain,
            const auto & frame_buffers,
            const auto & pipeline,
            const auto & vertex_buffer,
            const auto & index_buffer,
            auto & object_tree
        >
        inline void record(auto & descriptor_set, uint32_t image_index) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = 0; // Optional
            beginInfo.pInheritanceInfo = nullptr; // Optional

            if (vkBeginCommandBuffer(buffer, &beginInfo) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_record_vulkan_command_buffer();
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = render_pass.handle();
            renderPassInfo.framebuffer = frame_buffers[image_index].handle();
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swap_chain.extent;

            VkClearValue clearColor = { { { 0.59765625f, 0.59765625f, 0.59765625f, 1.0f } } };
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

//            VkSubpassDependency dependency{};
//            dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
//            dependency.dstSubpass = 0;
//            dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//            dependency.srcAccessMask = 0;
//            dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            vkCmdBeginRenderPass(buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.handle());

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(swap_chain.extent.width);
            viewport.height = static_cast<float>(swap_chain.extent.height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(buffer, 0, 1, &viewport);

            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = swap_chain.extent;
            vkCmdSetScissor(buffer, 0, 1, &scissor);

            VkBuffer vertex_buffers[] = { vertex_buffer.buffer_handle(), };
            VkDeviceSize vertex_offsets[] = { 0 };
            vkCmdBindVertexBuffers(buffer, 0, 1, vertex_buffers, vertex_offsets);

            vkCmdBindIndexBuffer(buffer, index_buffer.buffer_handle(), 0, VK_INDEX_TYPE_UINT32);

            vkCmdBindDescriptorSets(
                buffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipeline.layout_handle(),
                0,
                1,
                &descriptor_set.handle(),
                0,
                nullptr
            );

            object_tree.record_buffer();

            vkCmdEndRenderPass(buffer);

            if (vkEndCommandBuffer(buffer) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_record_vulkan_command_buffer();
            }
        }
    };
}