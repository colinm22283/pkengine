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
        inline DescriptorSet(LogicalDevice & _logical_device, DescriptorPool & descriptor_pool, DescriptorSetLayout & set_layout, void * next_pointer = nullptr):
            logical_device(_logical_device) {
            logger.debug() << "Initializing descriptor set...";

            VkDescriptorSetAllocateInfo allocInfo = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .pNext = next_pointer,
                .descriptorPool = descriptor_pool.handle(),
                .descriptorSetCount = 1,
                .pSetLayouts = &set_layout.handle(),
            };

            Util::throw_on_fail<Exceptions::InitError>(
                vkAllocateDescriptorSets(logical_device.handle(), &allocInfo, &descriptor_set)
            );

            logger.debug() << "Descriptor set initialized";
        }

        inline DescriptorSet(const DescriptorSet &) = delete;
        inline DescriptorSet(DescriptorSet && other) noexcept:
            logical_device(other.logical_device),
            descriptor_set(other.descriptor_set) { }

        [[nodiscard]] inline const VkDescriptorSet & handle() const noexcept { return descriptor_set; }
    };
}