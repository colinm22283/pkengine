#pragma once

#include <fstream>

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/util/exception.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class ShaderModule {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_EXCEPTION(FileNotFound, "Unable to locate file with specified path");

            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize shader module");
        };

    protected:
        static constexpr auto logger = Logger<"Shader Module">();

        LogicalDevice & logical_device;

        VkShaderModule shader_module = VK_NULL_HANDLE;

    public:
        inline ShaderModule(
            LogicalDevice & _logical_device,
            const char * path
        ):
            logical_device(_logical_device) {
            std::ifstream file(path, std::ios::ate | std::ios::binary);

            if (!file.is_open()) throw Exceptions::FileNotFound();

            size_t file_size = (size_t) file.tellg();

            std::vector<uint32_t> buffer((file_size + sizeof(uint32_t) - 1) / sizeof(uint32_t));

            file.seekg(0);

            file.read((char *) buffer.data(), file_size);

            file.close();

            VkShaderModuleCreateInfo createInfo = {
                .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                .pNext = nullptr,

                .codeSize = file_size,
                .pCode = buffer.data(),
            };

            Util::throw_on_fail<Exceptions::InitError>(
                vkCreateShaderModule(
                    logical_device.handle(),
                    &createInfo,
                    nullptr,
                    &shader_module
                )
            );

            logger.debug() << "Initialized shader module";
        }

        inline ~ShaderModule() {
            if (shader_module != VK_NULL_HANDLE) {
                vkDestroyShaderModule(logical_device.handle(), shader_module, nullptr);

                logger.debug() << "Destroyed shader module";
            }
        }

        [[nodiscard]] inline const VkShaderModule & handle() const noexcept { return shader_module; }
    };
}