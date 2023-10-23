#pragma once

#include "../../logger/logger.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & swap_chain>
    class RenderPass {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Render Pass">();

        VkRenderPass render_pass = VK_NULL_HANDLE;

    public:
        inline void init() {
            logger << "Initializing render pass...";

            VkAttachmentDescription color_attachment { };
            color_attachment.format = swap_chain.image_format;
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

            VkSubpassDependency dependency{};
            dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            dependency.dstSubpass = 0;
            dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency.srcAccessMask = 0;
            dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            render_pass_info.dependencyCount = 1;
            render_pass_info.pDependencies = &dependency;

            if (vkCreateRenderPass(logical_device.handle(), &render_pass_info, nullptr, &render_pass) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_render_pass();
            }

            logger.success() << "Render pass initialized";
        }

        inline void free() {
            logger << "Destroying render pass...";

            if (render_pass != VK_NULL_HANDLE) vkDestroyRenderPass(logical_device.handle(), render_pass, nullptr);

            logger.success() << "Render pass destroyed";
        }

        inline VkRenderPass handle() const noexcept { return render_pass; }
    };
}