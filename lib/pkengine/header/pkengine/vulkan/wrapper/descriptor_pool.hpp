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

        VkDescriptorPool descriptor_pool;

    public:
        struct PoolRatio {
            VkDescriptorType type;
            float ratio;
        };

        inline DescriptorPool(LogicalDevice & _logical_device, uint32_t max_sets, std::vector<PoolRatio> pool_ratios):
            logical_device(_logical_device) {
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

            logger.debug() << "Initialized descriptor pool";
        }

        inline ~DescriptorPool() {
            if (descriptor_pool != VK_NULL_HANDLE) {
                vkDestroyDescriptorPool(logical_device.handle(), descriptor_pool, nullptr);

                logger.debug() << "Destroyed descriptor pool";
            }
        }

        [[nodiscard]] inline const VkDescriptorPool & handle() const noexcept { return descriptor_pool; }
    };
}