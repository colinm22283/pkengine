#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/glfw/window.hpp>

#include <pkengine/vulkan/wrapper/surface.hpp>
#include <pkengine/vulkan/wrapper/physical_device.hpp>
#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/command_queue.hpp>
#include <pkengine/vulkan/wrapper/sync/semaphore.hpp>
#include <pkengine/vulkan/util/swap_chain_support.hpp>
#include <pkengine/vulkan/util/queue_family_indices.hpp>
#include <pkengine/vulkan/util/vulkan_exception.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class SwapChain {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize swap chain");
            PKENGINE_DEFINE_VULKAN_EXCEPTION(GetNextImageError, "Unable to get next swap chain image");
            PKENGINE_DEFINE_VULKAN_EXCEPTION(PresentQueueError, "Unable to present swap chain queue");
        };

    protected:
        static constexpr auto logger = Logger<"Swap Chain">();

        LogicalDevice & logical_device;

        VkSwapchainKHR swap_chain = VK_NULL_HANDLE;

        std::vector<VkImage> _images;
        VkFormat _image_format;
        VkExtent2D extent;

    public:
        using SwapChainIndex = uint32_t;

        inline SwapChain(
            GLFW::Window & window,
            PhysicalDevice & physical_device,
            LogicalDevice & _logical_device,
            Surface & surface,
            Util::QueueFamilyIndices & queue_family_indices
        ):
            logical_device(_logical_device) {
            logger.debug() << "Initializing swap chain...";

            auto indented_logger = logger.indent();

            Util::SwapChainSupport swap_chain_support(surface, physical_device.handle());

            const VkSurfaceFormatKHR & surface_format = swap_chain_support.choose_surface_format();
            const VkPresentModeKHR & present_mode = swap_chain_support.choose_present_mode();
            extent = swap_chain_support.choose_swap_extent(window);

            uint32_t image_count = swap_chain_support.capabilities().minImageCount + 1;
            if (swap_chain_support.capabilities().maxImageCount > 0 && image_count > swap_chain_support.capabilities().maxImageCount) {
                image_count = swap_chain_support.capabilities().maxImageCount;
            }

            indented_logger.debug() << "Image count: " << image_count;

            VkSwapchainCreateInfoKHR create_info {
                .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                .surface = surface.handle(),
                .minImageCount = image_count,
                .imageFormat = surface_format.format,
                .imageColorSpace = surface_format.colorSpace,
                .imageExtent = extent,
                .imageArrayLayers = 1,
                .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .preTransform = swap_chain_support.capabilities().currentTransform,
                .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                .presentMode = present_mode,
                .clipped = VK_TRUE,
                .oldSwapchain = VK_NULL_HANDLE,
            };

            uint32_t queueFamilyIndices[] = {
                queue_family_indices.graphics_family.value(),
                queue_family_indices.present_family.value()
            };

            if (queue_family_indices.graphics_family != queue_family_indices.present_family) {
                create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                create_info.queueFamilyIndexCount = 2;
                create_info.pQueueFamilyIndices = queueFamilyIndices;
            } else {
                create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                create_info.queueFamilyIndexCount = 0; // Optional
                create_info.pQueueFamilyIndices = nullptr; // Optional
            }

            Util::throw_on_fail<Exceptions::InitError>(
                vkCreateSwapchainKHR(logical_device.handle(), &create_info, nullptr, &swap_chain)
            );

            vkGetSwapchainImagesKHR(logical_device.handle(), swap_chain, &image_count, nullptr);
            _images.resize(image_count);
            vkGetSwapchainImagesKHR(logical_device.handle(), swap_chain, &image_count, _images.data());

            _image_format = surface_format.format;

            logger.success() << "Swap chain initialized";
        }

        inline ~SwapChain() {
            if (swap_chain != VK_NULL_HANDLE) {
                logger.debug() << "Destroying swap chain...";

                vkDestroySwapchainKHR(logical_device.handle(), swap_chain, nullptr);

                logger.success() << "Swap chain destroyed";
            }
        }

        [[nodiscard]] constexpr const VkSwapchainKHR & handle() const noexcept { return swap_chain; }
        [[nodiscard]] constexpr const std::vector<VkImage> & images() const noexcept { return _images; }
        [[nodiscard]] constexpr const VkFormat & image_format() const noexcept { return _image_format; }

        [[nodiscard]] inline SwapChainIndex next_image_index(Sync::Semaphore & image_available_semaphore) {
            SwapChainIndex index;
            Util::throw_on_fail<Exceptions::GetNextImageError>(
                vkAcquireNextImageKHR(
                    logical_device.handle(),
                    swap_chain,
                    UINT64_MAX,
                    image_available_semaphore.handle(),
                    VK_NULL_HANDLE,
                    &index
                )
            );
            return index;
        }

        inline void present(CommandQueue & device_queue, Sync::Semaphore & render_complete_semaphore, SwapChainIndex image_index) {
            VkPresentInfoKHR present_info {
                .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = &render_complete_semaphore.handle(),
                .swapchainCount = 1,
                .pSwapchains = &swap_chain,
                .pImageIndices = &image_index,
                .pResults = nullptr,
            };

            Util::throw_on_fail<Exceptions::PresentQueueError>(
                vkQueuePresentKHR(device_queue.handle(), &present_info)
            );
        }
    };
}