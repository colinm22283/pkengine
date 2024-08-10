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
        template<std::size_t n>
        inline void init(std::array<VkDescriptorBufferInfo, n> & buffer_infos) {
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

            VkWriteDescriptorSet descriptor_writes[buffer_infos.size()];

            for (std::size_t i = 0; i < buffer_infos.size(); i++) {
                descriptor_writes[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptor_writes[i].dstSet = descriptor_set;
                descriptor_writes[i].dstBinding = i;
                descriptor_writes[i].dstArrayElement = 0;
                descriptor_writes[i].descriptorCount = 1;
                descriptor_writes[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptor_writes[i].pImageInfo = nullptr;
                descriptor_writes[i].pBufferInfo = buffer_infos.data();
                descriptor_writes[i].pTexelBufferView = nullptr;
            }

            vkUpdateDescriptorSets(logical_device.handle(), buffer_infos.size(), descriptor_writes, 0, nullptr);
        }

        inline void free() { }

        [[nodiscard]] inline VkDescriptorSet & handle() noexcept { return descriptor_set; }
    };
}