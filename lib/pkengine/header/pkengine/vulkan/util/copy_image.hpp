#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/vulkan/wrapper/command_buffer.hpp>

namespace PKEngine::Vulkan::Util {
    inline void copy_image(Wrapper::CommandBuffer & command_buffer, VkImage src, VkImage dst, VkExtent2D src_extent, VkExtent2D dst_extent) {
        VkImageBlit2 blitRegion{
            .sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2,
            .pNext = nullptr,

            .srcSubresource = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = 0,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },

            .srcOffsets = {
                VkOffset3D { },
                VkOffset3D {
                    .x = (int32_t) src_extent.width,
                    .y = (int32_t) src_extent.height,
                    .z = 1,
                },
            },

            .dstSubresource = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = 0,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },

            .dstOffsets = {
                VkOffset3D { },
                VkOffset3D {
                    .x = (int32_t) dst_extent.width,
                    .y = (int32_t) dst_extent.height,
                    .z = 1,
                },
            },
        };

        VkBlitImageInfo2 blitInfo{ .sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2, .pNext = nullptr };
        blitInfo.dstImage = dst;
        blitInfo.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        blitInfo.srcImage = src;
        blitInfo.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        blitInfo.filter = VK_FILTER_LINEAR;
        blitInfo.regionCount = 1;
        blitInfo.pRegions = &blitRegion;

        vkCmdBlitImage2(command_buffer.handle(), &blitInfo);
    }
}