#pragma once

#include <vector>
#include <fstream>

#include <vulkan/vulkan.h>

#include <logger/logger.hpp>

#include <util/engine_exception.hpp>

namespace PKEngine {
    class ShaderModuleInstance {
    protected:
        constexpr static auto logger = FileLogger<Util::ANSI::CyanFg, "SHADER MODULE INST", "pkengine.log">();

        VkDevice & device;

        VkShaderModule _instance;

    public:
        inline ShaderModuleInstance(VkDevice & _device, const char * path): device(_device) {
            logger.log<"Creating shader module">();

            std::ifstream file(path);
            if (!file.is_open()) throw Exceptions::UnableToOpenShaderFile();

            std::vector<char> code;
            file.seekg(0, std::ios::end);
            code.resize(file.tellg());
            file.seekg(0, std::ios::beg);
            file.clear();
            file.read(code.data(), code.size());

            VkShaderModuleCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            create_info.codeSize = code.size();
            create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

            if (vkCreateShaderModule(device, &create_info, nullptr, &_instance) != VK_SUCCESS) {
                throw Exceptions::UnableToCreateShaderModule();
            }

            logger.success().log<"Done!">();
        }

        inline ~ShaderModuleInstance() {
            vkDestroyShaderModule(device, _instance, nullptr);

            logger.log<"Shader module destroyed">();
        }

        inline VkShaderModule & instance() { return _instance; }
        inline operator VkShaderModule &() { return instance(); }
    };
}