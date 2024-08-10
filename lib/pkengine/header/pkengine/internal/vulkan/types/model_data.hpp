#pragma once

#include <pkengine/internal/vulkan/types/matrix.hpp>

namespace PKEngine::Vulkan {
    struct __attribute__((packed)) ModelData {
        Mat4 mat;
    };
}