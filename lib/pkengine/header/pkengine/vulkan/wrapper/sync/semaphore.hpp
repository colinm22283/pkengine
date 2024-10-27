#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/util/vulkan_exception.hpp>

namespace PKEngine::Vulkan::Wrapper::Sync {
    class Semaphore {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Failed to initialize vulkan semaphore");
        };
    protected:
        static constexpr auto logger = Logger<"Semaphore">();

        Wrapper::LogicalDevice & logical_device;

        VkSemaphore semaphore = VK_NULL_HANDLE;

    public:
        inline Semaphore(Wrapper::LogicalDevice & _logical_device): logical_device(_logical_device) {
            logger.debug() << "Initialing vulkan semaphore...";

            VkSemaphoreCreateInfo create_info {
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            };

            Util::throw_on_fail<Exceptions::InitError>(
                vkCreateSemaphore(logical_device.handle(), &create_info, nullptr, &semaphore)
            );

            logger.success() << "Vulkan semaphore initialized";
        }
        inline ~Semaphore() {
            if (semaphore != VK_NULL_HANDLE) {
                logger.debug() << "Destroying vulkan semaphore...";

                vkDestroySemaphore(logical_device.handle(), semaphore, nullptr);

                logger.success() << "Vulkan semaphore destroyed...";
            }
        }

        inline Semaphore(const Semaphore &) = delete;
        inline Semaphore(Semaphore && other) noexcept:
            logical_device(other.logical_device),
            semaphore(other.semaphore) {
            other.semaphore = VK_NULL_HANDLE;
        }

        [[nodiscard]] constexpr const VkSemaphore & handle() const noexcept { return semaphore; }
    };
}