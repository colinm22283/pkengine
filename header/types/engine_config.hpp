#pragma once

#include <cstdint>

#include "version.hpp"

namespace PKEngine {
    struct engine_config_t {
        struct {
            const char * name       = "Untitled";
            const version_t version = 0;
        } application;
    };

    struct instance_config_t {
        struct {
            const int width          = 800;
            const int height         = 600;
            const char * title       = "Untitled";
        } window;
    };
}