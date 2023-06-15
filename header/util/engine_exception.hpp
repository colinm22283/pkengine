#pragma once

#include <exception>

#define PK_EXCEPTION(class_name, throw_string, error_code) \
  class class_name : public PKEngine::Exception {   \
  public:                                          \
    virtual const char * what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override { return throw_string; } \
    virtual const exception_code_t code() const override { return error_code; }                                \
    virtual const bool is_vulkan_error() const override { return false; }                                     \
    virtual const bool is_glfw_error() const override { return false; } \
  };

#define PK_VULKAN_EXCEPTION(class_name, throw_string, error_code) \
  class class_name : public PKEngine::Exception {   \
  public:                                          \
    virtual const char * what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override { return throw_string; } \
    virtual const exception_code_t code() const override { return error_code; }                                \
    virtual const bool is_vulkan_error() const override { return true; }; \
    virtual const bool is_glfw_error() const override { return false; } \
  };

#define PK_GLFW_EXCEPTION(class_name, throw_string, error_code) \
  class class_name : public PKEngine::Exception {   \
  public:                                          \
    virtual const char * what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override { return throw_string; } \
    virtual const exception_code_t code() const override { return error_code; }                                \
    virtual const bool is_vulkan_error() const override { return false; }; \
    virtual const bool is_glfw_error() const override { return true; }                                         \
  };

#define PK_DUAL_EXCEPTION(class_name, throw_string, error_code) \
  class class_name : public PKEngine::Exception {   \
  public:                                          \
    virtual const char * what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override { return throw_string; } \
    virtual const exception_code_t code() const override { return error_code; }                                \
    virtual const bool is_vulkan_error() const override { return true; }; \
    virtual const bool is_glfw_error() const override { return true; }                                         \
  };

namespace PKEngine {
    using exception_code_t = int;

    class Exception : public std::exception {
    public:
        virtual const char * what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW { return "Unknown exception"; }
        virtual const exception_code_t code() const = 0;
        virtual const bool is_vulkan_error() const = 0;
        virtual const bool is_glfw_error() const = 0;
    };

    namespace Exceptions {
        PK_EXCEPTION(VulkanNotSupported,  "Vulkan is not supported on current system",      1);
        PK_EXCEPTION(SuitableGPUNotFound, "Unable to locate a GPU with necessary features", 2);
        PK_EXCEPTION(UnableToOpenFile,        "Unable to open file",                        3)

        PK_GLFW_EXCEPTION(UnableToCreateGLFWWindow, "Unable to create glfw window handle", 1);
        PK_GLFW_EXCEPTION(GLFWInitError,            "Unable to initialize glfw",           2);

        PK_VULKAN_EXCEPTION(UnableToCreateVulkanInstance,      "Unable to create vulkan instance",           1);
        PK_VULKAN_EXCEPTION(UnableToFindGPUsWithVulkanSupport, "Unable to locate GPUs with vulkan support",  2);
        PK_VULKAN_EXCEPTION(UnableToCreateVulkanLogicalDevice, "Unable to create vulkan logical device",     3);
        PK_VULKAN_EXCEPTION(UnableToGetDeviceSurfaceSupport,   "Unable to get device surface support",       4);
        PK_VULKAN_EXCEPTION(UnableToCreateSwapChain,           "Unable to create swap chain",                5);
        PK_VULKAN_EXCEPTION(UnableToCreateImageView,           "Unable to create image view",                6);
        PK_VULKAN_EXCEPTION(UnableToOpenShaderFile,            "Unable to open shader file",                 7);
        PK_VULKAN_EXCEPTION(UnableToCreateShaderModule,        "Unable to create shader module",             8);
        PK_VULKAN_EXCEPTION(UnableToCreatePipelineLayout,      "Unable to create pipeline layout",           9);
        PK_VULKAN_EXCEPTION(UnableToCreateRenderPass,          "Unable to create render pass",              10);
        PK_VULKAN_EXCEPTION(UnableToCreatePipeline,            "Unable to create pipeline",                 11);
        PK_VULKAN_EXCEPTION(UnableToCreateFramebuffer,         "Unable to create framebuffer",              12);
        PK_VULKAN_EXCEPTION(UnableToCreateCommandPool,         "Unable to create command pool",             13);
        PK_VULKAN_EXCEPTION(UnableToCreateCommandBuffer,       "Unable to create command buffer",           14);
        PK_VULKAN_EXCEPTION(UnableToBeginCommandBuffer,        "Unable to begin command buffer",            15);

        PK_DUAL_EXCEPTION(UnableToCreateWindowSurface, "Unable to create window surface", 1);
    }
}