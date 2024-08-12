#pragma once

#include <pkengine/internal/vulkan/types/matrix.hpp>

namespace PKEngine::Vulkan::Descriptors {
    struct __attribute__((packed)) GlobalDescriptor {
        Mat4 view;
        Mat4 proj;
    };
}