#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <util/const_round.hpp>

namespace PKEngine {
    struct version_t {
        int major, minor, patch;

        consteval version_t(int _major, int _minor, int _patch):
          major(_major), minor(_minor), patch(_patch) { }

        consteval version_t(float version, int _patch):
            major((int) Util::const_round(version)), minor((int) (version - (int) version) * 10), patch(_patch) { }
        consteval version_t(float version):
            version_t(version, 0) { }

        [[nodiscard]] inline uint32_t make_version() const { return VK_MAKE_VERSION(major, minor, patch); }
    };
}