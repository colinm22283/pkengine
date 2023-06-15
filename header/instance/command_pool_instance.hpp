#pragma once

#include <vulkan/vulkan.h>

#include <query/query_queue_families.hpp>

namespace PKEngine {
    class CommandPoolInstance {
    protected:
        VkDevice device;

        VkCommandPool _instance;

    public:
        inline CommandPoolInstance(VkDevice _device, VkPhysicalDevice physical_device, VkSurfaceKHR surface): device(_device) {
            Query::queue_family_indices_t queueFamilyIndices = Query::query_queue_families(physical_device, surface);

            VkCommandPoolCreateInfo pool_info{};
            pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            pool_info.queueFamilyIndex = queueFamilyIndices.graphics_family.value();

            if (vkCreateCommandPool(device, &pool_info, nullptr, &_instance) != VK_SUCCESS) {
                throw Exceptions::UnableToCreateCommandPool();
            }
        }

        inline ~CommandPoolInstance() {
            vkDestroyCommandPool(device, _instance, nullptr);
        }

        inline auto instance() const noexcept { return _instance; }
        inline operator VkCommandPool() const noexcept { return instance(); }
    };
}