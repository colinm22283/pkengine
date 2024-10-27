#pragma once

#include <thread>
#include <vector>

#include <pkengine/glfw/glfw_instance.hpp>
#include <pkengine/glfw/window.hpp>

#include <pkengine/vulkan/frame_data.hpp>
#include <pkengine/vulkan/wrapper/vulkan_instance.hpp>
#include <pkengine/vulkan/wrapper/surface.hpp>
#include <pkengine/vulkan/wrapper/physical_device.hpp>
#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/command_queue.hpp>
#include <pkengine/vulkan/wrapper/swap_chain.hpp>
#include <pkengine/vulkan/wrapper/image_views.hpp>

#include <pkengine/util/make_array.hpp>

#include <render_config.hpp>

namespace PKEngine {
    using namespace Util;
    using namespace Vulkan;
    using namespace Vulkan::Wrapper;
    using namespace Vulkan::Util;

    class Context {
        friend class Engine;
    protected:
    public:
        GLFW::Window window;

        VulkanInstance vulkan_instance;
        Surface surface = Surface(vulkan_instance, window);
        PhysicalDevice physical_device = PhysicalDevice(vulkan_instance, surface);
        QueueFamilyIndices queue_family_indices = QueueFamilyIndices(surface, physical_device.handle());
        LogicalDevice logical_device = LogicalDevice(physical_device, queue_family_indices);

        CommandQueue graphics_queue = CommandQueue(
            logical_device,
            queue_family_indices.graphics_family.value()
        );
        CommandQueue present_queue = CommandQueue(
            logical_device,
            queue_family_indices.present_family.value()
        );

        SwapChain swap_chain = SwapChain(window, physical_device, logical_device, surface, queue_family_indices);
        ImageViews image_views = ImageViews(logical_device, swap_chain);

        std::size_t current_frame = 0;
        std::array<FrameData, Config::render_config.max_frames_in_flight> frames =
            make_array<FrameData, Config::render_config.max_frames_in_flight>(swap_chain, logical_device, queue_family_indices);

        [[nodiscard]] FrameData & next_frame() noexcept {
            FrameData & frame = frames[current_frame];

            current_frame = (current_frame + 1) % frames.size();

            return frame;
        }

    public:
        explicit inline Context() = default;

        inline Context(const Context &) = delete;
        inline Context(Context &&) noexcept = default;

        inline void update() {
            FrameData & frame = next_frame();

            frame.draw();
        }

        inline void exit() noexcept {
            window.close();
        }

        [[nodiscard]] inline bool should_exit() const noexcept {
            return window.should_close();
        }
    };
}