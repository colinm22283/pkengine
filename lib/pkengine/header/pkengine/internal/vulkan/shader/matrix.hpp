#pragma once

namespace PKEngine::Vulkan {
    class __attribute__((packed)) Mat4 {
    protected:
        float data[4][4];

    public:
        [[nodiscard]] inline float & at(unsigned int x, unsigned int y) noexcept { return data[x][y]; }
    };
}