#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

#include "staged_buffer.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & physical_device>
    class IndexBuffer : public StagedBuffer<logical_device, physical_device, uint32_t, VK_BUFFER_USAGE_INDEX_BUFFER_BIT> {
        
    };
}