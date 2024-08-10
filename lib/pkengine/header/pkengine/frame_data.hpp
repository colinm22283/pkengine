#pragma once

#include <pkengine/internal/vulkan/sync/semaphore.hpp>
#include <pkengine/internal/vulkan/sync/fence.hpp>

namespace PKEngine {
    template<auto & logical_device, auto & descriptor_set_layout, auto & descriptor_pool, auto & command_pool>
    struct FrameData {
        Vulkan::DescriptorSet<logical_device, descriptor_set_layout, descriptor_pool> descriptor_set;
        Vulkan::RenderCommandBuffer<logical_device, command_pool> command_buffer;
        Vulkan::Semaphore<logical_device> render_complete_semaphore;
        Vulkan::Fence<logical_device, true> in_flight_fence;
    };
}