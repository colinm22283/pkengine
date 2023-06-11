#pragma once

#include <vector>

#include <instance/frame_buffer_instance.hpp>
#include <instance/swap_chain_instance.hpp>
#include <instance/image_views_instance.hpp>

namespace PKEngine {
    class FrameBuffers {
    protected:
        std::vector<FrameBufferInstance *> instances;

    public:
        inline FrameBuffers(
            VkDevice device,
            SwapChainInstance & swap_chain,
            ImageViewsInstance & image_views,
            RenderPassInstance & render_pass
        ): instances(swap_chain.images.size()) {
            std::cout << "Creating buffers\n";

            for (std::size_t i = 0; i < image_views.size(); i++) {
                std::cout << "Size: " << image_views.size() << "\n";

                instances[i] = new FrameBufferInstance(device, image_views[i], swap_chain.extent, render_pass);
            }

            std::cout << "Done\n";
        }

        inline ~FrameBuffers() {
            for (auto & instance : instances) delete instance;
        }
    };
}