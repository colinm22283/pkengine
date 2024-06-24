#pragma once

#include <pkengine/internal/vulkan/shader/device_buffer.hpp>
#include <pkengine/internal/vulkan/shader/uniform_buffer_object.hpp>

#include <render_config.hpp>

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & physical_device>
    class UniformBuffer {
    protected:
        DeviceBuffer<
            logical_device,
            physical_device,
            UniformBufferObject,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        > device_buffer;

        UniformBufferObject * bound_buffer;

    public:
        inline void init() {
            device_buffer.init(1);

            vkMapMemory(
                logical_device.handle(),
                device_buffer.memory_handle(),
                0,
                sizeof(UniformBufferObject),
                0,
                (void **) &bound_buffer
            );

            bound_buffer->model.set_identity();
            bound_buffer->proj.set_identity();
            bound_buffer->view.set_scaling_matrix(0.4, 0.4, 0.4);
        }

        inline void free() {
            device_buffer.free();

            vkUnmapMemory(
                logical_device.handle(),
                device_buffer.memory_handle()
            );
        }

        [[nodiscard]] inline UniformBufferObject * buffer() const noexcept { return bound_buffer; }
        [[nodiscard]] inline VkBuffer buffer_handle() const noexcept { return device_buffer.buffer_handle(); }
    };
}