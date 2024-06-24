#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/exception/internal.hpp>

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & descriptor_set_layout, const auto & descriptor_pool>
    class DescriptorSet {
    protected:
        VkDescriptorSet descriptor_set = VK_NULL_HANDLE;

        VkDescriptorSetLayout layouts[1];

    public:
        inline void init(auto & uniform_buffer) {
            layouts[0] = descriptor_set_layout.handle();

            VkDescriptorSetAllocateInfo alloc_info {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .descriptorPool = descriptor_pool.handle(),
                .descriptorSetCount = 1,
                .pSetLayouts = layouts,
            };

            if (vkAllocateDescriptorSets(logical_device.handle(), &alloc_info, &descriptor_set) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_vulkan_descriptor_sets();
            }

            VkDescriptorBufferInfo buffer_info {
                .buffer = uniform_buffer.buffer_handle(),
                .offset = 0,
                .range = sizeof(UniformBufferObject),
            };

            VkWriteDescriptorSet descriptor_write {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet = descriptor_set,
                .dstBinding = 0,
                .dstArrayElement = 0,

                .descriptorCount = 1,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,

                .pImageInfo = nullptr,
                .pBufferInfo = &buffer_info,
                .pTexelBufferView = nullptr,
            };

            vkUpdateDescriptorSets(logical_device.handle(), 1, &descriptor_write, 0, nullptr);
        }

        inline void free() { }

        [[nodiscard]] inline VkDescriptorSet & handle() noexcept { return descriptor_set; }
    };
}