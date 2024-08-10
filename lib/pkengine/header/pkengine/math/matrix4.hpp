#pragma once

#include <cstdint>

#include <pkengine/math/vector3.hpp>

namespace PKEngine {
    class Matrix4 {
    public:
        float data[4][4];

        inline float & get(std::size_t x, std::size_t y) noexcept { return data[x][y]; }

        inline void set_identity() noexcept {
            for (std::size_t x = 0; x < 4; x++) {
                for (std::size_t y = 0; y < 4; y++) data[x][y] = x == y;
            }
        }

        [[nodiscard]] constexpr Matrix4 operator*(const Matrix4 & other) const noexcept {
            Matrix4 mat;

            for (std::size_t x = 0; x < 4; x++) {
                for (std::size_t y = 0; y < 4; y++) {
                    float temp = 0;

                    for (std::size_t i = 0; i < 4; i++) {
                        temp += data[x][i] * other.data[i][y];
                    }

                    mat.data[x][y] = temp;
                }
            }

            return mat;
        }
    };

    constexpr Matrix4 identity_matrix() noexcept {
        Matrix4 mat;

        for (std::size_t x = 0; x < 4; x++) {
            for (std::size_t y = 0; y < 4; y++) mat.data[x][y] = x == y;
        }

        return mat;
    }

    constexpr Matrix4 translation_matrix(float _x, float _y, float _z) {
        Matrix4 mat;

        for (std::size_t x = 0; x < 4; x++) {
            for (std::size_t y = 0; y < 4; y++) {
                if (x == 3) {
                    switch (y) {
                        case 0: mat.data[x][y] = _x; break;
                        case 1: mat.data[x][y] = _y; break;
                        case 2: mat.data[x][y] = _z; break;
                        case 3: mat.data[x][y] = 1; break;

                        default: break;
                    }
                }
                else mat.data[x][y] = x == y;
            }
        }

        return mat;
    }

    constexpr Matrix4 set_rotation_matrix_x(float theta) {
        Matrix4 mat;

        mat.data[0][0] = 1;
        mat.data[0][1] = 0;
        mat.data[0][2] = 0;
        mat.data[0][3] = 0;

        mat.data[1][0] = 0;
        mat.data[1][1] = std::cos(theta);
        mat.data[1][2] = std::sin(theta);
        mat.data[1][3] = 0;

        mat.data[2][0] = 0;
        mat.data[2][1] = -std::sin(theta);
        mat.data[2][2] = std::cos(theta);
        mat.data[2][3] = 0;

        mat.data[3][0] = 0;
        mat.data[3][1] = 0;
        mat.data[3][2] = 0;
        mat.data[3][3] = 1;

        return mat;
    }

    constexpr Matrix4 set_rotation_matrix_y(float theta) {
        Matrix4 mat;

        mat.data[0][0] = std::cos(theta);
        mat.data[0][1] = 0;
        mat.data[0][2] = -std::sin(theta);
        mat.data[0][3] = 0;

        mat.data[1][0] = 0;
        mat.data[1][1] = 1;
        mat.data[1][2] = 0;
        mat.data[1][3] = 0;

        mat.data[2][0] = std::sin(theta);
        mat.data[2][1] = 0;
        mat.data[2][2] = std::cos(theta);
        mat.data[2][3] = 0;

        mat.data[3][0] = 0;
        mat.data[3][1] = 0;
        mat.data[3][2] = 0;
        mat.data[3][3] = 1;

        return mat;
    }

    constexpr Matrix4 set_rotation_matrix_z(float theta) {
        Matrix4 mat;

        mat.data[0][0] = std::cos(theta);
        mat.data[0][1] = std::sin(theta);
        mat.data[0][2] = 0;
        mat.data[0][3] = 0;

        mat.data[1][0] = -std::sin(theta);
        mat.data[1][1] = std::cos(theta);
        mat.data[1][2] = 0;
        mat.data[1][3] = 0;

        mat.data[2][0] = 0;
        mat.data[2][1] = 0;
        mat.data[2][2] = 1;
        mat.data[2][3] = 0;

        mat.data[3][0] = 0;
        mat.data[3][1] = 0;
        mat.data[3][2] = 0;
        mat.data[3][3] = 1;

        return mat;
    }

    constexpr Matrix4 rotation_matrix(float _x, float _y, float _z) {
        return set_rotation_matrix_z(_z) * set_rotation_matrix_y(_y) * set_rotation_matrix_x(_x);
    }

    constexpr Matrix4 scale_matrix(float _x, float _y, float _z) {
        Matrix4 mat;

        for (std::size_t x = 0; x < 4; x++) {
            for (std::size_t y = 0; y < 4; y++) {
                if (x == y) {
                    switch (x) {
                        case 0:
                            mat.data[x][y] = _x;
                            break;
                        case 1:
                            mat.data[x][y] = _y;
                            break;
                        case 2:
                            mat.data[x][y] = _z;
                            break;
                        case 3:
                            mat.data[x][y] = 1;
                            break;

                        default:
                            break;
                    }
                }
                else mat.data[x][y] = 0;
            }
        }

        return mat;
    }

    constexpr Matrix4 perspective_matrix(float fov, float aspect, float near, float far) {
        Matrix4 mat;

        float inv_tan_fov_2 = 1.0f / std::tan(fov / 2);
        float inv_far_sub_near = 1.0f / (far - near);

        mat.data[0][0] = (1.0f / aspect) * inv_tan_fov_2;
        mat.data[0][1] = 0;
        mat.data[0][2] = 0;
        mat.data[0][3] = 0;

        mat.data[1][0] = 0;
        mat.data[1][1] = inv_tan_fov_2;
        mat.data[1][2] = 0;
        mat.data[1][3] = 0;

        mat.data[2][0] = 0;
        mat.data[2][1] = 0;
        mat.data[2][2] = -(far + near) * inv_far_sub_near;
        mat.data[2][3] = -1;

        mat.data[3][0] = 0;
        mat.data[3][1] = 0;
        mat.data[3][2] = -2 * far * near * inv_far_sub_near;
        mat.data[3][3] = 0;

        return mat;
    }
}