#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <instance/glfw_instance.hpp>
#include <instance/engine_instance.hpp>
#include <instance/vulkan_instance.hpp>
#include <instance/physical_device_instance.hpp>

#include <util/engine_exception.hpp>

#include <types/engine_config.hpp>
#include <types/forward_list.hpp>

namespace PKEngine {
    class Engine {
    protected:
        GLFWInstance glfw_instance;
        VulkanInstance vulkan_instance;

        ForwardList<EngineInstance> instances;

    public:
        inline explicit Engine(engine_config_t config):
          glfw_instance(config),
          vulkan_instance(config) { }
//        inline ~Engine() { }

        inline EngineInstance & create_instance(instance_config_t config) {
            return instances.emplace_front(vulkan_instance, config);
        }
    };
}