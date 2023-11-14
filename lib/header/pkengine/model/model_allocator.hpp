#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

#include "pkengine/internal/vulkan/shader/device_allocator.hpp"
#include "pkengine/internal/vulkan/shader/vertex.hpp"

namespace PKEngine {
    template<auto & vertex_allocator, auto & index_allocator, typename vertex_allocator_t, typename index_allocator_t>
    class ModelAllocator {
        friend class engine_instance;

    protected:
        inline void init() { }
        inline void free() { }

    public:
        class Allocation {
            friend class ModelAllocator<vertex_allocator, index_allocator, vertex_allocator_t, index_allocator_t>;

        protected:


            inline Allocation(vertex_allocator_t && va, index_allocator_t && ia) noexcept:
                vertex_allocation(std::move(va)),
                index_allocation(std::move(ia)) { }

        public:
            inline Allocation() = delete;

            vertex_allocator_t vertex_allocation;
            index_allocator_t index_allocation;
        };

        template<const auto & command_pool, const auto & vulkan_queue>
        inline Allocation allocate(
            Vulkan::Vertex * vertices, VkDeviceSize vertex_count,
            uint32_t * indices, VkDeviceSize index_count
        ) { return Allocation(
            vertex_allocator.template allocate<command_pool, vulkan_queue>(vertices, vertex_count),
            index_allocator.template allocate<command_pool, vulkan_queue>(indices, index_count)
        ); }
        template<const auto & command_pool, const auto & vulkan_queue, VkDeviceSize vn, VkDeviceSize in> [[nodiscard]] inline Allocation allocate(
            Vulkan::Vertex (& vertices)[vn], uint32_t (& indices)[in]
        ) { return allocate<command_pool, vulkan_queue>(vertices, vn, indices, in); }
        template<const auto & command_pool, const auto & vulkan_queue, VkDeviceSize vn, VkDeviceSize in> [[nodiscard]] inline Allocation allocate(
            Vulkan::Vertex (&& vertices)[vn], uint32_t (&& indices)[in]
        ) { return allocate<command_pool, vulkan_queue>(std::move(vertices), vn, std::move(indices), in); }
    };
}