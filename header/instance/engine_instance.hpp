#pragma once

#include <window.hpp>
#include <surface.hpp>

#include <instance/physical_device_instance.hpp>
#include <instance/queue_instance.hpp>
#include <instance/logical_device_instance.hpp>
#include <instance/swap_chain_instance.hpp>
#include <instance/image_views_instance.hpp>
#include <instance/pipeline_instance.hpp>
#include <instance/command_pool_instance.hpp>
#include <instance/command_buffer_instance.hpp>
#include <instance/render_pass_instance.hpp>

#include <frame_buffers.hpp>

#include <types/engine_config.hpp>

#include <query/query_queue_families.hpp>

#include <event/event_group.hpp>

namespace PKEngine {
    class EngineInstance {
    protected:
        Window window;
        Surface surface;
        PhysicalDeviceInstance physical_device;
        Query::queue_family_indices_t indices;
        LogicalDeviceInstance logical_device;
        QueueInstance graphics_queue, present_queue;
        SwapChainInstance swap_chain;
        ImageViewsInstance image_views;
        PipelineInstance pipeline;
        RenderPassInstance render_pass;

        FrameBuffers frame_buffers;

        CommandPoolInstance command_pool;
        CommandBufferInstance command_buffer;

    public:
        EventGroup<"Start Event", void()> start_group;
        EventGroup<"Update Event", void()> update_group;
        EventGroup<"Window Close Event", void()> close_group;

        inline explicit EngineInstance(VkInstance instance, instance_config_t & config):
            window(config),
            surface(instance, window),
            physical_device(instance, surface),
            indices(Query::query_queue_families(physical_device, surface)),
            logical_device(physical_device, indices),
            graphics_queue(logical_device, indices.graphics_family.value()),
            present_queue(logical_device, indices.present_family.value()),
            swap_chain(physical_device, logical_device, surface, window),
            image_views(logical_device, swap_chain),
            pipeline(logical_device, swap_chain),
            render_pass(logical_device, swap_chain.image_format),
            frame_buffers(logical_device, swap_chain, image_views, pipeline.render_pass()),
            command_pool(logical_device, physical_device, surface),
            command_buffer(logical_device, command_pool) {  }

        inline void start() {
            create_sync_objects();

            start_group();

            while (!glfwWindowShouldClose(window.handle())) {
                glfwPollEvents();

                update();

                update_group();
            }

            vkDeviceWaitIdle(logical_device);

            close_group();

            vkDestroySemaphore(logical_device, image_available_semaphore, nullptr);
            vkDestroySemaphore(logical_device, render_finished_semaphore, nullptr);
            vkDestroyFence(logical_device, in_flight_fence, nullptr);
        }

        VkSemaphore image_available_semaphore;
        VkSemaphore render_finished_semaphore;
        VkFence in_flight_fence;

        inline void create_sync_objects() {
            VkSemaphoreCreateInfo semaphore_info{};
            semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkFenceCreateInfo fence_info{};
            fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            if (vkCreateSemaphore(logical_device, &semaphore_info, nullptr, &image_available_semaphore) != VK_SUCCESS ||
                vkCreateSemaphore(logical_device, &semaphore_info, nullptr, &render_finished_semaphore) != VK_SUCCESS ||
                vkCreateFence(logical_device, &fence_info, nullptr, &in_flight_fence) != VK_SUCCESS) {
                throw std::runtime_error("failed to create semaphores!");
            }
        }

        inline void update() {
            vkWaitForFences(logical_device, 1, &in_flight_fence, VK_TRUE, UINT64_MAX);
            vkResetFences(logical_device, 1, &in_flight_fence);

            uint32_t image_index;
            vkAcquireNextImageKHR(logical_device, swap_chain, UINT64_MAX, image_available_semaphore, VK_NULL_HANDLE, &image_index);

            vkResetCommandBuffer(command_buffer, 0);
            command_buffer.record(image_index, render_pass, frame_buffers, swap_chain, pipeline);

            VkSubmitInfo submit_info{};
            submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            VkSemaphore wait_semaphores[] = {image_available_semaphore};
            VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submit_info.waitSemaphoreCount = 1;
            submit_info.pWaitSemaphores = wait_semaphores;
            submit_info.pWaitDstStageMask = wait_stages;
            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers = &command_buffer.instance();

            VkSemaphore signal_semaphores[] = {render_finished_semaphore };
            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores = signal_semaphores;

            if (vkQueueSubmit(graphics_queue, 1, &submit_info, in_flight_fence) != VK_SUCCESS) {
                throw std::runtime_error("failed to submit draw command buffer!");
            }

            VkPresentInfoKHR present_info{};
            present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

            present_info.waitSemaphoreCount = 1;
            present_info.pWaitSemaphores = signal_semaphores;

            VkSwapchainKHR swap_chains[] = {swap_chain };
            present_info.swapchainCount = 1;
            present_info.pSwapchains = swap_chains;
            present_info.pImageIndices = &image_index;

            present_info.pResults = nullptr;

            vkQueuePresentKHR(present_queue, &present_info);
        }

        EngineInstance(EngineInstance &) = delete;
        EngineInstance(EngineInstance &&) = delete;
    };
}