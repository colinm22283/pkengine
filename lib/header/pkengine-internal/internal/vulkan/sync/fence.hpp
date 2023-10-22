#pragma once

#include <vulkan/vulkan.h>

#include "../../../exception/internal.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device, bool start_signaled = false>
    class Fence {
    protected:
        VkFence fence = nullptr;

    public:
        inline void init() {
            VkFenceCreateInfo create_info {
                .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            };

            if constexpr (start_signaled) create_info.flags |= VK_FENCE_CREATE_SIGNALED_BIT;

            if (vkCreateFence(logical_device.handle(), &create_info, nullptr, &fence) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_fence();
            }
        }

        inline void free() {
            if (fence) vkDestroyFence(logical_device.handle(), fence, nullptr);
        }

        [[nodiscard]] inline VkFence handle() const noexcept { return fence; }

        inline void wait() {
            vkWaitForFences(logical_device.handle(), 1, &fence, VK_TRUE, UINT64_MAX);
            vkResetFences(logical_device.handle(), 1, &fence);
        }
    };
}