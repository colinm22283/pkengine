#pragma once

#include <list>

#include "../../../util/ansi.hpp"
#include "../../../logger/logger.hpp"
#include "vertex.hpp"

#include "device_buffer.hpp"

namespace PKEngine::Vulkan {
    template<const auto & vertex_buffer>
    class VertexAllocator { // TODO: look into better allocation techniques
    public:
        struct Exceptions {
            PKENGINE_RUNTIME_EXCEPTION(NotEnoughFreeSpace, "Not enough free space to make allocation");
        };

    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Vertex Allocator">();

        struct sector_t {
            bool free;
            VkDeviceSize offset, size;
        };

        std::list<sector_t> sectors = { sector_t { .free = true, .offset = 0, .size = vertex_buffer.capacity(), } };

    public:
        class Allocation {
            friend class VertexAllocator;
        protected:
            typename std::list<sector_t>::iterator itr;

            explicit inline Allocation(typename std::list<sector_t>::iterator && _itr): itr(std::move(_itr)) { }

        public:
            [[nodiscard]] inline VkDeviceSize offset() const noexcept { return itr->offset; }
            [[nodiscard]] inline VkDeviceSize size() const noexcept { return itr->size; }
        };

        inline VertexAllocator() = default;
        inline VertexAllocator(VertexAllocator &) = delete;
        inline VertexAllocator(VertexAllocator &&) = delete;

        inline Allocation allocate(Vertex * vertices, VkDeviceSize n) {
            logger << "Allocating " << n << " vertices to device (" << (n * sizeof(Vertex)) << " bytes)";

            for (auto itr = sectors.begin(); itr != sectors.end(); itr++) {
                if (itr->free && n <= itr->size) { // found a spot!
                    if (n == itr->size) {
                        itr->free = false;
                        return std::move(itr);
                    }
                    else {
                        itr->offset += n;
                        typename std::list<sector_t>::iterator allocation = sectors.emplace(itr, sector_t {
                            .free = false,
                            .offset = itr->offset,
                            .size = n,
                        });
                        return std::move(allocation);
                    }
                }
            }

            logger.error() << "Unable to allocate " << n << " vertices to device (" << (n * sizeof(Vertex)) << " bytes)";
            throw Exceptions::NotEnoughFreeSpace();
        }

        inline void free(Allocation && sector) {
            std::list<sector_t>::iterator & itr = sector.itr;


        }
    };
}