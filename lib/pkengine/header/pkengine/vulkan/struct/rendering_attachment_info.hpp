#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan::Struct {
    constexpr VkRenderingAttachmentInfo rendering_attachment_info(VkImageView image_view, VkClearValue * clear_value, VkImageLayout layout) {
        return VkRenderingAttachmentInfo {
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .pNext = nullptr,

            .imageView = image_view,
            .imageLayout = layout,

            .loadOp = (clear_value != nullptr) ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,

            .clearValue = (clear_value != nullptr) ? *clear_value : VkClearValue { },
        };
    }
}