#pragma once

#include <pkengine/internal/vulkan/shader/matrix.hpp>

namespace PKEngine::Vulkan {
    struct UniformBufferObject {
        Mat4 model;
        Mat4 view;
        Mat4 proj;
    };
}