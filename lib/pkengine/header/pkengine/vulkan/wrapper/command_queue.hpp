#pragma once

#include <cstdint>
#include <ranges>

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/command_buffer.hpp>
#include <pkengine/vulkan/wrapper/sync/semaphore.hpp>
#include <pkengine/vulkan/wrapper/sync/fence.hpp>

#include <pkengine/vulkan/util/vulkan_exception.hpp>

#include <pkengine/vulkan/struct/semaphore_submit_info.hpp>
#include <pkengine/vulkan/struct/command_buffer_submit_info.hpp>
#include <pkengine/vulkan/struct/submit_info.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class CommandQueue {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(SubmitError, "Unable to submit vulkan draw buffer");
        };

    protected:
        static constexpr auto logger = Logger<"Command Queue">();

        VkQueue queue = VK_NULL_HANDLE;

        std::vector<VkSemaphore> wait_stages;
        std::vector<VkPipelineStageFlags> wait_flags;

    public:
        inline CommandQueue(LogicalDevice & logical_device, uint32_t queue_index) {
            logger.debug() << "Initialing command queue...";

            vkGetDeviceQueue(logical_device.handle(), queue_index, 0, &queue);

            logger.success() << "Command queue initialized";
        }

        inline CommandQueue(const CommandQueue &) = delete;
        inline CommandQueue(CommandQueue && other) noexcept:
            queue(other.queue),
            wait_stages(std::move(other.wait_stages)),
            wait_flags(std::move(other.wait_flags)) {
            other.queue = VK_NULL_HANDLE;
        }

        [[nodiscard]] constexpr const VkQueue & handle() const noexcept { return queue; }

        inline void submit(
            CommandBuffer & command_buffer,
            Sync::Semaphore & swapchain_semaphore,
            Sync::Semaphore & render_semaphore,
            Sync::Fence & submit_fence
        ) {
            VkCommandBufferSubmitInfo command_buffer_submit_info = Struct::command_buffer_submit_info(
                command_buffer.handle()
            );

            VkSemaphoreSubmitInfo wait_info = Struct::semaphore_submit_info(
                VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,
                swapchain_semaphore.handle()
            );
            VkSemaphoreSubmitInfo signal_info = Struct::semaphore_submit_info(
                VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT,
                render_semaphore.handle()
            );

            VkSubmitInfo2 submit_info = Struct::submit_info(
                command_buffer_submit_info,
                signal_info,
                wait_info
            );

            Util::throw_on_fail<Exceptions::SubmitError>(
                vkQueueSubmit2(queue, 1, &submit_info, submit_fence.handle())
            );
        }

        inline void add_wait_semaphore(const Sync::Semaphore & semaphore, VkPipelineStageFlags flags) {
            wait_stages.push_back(semaphore.handle());
            wait_flags.push_back(flags);
        }
    };
}