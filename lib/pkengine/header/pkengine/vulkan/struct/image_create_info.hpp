#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan::Struct {
    constexpr VkImageCreateInfo image_create_info(
        VkFormat format,
        VkImageUsageFlags usage_flags,
        VkExtent3D extent
    ) {
        return VkImageCreateInfo {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = nullptr,

            .imageType = VK_IMAGE_TYPE_2D,

            .format = format,
            .extent = extent,

            .mipLevels = 1,
            .arrayLayers = 1,

            .samples = VK_SAMPLE_COUNT_1_BIT,

            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = usage_flags,
        };
    }
}