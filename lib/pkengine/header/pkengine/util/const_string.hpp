#pragma once

#include <algorithm>

namespace PKEngine::Util {
    template<std::size_t char_count>
    class ConstString {
    public:
        char data[char_count] = { 0 };

        consteval ConstString(const char (& _data)[char_count]) noexcept {
            std::copy_n(_data, char_count, data);
        }
        template<std::size_t an, std::size_t bn> requires(char_count == an + bn - 1)
        consteval ConstString(const char (& a)[an], const char (& b)[bn]) noexcept {
            std::copy_n(a, an - 1, data);
            std::copy_n(b, bn, data + an - 1);
        }

        [[nodiscard]] inline const char * c_str() const noexcept { return data; }
        [[nodiscard]] inline operator const char *() const noexcept { return data; }
        [[nodiscard]] consteval std::size_t size() const noexcept { return char_count - 1; }
    };

    namespace _ConstStringPrivate {
        template<typename T> consteval T const_abs(T num) noexcept { return num < 0 ? -num : num; }

        template<typename T> consteval std::size_t const_num_size(T num) noexcept {
            if (num < 0) return const_num_size(-num) + 1;
            else if (num < 10) return 1;
            else return 1 + const_num_size(num / 10);
        }
    }
}

template<std::size_t an, std::size_t bn>
consteval PKEngine::Util::ConstString<an + bn - 1> operator+(PKEngine::Util::ConstString<an> a, PKEngine::Util::ConstString<bn> b) noexcept {
    return PKEngine::Util::ConstString<an + bn - 1>(a.data, b.data);
}
template<std::size_t an, std::size_t bn>
consteval PKEngine::Util::ConstString<an + bn - 1> operator+(PKEngine::Util::ConstString<an> a, const char (& b)[bn]) noexcept {
    return PKEngine::Util::ConstString<an + bn - 1>(a.data, b);
}
template<std::size_t an, std::size_t bn>
consteval PKEngine::Util::ConstString<an + bn - 1> operator+(const char (& a)[an], PKEngine::Util::ConstString<bn> b) noexcept {
    return PKEngine::Util::ConstString<an + bn - 1>(a, b.data);
}

namespace PKEngine::Util {
    template<auto num> requires(num < 0)
    consteval ConstString<_ConstStringPrivate::const_num_size(num) + 1> const_itoa() noexcept { return "-" + const_itoa<-num>(); }
    template<auto num> requires(num >= 0 && num < 10)
    consteval ConstString<_ConstStringPrivate::const_num_size(num) + 1> const_itoa() noexcept { return ConstString({ '0' + num, '\0' }); }
    template<auto num> requires(num >= 10)
    consteval ConstString<_ConstStringPrivate::const_num_size(num) + 1> const_itoa() noexcept { return const_itoa<num / 10>() + ConstString({ '0' + (num % 10), '\0' }); }
}