#pragma once

#include "../util/ansi.hpp"

namespace PKEngine {
    namespace logger_config {
#ifdef PKENGINE_LOGGER_ENABLE
        constexpr bool enable = true;
#else
        constexpr bool enable = false;
#endif

        using success_color = Util::ANSI::GreenFg;
        using warning_color = Util::ANSI::YellowFg;
        using error_color   = Util::ANSI::RedFg::Bold;
        using reset_color   = Util::ANSI::Reset;
    };
}