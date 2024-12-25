#pragma once

#include <pkengine/vulkan/shader_struct/vec3.hpp>
#include <pkengine/vulkan/shader_struct/vec4.hpp>
#include <pkengine/vulkan/shader_struct/mat4.hpp>

namespace PKEngine::Vulkan {
    class CameraData {
    public:
        ShaderStruct::Vec3 position;
        ShaderStruct::Mat4 perspective;

        inline CameraData(ShaderStruct::Vec3 _position, float fov, float aspect, float far, float near):
            position(_position),
            perspective(ShaderStruct::mat4_perspective(fov, aspect, far, near)) { }

        inline void set_perspective(float fov, float aspect, float far, float near) {
            perspective = ShaderStruct::mat4_perspective(fov, aspect, far, near);
        }

        [[nodiscard]] inline ShaderStruct::Mat4 world_mat() const {
            return perspective * ShaderStruct::mat4_translation(-position);
        }
    };
}
