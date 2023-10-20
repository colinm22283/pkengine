#pragma once

#include <cstring>

#include "pkengine/internal/vulkan/util/choose_memory_type.hpp"

#include "pkengine/assert/runtime_assert.hpp"

#include "pkengine/exception/runtime.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & physical_device, typename T, VkBufferUsageFlags usage>
    class DeviceBuffer {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Device Buffer">();

        VkBuffer buffer;
        VkDeviceMemory memory;

        VkDeviceSize _size, capacity; // in bytes

    public:
        inline void init(VkDeviceSize _capacity) {
            logger << "Initializing device buffer...";

            _size = 0;
            capacity = _capacity * sizeof(T);

            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = capacity;
            bufferInfo.usage = usage;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(logical_device.handle(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_vulkan_device_buffer();
            }

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(logical_device.handle(), buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = choose_memory_type<physical_device>(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            if (vkAllocateMemory(logical_device.handle(), &allocInfo, nullptr, &memory) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_allocate_device_buffer();
            }
            if (vkBindBufferMemory(logical_device.handle(), buffer, memory, 0) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_bind_device_buffer();
            }

            logger.success() << "Device buffer initialized";
        }

        inline void free() {
            logger << "Destroying device buffer...";

            if (buffer != VK_NULL_HANDLE) vkDestroyBuffer(logical_device.handle(), buffer, nullptr);
            if (memory != VK_NULL_HANDLE) vkFreeMemory(logical_device.handle(), memory, nullptr);

            logger.success() << "Device buffer destroyed";
        }

        inline void memcpy(T * buf, VkDeviceSize __size) {
            PKENGINE_INLINE_RUNTIME_ASSERT(__size <= capacity, "Copy source size must be less than buffer size");

            _size = __size;

            void * bound_memory;
            vkMapMemory(logical_device.handle(), memory, 0, _size, 0, &bound_memory);
            std::memcpy(bound_memory, buf, (size_t) _size);
            vkUnmapMemory(logical_device.handle(), memory);
        }
        inline void load_buffer(T * buf, VkDeviceSize count) {
            memcpy(buf, count * sizeof(T));
        }
        template<VkDeviceSize count>
        inline void load_buffer(T (& buf)[count]) {
            load_buffer((T *) buf, count);
        }
        template<VkDeviceSize count>
        inline void load_buffer(T (&& buf)[count]) {
            load_buffer((T *) buf, count);
        }

        inline void resize(VkDeviceSize _capacity) {
            PKENGINE_INLINE_RUNTIME_THROW("Not implemented");
        }

        [[nodiscard]] inline VkBuffer buffer_handle() const noexcept { return buffer; }
        [[nodiscard]] inline VkDeviceMemory memory_handle() const noexcept { return memory; }
        [[nodiscard]] inline VkDeviceSize size() const noexcept { return _size / sizeof(T); }

        [[nodiscard]] consteval auto get_bind_info() const noexcept {
            return T::get_bind_info();
        }
    };
}