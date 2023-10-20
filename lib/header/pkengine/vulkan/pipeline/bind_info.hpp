#pragma once

namespace PKEngine::Vulkan::Pipeline {
    template<std::size_t attribute_count>
    struct bind_info_t {
        VkVertexInputBindingDescription binding_desc;
        std::array<VkVertexInputAttributeDescription, attribute_count> attribute_desc;
    };
}