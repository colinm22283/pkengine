#pragma once

#include <vulkan/vulkan.h>

#include "pkengine/exception/internal.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & command_pool>
    class CommandBuffer {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Command Buffer">();

        VkCommandBuffer buffer = nullptr;

    public:
        inline void init() {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = command_pool.handle();
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = 1;

            if (vkAllocateCommandBuffers(logical_device.handle(), &allocInfo, &buffer) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_vulkan_command_buffer();
            }
        }

        inline void free() {
            if (buffer) vkFreeCommandBuffers(logical_device.handle(), command_pool.handle(), 1, &buffer);
        }

        [[nodiscard]] inline VkCommandBuffer handle() const noexcept { return buffer; }

        inline void reset() {
            vkResetCommandBuffer(buffer, 0);
        }
    };
}