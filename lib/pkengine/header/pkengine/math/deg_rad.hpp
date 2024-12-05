#pragma once

#include <numbers>

namespace PKEngine::Math {
    constexpr float deg_to_rad(float deg) {
        return deg * std::numbers::pi_v<float> / 180;
    }

    constexpr float rad_to_deg(float rad) {
        return rad * 180 / std::numbers::pi_v<float>;
    }
}