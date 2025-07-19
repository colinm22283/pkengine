#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class Sampler {
    public:
        struct Exceptions {

        };

    protected:
        static constexpr auto logger = Logger<"Sampler">();

        LogicalDevice & logical_device;

        VkSampler sampler = VK_NULL_HANDLE;

    public:
        inline Sampler(LogicalDevice & _logical_device, VkFilter min_filter, VkFilter mag_filter):
            logical_device(_logical_device) {
            logger.debug() << "Initializing sampler...";

            VkSamplerCreateInfo create_info = {
                .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
                .magFilter = mag_filter,
                .minFilter = min_filter,
            };

            vkCreateSampler(logical_device.handle(), &create_info, nullptr, &sampler);

            logger.debug() << "Sampler initialized";
        }

        inline ~Sampler() {
            if (sampler != VK_NULL_HANDLE) {
                logger.debug() << "Destroying sampler...";

                vkDestroySampler(logical_device.handle(), sampler, nullptr);

                logger.debug() << "Sampler destroyed";
            }
        }

        [[nodiscard]] inline const VkSampler & handle() const noexcept { return sampler; }
    };
}