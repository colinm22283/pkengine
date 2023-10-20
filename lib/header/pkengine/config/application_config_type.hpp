#pragma once

#include "../core/types/version.hpp"

namespace PKEngine {
    struct engine_config_t {
        struct {
            const char * const name    = "Untitled PKEngine App";
            const version_t    version = 0.1;
        } application;
        struct {
            const char * const title = "Untitled PKEngine App";
            const int width = 800, height = 600;
        } window;
    };
}