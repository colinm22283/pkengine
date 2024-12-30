#pragma once

#include <vector>

#include <pkengine/vulkan/wrapper/descriptor_pool.hpp>
#include <pkengine/vulkan/wrapper/descriptor_set.hpp>
#include <pkengine/vulkan/wrapper/descriptor_set_layout.hpp>

namespace PKEngine::Vulkan {
    class DescriptorAllocator {
        static constexpr uint32_t max_pool_size = 4096;
        static constexpr uint32_t expansion_function(uint32_t x) {
            return x * 3 / 2;
        }

    public:
        using PoolRatio = Wrapper::DescriptorPool::PoolRatio;

        struct Allocation {
            Wrapper::DescriptorSetLayout layout;
            Wrapper::DescriptorSet set;
        };

    protected:
        Wrapper::LogicalDevice & logical_device;

        uint32_t sets_per_pool;

        std::vector<PoolRatio> ratios;
        std::vector<Wrapper::DescriptorPool> full_pools;
        std::vector<Wrapper::DescriptorPool> ready_pools;

        inline Wrapper::DescriptorPool & create_pool(uint32_t set_count, std::span<PoolRatio> pool_ratios) {
            return ready_pools.emplace_back(logical_device, set_count, pool_ratios);
        }

        inline Wrapper::DescriptorPool & get_pool() {
            if (!ready_pools.empty()) {
                Wrapper::DescriptorPool & pool = ready_pools.back();
                ready_pools.pop_back();
                return pool;
            }
            else {
                Wrapper::DescriptorPool & pool = create_pool(sets_per_pool, ratios);

                sets_per_pool = expansion_function(sets_per_pool);

                if (sets_per_pool > max_pool_size) {
                    sets_per_pool = max_pool_size;
                }

                return pool;
            }
        }

    public:
        inline DescriptorAllocator(Wrapper::LogicalDevice & _logical_device, uint32_t max_sets, std::span<PoolRatio> pool_ratios):
            logical_device(_logical_device) {
            ratios.assign(pool_ratios.begin(), pool_ratios.end());

            create_pool(max_sets, ratios);

            sets_per_pool = expansion_function(max_sets);
        }

        inline void clear_pools() {
            for (Wrapper::DescriptorPool & pool : full_pools) {
                ready_pools.push_back(std::move(pool));
            }
            full_pools.clear();

            for (Wrapper::DescriptorPool & pool : ready_pools) {
                pool.clear();
            }
        }

        inline Allocation allocate(Wrapper::DescriptorSetLayout && layout, void * next_pointer = nullptr) {
            Wrapper::DescriptorPool & pool = get_pool();

            try { // TODO: maybe try to avoid exceptions here
                Wrapper::DescriptorSet set(logical_device, pool, layout, next_pointer);

                Allocation allocation = {
                    .layout = std::move(layout),
                    .set = std::move(set),
                };

                ready_pools.push_back(std::move(pool));

                return allocation;
            }
            catch (const Wrapper::DescriptorSet::Exceptions::InitError & ex) {
                if (ex.vulkan_result() == VK_ERROR_OUT_OF_POOL_MEMORY || ex.vulkan_result() == VK_ERROR_FRAGMENTED_POOL) {
                    full_pools.push_back(std::move(pool));

                    Wrapper::DescriptorPool & new_pool = get_pool();

                    Allocation allocation = {
                        .layout = std::move(layout),
                        .set = Wrapper::DescriptorSet(logical_device, new_pool, allocation.layout, next_pointer),
                    };

                    ready_pools.push_back(std::move(pool));

                    return allocation;
                }
                else throw ex;
            }
        }
    };
}