#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & swap_chain>
    class ImageViews {
    protected:
        static constexpr auto logger = Logger<"Image Views">();

        std::vector<VkImageView> views;

    public:
        inline void init() {
            logger << "Creating image views...";

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

                if (vkCreateImageView(logical_device.handle(), &createInfo, nullptr, &views[i]) != VK_SUCCESS) {
                    throw Exception::Internal::vulkan_unable_to_create_image_view();
                }
            }

            logger.success() << "Image views initialized";
        }
        inline void free() {
            logger << "Destroying image views...";

            for (const auto & view : views) {
                if (view != VK_NULL_HANDLE) vkDestroyImageView(logical_device.handle(), view, nullptr);
            }

            logger.success() << "Image views destroyed";
        }

        inline auto size() const noexcept { return views.size(); }
        inline auto & data() const noexcept { return views; }
    };
}