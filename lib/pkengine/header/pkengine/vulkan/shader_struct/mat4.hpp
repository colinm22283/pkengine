#pragma once

#include <cmath>

namespace PKEngine::Vulkan::ShaderStruct {
    struct Mat4 {
        float values[4 * 4];

        [[nodiscard]] inline float & at(std::size_t x, std::size_t y) noexcept { return values[y + x * 4]; }
        [[nodiscard]] inline const float & at_const(std::size_t x, std::size_t y) const noexcept { return values[y + x * 4]; }

        [[nodiscard]] constexpr Mat4 operator*(const Mat4 & other) const noexcept {
            Mat4 result = { };

            for (int i = 0; i < 4; i++) { // this y
                for (int j = 0; j < 4; j++) { // other x
                    float acc = 0.0f;

                    for (int k = 0; k < 4; k++) { // this x & other y
                        acc += at_const(k, i) * other.at_const(j, k);
                    }

                    result.at(j, i) = acc;
                }
            }

            return result;
        }
        [[nodiscard]] constexpr Mat4 operator*(const Mat4 && other) const noexcept {
            Mat4 result = { };

            for (int i = 0; i < 4; i++) { // this y
                for (int j = 0; j < 4; j++) { // other x
                    float acc = 0.0f;

                    for (int k = 0; k < 4; k++) { // this x & other y
                        acc += at_const(k, i) * other.at_const(j, k);
                    }

                    result.at(j, i) = acc;
                }
            }

            return result;
        }

        [[nodiscard]] constexpr Mat4 transpose() const noexcept {
            return Mat4 {
                .values = {
                    values[0], values[4], values[8],  values[12],
                    values[1], values[5], values[9],  values[13],
                    values[2], values[6], values[10], values[14],
                    values[3], values[7], values[11], values[15],
                },
            };
        }
    };

    constexpr Mat4 mat4_identity() {
        return Mat4 {
            .values = {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1,
            },
        };
    }

    constexpr Mat4 mat4_translation(const Vec3 & trans) {
        return Mat4 {
            .values = {
                1, 0, 0, trans.x,
                0, 1, 0, trans.y,
                0, 0, 1, trans.z,
                0, 0, 0, 1,
            },
        }.transpose();
    }

    constexpr Mat4 mat4_translation(const Vec3 && trans) {
        return Mat4 {
            .values = {
                1, 0, 0, trans.x,
                0, 1, 0, trans.y,
                0, 0, 1, trans.z,
                0, 0, 0, 1,
            },
        }.transpose();
    }

    constexpr Mat4 mat4_perspective(float fov, float aspect, float far, float near) {
        return Mat4 {
            .values = {
                1.0f / (aspect * std::tan(fov / 2.0f)), 0, 0, 0,
                0, -1.0f / std::tan(fov / 2.0f), 0, 0,
                0, 0, -(far + near) / (far - near), -(2 * far * near) / (far - near),
                0, 0, -1, 0,
            },
        }.transpose();
    }
}