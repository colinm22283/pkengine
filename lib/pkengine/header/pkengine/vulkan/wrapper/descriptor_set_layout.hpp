#pragma once

#include <span>

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>

#include <pkengine/vulkan/util/vulkan_exception.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class DescriptorSetLayout {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize vulkan descriptor set layout");
        };

    protected:
        static constexpr auto logger = Logger<"Descriptor Set Layout">();

        LogicalDevice & logical_device;

        VkDescriptorSetLayout desc_set_layout = VK_NULL_HANDLE;

    public:
        inline DescriptorSetLayout(
            LogicalDevice & _logical_device,
            std::vector<VkDescriptorSetLayoutBinding> layout_bindings,
            VkShaderStageFlags stage_flags,
            VkDescriptorSetLayoutCreateFlags create_flags
        ):
            logical_device(_logical_device) {
            for (VkDescriptorSetLayoutBinding & binding : layout_bindings) {
                binding.stageFlags |= stage_flags;
            }

            VkDescriptorSetLayoutCreateInfo layout_info = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                .pNext = nullptr,

                .flags = create_flags,

                .bindingCount = (uint32_t) layout_bindings.size(),
                .pBindings = layout_bindings.data(),
            };

            Util::throw_on_fail<Exceptions::InitError>(
                vkCreateDescriptorSetLayout(logical_device.handle(), &layout_info, nullptr, &desc_set_layout)
            );

            logger.debug() << "Initialized descriptor set layout";
        }

        inline ~DescriptorSetLayout() {
            if (desc_set_layout != VK_NULL_HANDLE) {
                vkDestroyDescriptorSetLayout(logical_device.handle(), desc_set_layout, nullptr);

                logger.debug() << "Destroyed descriptor set layout";
            }
        }

        [[nodiscard]] inline const VkDescriptorSetLayout & handle() const noexcept { return desc_set_layout; }
    };
}