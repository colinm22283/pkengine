#pragma once

#include "../pipeline/bind_info.hpp"

namespace PKEngine::Vulkan {
    class __attribute__((packed)) Vertex {
    public:
        float x, y, z;

        Vertex(): x(0), y(0), z(0) { }
        Vertex(float _x, float _y, float _z): x(_x), y(_y), z(_z) { }

        inline Vertex operator+(Vertex & v) const noexcept { return Vertex(x + v.x, y + v.y, z + v.z); }
        inline Vertex operator+(Vertex && v) const noexcept { return Vertex(x + v.x, y + v.y, z + v.z); }
        inline Vertex operator-(Vertex & v) const noexcept { return Vertex(x - v.x, y - v.y, z - v.z); }
        inline Vertex operator-(Vertex && v) const noexcept { return Vertex(x - v.x, y - v.y, z - v.z); }
        inline Vertex operator*(float & v) const noexcept { return Vertex(x * v, y * v, z * v); }
        inline Vertex operator*(float && v) const noexcept { return Vertex(x * v, y * v, z * v); }
        inline Vertex operator/(float & v) const noexcept { return Vertex(x / v, y / v, z / v); }
        inline Vertex operator/(float && v) const noexcept { return Vertex(x / v, y / v, z / v); }

        inline Vertex & operator+=(Vertex & v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
        inline Vertex & operator+=(Vertex && v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
        inline Vertex & operator-=(Vertex & v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
        inline Vertex & operator-=(Vertex && v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
        inline Vertex & operator*=(float & v) noexcept { x *= v; y *= v; z *= v; return *this; }
        inline Vertex & operator*=(float && v) noexcept { x *= v; y *= v; z *= v; return *this; }
        inline Vertex & operator/=(float & v) noexcept { x /= v; y /= v; z /= v; return *this; }
        inline Vertex & operator/=(float && v) noexcept { x /= v; y /= v; z /= v; return *this; }

        inline float dot_prod(Vertex & v) const noexcept { return x * v.x + y * v.y + z * v.z; }
        inline float dot_prod(Vertex && v) const noexcept { return x * v.x + y * v.y + z * v.z; }

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