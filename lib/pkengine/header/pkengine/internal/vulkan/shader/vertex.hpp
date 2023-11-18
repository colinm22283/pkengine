#pragma once

#include "pkengine/internal/vulkan/pipeline/bind_info.hpp"

#include "pkengine/math/vector3.hpp"

namespace PKEngine::Vulkan {
    class __attribute__((packed)) Vertex {
    public:
        float x, y, z;

        constexpr Vertex(): x(0), y(0), z(0) { }
        constexpr Vertex(float _x, float _y, float _z): x(_x), y(_y), z(_z) { }
        constexpr ~Vertex() noexcept = default;
        constexpr Vertex(const Vertex & v) noexcept = default;
        constexpr Vertex(Vertex && v) noexcept = default;

        constexpr Vertex(Vector3 & v) noexcept: x(v.x), y(v.y), z(v.z) { }
        constexpr Vertex(Vector3 && v) noexcept: x(v.x), y(v.y), z(v.z) { }

        constexpr Vertex operator+(Vertex & v) const noexcept { return { x + v.x, y + v.y, z + v.z }; }
        constexpr Vertex operator+(Vertex && v) const noexcept { return { x + v.x, y + v.y, z + v.z }; }
        constexpr Vertex operator-(Vertex & v) const noexcept { return { x - v.x, y - v.y, z - v.z }; }
        constexpr Vertex operator-(Vertex && v) const noexcept { return { x - v.x, y - v.y, z - v.z }; }
        constexpr Vertex operator*(float & v) const noexcept { return { x * v, y * v, z * v }; }
        constexpr Vertex operator*(float && v) const noexcept { return { x * v, y * v, z * v }; }
        constexpr Vertex operator/(float & v) const noexcept { return { x / v, y / v, z / v }; }
        constexpr Vertex operator/(float && v) const noexcept { return { x / v, y / v, z / v }; }

        constexpr Vertex & operator+=(Vertex & v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
        constexpr Vertex & operator+=(Vertex && v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
        constexpr Vertex & operator-=(Vertex & v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
        constexpr Vertex & operator-=(Vertex && v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
        constexpr Vertex & operator*=(float & v) noexcept { x *= v; y *= v; z *= v; return *this; }
        constexpr Vertex & operator*=(float && v) noexcept { x *= v; y *= v; z *= v; return *this; }
        constexpr Vertex & operator/=(float & v) noexcept { x /= v; y /= v; z /= v; return *this; }
        constexpr Vertex & operator/=(float && v) noexcept { x /= v; y /= v; z /= v; return *this; }

        constexpr float dot_prod(Vertex & v) const noexcept { return x * v.x + y * v.y + z * v.z; }
        constexpr float dot_prod(Vertex && v) const noexcept { return x * v.x + y * v.y + z * v.z; }

        constexpr Vertex & operator=(Vertex const & v) = default;
        constexpr Vertex & operator=(Vertex && v) = default;

        [[nodiscard]] static consteval auto get_bind_info() noexcept {
            return Pipeline::bind_info_t<1> {
                .binding_desc = {
                    .binding = 0,
                    .stride = sizeof(Vertex),
                    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
                },
                .attribute_desc = {
                    VkVertexInputAttributeDescription {
                        .location = 0,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32B32_SFLOAT,
                        .offset = offsetof(Vertex, x),
                    },
                },
            };
        }
    };
}