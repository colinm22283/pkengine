#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

namespace PKEngine::Vulkan {
    template<const auto & physical_device>
    inline uint32_t choose_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties mem_properties;
        vkGetPhysicalDeviceMemoryProperties(physical_device.handle(), &mem_properties);

        for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw Exception::Internal::unable_to_get_suitable_memory_type();
    }
}