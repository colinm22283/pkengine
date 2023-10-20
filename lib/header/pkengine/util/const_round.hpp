#pragma once

namespace PKEngine::Util {
    consteval int const_round(float x) {
        float diff = x - (int) x;

        if (diff > 0.5f) return (int) x + 1;
        else  return (int) x;
    }
}