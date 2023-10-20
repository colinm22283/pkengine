#pragma once

#include <set>
#include <string>

#include <vulkan/vulkan.h>

#include "../extensions.hpp"

namespace PKEngine::Vulkan {
    inline bool are_extensions_supported(const VkPhysicalDevice & device) { // TODO: optimize!!!
        uint32_t extension_count;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

        std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

        for (const auto & extension : available_extensions) {
            required_extensions.erase(extension.extensionName);
        }

        return required_extensions.empty();
    }
}