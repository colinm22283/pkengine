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
//        struct Exceptions {
//            PKENGINE_RUNTIME_EXCEPTION(NotEnoughFreeSpace, "Not enough free space to make allocation");
//        };

    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Vertex Allocator">();

        struct sector_t {
            bool free;
            VkDeviceSize offset, size;
        };

        using sector_list_t = std::list<sector_t>;
        using sector_list_iterator_t = typename sector_list_t::iterator;

        sector_list_t sectors = { sector_t { .free = true, .offset = 0, .size = 0, } };

    public:
        class Allocation {
            friend class VertexAllocator;
        protected:
            bool was_moved = false;
            VertexAllocator<vertex_buffer> & allocator;
            sector_list_iterator_t itr;

            explicit inline Allocation(VertexAllocator<vertex_buffer> & _allocator, sector_list_iterator_t && _itr):
                allocator(_allocator),
                itr(std::move(_itr)) { }

        public:
            inline Allocation(Allocation &) = delete;
            inline Allocation(Allocation && a): allocator(a.allocator), itr(a.itr) { a.was_moved = true; }

            inline ~Allocation() { if (!was_moved) allocator.dealloc(*this); }

            [[nodiscard]] inline VkDeviceSize offset() const noexcept { return itr->offset; }
            [[nodiscard]] inline VkDeviceSize size() const noexcept { return itr->size; }
        };

        inline VertexAllocator() = default;
        inline VertexAllocator(VertexAllocator &) = delete;
        inline VertexAllocator(VertexAllocator &&) = delete;

        inline void init() { sectors.front().size = vertex_buffer.capacity(); }
        inline void free() { }

        [[nodiscard]] inline Allocation allocate(Vertex * vertices, VkDeviceSize n) {
            logger << "Allocating " << n << " vertices to device (" << (n * sizeof(Vertex)) << " bytes)";

            for (auto itr = sectors.begin(); itr != sectors.end(); itr++) {
                if (itr->free && n <= itr->size) { // found a spot!
                    if (n == itr->size) {
                        itr->free = false;
                        logger << "\tAllocated at offset of " << itr->offset;
                        return Allocation(*this, std::move(itr));
                    }
                    else {
                        sector_list_iterator_t allocation = sectors.emplace(itr, sector_t {
                            .free = false,
                            .offset = itr->offset,
                            .size = n,
                        });
                        itr->offset += n;
                        itr->size -= n;
                        logger << "\tAllocated at offset of " << allocation->offset;
                        return Allocation(*this, std::move(allocation));
                    }
                }
            }

            logger.error() << "Unable to allocate " << n << " vertices to device (" << (n * sizeof(Vertex)) << " bytes)";
//            throw typename Exceptions::NotEnoughFreeSpace();
            return Allocation(*this, std::move(sectors.begin()));
        }

        inline void log_sectors() {
            for (auto & sec : sectors) {
                logger << "Sector: offset = " << sec.offset << ", size = " << sec.size << ", " << (sec.free ? "free" : "reserved");
            }
        }

    protected:
        inline void dealloc(Allocation & sector) {
            logger << "Freeing sector at offset of " << sector.offset();

            sector.itr->free = true;

            // check ahead
            if (sector.itr != sectors.end()) {
                sector_list_iterator_t itr = sector.itr;
                itr++;
                if (itr->free) {
                    sector.itr->size += itr->size;
                    sectors.erase(itr);
                }
            }

            // check behind
            if (sector.itr != sectors.begin()) {
                sector_list_iterator_t itr = sector.itr;
                itr--;
                if (itr->free) {
                    itr->size += sector.itr->size;
                    sectors.erase(sector.itr);
                }
            }
        }
    };
}