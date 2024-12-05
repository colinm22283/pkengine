#pragma once

#include <array>

namespace PKEngine::Vulkan {
#ifdef ENABLE_VALIDATION_LAYERS
    static constexpr std::array<const char *, 1> validation_layers = {
        "VK_LAYER_KHRONOS_validation",
    };
#else
    static constexpr std::array<const char *, 0> validation_layers = {};
#endif
}