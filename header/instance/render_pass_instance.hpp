#pragma once

#include <vulkan/vulkan.h>

#include <logger/logger.hpp>

#include <util/engine_exception.hpp>

namespace PKEngine {
    class RenderPassInstance {
    protected:
        static constexpr auto logger = FileLogger<Util::ANSI::CyanFg, "RENDER PASS INST", "pkengine.log">();

        VkDevice device;

        VkRenderPass render_pass;
    public:
        inline RenderPassInstance(VkDevice _device, VkFormat & swap_chain_image_format): device(_device) {
            logger.log<"Creating render pass instance...">();

            VkAttachmentDescription color_attachment { };
            color_attachment.format = swap_chain_image_format;
            color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
            color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference color_attachment_ref { };
            color_attachment_ref.attachment = 0;
            color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass { };
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &color_attachment_ref;

            VkRenderPassCreateInfo render_pass_info{};
            render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            render_pass_info.attachmentCount = 1;
            render_pass_info.pAttachments = &color_attachment;
            render_pass_info.subpassCount = 1;
            render_pass_info.pSubpasses = &subpass;

            if (vkCreateRenderPass(device, &render_pass_info, nullptr, &render_pass) != VK_SUCCESS) {
                throw Exceptions::UnableToCreateRenderPass();
            }

            logger.success().log<"Done!">();
        }

        inline ~RenderPassInstance() {
            vkDestroyRenderPass(device, render_pass, nullptr);
            logger.log<"Render pass instance destroyed">();
        }

        inline auto & instance() { return render_pass; }
        inline operator auto &() { return instance(); }
    };
}