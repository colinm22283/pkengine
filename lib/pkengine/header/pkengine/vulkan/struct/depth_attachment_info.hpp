#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan::Struct {
    constexpr VkRenderingAttachmentInfo depth_attachment_info(VkImageView image_view, VkImageLayout layout) {
        return VkRenderingAttachmentInfo {
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .pNext = nullptr,

            .imageView = image_view,
            .imageLayout = layout,

            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,

            .clearValue = {
                .depthStencil = {
                    .depth = 1.0f,
                },
            },
        };
    }
}