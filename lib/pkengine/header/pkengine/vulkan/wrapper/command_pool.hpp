#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/util/queue_family_indices.hpp>
#include <pkengine/vulkan/util/vulkan_exception.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class CommandPool {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize vulkan command pool");
        };

    protected:
        static constexpr auto logger = Logger<"Command Pool">();

        LogicalDevice & logical_device;

        VkCommandPool command_pool = VK_NULL_HANDLE;

    public:
        inline CommandPool(LogicalDevice & _logical_device, uint32_t queue_family_index):
            logical_device(_logical_device) {
            logger.debug() << "Initializing command pool...";

            VkCommandPoolCreateInfo create_info {
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                .queueFamilyIndex = queue_family_index,
            };

            Util::throw_on_fail<Exceptions::InitError>(
                vkCreateCommandPool(logical_device.handle(), &create_info, nullptr, &command_pool)
            );

            logger.debug() << "Command pool initialized";
        }

        inline ~CommandPool() {
            if (command_pool != VK_NULL_HANDLE) {
                logger.debug() << "Destroying command pool...";

                vkDestroyCommandPool(logical_device.handle(), command_pool, nullptr);

                logger.debug() << "Command pool destroyed";
            }
        }

        constexpr CommandPool(const CommandPool &) = delete;
        constexpr CommandPool(CommandPool && other) noexcept:
            logical_device(other.logical_device),
            command_pool(other.command_pool) {
            other.command_pool = VK_NULL_HANDLE;
        }

        inline CommandPool & operator=(CommandPool && other) noexcept {
            vkDestroyCommandPool(logical_device.handle(), command_pool, nullptr);

            command_pool = other.command_pool;

            other.command_pool = VK_NULL_HANDLE;

            return *this;
        }

        [[nodiscard]] constexpr const VkCommandPool & handle() const noexcept { return command_pool; }
    };
}