#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "pkengine/engine_info.hpp"

#include "engine_config.hpp"

#include "pkengine/logger/logger.hpp"

namespace PKEngine::Vulkan {
    class VulkanInstance {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Vulkan Instance">();

        VkInstance instance = nullptr;

    public:
        inline void init() {
            logger << "Initializing vulkan instance...";

            VkApplicationInfo application_info = {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName = engine_config.application.name,
                .applicationVersion = engine_config.application.version.make_vulkan_version(),
                .pEngineName = engine_info.name,
                .engineVersion = engine_info.version.make_vulkan_version(),
                .apiVersion = VK_API_VERSION_1_3,
            };
            VkInstanceCreateInfo create_info = {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pApplicationInfo = &application_info,
                .enabledLayerCount = 0,
            };

            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            create_info.enabledExtensionCount   = glfwExtensionCount;
            create_info.ppEnabledExtensionNames = glfwExtensions;

            if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS) throw Exception::Internal::vulkan_instance_creation();

            uint32_t extension_count = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
            std::vector<VkExtensionProperties> extensions(extension_count);
            vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

            logger << "Available Extensions:";
            for (const auto& extension : extensions) {
                logger << "\t" << extension.extensionName;
            }

            logger.success() << "Vulkan instance initialized";
        }
        inline void free() {
            if (instance != VK_NULL_HANDLE) {
                logger << "Destroying vulkan instance";

                vkDestroyInstance(instance, nullptr);

                logger.success() << "Vulkan instance destroyed";
            }
        }

        [[nodiscard]] inline VkInstance handle() const noexcept { return instance; }
    };
}