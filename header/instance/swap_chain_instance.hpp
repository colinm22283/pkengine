#pragma once

#include <vulkan/vulkan.h>

#include <logger/logger.hpp>

#include <query/query_swap_chain_support.hpp>
#include <query/query_queue_families.hpp>

#include <util/choose_swap_surface_format.hpp>
#include <util/choose_swap_present_mode.hpp>
#include <util/choose_swap_extent.hpp>

namespace PKEngine {
    class SwapChainInstance {
    protected:
        static constexpr auto logger = FileLogger<Util::ANSI::CyanFg, "SC INST", "pkengine.log">();

        VkSwapchainKHR _handle;
        VkDevice device;

    public:
        std::vector<VkImage> images;
        VkFormat image_format;
        VkExtent2D extent;

        inline SwapChainInstance(VkPhysicalDevice physical_device, VkDevice _device, VkSurfaceKHR surface, GLFWwindow * window):
          device(_device) {
            logger.log<"Creating swap chain...">();

            swap_chain_support_info_t swapChainSupport = query_swap_chain_support(physical_device, surface);

            VkSurfaceFormatKHR surfaceFormat = choose_swap_surface_format(swapChainSupport.formats);
            VkPresentModeKHR presentMode = choose_swap_present_mode(swapChainSupport.present_modes);
            VkExtent2D _extent = choose_swap_extent(window, swapChainSupport.capabilities);

            uint32_t image_count = swapChainSupport.capabilities.minImageCount + 1;

            if (swapChainSupport.capabilities.maxImageCount > 0 && image_count > swapChainSupport.capabilities.maxImageCount) {
                image_count = swapChainSupport.capabilities.maxImageCount;
            }

            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = surface;
            createInfo.minImageCount = image_count;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = _extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            Query::queue_family_indices_t indices = Query::query_queue_families(physical_device, surface);
            uint32_t queueFamilyIndices[] = { indices.graphics_family.value(), indices.present_family.value() };

            if (indices.graphics_family != indices.present_family) {
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

            if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &_handle) != VK_SUCCESS) {
                throw Exceptions::UnableToCreateSwapChain();
            }

            vkGetSwapchainImagesKHR(device, _handle, &image_count, nullptr);
            images.resize(image_count);
            vkGetSwapchainImagesKHR(device, _handle, &image_count, images.data());

            image_format = surfaceFormat.format;
            extent = _extent;

            logger.success().log<"Done!">();
        }
        inline ~SwapChainInstance() {
            if (_handle) vkDestroySwapchainKHR(device, _handle, nullptr);

            logger.log<"Swap Chain Destroyed">();
        }

        inline VkSwapchainKHR handle() { return _handle; }
        inline operator VkSwapchainKHR() { return handle(); }
    };
}