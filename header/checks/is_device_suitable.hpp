#pragma once

#include <vulkan/vulkan.h>

#include <query/query_queue_families.hpp>
#include <query/query_swap_chain_support.hpp>

#include <checks/check_device_extension_support.hpp>

namespace PKEngine::Checks {
    inline bool is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
        Query::queue_family_indices_t indices = Query::query_queue_families(device, surface);

        bool extensions_supported = check_device_extension_support(device);

        bool swap_chain_adequate = false;
        if (extensions_supported) {
            swap_chain_support_info_t swapChainSupport = query_swap_chain_support(device, surface);
            swap_chain_adequate = !swapChainSupport.formats.empty() && !swapChainSupport.present_modes.empty();
        }

        return indices.is_complete() && extensions_supported && swap_chain_adequate;
    }
}