#pragma once

#include <pkengine/vulkan/shader_struct/mat4.hpp>
#include <pkengine/vulkan/shader_struct/vec4.hpp>

namespace PKEngine::Vulkan::ShaderStruct {
    struct SceneData {
        Mat4 view;
        Mat4 proj;
        Mat4 view_proj;
    };
}