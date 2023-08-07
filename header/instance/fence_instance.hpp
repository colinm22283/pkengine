#pragma once

#include <vulkan/vulkan.h>

#include <util/engine_exception.hpp>
#include <logger/logger.hpp>

namespace PKEngine {
    class FenceInstance {
    protected:
        static constexpr auto logger = FileLogger<Util::ANSI::CyanFg, "FENCE INST", "pkengine.log">();

        VkDevice device;

        VkFence _instance;

    public:
        inline FenceInstance(VkDevice _device): device(_device) {
            logger.log<"Creating fence instance...">();

            VkFenceCreateInfo fence_info{};
            fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            if (vkCreateFence(device, &fence_info, nullptr, &_instance) != VK_SUCCESS) {
                throw Exceptions::UnableToCreateFence();
            }

            logger.success().log<"Done!">();
        }

        inline ~FenceInstance() {
            vkDestroyFence(device, _instance, nullptr);

            logger.log<"Fence instance destroyed">();
        }

        inline FenceInstance(FenceInstance &) = delete;
        inline FenceInstance(FenceInstance &&) = delete;

        inline VkFence instance() const noexcept { return _instance; }
        inline operator VkFence() const noexcept { return instance(); }
    };
}