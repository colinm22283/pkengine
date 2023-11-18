#pragma once

#include <vulkan/vulkan.h>

#include "pkengine/exception/internal.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device>
    class Semaphore {
    protected:
        VkSemaphore semaphore = nullptr;

    public:
        inline void init() {
            VkSemaphoreCreateInfo create_info {
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            };

            if (vkCreateSemaphore(logical_device.handle(), &create_info, nullptr, &semaphore) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_semaphore();
            }
        }

        inline void free() {
            if (semaphore) vkDestroySemaphore(logical_device.handle(), semaphore, nullptr);
        }

        [[nodiscard]] inline VkSemaphore handle() const noexcept { return semaphore; }
    };
}