#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>

#include <pkengine/vulkan/shader_struct/graphical_push_constants.hpp>

#include <pkengine/vulkan/util/vulkan_exception.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class GraphicsPipeline {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(LayoutInitError, "Unable to initialize graphics pipeline layout");
            PKENGINE_DEFINE_VULKAN_EXCEPTION(InitError, "Unable to initialize graphics pipeline");
        };

    protected:
        static constexpr auto logger = Logger<"Graphics Pipeline">();

        LogicalDevice & logical_device;

        VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
        VkPipeline pipeline = VK_NULL_HANDLE;

    public:
        inline GraphicsPipeline(
            LogicalDevice & _logical_device,

            std::span<VkPipelineShaderStageCreateInfo> shader_stages,
            VkPipelineInputAssemblyStateCreateInfo input_assembly,
            VkPipelineRasterizationStateCreateInfo rasterizer,
            VkPipelineColorBlendAttachmentState color_blend_attachment,
            VkPipelineMultisampleStateCreateInfo multisampling,
            VkPipelineDepthStencilStateCreateInfo depth_stencil,
            VkPipelineRenderingCreateInfo render_info
        ):
            logical_device(_logical_device) {
            logger.debug() << "Initializing graphics pipeline...";

            VkPipelineViewportStateCreateInfo viewport_state = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                .pNext = nullptr,
                .viewportCount = 1,
                .scissorCount = 1,
            };

            VkPipelineColorBlendStateCreateInfo color_blending = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .pNext = nullptr,

                .logicOpEnable = VK_FALSE,
                .logicOp = VK_LOGIC_OP_COPY,
                .attachmentCount = 1,
                .pAttachments = &color_blend_attachment,
            };

            VkPipelineVertexInputStateCreateInfo vertex_input_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            };

            VkDynamicState dynamic_states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
            VkPipelineDynamicStateCreateInfo dynamic_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                .dynamicStateCount = 2,
                .pDynamicStates = &dynamic_states[0],
            };

            VkPushConstantRange push_constant_range = {
                .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,

                .offset = 0,
                .size = sizeof(ShaderStruct::GraphicalPushConstants),
            };

            VkPipelineLayoutCreateInfo layout_info = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                .pNext = nullptr,
                .setLayoutCount = (uint32_t) 0,

                .pushConstantRangeCount = 1,
                .pPushConstantRanges = &push_constant_range,
            };

            Util::throw_on_fail<Exceptions::LayoutInitError>(
                vkCreatePipelineLayout(
                    logical_device.handle(),
                    &layout_info,
                    nullptr,
                    &pipeline_layout
                )
            );

            VkGraphicsPipelineCreateInfo pipeline_info = {
                .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                .pNext = &render_info,
                .stageCount = (uint32_t) shader_stages.size(),
                .pStages = shader_stages.data(),
                .pVertexInputState = &vertex_input_info,
                .pInputAssemblyState = &input_assembly,
                .pViewportState = &viewport_state,
                .pRasterizationState = &rasterizer,
                .pMultisampleState = &multisampling,
                .pDepthStencilState = &depth_stencil,
                .pColorBlendState = &color_blending,
                .pDynamicState = &dynamic_info,
                .layout = pipeline_layout,
            };

            Util::throw_on_fail<Exceptions::InitError>(
                vkCreateGraphicsPipelines(
                    logical_device.handle(),
                    VK_NULL_HANDLE,
                    1,
                    &pipeline_info,
                    nullptr,
                    &pipeline
                )
            );

            logger.debug() << "Graphical pipeline initialized";
        }

        inline ~GraphicsPipeline() {
            if (pipeline_layout != VK_NULL_HANDLE) vkDestroyPipelineLayout(logical_device.handle(), pipeline_layout, nullptr);
            if (pipeline != VK_NULL_HANDLE) {
                logger.debug() << "Destroying graphical pipeline...";

                vkDestroyPipeline(logical_device.handle(), pipeline, nullptr);

                logger.debug() << "Graphical pipeline destroyed";
            }
        }

        inline GraphicsPipeline(const GraphicsPipeline &) = delete;
        inline GraphicsPipeline(GraphicsPipeline && other) noexcept:
            logical_device(other.logical_device),
            pipeline_layout(other.pipeline_layout),
            pipeline(other.pipeline) {
            other.pipeline_layout = VK_NULL_HANDLE;
            other.pipeline = VK_NULL_HANDLE;
        }

        [[nodiscard]] inline const VkPipeline & handle() const noexcept { return pipeline; }
        [[nodiscard]] inline const VkPipelineLayout & layout() const noexcept { return pipeline_layout; }
    };
}