#pragma once

#include <pkengine/util/const_string.hpp>

namespace PKEngine::Util {
    template<typename... Elements>
    class _ANSIColor {
    protected:
        template<ConstString _string>
        struct Element {
            static constexpr decltype(_string) string = _string;
        };

        template<typename ele, typename... eles>
        struct ElementJoiner {
            static constexpr auto string = []() {
                if constexpr (sizeof...(eles) == 0) return ele::string;
                else {
                    return ele::string + ";" + ElementJoiner<eles...>::string;
                }
            }();
        };

    public:
        using Reset = _ANSIColor<Elements..., Element<"0">>;
        using Bold = _ANSIColor<Elements..., Element<"1">>;
        using Dimmed = _ANSIColor<Elements..., Element<"2">>;

        using BlackFg = _ANSIColor<Elements..., Element<"30">>;
        using RedFg = _ANSIColor<Elements..., Element<"31">>;
        using GreenFg = _ANSIColor<Elements..., Element<"32">>;
        using YellowFg = _ANSIColor<Elements..., Element<"33">>;
        using BlueFg = _ANSIColor<Elements..., Element<"34">>;
        using MagentaFg = _ANSIColor<Elements..., Element<"35">>;
        using CyanFg = _ANSIColor<Elements..., Element<"36">>;
        using WhiteFg = _ANSIColor<Elements..., Element<"37">>;
        using DefaultFg = _ANSIColor<Elements..., Element<"39">>;

        using BlackBg = _ANSIColor<Elements..., Element<"40">>;
        using RedBg = _ANSIColor<Elements..., Element<"41">>;
        using GreenBg = _ANSIColor<Elements..., Element<"42">>;
        using YellowBg = _ANSIColor<Elements..., Element<"43">>;
        using BlueBg = _ANSIColor<Elements..., Element<"44">>;
        using MagentaBg = _ANSIColor<Elements..., Element<"45">>;
        using CyanBg = _ANSIColor<Elements..., Element<"46">>;
        using WhiteBg = _ANSIColor<Elements..., Element<"47">>;
        using DefaultBg = _ANSIColor<Elements..., Element<"49">>;

        template<unsigned char r, unsigned char g, unsigned char b>
        using RGBFg = _ANSIColor<Elements..., Element<"38">, Element<"2">, Element<const_utoa<r>()>, Element<const_utoa<g>()>, Element<const_utoa<b>()>>;

        template<unsigned char r, unsigned char g, unsigned char b>
        using RGBBg = _ANSIColor<Elements..., Element<"48">, Element<"2">, Element<const_utoa<r>()>, Element<const_utoa<g>()>, Element<const_utoa<b>()>>;

        static constexpr auto const_string = []() {
            if constexpr (sizeof...(Elements) == 0) return ConstString("");
            else return ConstString("\033[") + ElementJoiner<Elements...>::string + "m";
        }();

        static inline std::string_view string_view() noexcept { return const_string.string_view(); }

        static inline const char * c_str() noexcept { return const_string.c_str(); }
    };

    using ANSIColor = _ANSIColor<>;
}