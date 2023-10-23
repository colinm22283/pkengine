#pragma once

namespace PKEngine::Vulkan::Pipeline {
    enum shader_sequence_stage_type_t { VERTEX_SHADER, FRAGMENT_SHADER };

    template<Util::ConstString _entry_point, shader_sequence_stage_type_t _type, const auto & _shader_module>
    struct ShaderSequenceStage {
        static constexpr auto entry_point = _entry_point;
        static constexpr auto type = _type;
        static constexpr const auto & shader_module = _shader_module;
    };

    template<typename... Stages>
    class ShaderSequence {
    protected:
        template<typename Stage>
        static inline VkPipelineShaderStageCreateInfo get_shader_info() {
            return VkPipelineShaderStageCreateInfo {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage = Stage::type == VERTEX_SHADER ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT,
                .module = Stage::shader_module.handle(),
                .pName = Stage::entry_point,
            };
        }

    public:
        static inline std::array<VkPipelineShaderStageCreateInfo, sizeof...(Stages)> get_create_infos() {
            return { get_shader_info<Stages>()... };
        };

        static constexpr bool is_shader_sequence = true;
    };
}