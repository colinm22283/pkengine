#pragma once

namespace PKEngine::Vulkan::ShaderStruct {
    struct __attribute__((packed)) Vec3 {
        float x, y, z;

        [[nodiscard]] constexpr Vec3 operator-() const noexcept { return Vec3 {  -x, -y, -z, }; }
        [[nodiscard]] constexpr Vec3 operator-(Vec3 v) const noexcept { return Vec3 { x - v.x, y - v.y, z - v.z, }; }
        [[nodiscard]] constexpr Vec3 operator*(float v) const noexcept { return Vec3 { v * x, v * y, v * z, }; }
    };
}