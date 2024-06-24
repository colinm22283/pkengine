#pragma once

namespace PKEngine::Vulkan {
    class __attribute__((packed)) Mat4 {
    public:
        float data[4][4];

        inline void set_identity() noexcept {
            for (std::size_t x = 0; x < 4; x++) {
                for (std::size_t y = 0; y < 4; y++) data[x][y] = x == y;
            }
        }
        inline void set_scaling_matrix(float _x, float _y, float _z) noexcept {
            for (std::size_t x = 0; x < 4; x++) {
                for (std::size_t y = 0; y < 4; y++) {
                    if (x == y) {
                        switch (x) {
                            case 0:
                                data[x][y] = _x;
                                break;
                            case 1:
                                data[x][y] = _y;
                                break;
                            case 2:
                                data[x][y] = _z;
                                break;
                            case 3:
                                data[x][y] = 1;
                                break;

                            default:
                                break;
                        }
                    }
                    else data[x][y] = 0;
                }
            }
        }
    };
}