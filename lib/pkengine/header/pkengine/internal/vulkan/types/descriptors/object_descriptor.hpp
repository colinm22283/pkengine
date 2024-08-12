#pragma once

#include <pkengine/internal/vulkan/types/matrix.hpp>

namespace PKEngine::Vulkan::Descriptors {
    struct __attribute__((packed)) ObjectDescriptor {
        Mat4 mat;
    };
}