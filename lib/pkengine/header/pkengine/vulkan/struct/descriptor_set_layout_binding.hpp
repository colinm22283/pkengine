#pragma once

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan::Struct {
    constexpr VkDescriptorSetLayoutBinding descriptor_set_layout_binding(
        uint32_t location,
        VkDescriptorType type
    ) {
        return VkDescriptorSetLayoutBinding {
            .binding = location,
            .descriptorType = type,
            .descriptorCount = 1,
            .stageFlags = 0,
            .pImmutableSamplers = nullptr,
        };
    }
}