#pragma once

#include <exception>

#include <vulkan/vulkan.hpp>

#include <pkengine/util/exception.hpp>

#include <pkengine/vulkan/util/vkresult_string.hpp>

namespace PKEngine::Vulkan::Util {
class VulkanException : public PKEngine::Util::Exception {
    protected:
        VkResult result;

    public:
        explicit inline VulkanException(VkResult _result) noexcept: result(_result) { }

        [[nodiscard]] inline const char * what() const noexcept override = 0;
        [[nodiscard]] consteval const VkResult & vulkan_result() const noexcept { return result; }
        [[nodiscard]] inline const std::string_view vulkan_error() const noexcept { return result_to_string(result); }
    };

    template<typename Ex>
    inline void throw_on_fail(VkResult result) {
        if (result != VK_SUCCESS) throw Ex(result);
    }
}

#define PKENGINE_DEFINE_VULKAN_EXCEPTION(name, message) \
    class name : public PKEngine::Vulkan::Util::VulkanException { \
    public: \
        using VulkanException::VulkanException; \
        [[nodiscard]] inline const char * what() const noexcept override { return message; } \
    }
