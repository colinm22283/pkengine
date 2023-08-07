#pragma once

#include <color_vertex.hpp>

#include <util/choose_memory_type.hpp>
#include <cstring>

const color_vertex_t vertices[] = {
    {0.0f, -1.0f, 1.0f, 0.0f, 0.0f},
    {0.5f, 0.5f, 0.0f, 1.0f, 0.0f},
    {-0.5f, 0.5f, 0.0f, 0.0f, 1.0f}
};

namespace PKEngine {
    class VertexBufferInstance {
    protected:
        VkDevice device;

        VkBuffer vertex_buffer;
        VkDeviceMemory vertex_buffer_memory;

    public:
        inline VertexBufferInstance(VkDevice _device, VkPhysicalDevice physical_device): device(_device) {
            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = sizeof(vertices);
            bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(device, &bufferInfo, nullptr, &vertex_buffer) != VK_SUCCESS) {
                throw Exceptions::UnableToCreateVertexBuffer();
            }

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(device, vertex_buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = choose_memory_type(physical_device, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            if (vkAllocateMemory(device, &allocInfo, nullptr, &vertex_buffer_memory) != VK_SUCCESS) {
                throw Exceptions::UnableToAllocateBufferMemory();
            }
            vkBindBufferMemory(device, vertex_buffer, vertex_buffer_memory, 0);

            void * data;
            vkMapMemory(device, vertex_buffer_memory, 0, bufferInfo.size, 0, &data);
            std::memcpy(data, vertices, (size_t) bufferInfo.size);
            vkUnmapMemory(device, vertex_buffer_memory);


        }
        inline ~VertexBufferInstance() {
            vkDestroyBuffer(device, vertex_buffer, nullptr);
            vkFreeMemory(device, vertex_buffer_memory, nullptr);
        }

        inline operator VkBuffer() const noexcept { return vertex_buffer; }
    };
}