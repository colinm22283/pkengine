#pragma once

#include <vulkan/vulkan.h>

#include "device_buffer.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & physical_device>
    using IndexBuffer = DeviceBuffer<logical_device, physical_device, VkDeviceSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT>;
}