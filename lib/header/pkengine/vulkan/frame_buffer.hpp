#pragma once

namespace PKEngine::Vulkan {
    template<const auto & logical_device, const auto & swap_chain, const auto & render_pass>
    class FrameBuffer {
    protected:
        static constexpr auto logger = Logger<Util::ANSI::BlueFg, "Frame Buffer">();

        VkFramebuffer frame_buffer = VK_NULL_HANDLE;

    public:
        inline void init(VkImageView image_view) {
            logger << "Initializing frame buffer...";

            VkImageView attachments[] = {
                image_view
            };

            VkFramebufferCreateInfo frame_buffer_info{};
            frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            frame_buffer_info.renderPass = render_pass.handle();
            frame_buffer_info.attachmentCount = 1;
            frame_buffer_info.pAttachments = attachments;
            frame_buffer_info.width = swap_chain.extent.width;
            frame_buffer_info.height = swap_chain.extent.height;
            frame_buffer_info.layers = 1;

            if (vkCreateFramebuffer(logical_device.handle(), &frame_buffer_info, nullptr, &frame_buffer) != VK_SUCCESS) {
                throw Exception::Internal::vulkan_unable_to_create_vulkan_frame_buffer();
            }

            logger.success() << "Frame buffer initialized";
        }

        inline void free() {
            logger << "Destroying frame buffer...";

            if (frame_buffer != VK_NULL_HANDLE) vkDestroyFramebuffer(logical_device.handle(), frame_buffer, nullptr);

            logger.success() << "Frame buffer destroyed";
        }

        [[nodiscard]] inline VkFramebuffer handle() const noexcept { return frame_buffer; }
    };
}