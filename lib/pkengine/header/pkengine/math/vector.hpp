#pragma once

#include <pkengine/vulkan/shader_struct/vec3.hpp>

namespace PKEngine::Math {
    class Vector3 {
    public:
        float x, y, z;

        inline Vector3(): x(0), y(0), z(0) { }
        inline Vector3(float value): x(value), y(value), z(value) { }
        inline Vector3(float _x, float _y, float _z): x(_x), y(_y), z(_z) { }

        inline Vector3 operator*(float v) const noexcept {
            return Vector3(x * v, y * v, z * v);
        }

        inline Vector3 operator+(Vector3 v) const noexcept {
            return Vector3(x + v.x, y + v.y, z + v.z);
        }

        inline Vector3 operator-(Vector3 v) const noexcept {
            return Vector3(x - v.x, y - v.y, z - v.z);
        }

        inline Vulkan::ShaderStruct::Vec3 vec3() const noexcept {
            return Vulkan::ShaderStruct::Vec3(x, y, z);
        }
    };
}