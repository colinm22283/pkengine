#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan::Struct {
    constexpr VkRenderingInfo rendering_info(VkExtent2D extent, VkRenderingAttachmentInfo * color_attachment, VkRenderingAttachmentInfo * depth_attachment) {
        return VkRenderingInfo {
            .sType                = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR,
            .pNext                = nullptr,

            .renderArea           = VkRect2D { .offset = { .x = 0, .y = 0, }, .extent = extent, },
            .layerCount           = 1,

            .colorAttachmentCount = 1,
            .pColorAttachments    = color_attachment,
            .pDepthAttachment     = depth_attachment,
            .pStencilAttachment   = nullptr,
        };
    }
}