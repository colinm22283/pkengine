#pragma once

#include <pkengine/config/memory_config_type.hpp>

namespace PKEngine {
    constexpr memory_config_t memory_config = {
        .vertex_buffer_allocation = 2048,
        .index_buffer_allocation = 2048,
    };
}