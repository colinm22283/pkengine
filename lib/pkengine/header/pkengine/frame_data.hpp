#pragma once

#include <pkengine/internal/vulkan/sync/semaphore.hpp>
#include <pkengine/internal/vulkan/sync/fence.hpp>

#include <pkengine/internal/vulkan/types/descriptors/global_descriptor.hpp>
#include <pkengine/internal/vulkan/types/descriptors/object_descriptor.hpp>

namespace PKEngine {
    template<auto & logical_device, auto & physical_device, auto & descriptor_set_layout, auto & descriptor_pool, auto & command_pool>
    struct FrameData {
        Vulkan::DescriptorBuffer<logical_device, physical_device, Vulkan::Descriptors::GlobalDescriptor> global_descriptor_buffer;
        Vulkan::DescriptorBuffer<logical_device, physical_device, Vulkan::Descriptors::GlobalDescriptor> per_pass_descriptor_buffer;
        Vulkan::DescriptorBuffer<logical_device, physical_device, Vulkan::Descriptors::GlobalDescriptor> material_descriptor_buffer;
        Vulkan::DescriptorBuffer<logical_device, physical_device, Vulkan::Descriptors::ObjectDescriptor> object_descriptor_buffer;

        Vulkan::DescriptorSet<logical_device, descriptor_set_layout, descriptor_pool> descriptor_set;

        Vulkan::RenderCommandBuffer<logical_device, command_pool> command_buffer;

        Vulkan::Semaphore<logical_device> image_available_semaphore;
        Vulkan::Semaphore<logical_device> render_complete_semaphore;
        Vulkan::Fence<logical_device, true> in_flight_fence;

        inline void init() {
            global_descriptor_buffer.init();
            per_pass_descriptor_buffer.init();
            material_descriptor_buffer.init();
            object_descriptor_buffer.init();

            std::array<VkDescriptorBufferInfo, 2> buffer_infos = {
                VkDescriptorBufferInfo {
                    .buffer = global_descriptor_buffer.buffer_handle(),
                    .offset = 0,
                    .range = global_descriptor_buffer.size(),},
                VkDescriptorBufferInfo {
                    .buffer = object_descriptor_buffer.buffer_handle(),
                    .offset = 0,
                    .range = object_descriptor_buffer.size(),
                },
            };
            descriptor_set.init(buffer_infos);

            command_buffer.init();

            image_available_semaphore.init();
            render_complete_semaphore.init();
            in_flight_fence.init();
        }

        inline void free() {
            in_flight_fence.free();
            render_complete_semaphore.free();
            image_available_semaphore.free();

            command_buffer.free();

            descriptor_set.free();

            per_pass_descriptor_buffer.free();
            material_descriptor_buffer.free();
            object_descriptor_buffer.free();
            global_descriptor_buffer.free();
        }
    };
}