#pragma once

#include <algorithm>
#include <string_view>

namespace PKEngine::Util {
    template<std::size_t n>
    class ConstString {
    public:
        char _data[n];

        consteval ConstString() { }

        consteval ConstString(const char (& __data)[n]) {
            for (std::size_t i = 0; i < n; i++) _data[i] = __data[i];
        }

        consteval ConstString(const char (&& __data)[n]) {
            for (std::size_t i = 0; i < n; i++) _data[i] = __data[i];
        }

        consteval ConstString(const ConstString<n> & str) {
            for (std::size_t i = 0; i < n; i++) _data[i] = str._data[i];
        }

        consteval ConstString(const ConstString<n> && str) {
            for (std::size_t i = 0; i < n; i++) _data[i] = str._data[i];
        }

        [[nodiscard]] inline const std::string_view string_view() const noexcept {
            return std::string_view(_data, n - 1);
        }

        [[nodiscard]] inline operator const std::string_view() const noexcept { return string_view(); }

        [[nodiscard]] constexpr const char & operator[](std::size_t index) const noexcept { return _data[index]; }

        [[nodiscard]] constexpr const char * c_str() const noexcept { return _data; }

        [[nodiscard]] consteval std::size_t size() const noexcept { return n - 1; }
    };

    template<std::size_t na, std::size_t nb>
    [[nodiscard]] consteval ConstString<na + nb - 1>
    operator+(const ConstString<na> a, const ConstString<nb> b) noexcept {
        char temp[na + nb - 1];
        std::copy_n(a._data, na - 1, temp);
        std::copy_n(b._data, nb, temp + na - 1);
        return temp;
    }

    template<std::size_t na, std::size_t nb>
    [[nodiscard]] consteval ConstString<na + nb - 1> operator+(const ConstString<na> a, const char (& b)[nb]) noexcept {
        char temp[na + nb - 1];
        std::copy_n(a._data, na - 1, temp);
        std::copy_n(b, nb, temp + na - 1);
        return temp;
    }

    template<ConstString a, auto repeat_count>
    static constexpr auto const_string_repeat = []() {
        if constexpr (repeat_count == 0) return ConstString("");
        else if constexpr (repeat_count == 1) return a;
        else {
            return const_string_repeat < a, repeat_count - 1 > +a;
        }
    }();

    template<unsigned long long val>
    consteval auto const_utoa() {
        ConstString<2> lowest;

        lowest._data[0] = '0' + (val % 10);
        lowest._data[1] = '\0';

        if constexpr (val < 10) return lowest;
        else return const_utoa<val / 10>() + lowest;
    }
}