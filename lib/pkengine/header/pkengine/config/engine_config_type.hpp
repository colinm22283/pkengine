#pragma once

#include <pkengine/util/version.hpp>

namespace PKEngine::Config {
    struct engine_config_t {
        struct {
            const char * const name    = "Untitled PKEngine App";
            const Version    version = 0.1;
        } application;
        struct {
            const char * const title = "Untitled PKEngine App";
            const int width = 800, height = 600;
        } window;
    };
}