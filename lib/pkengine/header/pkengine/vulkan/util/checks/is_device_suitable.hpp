#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/vulkan/wrapper/surface.hpp>
#include <pkengine/vulkan/util/queue_family_indices.hpp>
#include <pkengine/vulkan/util/swap_chain_support.hpp>
#include <pkengine/vulkan/util/checks/are_extensions_supported.hpp>

namespace PKEngine::Vulkan::Util::Checks {
    inline bool is_device_suitable(Wrapper::Surface & surface, VkPhysicalDevice device) {
        QueueFamilyIndices indices(surface, device);

        bool extensions_supported = are_extensions_supported(device);

        bool swap_chain_adequate = false;
        if (extensions_supported) {
            SwapChainSupport swap_chain_support(surface, device);
            swap_chain_adequate = !swap_chain_support.formats().empty() && !swap_chain_support.present_modes().empty();
        }

        return indices.is_complete() && extensions_supported && swap_chain_adequate;
    }
}