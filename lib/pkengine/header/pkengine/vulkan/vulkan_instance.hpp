#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <pkengine/vulkan/engine_info.hpp>

#include <pkengine/logger/logger.hpp>
#include <pkengine/vulkan/util/vulkan_exception.hpp>

#include <engine_config.hpp>

namespace PKEngine::Vulkan {
    class VulkanInstance {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitFailed, "Unable to initialize vulkan instance");
        };
    protected:
        static constexpr auto logger = Logger<"Vulkan Instance">();

        VkInstance instance = nullptr;

    public:
        inline VulkanInstance() {
            logger.debug() << "Initializing vulkan instance...";

            VkApplicationInfo application_info = {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName = Config::engine_config.application.name,
                .applicationVersion = Config::engine_config.application.version.make_vulkan_version(),
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
            const char ** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            create_info.enabledExtensionCount   = glfwExtensionCount;
            create_info.ppEnabledExtensionNames = glfwExtensions;

            Util::throw_on_fail<Exceptions::InitFailed>(
                vkCreateInstance(&create_info, nullptr, &instance)
            );

            uint32_t extension_count = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
            std::vector<VkExtensionProperties> extensions(extension_count);
            vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

            logger.debug() << "Available Extensions:";
            auto indented_logger = logger.indent();
            for (const auto & extension : extensions) {
                indented_logger.debug() << extension.extensionName;
            }

            logger.success() << "Vulkan instance initialized";
        }

        inline ~VulkanInstance() {
            if (instance != nullptr) {
                logger.debug() << "Destroying vulkan instance...";

                vkDestroyInstance(instance, nullptr);

                logger.success() << "Vulkan instance destroyed";
            }
        }

        inline VulkanInstance(const VulkanInstance &) = delete;
        inline VulkanInstance(VulkanInstance && other) noexcept:
            instance(other.instance) {
            other.instance = nullptr;
        }

        [[nodiscard]] constexpr const VkInstance & handle() noexcept { return instance; }
    };
}