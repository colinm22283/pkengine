#pragma once

namespace PKEngine {
    struct Vector3 {
        float x, y, z;

        constexpr Vector3() noexcept: x(0), y(0), z(0) { }
        constexpr Vector3(float _x, float _y, float _z) noexcept: x(_x), y(_y), z(_z) { }
        constexpr Vector3(Vector3 const & v) noexcept = default;
        constexpr Vector3(Vector3 && v) noexcept = default;

        constexpr Vector3 operator+(const Vector3 & v) const noexcept { return { x + v.x, y + v.y, z + v.z }; }
        constexpr Vector3 operator+(const Vector3 && v) const noexcept { return { x + v.x, y + v.y, z + v.z }; }
        constexpr Vector3 operator-(const Vector3 & v) const noexcept { return { x - v.x, y - v.y, z - v.z }; }
        constexpr Vector3 operator-(const Vector3 && v) const noexcept { return { x - v.x, y - v.y, z - v.z }; }
        constexpr Vector3 operator*(const float & v) const noexcept { return { x * v, y * v, z * v }; }
        constexpr Vector3 operator*(const float && v) const noexcept { return { x * v, y * v, z * v }; }
        constexpr Vector3 operator/(const float & v) const noexcept { return { x / v, y / v, z / v }; }
        constexpr Vector3 operator/(const float && v) const noexcept { return { x / v, y / v, z / v }; }

        constexpr Vector3 & operator+=(Vector3 & v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
        constexpr Vector3 & operator+=(Vector3 && v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
        constexpr Vector3 & operator-=(Vector3 & v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
        constexpr Vector3 & operator-=(Vector3 && v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
        constexpr Vector3 & operator*=(float & v) noexcept { x *= v; y *= v; z *= v; return *this; }
        constexpr Vector3 & operator*=(float && v) noexcept { x *= v; y *= v; z *= v; return *this; }
        constexpr Vector3 & operator/=(float & v) noexcept { x /= v; y /= v; z /= v; return *this; }
        constexpr Vector3 & operator/=(float && v) noexcept { x /= v; y /= v; z /= v; return *this; }

        [[nodiscard]] constexpr float dot(const Vector3 & v) const noexcept { return x * v.x + y * v.y + z * v.z; }
        [[nodiscard]] constexpr float dot(const Vector3 && v) const noexcept { return x * v.x + y * v.y + z * v.z; }
        [[nodiscard]] constexpr Vector3 cross(const Vector3 & v) const noexcept {
            return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        }
        [[nodiscard]] constexpr Vector3 cross(const Vector3 && v) const noexcept {
            return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        }

        [[nodiscard]] constexpr float magnitude() const noexcept { return std::sqrt((x * x) + (y * y) + (z * z)); }
        [[nodiscard]] constexpr Vector3 normalized() const noexcept { return *this / magnitude(); }

        constexpr Vector3 & operator=(Vector3 const & v) noexcept = default;
        constexpr Vector3 & operator=(Vector3 && v) noexcept = default;
    };
}