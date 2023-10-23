#pragma once

#include <vulkan/vulkan.h>

#include "../../logger/logger.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device>
    class VulkanQueue {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Vulkan Queue">();

        VkQueue queue = VK_NULL_HANDLE;

    public:
        inline void init(uint32_t queue_index) {
            vkGetDeviceQueue(logical_device.handle(), queue_index, 0, &queue);
        }
        inline void free() { }

        template<
            const auto & image_available_semaphore,
            const auto & render_finished_semaphore,
            const auto & in_flight_fence,
            const auto & command_buffer
        >
        inline void submit() {
            VkSubmitInfo submit_info{};
            submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            VkSemaphore wait_semaphores[] = { image_available_semaphore.handle() };
            VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submit_info.waitSemaphoreCount = 1;
            submit_info.pWaitSemaphores = wait_semaphores;
            submit_info.pWaitDstStageMask = waitStages;
            submit_info.commandBufferCount = 1;
            auto cmd_buf = command_buffer.handle();
            submit_info.pCommandBuffers = &cmd_buf;

            VkSemaphore signal_semaphores[] = { render_finished_semaphore.handle() };
            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores = signal_semaphores;

            if (vkQueueSubmit(queue, 1, &submit_info, in_flight_fence.handle()) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_submit_draw_buffer();
            }
        }

        [[nodiscard]] inline VkQueue handle() const noexcept { return queue; }
    };
}