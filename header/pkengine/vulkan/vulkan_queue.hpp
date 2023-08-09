#pragma once

#include <vulkan/vulkan.h>

#include "../logger/logger.hpp"

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

        [[nodiscard]] inline VkQueue handle() const noexcept { return queue; }
    };
}