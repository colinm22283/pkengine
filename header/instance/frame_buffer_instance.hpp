#pragma once

#include <vulkan/vulkan.h>

#include <logger/logger.hpp>

#include <instance/swap_chain_instance.hpp>
#include <instance/render_pass_instance.hpp>

namespace PKEngine {
    class FrameBufferInstance {
    protected:
        static constexpr auto logger = FileLogger<Util::ANSI::CyanFg, "FRAME BUFFER INST", "pkengine.log">();

        VkDevice device = nullptr;

        VkFramebuffer _instance = nullptr;

    public:
        inline FrameBufferInstance(
            VkDevice _device,
            VkImageView image_view,
            VkExtent2D & swap_chain_extent,
            RenderPassInstance & render_pass
        ): device(_device) {
            logger.log<"Creating frame buffer...">();

            VkImageView attachments[] = {
                image_view
            };

            VkFramebufferCreateInfo framebuffer_info{};
            framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_info.renderPass = render_pass;
            framebuffer_info.attachmentCount = 1;
            framebuffer_info.pAttachments = attachments;
            framebuffer_info.width = swap_chain_extent.width;
            framebuffer_info.height = swap_chain_extent.height;
            framebuffer_info.layers = 1;

            if (vkCreateFramebuffer(device, &framebuffer_info, nullptr, &_instance) != VK_SUCCESS) {
                throw Exceptions::UnableToCreateFramebuffer();
            }

            logger.success().log<"Done!">();
        }

        inline ~FrameBufferInstance() {
            if (_instance != nullptr) {
                logger << "TESTESTSET";

                vkDestroyFramebuffer(device, _instance, nullptr);

                logger.log<"Frame buffer destroyed">();
            }
        }

        FrameBufferInstance(FrameBufferInstance &) = delete;
        FrameBufferInstance(FrameBufferInstance &&) = delete;

        inline auto & instance() { return _instance; }
        inline operator auto &() { return instance(); }
    };
}