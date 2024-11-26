#pragma once

#include <vulkan/vulkan.h>

#include <pkengine/logger/logger.hpp>

#include <pkengine/vulkan/wrapper/logical_device.hpp>

namespace PKEngine::Vulkan::Wrapper {
    class GraphicsPipeline {
    public:
        struct Exceptions {

        };

    protected:
        static constexpr auto logger = Logger<"Graphics Pipeline">();

        LogicalDevice & logical_device;

        VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
        VkPipeline pipeline = VK_NULL_HANDLE;

    public:
        inline GraphicsPipeline(LogicalDevice & _logical_device):
            logical_device(_logical_device) {
            
        }
    };
}