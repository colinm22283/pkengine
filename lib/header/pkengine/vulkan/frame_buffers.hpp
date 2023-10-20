#pragma once

#include <vector>

#include "frame_buffer.hpp"

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & swap_chain, const auto & render_pass, const auto & image_views>
    class FrameBuffers {
    protected:
        std::vector<FrameBuffer<logical_device, swap_chain, render_pass>> frame_buffers;

    public:
        inline void init() {
            frame_buffers.reserve(image_views.size());

            for (std::size_t i = 0; i < image_views.size(); i++) {
                frame_buffers[i].init(image_views.data()[i]);
            }
        }

        inline void free() {
            for (std::size_t i = 0; i < image_views.size(); i++) {
                frame_buffers[i].free();
            }
        }

        [[nodiscard]] inline const auto & operator[](auto i) const noexcept { return frame_buffers[i]; }
    };
}