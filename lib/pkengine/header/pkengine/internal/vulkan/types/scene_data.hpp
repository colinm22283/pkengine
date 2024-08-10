#pragma once

#include "pkengine/internal/vulkan/types/matrix.hpp"

namespace PKEngine::Vulkan {
    struct __attribute__((packed)) SceneData {
        Mat4 view;
        Mat4 proj;
    };
}