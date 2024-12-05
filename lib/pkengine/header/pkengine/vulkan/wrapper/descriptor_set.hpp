#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/descriptor_pool.hpp>
#include <pkengine/vulkan/wrapper/descriptor_set_layout.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class DescriptorSet {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize descriptor set");
        };

    protected:
        static constexpr auto logger = Logger<"Descriptor Set">();

        LogicalDevice & logical_device;

        VkDescriptorSet descriptor_set = VK_NULL_HANDLE;

    public:
        inline DescriptorSet(LogicalDevice & _logical_device, DescriptorPool & descriptor_pool, DescriptorSetLayout & set_layout):
            logical_device(_logical_device) {
            logger.debug() << "Initializing descriptor set...";

            VkDescriptorSetAllocateInfo allocInfo = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .pNext = nullptr,
                .descriptorPool = descriptor_pool.handle(),
                .descriptorSetCount = 1,
                .pSetLayouts = &set_layout.handle(),
            };

            Util::throw_on_fail<Exceptions::InitError>(
                vkAllocateDescriptorSets(logical_device.handle(), &allocInfo, &descriptor_set)
            );

            logger.debug() << "Descriptor set initialized";
        }

        [[nodiscard]] inline const VkDescriptorSet & handle() const noexcept { return descriptor_set; }
    };
}