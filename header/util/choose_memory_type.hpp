#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

#include <util/engine_exception.hpp>

namespace PKEngine {
    uint32_t choose_memory_type(VkPhysicalDevice physical_device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties mem_properties;
        vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_properties);

        for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw Exceptions::UnableToGetSuitableMemoryType();
    }
}