#pragma once

#include <list>

#include "pkengine/util/ansi.hpp"
#include "pkengine/logger/logger.hpp"
#include "vertex.hpp"

#include "vertex_buffer.hpp"

namespace PKEngine::Vulkan {
    template<typename T, auto & buffer>
    class DeviceAllocator { // TODO: look into better allocation techniques
    public:
        struct Exceptions {
            PKENGINE_RUNTIME_EXCEPTION(NotEnoughFreeSpace, "Not enough free space to make allocation");
        };

    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Device Allocator">();

        struct sector_t {
            bool free;
            VkDeviceSize offset, size;
        };

        using sector_list_t = std::list<sector_t>;
        using sector_list_iterator_t = typename sector_list_t::iterator;

        sector_list_t sectors = { sector_t { .free = true, .offset = 0, .size = 0, } };

    public:
        class Allocation {
            friend class DeviceAllocator;
        protected:
            bool was_moved = false;
            DeviceAllocator<T, buffer> & allocator;
            sector_list_iterator_t itr;

            explicit inline Allocation(DeviceAllocator<T, buffer> & _allocator, sector_list_iterator_t && _itr):
                allocator(_allocator),
                itr(std::move(_itr)) { }

        public:
            inline Allocation(Allocation &) = delete;
            inline Allocation(Allocation && a) noexcept: allocator(a.allocator), itr(a.itr) { a.was_moved = true; }

            inline ~Allocation() { if (!was_moved) allocator.dealloc(*this); }

            /// @brief offset in elements
            [[nodiscard]] inline VkDeviceSize offset() const noexcept { return itr->offset; }
            /// @brief size in elements
            [[nodiscard]] inline VkDeviceSize size() const noexcept { return itr->size; }
        };

        inline DeviceAllocator() = default;
        inline DeviceAllocator(DeviceAllocator &) = delete;
        inline DeviceAllocator(DeviceAllocator &&) = delete;

        inline void init() { sectors.front().size = buffer.capacity(); }
        inline void free() { }

        template<const auto & command_pool, const auto & vulkan_queue>
        [[nodiscard]] inline Allocation allocate(T * vertices, VkDeviceSize n) {
            logger << "Allocating " << n << " elements to device (" << (n * sizeof(T)) << " bytes)";

            for (auto itr = sectors.begin(); itr != sectors.end(); itr++) {
                if (itr->free && n <= itr->size) { // found a spot!
                    if (n == itr->size) {
                        itr->free = false;
                        logger << "\tAllocated at offset of " << itr->offset;
                        buffer.template begin_transfer<command_pool, vulkan_queue>().add(vertices, n, itr->offset).commit();
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
                        buffer.template begin_transfer<command_pool, vulkan_queue>().add(vertices, n, allocation->offset).commit();
                        return Allocation(*this, std::move(allocation));
                    }
                }
            }

            logger.error() << "Unable to allocate " << n << " elements to device (" << (n * sizeof(Vertex)) << " bytes)";
            throw typename Exceptions::NotEnoughFreeSpace();
        }
        template<const auto & command_pool, const auto & vulkan_queue, VkDeviceSize n> [[nodiscard]] inline Allocation allocate(Vertex (& vertices)[n]) { return allocate<command_pool, vulkan_queue>(vertices, n); }
        template<const auto & command_pool, const auto & vulkan_queue, VkDeviceSize n> [[nodiscard]] inline Allocation allocate(Vertex (&& vertices)[n]) { return allocate<command_pool, vulkan_queue>(std::move(vertices), n); }

        inline void log_sectors() {
            for (auto & sec : sectors) {
                logger << "Sector: offset = " << sec.offset << ", size = " << sec.size << ", " << (sec.free ? "free" : "reserved");
            }
        }

    protected:
        inline void dealloc(Allocation & sector) {
            logger << "Freeing sector at offset of " << sector.offset() << " with a size of " << sector.size() << " elements (" << (sector.size() * sizeof(T)) << " bytes)";

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