#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/util/vulkan_exception.hpp>

namespace PKEngine::Vulkan::Wrapper::Sync {
    class Fence {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize vulkan fence");
        };
    protected:
        static constexpr auto logger = Logger<"Fence">();

        Wrapper::LogicalDevice & logical_device;

        VkFence fence = VK_NULL_HANDLE;

    public:
        inline Fence(Wrapper::LogicalDevice & _logical_device, bool start_signaled = false): logical_device(_logical_device) {
            logger.debug() << "Initialing vulkan queue...";

            VkFenceCreateInfo create_info {
                .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                .flags = (start_signaled ? VK_FENCE_CREATE_SIGNALED_BIT : VkFenceCreateFlags{}),
            };

            Util::throw_on_fail<Exceptions::InitError>(
                vkCreateFence(logical_device.handle(), &create_info, nullptr, &fence)
            );

            logger.success() << "Vulkan queue initialized";
        }
        inline ~Fence() {
            if (fence != VK_NULL_HANDLE) {
                logger.debug() << "Destroying vulkan fence...";

                vkDestroyFence(logical_device.handle(), fence, nullptr);

                logger.success() << "Vulkan semaphore destroyed...";
            }
        }

        inline Fence(const Fence &) = delete;
        inline Fence(Fence && other) noexcept:
            logical_device(other.logical_device),
            fence(other.fence) {
            other.fence = VK_NULL_HANDLE;
        }

        [[nodiscard]] constexpr const VkFence & handle() const noexcept { return fence; }

        inline void await() {
            // TODO: check errors
            vkWaitForFences(logical_device.handle(), 1, &fence, true, 1000000000);
        }

        inline void reset() {
            // TODO: check errors
            vkResetFences(logical_device.handle(), 1, &fence);
        }
    };
}