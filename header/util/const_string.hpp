#pragma once

#include <algorithm>

namespace PKEngine::Util {
    template<std::size_t _size>
    class ConstString {
    public:
        char data[_size];

        ConstString() = delete;

        consteval ConstString(const char (& _data)[_size]) noexcept { std::copy_n(_data, _size, data); }
        template<std::size_t... sizes>
        consteval ConstString(const char (&... _data)[sizes]) noexcept {
            std::size_t i = 0;
            ((std::copy_n(_data, sizes, data + i), i += sizes - 1), ...);
            data[i] = '\0';
        }

        inline operator const char *() const noexcept { return data; }

        static consteval std::size_t static_size() noexcept { return _size; }
        [[nodiscard]] consteval std::size_t size() const noexcept { return _size; }
    };

    template<std::size_t an, std::size_t bn>
    consteval ConstString<an + bn - 1> operator+(ConstString<an> a, ConstString<bn> b) noexcept {
        return ConstString<an + bn - 1>(a.data, b.data);
    }
    template<std::size_t an, std::size_t bn>
    consteval ConstString<an + bn - 1> operator+(ConstString<an> a, const char (& b)[bn]) noexcept {
        return ConstString<an + bn - 1>(a.data, b);
    }
    template<std::size_t an, std::size_t bn>
    consteval ConstString<an + bn - 1> operator+(const char (& a)[an], ConstString<bn> b) noexcept {
        return ConstString<an + bn - 1>(a, b.data);
    }

    namespace _ConstStringPrivate {
        template<typename T> consteval T const_abs(T num) noexcept { return num < 0 ? -num : num; }

        template<typename T> consteval std::size_t const_num_size(T num) noexcept {
            if (num < 0) return const_num_size(-num) + 1;
            else if (num < 10) return 1;
            else return 1 + const_num_size(num / 10);
        }
    }

    using namespace _ConstStringPrivate;

    template<auto num> requires(num < 0)
    consteval ConstString<const_num_size(num) + 1> const_itoa() noexcept { return "-" + const_itoa<-num>(); }
    template<auto num> requires(num >= 0 && num < 10)
    consteval ConstString<const_num_size(num) + 1> const_itoa() noexcept { return ConstString({ '0' + num, '\0' }); }
    template<auto num> requires(num >= 10)
    consteval ConstString<const_num_size(num) + 1> const_itoa() noexcept { return const_itoa<num / 10>() + ConstString({ '0' + (num % 10), '\0' }); }
}