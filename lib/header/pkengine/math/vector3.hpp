#pragma once

namespace PKEngine {
    struct Vector3 {
        float x, y, z;

        constexpr Vector3(): x(0), y(0), z(0) { }
        constexpr Vector3(float _x, float _y, float _z): x(_x), y(_y), z(_z) { }
        constexpr Vector3(Vector3 const & v) noexcept = default;
        constexpr Vector3(Vector3 && v) noexcept = default;

        constexpr Vector3 operator+(Vector3 & v) const noexcept { return { x + v.x, y + v.y, z + v.z }; }
        constexpr Vector3 operator+(Vector3 && v) const noexcept { return { x + v.x, y + v.y, z + v.z }; }
        constexpr Vector3 operator-(Vector3 & v) const noexcept { return { x - v.x, y - v.y, z - v.z }; }
        constexpr Vector3 operator-(Vector3 && v) const noexcept { return { x - v.x, y - v.y, z - v.z }; }
        constexpr Vector3 operator*(float & v) const noexcept { return { x * v, y * v, z * v }; }
        constexpr Vector3 operator*(float && v) const noexcept { return { x * v, y * v, z * v }; }
        constexpr Vector3 operator/(float & v) const noexcept { return { x / v, y / v, z / v }; }
        constexpr Vector3 operator/(float && v) const noexcept { return { x / v, y / v, z / v }; }

        constexpr Vector3 & operator+=(Vector3 & v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
        constexpr Vector3 & operator+=(Vector3 && v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
        constexpr Vector3 & operator-=(Vector3 & v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
        constexpr Vector3 & operator-=(Vector3 && v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
        constexpr Vector3 & operator*=(float & v) noexcept { x *= v; y *= v; z *= v; return *this; }
        constexpr Vector3 & operator*=(float && v) noexcept { x *= v; y *= v; z *= v; return *this; }
        constexpr Vector3 & operator/=(float & v) noexcept { x /= v; y /= v; z /= v; return *this; }
        constexpr Vector3 & operator/=(float && v) noexcept { x /= v; y /= v; z /= v; return *this; }

        constexpr float dot_prod(Vector3 & v) const noexcept { return x * v.x + y * v.y + z * v.z; }
        constexpr float dot_prod(Vector3 && v) const noexcept { return x * v.x + y * v.y + z * v.z; }

        constexpr Vector3 & operator=(Vector3 const & v) noexcept = default;
        constexpr Vector3 & operator=(Vector3 && v) noexcept = default;
    };
}