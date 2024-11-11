#pragma once

#include <pkengine/util/const_string.hpp>

namespace PKEngine::Util {
    template<ConstString file_path>
    consteval const char * const_file_name() noexcept {
        std::size_t slash_idx = file_path.size() - 1;
        while (file_path[slash_idx] != '/' && file_path[slash_idx] != '\\') {
            slash_idx--;
        }
        return file_path.c_str() + slash_idx + 1;
    }
}