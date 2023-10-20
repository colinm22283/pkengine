#pragma once

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & queue_family_indices>
    class CommandPool {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Command Pool">();

        VkCommandPool command_pool;

    public:
        inline void init() {
            logger << "Initializing command pool...";

            VkCommandPoolCreateInfo pool_info{};
            pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

            if (vkCreateCommandPool(logical_device.handle(), &pool_info, nullptr, &command_pool) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_vulkan_command_pool();
            }

            logger.success() << "Command pool initialized";
        }

        inline void free() {
            logger << "Destroying command pool...";

            if (command_pool != VK_NULL_HANDLE) {
                vkDestroyCommandPool(logical_device.handle(), command_pool, nullptr);
            }

            logger.success() << "Command pool destroyed";
        }

        [[nodiscard]] inline VkCommandPool handle() const noexcept { return command_pool; }
    };
}