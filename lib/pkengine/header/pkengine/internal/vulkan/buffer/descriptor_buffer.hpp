#pragma once

#include "pkengine/internal/vulkan/buffer/device_buffer.hpp"
#include "pkengine/math/matrix4.hpp"

#include "render_config.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & physical_device, typename T>
    class DescriptorBuffer {
    protected:
        DeviceBuffer<
            logical_device,
            physical_device,
            T,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        > device_buffer;

        T * bound_buffer;

    public:
        inline void init(VkDeviceSize count = 1) {
            device_buffer.init(count);

            vkMapMemory(
                logical_device.handle(),
                device_buffer.memory_handle(),
                0,
                count * sizeof(T),
                0,
                (void **) &bound_buffer
            );
        }

        inline void free() {
            device_buffer.free();

            vkUnmapMemory(
                logical_device.handle(),
                device_buffer.memory_handle()
            );
        }

        [[nodiscard]] inline T * buffer() const noexcept { return bound_buffer; }
        [[nodiscard]] inline VkBuffer buffer_handle() const noexcept { return device_buffer.buffer_handle(); }
        [[nodiscard]] inline std::size_t size() const noexcept { return sizeof(T); }
    };
}