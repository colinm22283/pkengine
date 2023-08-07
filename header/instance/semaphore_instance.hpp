#pragma once

#include <vulkan/vulkan.h>

#include <util/engine_exception.hpp>

#include <logger/logger.hpp>

namespace PKEngine {
    class SemaphoreInstance {
    protected:
        static constexpr auto logger = FileLogger<Util::ANSI::CyanFg, "SEMAPHORE INST", "pkengine.log">();

        VkDevice device;

        VkSemaphore _instance;

    public:
        inline SemaphoreInstance(VkDevice _device): device(_device) {
            logger.log<"Creating semaphore instance...">();

            VkSemaphoreCreateInfo semaphore_info{
                .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            };

            if (vkCreateSemaphore(device, &semaphore_info, nullptr, &_instance) != VK_SUCCESS) {
                throw Exceptions::UnableToCreateSemaphore();
            }

            logger.success().log<"Done!">();
        }

        inline ~SemaphoreInstance() {
            vkDestroySemaphore(device, _instance, nullptr);

            logger.log<"Semaphore instance destroyed">();
        }

        inline SemaphoreInstance(SemaphoreInstance &) = delete;
        inline SemaphoreInstance(SemaphoreInstance &&) = delete;

        inline VkSemaphore instance() const noexcept { return _instance; }
        inline operator VkSemaphore() const noexcept { return instance(); }
    };
}