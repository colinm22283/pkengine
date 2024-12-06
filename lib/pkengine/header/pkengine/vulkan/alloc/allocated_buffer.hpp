#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/alloc/vulkan_allocator.hpp>
#include <pkengine/vulkan/util/vulkan_exception.hpp>

namespace PKEngine::Vulkan::Alloc {
    template<typename T>
    class AllocatedBuffer {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to allocate vulkan buffer");
        };

    protected:
        static constexpr auto logger = Logger<"Allocated Buffer">();

        VulkanAllocator & allocator;

        std::size_t _size;

        VkBuffer buffer = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
        VmaAllocationInfo info = { };

    public:
        inline AllocatedBuffer(VulkanAllocator & _allocator, std::size_t _size_, VkBufferUsageFlags usage, VmaMemoryUsage memory_usage):
            allocator(_allocator),
            _size(_size_) {
            logger.debug() << "Allocating vulkan buffer";

            VkBufferCreateInfo bufferInfo = {
                .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                .pNext = nullptr,

                .size = _size * sizeof(T),

                .usage = usage,
            };

            VmaAllocationCreateInfo alloc_info = {
                .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,

                .usage = memory_usage,
            };

            // allocate the buffer
            Util::throw_on_fail<typename Exceptions::InitError>(
                vmaCreateBuffer(
                    allocator.handle(),
                    &bufferInfo,
                    &alloc_info,
                    &buffer,
                    &allocation,
                    &info
                )
            );

            logger.debug() << "Vulkan buffer allocated";
        }

        inline ~AllocatedBuffer() {
            if (allocation != VK_NULL_HANDLE) {
                logger.debug() << "Deallocating vulkan buffer";

                vmaDestroyBuffer(allocator.handle(), buffer, allocation);

                logger.debug() << "Vulkan buffer deallocated";
            }
        }

        inline AllocatedBuffer(const AllocatedBuffer &) = delete;
        inline AllocatedBuffer(AllocatedBuffer && other) noexcept:
            allocator(other.allocator),
            _size(other._size),
            buffer(other.buffer),
            allocation(other.allocation),
            info(other.info) {
            other.allocation = VK_NULL_HANDLE;
            other.buffer = VK_NULL_HANDLE;
        }

        [[nodiscard]] constexpr const VkBuffer & handle() const noexcept { return buffer; }

        [[nodiscard]] inline void * get_mapped_data() const noexcept { return info.pMappedData; }
        [[nodiscard]] constexpr const std::size_t & size() const noexcept { return _size; }
    };
}