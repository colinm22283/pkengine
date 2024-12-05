#pragma once

#include <array>

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan {
    static constexpr std::array<const char *, 2> required_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
    };
}