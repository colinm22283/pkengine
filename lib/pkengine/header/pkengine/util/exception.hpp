#pragma once

#include <exception>

#include <vulkan/vulkan.hpp>

#include <pkengine/vulkan/util/vkresult_string.hpp>

namespace PKEngine::Util {
    class Exception : public std::exception {
    public:
        [[nodiscard]] inline const char * what() const noexcept override = 0;
    };
}

#define PKENGINE_DEFINE_EXCEPTION(name, message) \
    class name : public PKEngine::Util::Exception { \
    public: \
        [[nodiscard]] inline const char * what() const noexcept override { return message; } \
    }
