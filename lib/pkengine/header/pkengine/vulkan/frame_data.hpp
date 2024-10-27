#pragma once

#include <pkengine/vulkan/wrapper/swap_chain.hpp>
#include <pkengine/vulkan/wrapper/command_pool.hpp>
#include <pkengine/vulkan/wrapper/command_buffer.hpp>
#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/sync/semaphore.hpp>
#include <pkengine/vulkan/wrapper/sync/fence.hpp>
#include <pkengine/vulkan/util/queue_family_indices.hpp>

namespace PKEngine::Vulkan {
    struct FrameData {
        Wrapper::SwapChain & swap_chain;

        Wrapper::CommandPool command_pool;
        Wrapper::CommandBuffer command_buffer;

        Wrapper::Sync::Semaphore swapchain_semaphore, render_semaphore;
        Wrapper::Sync::Fence render_fence;

        inline FrameData(
            Wrapper::SwapChain & _swap_chain,
            Wrapper::LogicalDevice & logical_device,
            Util::QueueFamilyIndices & queue_family_indices
        ):
            swap_chain(_swap_chain),
            command_pool(logical_device, queue_family_indices.graphics_family.value()),
            command_buffer(logical_device, command_pool),
            swapchain_semaphore(logical_device),
            render_semaphore(logical_device),
            render_fence(logical_device) { }

        inline void draw() {
            // TODO: check results

            render_fence.await();
            render_fence.reset();

            uint32_t swap_chain_image_index = swap_chain.next_image_index(swapchain_semaphore);

            command_buffer.reset();
            command_buffer.begin_one_time();


        }
    };
}