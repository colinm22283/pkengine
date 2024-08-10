#pragma once

#include "pkengine/math/matrix4.hpp"

namespace PKEngine::Vulkan {
    class __attribute__((packed)) Mat4 {
    protected:
        float data[4][4];

    public:
        inline Mat4 & operator=(const Matrix4 & other) noexcept {
            std::memcpy(data, other.data, 4 * 4 * sizeof(float)); return *this;
        }
        inline Mat4 & operator=(const Matrix4 && other) noexcept {
            std::memcpy(data, other.data, 4 * 4 * sizeof(float)); return *this;
        }
    };
}