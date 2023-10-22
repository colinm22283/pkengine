#pragma once

#include <cstdint>

#include "const_string.hpp"

namespace PKEngine::Util {
    template<uint8_t code, uint8_t... codes>
    struct ansi_t {
        static constexpr ConstString string = ConstString("\033[") + const_itoa<code>() + ((";" + const_itoa<codes>()) + ...) + "m";

        using Reset = ansi_t<code, codes..., 0>;
        using Bold = ansi_t<code, codes..., 1>;

        using BlackFg = ansi_t<code, codes..., 30>;
        using RedFg = ansi_t<code, codes..., 31>;
        using GreenFg = ansi_t<code, codes..., 32>;
        using YellowFg = ansi_t<code, codes..., 33>;
        using BlueFg = ansi_t<code, codes..., 34>;
        using MagentaFg = ansi_t<code, codes..., 35>;
        using CyanFg = ansi_t<code, codes..., 36>;
        using WhiteFg = ansi_t<code, codes..., 37>;

        using BlackBg = ansi_t<code, codes..., 40>;
        using RedBg = ansi_t<code, codes..., 41>;
        using GreenBg = ansi_t<code, codes..., 42>;
        using YellowBg = ansi_t<code, codes..., 43>;
        using BlueBg = ansi_t<code, codes..., 44>;
        using MagentaBg = ansi_t<code, codes..., 45>;
        using CyanBg = ansi_t<code, codes..., 46>;
        using WhiteBg = ansi_t<code, codes..., 47>;
    };
    template<uint8_t code>
    struct ansi_t<code> {
        static constexpr ConstString string = ConstString("\033[") + const_itoa<code>() + "m";

        using Reset = ansi_t<code, 0>;
        using Bold = ansi_t<code, 1>;

        using BlackFg = ansi_t<code, 30>;
        using RedFg = ansi_t<code, 31>;
        using GreenFg = ansi_t<code, 32>;
        using YellowFg = ansi_t<code, 33>;
        using BlueFg = ansi_t<code, 34>;
        using MagentaFg = ansi_t<code, 35>;
        using CyanFg = ansi_t<code, 36>;
        using WhiteFg = ansi_t<code, 37>;

        using BlackBg = ansi_t<code, 40>;
        using RedBg = ansi_t<code, 41>;
        using GreenBg = ansi_t<code, 42>;
        using YellowBg = ansi_t<code, 43>;
        using BlueBg = ansi_t<code, 44>;
        using MagentaBg = ansi_t<code, 45>;
        using CyanBg = ansi_t<code, 46>;
        using WhiteBg = ansi_t<code, 47>;
    };

    namespace ANSI {
        using Reset = ansi_t<0>;
        using Bold = ansi_t<1>;

        using BlackFg = ansi_t<30>;
        using RedFg = ansi_t<31>;
        using GreenFg = ansi_t<32>;
        using YellowFg = ansi_t<33>;
        using BlueFg = ansi_t<34>;
        using MagentaFg = ansi_t<35>;
        using CyanFg = ansi_t<36>;
        using WhiteFg = ansi_t<37>;

        using BlackBg = ansi_t<40>;
        using RedBg = ansi_t<41>;
        using GreenBg = ansi_t<42>;
        using YellowBg = ansi_t<43>;
        using BlueBg = ansi_t<44>;
        using MagentaBg = ansi_t<45>;
        using CyanBg = ansi_t<46>;
        using WhiteBg = ansi_t<47>;
    }
}