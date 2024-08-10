#pragma once

namespace PKEngine::Vulkan {
    template<const auto & logical_device>
    class DescriptorSetLayout {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Vulkan Descriptor Set Layout">();
        VkDescriptorSetLayout layout = VK_NULL_HANDLE;

    public:
        inline void init() {
            logger << "Initializing vulkan descriptor set layout...";

            VkDescriptorSetLayoutBinding layout_binding = {
                .binding = 0,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = 1,
                .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = nullptr,
            };

            VkDescriptorSetLayoutCreateInfo layout_info{};
            layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layout_info.bindingCount = 1;
            layout_info.pBindings = &layout_binding;

            if (vkCreateDescriptorSetLayout(logical_device.handle(), &layout_info, nullptr, &layout) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_vulkan_descriptor_set_layout();
            }

            logger.success() << "Vulkan descriptor set layout initialized";
        }

        inline void free() {
            logger << "Destroying vulkan descriptor set layout...";

            if (layout != VK_NULL_HANDLE) vkDestroyDescriptorSetLayout(logical_device.handle(), layout, nullptr);

            logger.success() << "Vulkan descriptor set layout destroyed";
        }

        [[nodiscard]] inline VkDescriptorSetLayout handle() const noexcept { return layout; }
    };
}