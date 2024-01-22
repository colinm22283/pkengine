#pragma once

#include "pkengine/internal/vulkan/shader_module.hpp"
#include "pkengine/internal/vulkan/pipeline/shader_sequence.hpp"

namespace PKEngine::Manifest {
    using namespace Vulkan;
    using namespace Pipeline;

    class ShaderManifest {
    public:
        static ShaderModule
            default_vertex_shader, default_fragment_shader;

        using DefaultShaderSequence = ShaderSequence<
            ShaderSequenceStage<"main", VERTEX_SHADER, default_vertex_shader>,
            ShaderSequenceStage<"main", FRAGMENT_SHADER, default_fragment_shader>
        >;

        template<const auto & logical_device>
        static inline void init() {
            default_vertex_shader.init<logical_device>("shaders/shader.vert.spv");
            default_fragment_shader.init<logical_device>("shaders/shader.frag.spv");
        }
        template<const auto & logical_device>
        static inline void free() {
            default_fragment_shader.free<logical_device>();
            default_vertex_shader.free<logical_device>();
        }
    };
}