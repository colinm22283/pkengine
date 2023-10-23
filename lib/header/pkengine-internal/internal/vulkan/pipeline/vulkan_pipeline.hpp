#pragma once

#include <vulkan/vulkan.h>

#include "shader_sequence.hpp"

namespace PKEngine::Vulkan::Pipeline {
    template<const auto & logical_device, const auto & swap_chain, typename _ShaderSequence, const auto & render_pass, const auto & vertex_buffer>
    class VulkanPipeline { // TODO: finish
    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Vulkan Pipeline">();

        VkPipeline pipeline = VK_NULL_HANDLE;
        VkPipelineLayout layout = VK_NULL_HANDLE;

    public:
        inline void init() {
            logger << "Initializing vulkan pipeline...";

            const auto shader_stages = _ShaderSequence::get_create_infos();

            std::array<VkDynamicState, 2> dynamic_states = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR
            };

            VkPipelineDynamicStateCreateInfo dynamic_state{};
            dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
            dynamic_state.pDynamicStates = dynamic_states.data();

            VkPipelineVertexInputStateCreateInfo vertex_input_info{};
            vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

            auto binding_desc = vertex_buffer.get_bind_info().binding_desc;
            vertex_input_info.vertexBindingDescriptionCount = 1;
            vertex_input_info.pVertexBindingDescriptions = &binding_desc;

            auto attribute_descs = vertex_buffer.get_bind_info().attribute_desc;
            vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertex_buffer.get_bind_info().attribute_desc.size());
            vertex_input_info.pVertexAttributeDescriptions = attribute_descs.data();

            VkPipelineInputAssemblyStateCreateInfo input_assembly{};
            input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            input_assembly.primitiveRestartEnable = VK_FALSE;

            VkViewport viewport { };
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float) swap_chain.extent.width;
            viewport.height = (float) swap_chain.extent.height;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            VkPipelineMultisampleStateCreateInfo multisampling{};
            multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            multisampling.sampleShadingEnable = VK_FALSE;
            multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            multisampling.minSampleShading = 1.0f;
            multisampling.pSampleMask = nullptr;
            multisampling.alphaToCoverageEnable = VK_FALSE;
            multisampling.alphaToOneEnable = VK_FALSE;

            VkPipelineColorBlendAttachmentState color_blend_attachment{};
            color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            color_blend_attachment.blendEnable = VK_TRUE;
            color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
            color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

            VkPipelineColorBlendStateCreateInfo color_blending{};
            color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            color_blending.logicOpEnable = VK_FALSE;
            color_blending.logicOp = VK_LOGIC_OP_COPY;
            color_blending.attachmentCount = 1;
            color_blending.pAttachments = &color_blend_attachment;
            color_blending.blendConstants[0] = 0.0f;
            color_blending.blendConstants[1] = 0.0f;
            color_blending.blendConstants[2] = 0.0f;
            color_blending.blendConstants[3] = 0.0f;

            VkPipelineLayoutCreateInfo pipeline_layout_info{};
            pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipeline_layout_info.setLayoutCount = 0; // Optional
            pipeline_layout_info.pSetLayouts = nullptr; // Optional
            pipeline_layout_info.pushConstantRangeCount = 0; // Optional
            pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

            if (vkCreatePipelineLayout(logical_device.handle(), &pipeline_layout_info, nullptr, &layout) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_pipeline_layout();
            }

            VkRect2D scissor {
                .offset = { 0, 0 },
                .extent = swap_chain.extent,
            };

            VkPipelineViewportStateCreateInfo viewport_state {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                .viewportCount = 1,
                .pViewports = &viewport,
                .scissorCount = 1,
                .pScissors = &scissor,
            };

            VkPipelineRasterizationStateCreateInfo rasterizer {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                .depthClampEnable = VK_FALSE,
                .rasterizerDiscardEnable = VK_FALSE,
                .polygonMode = VK_POLYGON_MODE_FILL,
                .cullMode = VK_CULL_MODE_BACK_BIT,
                .frontFace = VK_FRONT_FACE_CLOCKWISE,

                .depthBiasEnable = VK_FALSE,
                .depthBiasConstantFactor = 0.0f,
                .depthBiasClamp = 0.0f,
                .depthBiasSlopeFactor = 0.0f,

                .lineWidth = 1.0f,
            };

            VkGraphicsPipelineCreateInfo pipelineInfo{};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineInfo.stageCount = 2;
            pipelineInfo.pStages = shader_stages.data();
            pipelineInfo.pVertexInputState = &vertex_input_info;
            pipelineInfo.pInputAssemblyState = &input_assembly;
            pipelineInfo.pViewportState = &viewport_state;
            pipelineInfo.pRasterizationState = &rasterizer;
            pipelineInfo.pMultisampleState = &multisampling;
            pipelineInfo.pDepthStencilState = nullptr; // Optional
            pipelineInfo.pColorBlendState = &color_blending;
            pipelineInfo.pDynamicState = &dynamic_state;
            pipelineInfo.layout = layout;
            pipelineInfo.renderPass = render_pass.handle();
            pipelineInfo.subpass = 0;
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
            pipelineInfo.basePipelineIndex = -1;

            if (vkCreateGraphicsPipelines(logical_device.handle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_pipeline();
            }

            logger.success() << "Vulkan pipeline initialized";
        }
        inline void free() {
            logger << "Destroying vulkan pipeline...";

            if (pipeline != VK_NULL_HANDLE) vkDestroyPipeline(logical_device.handle(), pipeline, nullptr);
            if (layout != VK_NULL_HANDLE) vkDestroyPipelineLayout(logical_device.handle(), layout, nullptr);

            logger.success() << "Vulkan pipeline destroyed";
        }

        [[nodiscard]] inline VkPipeline handle() const noexcept { return pipeline; }


    };
}