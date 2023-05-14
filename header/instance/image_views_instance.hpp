#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include <instance/swap_chain_instance.hpp>

#include <util/engine_exception.hpp>

namespace PKEngine {
    class ImageViewsInstance {
    protected:
        std::vector<VkImageView> views;
        VkDevice device;

    public:
        inline explicit ImageViewsInstance(VkDevice _device, SwapChainInstance & swap_chain):
          device(_device) {
            views.resize(swap_chain.images.size());

            for (size_t i = 0; i < swap_chain.images.size(); i++) {
                VkImageViewCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                createInfo.image = swap_chain.images[i];
                createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                createInfo.format = swap_chain.image_format;
                createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                createInfo.subresourceRange.baseMipLevel = 0;
                createInfo.subresourceRange.levelCount = 1;
                createInfo.subresourceRange.baseArrayLayer = 0;
                createInfo.subresourceRange.layerCount = 1;

                if (vkCreateImageView(device, &createInfo, nullptr, &views[i]) != VK_SUCCESS) {
                    throw Exceptions::UnableToCreateImageView();
                }
            }
        }

        inline ~ImageViewsInstance() {
            for (auto view : views) {
                vkDestroyImageView(device, view, nullptr);
            }
        }
    };
}