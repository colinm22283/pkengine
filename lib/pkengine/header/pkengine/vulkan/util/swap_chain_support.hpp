#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include <pkengine/glfw/window.hpp>

#include <pkengine/vulkan/surface.hpp>
#include <pkengine/vulkan/util/vulkan_exception.hpp>

namespace PKEngine::Vulkan::Util {
    class SwapChainSupport {
    public:
        struct Exceptions {
            PKENGINE_DEFINE_VULKAN_EXCEPTION(GetSurfaceCapabilitiesError, "Unable to read surface capabilities from device");
            PKENGINE_DEFINE_VULKAN_EXCEPTION(GetSurfaceFormatsError, "Unable to read surface formats from device");
            PKENGINE_DEFINE_VULKAN_EXCEPTION(GetSurfacePresentModesError, "Unable to read surface present modes from device");
        };
    protected:
        VkSurfaceCapabilitiesKHR _capabilities = { };
        std::vector<VkSurfaceFormatKHR> _formats;
        std::vector<VkPresentModeKHR> _present_modes;

    public:
        inline SwapChainSupport(Surface & surface, const VkPhysicalDevice & device) {
            Util::throw_on_fail<Exceptions::GetSurfaceCapabilitiesError>(
                vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
                    device,
                    surface.handle(),
                    &_capabilities
                )
            );

            uint32_t format_count;
            Util::throw_on_fail<Exceptions::GetSurfaceFormatsError>(
                vkGetPhysicalDeviceSurfaceFormatsKHR(
                    device,
                    surface.handle(),
                    &format_count,
                    nullptr
                )
            );

            if (format_count != 0) {
                _formats.resize(format_count);
                Util::throw_on_fail<Exceptions::GetSurfaceFormatsError>(
                    vkGetPhysicalDeviceSurfaceFormatsKHR(
                        device,
                        surface.handle(),
                        &format_count,
                        _formats.data()
                    )
                );
            }

            uint32_t present_mode_count;
            Util::throw_on_fail<Exceptions::GetSurfacePresentModesError>(
                vkGetPhysicalDeviceSurfacePresentModesKHR(
                    device,
                    surface.handle(),
                    &present_mode_count,
                    nullptr
                )
            );

            if (present_mode_count != 0) {
                _present_modes.resize(present_mode_count);
                Util::throw_on_fail<Exceptions::GetSurfacePresentModesError>(
                    vkGetPhysicalDeviceSurfacePresentModesKHR(
                        device,
                        surface.handle(),
                        &present_mode_count,
                        _present_modes.data()
                    )
                );
            }
        }

        [[nodiscard]] constexpr const VkSurfaceCapabilitiesKHR & capabilities() const noexcept { return _capabilities; }
        [[nodiscard]] constexpr const std::vector<VkSurfaceFormatKHR> & formats() const noexcept { return _formats; }
        [[nodiscard]] constexpr const std::vector<VkPresentModeKHR> & present_modes() const noexcept { return _present_modes; }

        [[nodiscard]] inline const VkSurfaceFormatKHR & choose_surface_format() const noexcept {
            for (const auto & available_format : _formats) {
                if (
                    available_format.format == VK_FORMAT_B8G8R8A8_SRGB &&
                    available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
                ) {
                    return available_format;
                }
            }

            return _formats[0];
        }

        [[nodiscard]] inline VkPresentModeKHR choose_present_mode() const noexcept {
            for (const auto & available_present_mode : _present_modes) {
                if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    return available_present_mode;
                }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }

        [[nodiscard]] inline VkExtent2D choose_swap_extent(GLFW::Window & window) const noexcept {
            if (_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return _capabilities.currentExtent;
            } else {
                int width, height;
                glfwGetFramebufferSize(window.handle(), &width, &height);

                VkExtent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
                };

                actualExtent.width = std::clamp(
                    actualExtent.width,
                    _capabilities.minImageExtent.width,
                    _capabilities.maxImageExtent.width
                );
                actualExtent.height = std::clamp(
                    actualExtent.height,
                    _capabilities.minImageExtent.height,
                    _capabilities.maxImageExtent.height);

                return actualExtent;
            }
        }
    };
}