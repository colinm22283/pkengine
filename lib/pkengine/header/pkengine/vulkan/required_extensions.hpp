#pragma once

#include <array>

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan {
    static const std::array<std::string_view, 1> required_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };
}