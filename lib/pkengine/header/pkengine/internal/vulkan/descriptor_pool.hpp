#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/exception/internal.hpp>

#include <render_config.hpp>

namespace PKEngine::Vulkan {
    template<const auto & logical_device>
    class DescriptorPool {
    protected:
        VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;

    public:
        template<std::size_t n>
        inline void init(std::array<VkDescriptorPoolSize, n> & pool_sizes) {
            VkDescriptorPoolCreateInfo create_info {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                .maxSets = static_cast<uint32_t>(render_config.max_frames_in_flight),
                .poolSizeCount = (uint32_t) pool_sizes.size(),
                .pPoolSizes = pool_sizes.data(),
            };

            if (vkCreateDescriptorPool(logical_device.handle(), &create_info, nullptr, &descriptor_pool) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_vulkan_descriptor_pool();
            }
        }

        inline void free() {
            if (descriptor_pool != VK_NULL_HANDLE) vkDestroyDescriptorPool(logical_device.handle(), descriptor_pool, nullptr);
        }

        [[nodiscard]] inline VkDescriptorPool handle() const noexcept { return descriptor_pool; }
    };
}