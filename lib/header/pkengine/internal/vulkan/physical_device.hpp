#pragma once

#include <vulkan/vulkan.h>

#include "vulkan_instance.hpp"
#include "surface.hpp"

#include "pkengine/internal/vulkan/util/queue_family_indices.hpp"
#include "pkengine/internal/vulkan/util/are_extensions_supported.hpp"
#include "pkengine/internal/vulkan/util/swap_chain_support.hpp"

namespace PKEngine::Vulkan {
    template<const VulkanInstance & vulkan_instance, const auto & surface>
    class PhysicalDevice {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Physical Device">();

        VkPhysicalDevice physical_device = VK_NULL_HANDLE;

        static inline bool is_device_suitable(VkPhysicalDevice device) {
            QueueFamilyIndices<surface> indices(device);

            bool extensions_supported = are_extensions_supported(device);

            bool swap_chain_adequate = false;
            if (extensions_supported) {
                SwapChainSupport<surface> swap_chain_support(device);
                swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
            }

            return indices.is_complete() && extensions_supported && swap_chain_adequate;
        }

    public:
        inline void init() {
            logger << "Initializing physical device...";

            uint32_t device_count = 0;
            vkEnumeratePhysicalDevices(vulkan_instance.handle(), &device_count, nullptr);
            if (device_count == 0) throw Exception::Internal::cant_find_vulkan_capable_gpu();
            std::vector<VkPhysicalDevice> devices(device_count);
            vkEnumeratePhysicalDevices(vulkan_instance.handle(), &device_count, devices.data());

            for (const auto & device : devices) {
                if (is_device_suitable(device)) {
                    physical_device = device;
                    break;
                }
            }

            if (physical_device == VK_NULL_HANDLE) throw Exception::Internal::cant_find_vulkan_capable_gpu();

            logger.success() << "Physical device initialized";
        }
        inline void free() { }

        inline VkPhysicalDevice handle() const noexcept { return physical_device; }
    };
}