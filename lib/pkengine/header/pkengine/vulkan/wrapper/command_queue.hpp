#pragma once

#include <cstdint>
#include <ranges>

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/sync/semaphore.hpp>
#include <pkengine/vulkan/wrapper/sync/fence.hpp>
#include <pkengine/vulkan/util/vulkan_exception.hpp>

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

        inline void submit_queue(Sync::Fence & submit_fence) { // TODO: complete submit_info
            VkSubmitInfo submit_info = {
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .waitSemaphoreCount = (uint32_t) wait_stages.size(),
                .pWaitSemaphores = wait_stages.data(),
                .pWaitDstStageMask = wait_flags.data(),
            };

            Util::throw_on_fail<Exceptions::SubmitError>(
                vkQueueSubmit(queue, 1, &submit_info, submit_fence.handle())
            );
        }

        inline void add_wait_semaphore(const Sync::Semaphore & semaphore, VkPipelineStageFlags flags) {
            wait_stages.push_back(semaphore.handle());
            wait_flags.push_back(flags);
        }
    };
}