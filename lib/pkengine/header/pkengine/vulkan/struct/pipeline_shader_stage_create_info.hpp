#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan::Struct {
    constexpr VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(
        VkShaderStageFlagBits stage,
        VkShaderModule shader_module
    ) {
        return VkPipelineShaderStageCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .stage = stage,
            .module = shader_module,
            .pName = "main",
        };
    }
}