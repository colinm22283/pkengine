#pragma once

#include "pkengine/util/ansi.hpp"

namespace PKEngine {
    namespace LoggerConfig {
        using success_color = Util::ANSI::GreenFg;
        using warning_color = Util::ANSI::YellowFg;
        using error_color   = Util::ANSI::RedFg::Bold;
        using reset_color   = Util::ANSI::Reset;
    };
}