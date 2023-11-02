#pragma once

#include <forward_list>
#include <list>

#include "device_buffer.hpp"
#include "../../../util/deletable_forward_list.hpp"

namespace PKEngine::Vulkan {
    template<const auto & vertex_buffer>
    class VertexAllocator {
    public:
        struct Exceptions {
            PKENGINE_RUNTIME_EXCEPTION(NotEnoughFreeSpace, "Not enough free space to make allocation");
        };

    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Vertex Allocator">();

        struct sector_t {
            VkDeviceSize start, size;
        };

        Util::DeletableForwardList<sector_t> free_space = { sector_t { .start = 0, .size = vertex_buffer.size(), } };

    public:
        inline VertexAllocator() = default;
        inline VertexAllocator(VertexAllocator &) = delete;
        inline VertexAllocator(VertexAllocator &&) = delete;

        inline void allocate(Vertex * vertices, VkDeviceSize n) {
            logger << "Allocating " << n << " vertices to device (" << (n * sizeof(Vertex)) << " bytes)";

            for (auto itr = free_space.begin(); itr != free_space.end(); itr++) {
                if (n <= itr->size) { // found a spot!
                    if (n == itr->size) {

                    }

                    return;
                }
            }

            logger.error() << "Unable to allocate " << n << " vertices to device (" << (n * sizeof(Vertex)) << " bytes)";
            throw Exceptions::NotEnoughFreeSpace();
        }
    };
}