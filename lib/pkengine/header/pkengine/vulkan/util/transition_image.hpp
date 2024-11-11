#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/vulkan/wrapper/command_buffer.hpp>

namespace PKEngine::Vulkan::Util {
    inline void transition_image(
        Wrapper::CommandBuffer & command_buffer,
        VkImage image,
        VkImageLayout current_layout,
        VkImageLayout target_layout
    ) noexcept {
        VkImageAspectFlags aspect_mask = (target_layout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;

        VkImageMemoryBarrier2 image_barrier {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
            .pNext = nullptr,

            .srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT, // TODO: use stage masks https://vkguide.dev/docs/new_chapter_1/vulkan_mainloop_code/
            .srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
            .dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT,

            .oldLayout = current_layout,
            .newLayout = target_layout,

            .image = image,

            .subresourceRange = {
                .aspectMask = aspect_mask,
                .baseMipLevel = 0,
                .levelCount = VK_REMAINING_MIP_LEVELS,
                .baseArrayLayer = 0,
                .layerCount = VK_REMAINING_ARRAY_LAYERS,
            },
        };

        VkDependencyInfo depInfo {
            .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
            .pNext = nullptr,

            .imageMemoryBarrierCount = 1,
            .pImageMemoryBarriers = &image_barrier,
        };

        vkCmdPipelineBarrier2(command_buffer.handle(), &depInfo);
    }
}