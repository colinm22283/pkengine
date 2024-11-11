#pragma once

#include <exception>

#include <vulkan/vulkan.hpp>

#include <pkengine/vulkan/util/vkresult_string.hpp>
#include <pkengine/util/file_name.hpp>

namespace PKEngine::Util {
    class Exception : public std::exception {
    public:
        [[nodiscard]] virtual inline const char * file() const noexcept = 0;
        [[nodiscard]] inline const char * what() const noexcept override = 0;
    };
}

#define PKENGINE_DEFINE_EXCEPTION(name, message) \
    class name : public PKEngine::Util::Exception { \
    public: \
        [[nodiscard]] inline const char * file() const noexcept override { return PKEngine::Util::const_file_name<__FILE__>(); } \
        [[nodiscard]] inline const char * what() const noexcept override { return message; } \
    }
