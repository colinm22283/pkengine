#pragma once

#include <vulkan/vulkan.h>

#include <query/query_queue_families.hpp>

#include <instance/logical_device_instance.hpp>

namespace PKEngine {
    class QueueInstance {
    protected:
        VkQueue _handle;

    public:
        inline QueueInstance(LogicalDeviceInstance & device, uint32_t queue_index) {
            vkGetDeviceQueue(device, queue_index, 0, &_handle);
        }

        inline QueueInstance(QueueInstance && other) noexcept: _handle(other._handle) { other._handle = nullptr; }
        inline QueueInstance(QueueInstance &) = delete;

        [[nodiscard]] inline VkQueue const handle() const { return _handle; }
        [[nodiscard]] inline operator VkQueue const() const { return handle(); }
    };
}