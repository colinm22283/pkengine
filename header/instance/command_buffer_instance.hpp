#pragma once

#include <vulkan/vulkan.h>

#include <util/engine_exception.hpp>

#include <engine/frame_buffers.hpp>

#include <instance/swap_chain_instance.hpp>
#include <instance/pipeline_instance.hpp>
#include <instance/vertex_buffer_instance.hpp>

namespace PKEngine {
    class CommandBufferInstance {
    protected:
        VkDevice device;

        VkCommandBuffer command_buffer;

    public:
        inline CommandBufferInstance(VkDevice _device, VkCommandPool command_pool): device(_device) {
            VkCommandBufferAllocateInfo alloc_info{};
            alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            alloc_info.commandPool = command_pool;
            alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            alloc_info.commandBufferCount = 1;

            if (vkAllocateCommandBuffers(device, &alloc_info, &command_buffer) != VK_SUCCESS) {
                throw Exceptions::UnableToCreateCommandBuffer();
            }
        }

        inline void record(
            uint32_t image_index,
            VkRenderPass render_pass,
            FrameBuffers & frame_buffers,
            SwapChainInstance & swap_chain,
            PipelineInstance & pipeline,
            VertexBufferInstance & vertex_buffer

        ) {
            VkCommandBufferBeginInfo begin_info{};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin_info.flags = 0;
            begin_info.pInheritanceInfo = nullptr;

            if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
                throw Exceptions::UnableToBeginCommandBuffer();
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = render_pass;
            renderPassInfo.framebuffer = frame_buffers[image_index];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swap_chain.extent;

            VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(command_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(swap_chain.extent.width);
            viewport.height = static_cast<float>(swap_chain.extent.height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(command_buffer, 0, 1, &viewport);

            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = swap_chain.extent;
            vkCmdSetScissor(command_buffer, 0, 1, &scissor);

            VkBuffer vertexBuffers[] = { vertex_buffer };
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(command_buffer, 0, 1, vertexBuffers, offsets);

            vkCmdDraw(command_buffer, 3, 1, 0, 0);

            vkCmdEndRenderPass(command_buffer);

            if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }

        inline VkCommandBuffer & instance() noexcept { return command_buffer; }
        inline operator VkCommandBuffer &() noexcept { return instance(); }
    };
}