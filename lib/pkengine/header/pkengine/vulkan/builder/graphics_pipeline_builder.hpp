#pragma once

#include <span>
#include <array>

#include <vulkan/vulkan.h>

#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/graphics_pipeline.hpp>
#include <pkengine/vulkan/wrapper/shader_module.hpp>

#include <pkengine/vulkan/struct/pipeline_shader_stage_create_info.hpp>

namespace PKEngine::Vulkan::Builder {
    class GraphicsPipelineBuilder {
    protected:
        std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
        VkPipelineInputAssemblyStateCreateInfo input_assembly = { .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
        VkPipelineRasterizationStateCreateInfo rasterizer = { .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
        VkPipelineColorBlendAttachmentState color_blend_attachment;
        VkPipelineMultisampleStateCreateInfo multisampling = { .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
        VkPipelineDepthStencilStateCreateInfo depth_stencil = { .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
        VkPipelineRenderingCreateInfo render_info = { .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO };
        VkFormat color_attachment_format;

    public:
        inline GraphicsPipelineBuilder & set_shaders(
            Wrapper::ShaderModule & vertex_shader,
            Wrapper::ShaderModule & fragment_shader
        ) {
            shader_stages.push_back(
                Struct::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, vertex_shader.handle())
            );
            shader_stages.push_back(
                Struct::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, fragment_shader.handle())
            );

            return *this;
        }

        inline GraphicsPipelineBuilder & set_input_topology(VkPrimitiveTopology topology) {
            input_assembly.topology = topology;
            input_assembly.primitiveRestartEnable = VK_FALSE;

            return *this;
        }

        inline GraphicsPipelineBuilder & set_polygon_mode(VkPolygonMode mode) {
            rasterizer.polygonMode = mode;
            rasterizer.lineWidth = 1.0f;

            return *this;
        }

        inline GraphicsPipelineBuilder & set_cull_mode(VkCullModeFlags mode, VkFrontFace front_face) {
            rasterizer.cullMode = mode;
            rasterizer.frontFace = front_face;

            return *this;
        }

        inline GraphicsPipelineBuilder & set_multisampling_none() {
            multisampling.sampleShadingEnable = VK_FALSE;
            multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            multisampling.minSampleShading = 1.0f;
            multisampling.pSampleMask = nullptr;
            multisampling.alphaToCoverageEnable = VK_FALSE;
            multisampling.alphaToOneEnable = VK_FALSE;

            return *this;
        }

        inline GraphicsPipelineBuilder & disable_blending() {
            color_blend_attachment.colorWriteMask =
                VK_COLOR_COMPONENT_R_BIT |
                VK_COLOR_COMPONENT_G_BIT |
                VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT;
            color_blend_attachment.blendEnable = VK_FALSE;

            return *this;
        }

        inline GraphicsPipelineBuilder & set_color_attachment_format(VkFormat format) {
            color_attachment_format = format;
            render_info.colorAttachmentCount = 1;
            render_info.pColorAttachmentFormats = &color_attachment_format;

            return *this;
        }

        inline GraphicsPipelineBuilder & set_depth_format(VkFormat format) {
            render_info.depthAttachmentFormat = format;

            return *this;
        }

        inline GraphicsPipelineBuilder & disable_depthtest() {
            depth_stencil.depthTestEnable = VK_FALSE;
            depth_stencil.depthWriteEnable = VK_FALSE;
            depth_stencil.depthCompareOp = VK_COMPARE_OP_NEVER;
            depth_stencil.depthBoundsTestEnable = VK_FALSE;
            depth_stencil.stencilTestEnable = VK_FALSE;
            depth_stencil.front = {};
            depth_stencil.back = {};
            depth_stencil.minDepthBounds = 0.0f;
            depth_stencil.maxDepthBounds = 1.0f;

            return *this;
        }
        inline GraphicsPipelineBuilder & enable_depthtest(bool depth_write_enable, VkCompareOp op) {
            depth_stencil.depthTestEnable = VK_TRUE;
            depth_stencil.depthWriteEnable = depth_write_enable;
            depth_stencil.depthCompareOp = op;
            depth_stencil.depthBoundsTestEnable = VK_FALSE;
            depth_stencil.stencilTestEnable = VK_FALSE;
            depth_stencil.front = { };
            depth_stencil.back = { };
            depth_stencil.minDepthBounds = 0.f;
            depth_stencil.maxDepthBounds = 1.f;

            return *this;
        }

        inline GraphicsPipelineBuilder & enable_blending_additive() {
            color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            color_blend_attachment.blendEnable = VK_TRUE;
            color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
            color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
            color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

            return *this;
        }

        inline GraphicsPipelineBuilder & enable_blending_alphablend() {
            color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            color_blend_attachment.blendEnable = VK_TRUE;
            color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
            color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

            return *this;
        }

        inline auto build(Wrapper::LogicalDevice & logical_device) {
            return Wrapper::GraphicsPipeline(
                logical_device,
                shader_stages,
                input_assembly,
                rasterizer,
                color_blend_attachment,
                multisampling,
                depth_stencil,
                render_info
            );
        }
    };
}