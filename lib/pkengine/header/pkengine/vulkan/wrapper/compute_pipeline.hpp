#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/shader_module.hpp>
#include <pkengine/vulkan/wrapper/descriptor_set_layout.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class ComputePipeline {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(LayoutInitError, "Unable to initialize vulkan compute pipeline layout");
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize vulkan compute pipeline");
        };

    protected:
        static constexpr auto logger = Logger<"Vulkan Pipeline">();

        LogicalDevice & logical_device;

        VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
        VkPipeline pipeline = VK_NULL_HANDLE;

    public:
        inline ComputePipeline(
            LogicalDevice & _logical_device,
            std::vector<VkDescriptorSetLayout> descriptor_set_layouts,
            ShaderModule & shader_module
        ):
            logical_device(_logical_device) {
            VkPipelineLayoutCreateInfo compute_layout{
                .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                .pNext = nullptr,
                .setLayoutCount = (uint32_t) descriptor_set_layouts.size(),
                .pSetLayouts = descriptor_set_layouts.data(),
            };

            Util::throw_on_fail<Exceptions::LayoutInitError>(
                vkCreatePipelineLayout(
                    logical_device.handle(),
                    &compute_layout,
                    nullptr,
                    &pipeline_layout
                )
            );

            VkPipelineShaderStageCreateInfo stage_info {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .pNext = nullptr,
                .stage = VK_SHADER_STAGE_COMPUTE_BIT,
                .module = shader_module.handle(),
                .pName = "main",
            };

            VkComputePipelineCreateInfo pipeline_create_info {
                .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
                .pNext = nullptr,
                .stage = stage_info,
                .layout = pipeline_layout,
            };

            Util::throw_on_fail<Exceptions::LayoutInitError>(
                vkCreateComputePipelines(
                    logical_device.handle(),
                    VK_NULL_HANDLE,
                    1,
                    &pipeline_create_info,
                    nullptr,
                    &pipeline
                )
            );

            logger.debug() << "Initialized compute pipeline";
        }

        inline ~ComputePipeline() {
            if (pipeline_layout != VK_NULL_HANDLE) vkDestroyPipelineLayout(logical_device.handle(), pipeline_layout, nullptr);
            if (pipeline != VK_NULL_HANDLE) {
                vkDestroyPipeline(logical_device.handle(), pipeline, nullptr);

                logger.debug() << "Destroyed compute pipeline";
            }
        }

        [[nodiscard]] inline const VkPipeline & handle() const noexcept { return pipeline; }
        [[nodiscard]] inline const VkPipelineLayout & layout() const noexcept { return pipeline_layout; }
    };
}