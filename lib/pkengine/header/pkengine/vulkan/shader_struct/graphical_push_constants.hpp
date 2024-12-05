#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/vulkan/shader_struct/mat4.hpp>

namespace PKEngine::Vulkan::ShaderStruct {
    struct GraphicalPushConstants {
        Mat4 world_matrix;
        VkDeviceAddress vertex_buffer;
    };
}