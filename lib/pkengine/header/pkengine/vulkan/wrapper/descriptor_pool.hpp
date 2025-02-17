#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class DescriptorPool {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize descriptor pool");
        };

    protected:
        static constexpr auto logger = Logger<"Descriptor Pool">();

        LogicalDevice & logical_device;

        VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;

    public:
        struct PoolRatio {
            VkDescriptorType type;
            float ratio;
        };

        inline DescriptorPool(LogicalDevice & _logical_device, uint32_t max_sets, std::span<PoolRatio> pool_ratios):
            logical_device(_logical_device) {
            logger.debug() << "Initializing descriptor pool...";

            VkDescriptorPoolSize pool_sizes[pool_ratios.size()];

            for (std::size_t i = 0; i < pool_ratios.size(); i++) {
                pool_sizes[i].type = pool_ratios[i].type;
                pool_sizes[i].descriptorCount = uint32_t(pool_ratios[i].ratio * (float) max_sets);
            }

            VkDescriptorPoolCreateInfo pool_info = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                .flags = 0,
                .maxSets = max_sets,
                .poolSizeCount = (uint32_t) pool_ratios.size(),
                .pPoolSizes = pool_sizes,
            };

            Util::throw_on_fail<Exceptions::InitError>(
                vkCreateDescriptorPool(logical_device.handle(), &pool_info, nullptr, &descriptor_pool)
            );

            logger.debug() << "Descriptor pool initialized";
        }

        inline ~DescriptorPool() {
            if (descriptor_pool != VK_NULL_HANDLE) {
                logger.debug() << "Destroying descriptor pool...";

                vkDestroyDescriptorPool(logical_device.handle(), descriptor_pool, nullptr);

                logger.debug() << "Descriptor pool destroyed";
            }
        }

        inline DescriptorPool(const DescriptorPool &) = delete;
        inline DescriptorPool(DescriptorPool && other) noexcept:
            logical_device(other.logical_device),
            descriptor_pool(other.descriptor_pool) {
            other.descriptor_pool = VK_NULL_HANDLE;
        }

        [[nodiscard]] inline const VkDescriptorPool & handle() const noexcept { return descriptor_pool; }

        inline void clear() {
            vkResetDescriptorPool(logical_device.handle(), descriptor_pool, 0);
        }
    };
}