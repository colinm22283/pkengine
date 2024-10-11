#pragma once

#include <cstdint>
#include <ranges>

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/logical_device.hpp>
#include <pkengine/vulkan/sync/semaphore.hpp>
#include <pkengine/vulkan/sync/fence.hpp>
#include <pkengine/vulkan/util/vulkan_exception.hpp>

namespace PKEngine::Vulkan {
    class DeviceQueue {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(SubmitError, "Unable to submit vulkan draw buffer");
        };

    protected:
        static constexpr auto logger = Logger<"Physical Device">();

        VkQueue queue = VK_NULL_HANDLE;

        Sync::Fence & submit_fence;
        std::vector<VkSemaphore> wait_stages;
        std::vector<VkPipelineStageFlags> wait_flags;

    public:
        inline DeviceQueue(LogicalDevice & logical_device, Sync::Fence & _submit_fence, uint32_t queue_index):
            submit_fence(_submit_fence) {
            logger.debug() << "Initialing vulkan queue...";

            vkGetDeviceQueue(logical_device.handle(), queue_index, 0, &queue);

            logger.success() << "Vulkan queue initialized";
        }

        inline DeviceQueue(const DeviceQueue &) = delete;
        inline DeviceQueue(DeviceQueue && other) noexcept:
            queue(other.queue),
            submit_fence(other.submit_fence),
            wait_stages(std::move(other.wait_stages)),
            wait_flags(std::move(other.wait_flags)) {
            other.queue = VK_NULL_HANDLE;
        }

        inline void submit_queue() { // TODO: complete submit_info
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