#pragma once

namespace PKEngine::Vulkan::ShaderStruct {
    struct Mat4 {
        float values[4 * 4];
    };

    constexpr Mat4 mat4_identity = Mat4 {
        .values = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        },
    };
}