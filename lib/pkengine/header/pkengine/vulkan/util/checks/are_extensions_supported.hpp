#pragma once

#include <set>
#include <string>

#include <vulkan/vulkan.h>

#include <pkengine/vulkan/required_extensions.hpp>

namespace PKEngine::Vulkan {
    inline bool are_extensions_supported(VkPhysicalDevice device) { // TODO: optimize!!!
        // TODO: throw on errors
        static constexpr auto logger = Logger<"Ext Support Check">();

        uint32_t extension_count;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

        VkExtensionProperties available_extensions[extension_count];
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions);

        std::array<bool, required_extensions.size()> exts_found({ });
        exts_found.fill(false);

        logger.debug() << "Checking for extension support:";
        auto indented_logger = logger.debug().indent();
        for (const auto & extension : required_extensions) {
            bool extension_found = false;
            for (uint32_t i = 0; i < extension_count; i++) {
                if (extension == available_extensions[i].extensionName) {
                    extension_found = true;
                    break;
                }
            }

            indented_logger << extension << ": " << (extension_found ? "FOUND" : "NOT FOUND");

            if (!extension_found) return false;
        }

        return true;
    }
}