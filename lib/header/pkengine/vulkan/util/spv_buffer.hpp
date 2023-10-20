#pragma once

#include <cstdint>
#include <algorithm>

namespace PKEngine::Vulkan {
    template<std::size_t _size>
    class SPVBuffer {
    protected:
        char _data[_size];

    public:
        SPVBuffer() = delete;

        explicit consteval SPVBuffer(const char (& data_in)[_size]) noexcept: _data({ 0 }) {
            std::copy_n(data_in, _size, _data);
        }

        [[nodiscard]] consteval std::size_t size() const noexcept { return _size; }
        [[nodiscard]] consteval const char * data() const noexcept { return _data; }
    };
}