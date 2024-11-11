#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/command_pool.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class CommandBuffer {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize vulkan command buffer");
        };
    protected:
        static constexpr auto logger = Logger<"Command Buffer">();

        LogicalDevice & logical_device;
        CommandPool & command_pool;

        VkCommandBuffer command_buffer = VK_NULL_HANDLE;

    public:
        inline CommandBuffer(LogicalDevice & _logical_device, CommandPool & _command_pool):
            logical_device(_logical_device),
            command_pool(_command_pool) {
            logger.debug() << "Initializing command buffer...";

            VkCommandBufferAllocateInfo alloc_info {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .commandPool = command_pool.handle(),
                .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                .commandBufferCount = 1,
            };

            Util::throw_on_fail<Exceptions::InitError>(
                vkAllocateCommandBuffers(logical_device.handle(), &alloc_info, &command_buffer)
            );

            logger.success() << "Command buffer initialized";
        }

        inline ~CommandBuffer() {
            if (command_buffer != VK_NULL_HANDLE) {
                logger.debug() << "Destroying command buffer...";

                vkFreeCommandBuffers(logical_device.handle(), command_pool.handle(), 1, &command_buffer);

                logger.success() << "Command buffer destroyed";
            }
        }

	inline CommandBuffer(const CommandBuffer &) = delete;
	inline CommandBuffer(CommandBuffer && other) noexcept:
	    logical_device(other.logical_device),
	    command_pool(other.command_pool),
	    command_buffer(other.command_buffer) {
	    other.command_buffer = VK_NULL_HANDLE;
	}

        [[nodiscard]] constexpr const VkCommandBuffer & handle() const noexcept { return command_buffer; }

        inline void reset() {
            vkResetCommandBuffer(command_buffer, 0);
        }

        inline void begin_one_time() {
            VkCommandBufferBeginInfo begin_info {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .pNext = nullptr,
                .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
                .pInheritanceInfo = nullptr,
            };

            // TODO: handle errors
            vkBeginCommandBuffer(command_buffer, &begin_info);
        }

        inline void end() {
            // TODO: handle errors
            vkEndCommandBuffer(command_buffer);
        }
    };
}
