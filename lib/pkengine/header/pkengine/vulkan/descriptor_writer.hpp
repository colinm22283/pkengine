#pragma once

#include <vector>
#include <deque>

#include <vulkan/vulkan.h>

#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/descriptor_set.hpp>

namespace PKEngine::Vulkan {
    class DescriptorWriter {
    protected:
        Wrapper::LogicalDevice & logical_device;

        std::deque<VkDescriptorImageInfo> image_infos;
        std::deque<VkDescriptorBufferInfo> buffer_infos;

        std::vector<VkWriteDescriptorSet> descriptor_writes;

    public:
        inline DescriptorWriter(Wrapper::LogicalDevice & _logical_device):
            logical_device(_logical_device) { }

        inline void write_image(uint32_t binding, VkImageView image_view, VkSampler sampler,  VkImageLayout layout, VkDescriptorType type) {
            VkDescriptorImageInfo & image_info = image_infos.emplace_back(VkDescriptorImageInfo {
                .sampler = sampler,
                .imageView = image_view,
                .imageLayout = layout,
            });

            descriptor_writes.emplace_back(VkWriteDescriptorSet {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = nullptr,

                .dstSet = VK_NULL_HANDLE,
                .dstBinding = binding,
                .descriptorCount = 1,
                .descriptorType = type,
                .pImageInfo = &image_info,
            });
        }

        inline void write_buffer(uint32_t binding, VkBuffer buffer, std::size_t size, std::size_t offset, VkDescriptorType type) {
            VkDescriptorBufferInfo & buffer_info = buffer_infos.emplace_back(VkDescriptorBufferInfo {
                .buffer = buffer,
                .offset = offset,
                .range = size,
            });

            descriptor_writes.emplace_back(VkWriteDescriptorSet {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = nullptr,

                .dstSet = VK_NULL_HANDLE,
                .dstBinding = binding,
                .descriptorCount = 1,
                .descriptorType = type,
                .pBufferInfo = &buffer_info,
            });
        }

        inline void clear() {
            image_infos.clear();
            buffer_infos.clear();
            descriptor_writes.clear();
        }

        inline void update_set(Wrapper::DescriptorSet & descriptor_set) {
            for (VkWriteDescriptorSet & write : descriptor_writes) {
                write.dstSet = descriptor_set.handle();
            }

            vkUpdateDescriptorSets(logical_device.handle(), (uint32_t) descriptor_writes.size(), descriptor_writes.data(), 0, nullptr);
        }
    };
}