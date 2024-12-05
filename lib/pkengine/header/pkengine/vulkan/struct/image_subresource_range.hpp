#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan::Struct {
    constexpr VkImageSubresourceRange image_subresource_range(VkImageAspectFlags aspect_mask) {
        return VkImageSubresourceRange {
            .aspectMask = aspect_mask,

            .baseMipLevel = 0,
            .levelCount = VK_REMAINING_MIP_LEVELS,

            .baseArrayLayer = 0,
            .layerCount = VK_REMAINING_ARRAY_LAYERS,
        };
    }
}