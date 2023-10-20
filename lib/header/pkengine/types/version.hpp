#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "pkengine/util/const_round.hpp"

namespace PKEngine {
    class version_t {
    protected:
        static consteval unsigned int get_minor_version(float version) noexcept {
            return (unsigned int) ((version - (float) ((unsigned int) version)) * 10);
        }

    public:
        unsigned int major, minor, patch;

        consteval version_t(unsigned int _major, unsigned int _minor, unsigned int _patch) noexcept:
          major(_major), minor(_minor), patch(_patch) { }

        consteval version_t(float version, unsigned int _patch) noexcept:
            major((unsigned int) version), minor(get_minor_version(version)), patch(_patch) { }
        consteval version_t(float version):
            version_t(version, 0) { }

        [[nodiscard]] inline uint32_t make_vulkan_version() const { return VK_MAKE_VERSION(major, minor, patch); }
        inline std::string to_string() const noexcept {
            int length = snprintf(nullptr, 0, "%u.%u.%u", major, minor, patch);
            std::string buf(length, '\0');
            sprintf((char *) buf.c_str(), "%u.%u.%u", major, minor, patch);
            return buf;
        }
    };
}