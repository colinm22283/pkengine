#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/internal/vulkan/sync/semaphore.hpp>

namespace PKEngine::Vulkan {
    template<const auto & physical_device, const auto & logical_device, const auto &surface, const auto & window, const auto & queue_family_indices>
    class SwapChain {
    protected:
        static constexpr auto logger = Logger<"Swap Chain">();

        static inline VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR> & available_formats) {
            for (const auto & availableFormat : available_formats) {
                if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    return availableFormat;
                }
            }

            return available_formats[0];
        }
        static inline VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR> & available_present_modes) {
            for (const auto& availablePresentMode : available_present_modes) {
                if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    return availablePresentMode;
                }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }
        static inline VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities) {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return capabilities.currentExtent;
            } else {
                int width, height;
                glfwGetFramebufferSize(window.handle(), &width, &height);

                VkExtent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
                };

                actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                                                capabilities.maxImageExtent.width);
                actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                                                 capabilities.maxImageExtent.height);

                return actualExtent;
            }
        }

        VkSwapchainKHR swap_chain = VK_NULL_HANDLE;

    public:
        std::vector<VkImage> images;
        VkFormat image_format;
        VkExtent2D extent;

        inline void init() {
            logger << "Creating swap chain...";

            SwapChainSupport<surface> swapChainSupport(physical_device.handle());

            VkSurfaceFormatKHR surfaceFormat = choose_swap_surface_format(swapChainSupport.formats);
            VkPresentModeKHR presentMode = choose_swap_present_mode(swapChainSupport.present_modes);
            VkExtent2D _extent = choose_swap_extent(swapChainSupport.capabilities);

            uint32_t image_count = swapChainSupport.capabilities.minImageCount + 1;

            if (swapChainSupport.capabilities.maxImageCount > 0 && image_count > swapChainSupport.capabilities.maxImageCount) {
                image_count = swapChainSupport.capabilities.maxImageCount;
            }

            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = surface.handle();
            createInfo.minImageCount = image_count;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = _extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            uint32_t queueFamilyIndices[] = {
                queue_family_indices.graphics_family.value(),
                queue_family_indices.present_family.value()
            };

            if (queue_family_indices.graphics_family != queue_family_indices.present_family) {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            } else {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                createInfo.queueFamilyIndexCount = 0; // Optional
                createInfo.pQueueFamilyIndices = nullptr; // Optional
            }

            createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = presentMode;
            createInfo.clipped = VK_TRUE;
            createInfo.oldSwapchain = VK_NULL_HANDLE;

            if (vkCreateSwapchainKHR(logical_device.handle(), &createInfo, nullptr, &swap_chain) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_swap_chain();
            }

            vkGetSwapchainImagesKHR(logical_device.handle(), swap_chain, &image_count, nullptr);
            images.resize(image_count);
            vkGetSwapchainImagesKHR(logical_device.handle(), swap_chain, &image_count, images.data());

            image_format = surfaceFormat.format;
            extent = _extent;

            logger.success() << "Swap chain created";
        }
        inline void free() {
            if (swap_chain != VK_NULL_HANDLE) {
                logger << "Destroying swap chain...";

                vkDestroySwapchainKHR(logical_device.handle(), swap_chain, nullptr);

                logger.success() << "Swap chain destroyed";
            }
        }

        [[nodiscard]] inline VkSwapchainKHR handle() const noexcept { return swap_chain; }

        [[nodiscard]] inline uint32_t get_next_image_index(Vulkan::Semaphore<logical_device> & image_available_semaphore) {
            uint32_t index;
            vkAcquireNextImageKHR(logical_device.handle(), swap_chain, UINT64_MAX, image_available_semaphore.handle(), VK_NULL_HANDLE, &index);
            return index;
        }

        template<const auto & present_queue>
        inline void present(uint32_t image_index, const auto & render_complete_semaphore) {
            VkPresentInfoKHR presentInfo{};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

            presentInfo.waitSemaphoreCount = 1;
            VkSemaphore semaphores[] = { render_complete_semaphore.handle() };
            presentInfo.pWaitSemaphores = semaphores;
            VkSwapchainKHR swapChains[] = { swap_chain };
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapChains;
            presentInfo.pImageIndices = &image_index;
            presentInfo.pResults = nullptr;
            vkQueuePresentKHR(present_queue.handle(), &presentInfo);
        }
    };
}