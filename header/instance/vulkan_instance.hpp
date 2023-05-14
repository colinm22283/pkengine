#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "config/info.hpp"

#include <logger/logger.hpp>

#include <types/engine_config.hpp>

#include <config/required_extensions.hpp>

namespace PKEngine {
    class VulkanInstance {
    protected:
        VkInstance _instance;

    public:
        inline explicit VulkanInstance(engine_config_t & config) {
            Log::info() << "VK Instance Created";

            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = config.application.name;
            appInfo.applicationVersion = config.application.version.make_version();
            appInfo.pEngineName = Info::engine_name;
            appInfo.engineVersion = Info::engine_version.make_version();
            appInfo.apiVersion = VK_API_VERSION_1_3;

            VkInstanceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;

            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            createInfo.enabledExtensionCount = glfwExtensionCount;
            createInfo.ppEnabledExtensionNames = glfwExtensions;

            createInfo.enabledLayerCount = 0;

            if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) throw Exceptions::UnableToCreateVulkanInstance();

            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            std::vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

            Log::info() << "Available Extensions:";
            for (const auto& extension : extensions) {
                Log::info() << '\t' << extension.extensionName;
            }
        }
        inline ~VulkanInstance() {
            Log::info() << "VK Instance Destroyed";

            vkDestroyInstance(_instance, nullptr);
        }

        inline VkInstance & instance() { return _instance; }
        inline operator VkInstance &() { return instance(); }
    };
}