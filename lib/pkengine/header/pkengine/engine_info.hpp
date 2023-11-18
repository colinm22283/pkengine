#pragma once

#include "pkengine/types/version.hpp"

namespace PKEngine {
    constexpr struct {
        const char * name = "PKEngine";
        const version_t version = 0.1;
    } engine_info;
}