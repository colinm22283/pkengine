#pragma once

#include <cstdint>

#include <array>

namespace PKEngine::Util {
    template<typename T, std::size_t n, typename... Args>
    constexpr std::array<T, n> make_array(Args &... args) {
        return [&args...]<std::size_t... Is>(std::index_sequence<Is...> index_sequence) {
            return std::array<T, n>({ (static_cast<void>(Is), T(args...))... });
        }(std::make_index_sequence<n>());
    }
}