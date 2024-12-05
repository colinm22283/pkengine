#pragma once

#include <pkengine/vulkan/shader_struct/vec3.hpp>
#include <pkengine/vulkan/shader_struct/vec4.hpp>

namespace PKEngine::Vulkan::ShaderStruct {
    struct __attribute__((packed)) Vertex {
        Vec3 position;
        float uv_x;
        Vec3 normal;
        float uv_y;
        Vec4 color;
    };
}