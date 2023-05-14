#pragma once

#include <window.hpp>
#include <surface.hpp>

#include <instance/physical_device_instance.hpp>
#include <instance/queue_instance.hpp>
#include <instance/logical_device_instance.hpp>
#include <instance/swap_chain_instance.hpp>
#include <instance/image_views_instance.hpp>

#include <types/engine_config.hpp>

#include <query/query_queue_families.hpp>

namespace PKEngine {
    class EngineInstance {
    public:
        Window window;
        Surface surface;
        PhysicalDeviceInstance physical_device;
        Query::queue_family_indices_t indices;
        LogicalDeviceInstance logical_device;
        QueueInstance graphics_queue, present_queue;
        SwapChainInstance swap_chain;
        ImageViewsInstance image_views;

        inline explicit EngineInstance(VkInstance instance, instance_config_t & config):
            window(config),
            surface(instance, window),
            physical_device(instance, surface),
            indices(Query::query_queue_families(physical_device, surface)),
            logical_device(physical_device, indices),
            graphics_queue(logical_device, indices.graphics_family.value()),
            present_queue(logical_device, indices.present_family.value()),
            swap_chain(physical_device, logical_device, surface, window),
            image_views(logical_device, swap_chain) { }

        EngineInstance(EngineInstance &) = delete;
        EngineInstance(EngineInstance &&) = delete;
    };
}