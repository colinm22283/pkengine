#pragma once

#include "../util/ansi.hpp"

namespace PKEngine {
    namespace logger_config {
        constexpr bool enable = true;

        using success_color = Util::ANSI::GreenFg;
        using warning_color = Util::ANSI::YellowFg;
        using error_color   = Util::ANSI::RedFg::Bold;
        using reset_color   = Util::ANSI::Reset;
    };
}