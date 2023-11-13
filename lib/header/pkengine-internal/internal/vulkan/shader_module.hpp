#pragma once

#include <vulkan/vulkan.h>

#include <fstream>

#include "../../exception/internal.hpp"

#include "../../util/const_string.hpp"
#include "../../logger/logger.hpp"

#include "../../internal/vulkan/util/spv_buffer.hpp"

namespace PKEngine::Vulkan {
    class ShaderModule {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Shader Module">();

    public:
        VkShaderModule _shader_module = VK_NULL_HANDLE;

        template<const auto & logical_device>
        inline void init(const char * path) {
            logger << "Creating shader module...";

            std::ifstream fs(path, std::ifstream::ate | std::ifstream::binary);
            const long size = fs.tellg();
            char data[size];
            fs.seekg(std::ifstream::beg);
            fs.read(data, size * (long) sizeof(char));

            VkShaderModuleCreateInfo create_info = {
                .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                .codeSize = (std::size_t) size,
                .pCode = reinterpret_cast<const uint32_t*>(data),
            };

            if (vkCreateShaderModule(logical_device.handle(), &create_info, nullptr, &_shader_module) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_shader_module();
            }

            logger.success() << "Shader module created";
        }
        template<const auto & logical_device>
        inline void init(const char * buffer, std::size_t size) {
            logger << "Creating shader module...";

            VkShaderModuleCreateInfo create_info = {
                .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                .codeSize = size,
                .pCode = reinterpret_cast<const uint32_t*>(buffer),
            };

            if (vkCreateShaderModule(logical_device.handle(), &create_info, nullptr, &_shader_module) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_shader_module();
            }

            logger.success() << "Shader module created";
        }

        template<const auto & logical_device>
        inline void free() {
            if (_shader_module != VK_NULL_HANDLE) {
                logger << "Destroying shader module...";

                vkDestroyShaderModule(logical_device.handle(), _shader_module, nullptr);

                logger.success() << "Shader module destroyed";
            }
        }

        [[nodiscard]] inline VkShaderModule handle() const noexcept { return _shader_module; }
    };
}