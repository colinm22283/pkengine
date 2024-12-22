#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/swap_chain.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class ImageViews {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize vulkan image view");
        };

    protected:
        static constexpr auto logger = Logger<"Image View">();

        LogicalDevice & logical_device;

        std::vector<VkImageView> image_views;

    public:
        inline ImageViews(LogicalDevice & _logical_device, SwapChain & swap_chain):
            logical_device(_logical_device),
            image_views(swap_chain.images().size(), VK_NULL_HANDLE) {
            logger.debug() << "Initializing image views...";

            VkImageViewCreateInfo create_info {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = swap_chain.image_format(),
                .components = {
                    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .a = VK_COMPONENT_SWIZZLE_IDENTITY,
                },
                .subresourceRange = {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1,
                },
            };

            for (SwapChain::SwapChainIndex i = 0; i < swap_chain.images().size(); i++) {
                create_info.image = swap_chain.images()[i];

                Util::throw_on_fail<Exceptions::InitError>(
                    vkCreateImageView(logical_device.handle(), &create_info, nullptr, &image_views[i])
                );
            }

            logger.debug() << "Image views initialized";
        }

        inline ~ImageViews() {
            logger.debug() << "Destroying image views...";

            for (VkImageView & image_view : image_views) {
                if (image_view != VK_NULL_HANDLE) {
                    vkDestroyImageView(logical_device.handle(), image_view, nullptr);
                }
            }

            logger.debug() << "Image view destroyed";
        }

        inline ImageViews & operator=(ImageViews && other) noexcept {
            for (VkImageView & image_view : image_views) {
                vkDestroyImageView(logical_device.handle(), image_view, nullptr);
            }

            image_views = std::move(other.image_views);

            for (VkImageView & image_view : other.image_views) image_view = nullptr;

            return *this;
        }

        [[nodiscard]] constexpr const VkImageView & handle(std::size_t index) const noexcept { return image_views[index]; }
    };
}