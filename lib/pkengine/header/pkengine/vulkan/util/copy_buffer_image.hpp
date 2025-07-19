#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/vulkan/wrapper/command_buffer.hpp>

namespace PKEngine::Vulkan::Util {
    inline void copy_buffer_image(Wrapper::CommandBuffer & command_buffer, VkBuffer src, VkImage dst, VkExtent3D dst_extent) {
        VkBufferImageCopy copyRegion = {};
        copyRegion.bufferOffset = 0;
        copyRegion.bufferRowLength = 0;
        copyRegion.bufferImageHeight = 0;

        copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copyRegion.imageSubresource.mipLevel = 0;
        copyRegion.imageSubresource.baseArrayLayer = 0;
        copyRegion.imageSubresource.layerCount = 1;
        copyRegion.imageExtent = dst_extent;

        vkCmdCopyBufferToImage(
            command_buffer.handle(),
            src,
            dst,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &copyRegion
        );
    }
}