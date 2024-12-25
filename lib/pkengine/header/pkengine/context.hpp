#pragma once

#include <thread>
#include <vector>

#include <pkengine/math/deg_rad.hpp>

#include <pkengine/glfw/glfw_instance.hpp>
#include <pkengine/glfw/window.hpp>

#include <pkengine/vulkan/frame_data.hpp>
#include <pkengine/vulkan/mesh.hpp>
#include <pkengine/vulkan/camera_data.hpp>

#include <pkengine/vulkan/wrapper/vulkan_instance.hpp>
#include <pkengine/vulkan/wrapper/surface.hpp>
#include <pkengine/vulkan/wrapper/physical_device.hpp>
#include <pkengine/vulkan/wrapper/logical_device.hpp>
#include <pkengine/vulkan/wrapper/command_queue.hpp>
#include <pkengine/vulkan/wrapper/swap_chain.hpp>
#include <pkengine/vulkan/wrapper/image_views.hpp>
#include <pkengine/vulkan/wrapper/descriptor_pool.hpp>
#include <pkengine/vulkan/wrapper/descriptor_set_layout.hpp>
#include <pkengine/vulkan/wrapper/descriptor_set.hpp>
#include <pkengine/vulkan/wrapper/shader_module.hpp>
#include <pkengine/vulkan/wrapper/compute_pipeline.hpp>
#include <pkengine/vulkan/wrapper/graphics_pipeline.hpp>

#include <pkengine/vulkan/builder/graphics_pipeline_builder.hpp>

#include <pkengine/vulkan/alloc/allocated_image.hpp>

#include <pkengine/util/make_array.hpp>
#include <pkengine/util/erasable_list.hpp>

#include <render_config.hpp>

namespace PKEngine {
    using namespace Util;
    using namespace Vulkan;
    using namespace Vulkan::Wrapper;
    using namespace Vulkan::Builder;
    using namespace Vulkan::Util;

    class Context {
        friend class Engine;

    public:
        using MeshList = PKEngine::Util::ErasableList<Mesh>;

    protected:
        static constexpr auto logger = Logger<"Context">();

        volatile struct {
            bool ready = false;
            int target_width = 0, target_height = 0;
        } resize_info;

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

        CommandPool imm_command_pool = CommandPool(
            logical_device,
            queue_family_indices.graphics_family.value()
        );
        CommandBuffer imm_command_buffer = CommandBuffer(
            logical_device,
            imm_command_pool
        );
        Sync::Fence imm_fence = Sync::Fence(logical_device, true);

        SwapChain swap_chain = SwapChain(window, physical_device, logical_device, surface, queue_family_indices);
        ImageViews image_views = ImageViews(logical_device, swap_chain);

        Alloc::VulkanAllocator allocator = Alloc::VulkanAllocator(vulkan_instance, physical_device, logical_device);

        float render_scale = 1.0f;
        Alloc::AllocatedImage draw_image = Alloc::AllocatedImage(
            logical_device,
            allocator,
            VK_FORMAT_R16G16B16A16_SFLOAT,
            VkExtent3D {
                .width = (uint32_t) window.width(),
                .height = (uint32_t) window.height(),
                .depth = 1,
            },
            (
                VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                VK_IMAGE_USAGE_STORAGE_BIT |
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
            ),
            VMA_MEMORY_USAGE_GPU_ONLY,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            VK_IMAGE_ASPECT_COLOR_BIT
        );
        Alloc::AllocatedImage depth_image = Alloc::AllocatedImage(
            logical_device,
            allocator,
            VK_FORMAT_D32_SFLOAT,
            draw_image.extent(),
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VMA_MEMORY_USAGE_GPU_ONLY,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            VK_IMAGE_ASPECT_DEPTH_BIT
        );

        DescriptorPool global_descriptor_pool = DescriptorPool(
            logical_device,
            100,
            std::vector<DescriptorPool::PoolRatio>({
                DescriptorPool::PoolRatio {
                    .type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                    .ratio = 1.0f,
                },
            })
        );
        DescriptorSetLayout draw_image_descriptor_set_layout = DescriptorSetLayout(
            logical_device,
            std::vector<VkDescriptorSetLayoutBinding>({
                VkDescriptorSetLayoutBinding {
                    .binding = 0,
                    .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                    .descriptorCount = 1,
                },
            }),
            VK_SHADER_STAGE_COMPUTE_BIT,
            0
        );

        DescriptorSet draw_image_descriptor_set = DescriptorSet(
            logical_device,
            global_descriptor_pool,
            draw_image_descriptor_set_layout
        );

        ShaderModule vert_shader = ShaderModule(logical_device, "shaders/colored_triangle.vert.spv");
        ShaderModule frag_shader = ShaderModule(logical_device, "shaders/colored_triangle.frag.spv");

        GraphicsPipeline graphics_pipeline = GraphicsPipelineBuilder()
            .set_shaders(vert_shader, frag_shader)
            .set_input_topology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
            .set_polygon_mode(VK_POLYGON_MODE_FILL)
            .set_cull_mode(VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE)
            .set_multisampling_none()
            .enable_blending_alphablend()
            .enable_depthtest(true, VK_COMPARE_OP_LESS_OR_EQUAL)
            .set_color_attachment_format(draw_image.format())
            .set_depth_format(depth_image.format())
            .build(logical_device);

        MeshList meshes;
        CameraData camera_data = CameraData(
            ShaderStruct::Vec3 {
                .x = 0,
                .y = 0,
                .z = 5
            },
            PKEngine::Math::deg_to_rad(70.0f),
            (float) draw_image.extent().width / (float) draw_image.extent().height,
            10000.0f,
            0.1f
        );

        std::size_t current_frame = 0;
        std::array<FrameData, Config::render_config.max_frames_in_flight> frames =
            make_array<FrameData, Config::render_config.max_frames_in_flight>(
                swap_chain,
                graphics_queue,
                render_scale,
                draw_image,
                depth_image,
                graphics_pipeline,
                logical_device,
                queue_family_indices,
                meshes,
                camera_data
            );

        [[nodiscard]] FrameData & next_frame() noexcept {
            FrameData & frame = frames[current_frame];

            current_frame = (current_frame + 1) % frames.size();

            return frame;
        }

        inline void update() {
            FrameData & frame = next_frame();

            try {
                frame.draw(draw_image_descriptor_set);
            }
            catch (const Wrapper::SwapChain::Exceptions::OutOfDateError & ex) {
                logical_device.wait_idle();

                swap_chain = SwapChain(
                    window,
                    physical_device,
                    logical_device,
                    surface,
                    queue_family_indices
                );
                draw_image = Alloc::AllocatedImage(
                    logical_device,
                    allocator,
                    VK_FORMAT_R16G16B16A16_SFLOAT,
                    VkExtent3D {
                        .width = (uint32_t) window.width(),
                        .height = (uint32_t) window.height(),
                        .depth = 1,
                    },
                    (
                        VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                        VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                        VK_IMAGE_USAGE_STORAGE_BIT |
                        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
                    ),
                    VMA_MEMORY_USAGE_GPU_ONLY,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                    VK_IMAGE_ASPECT_COLOR_BIT
                );
                depth_image = Alloc::AllocatedImage(
                    logical_device,
                    allocator,
                    VK_FORMAT_D32_SFLOAT,
                    draw_image.extent(),
                    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                    VMA_MEMORY_USAGE_GPU_ONLY,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                    VK_IMAGE_ASPECT_DEPTH_BIT
                );

                // TODO: recreate frame semaphores
            }
        }

        static constexpr float z1 = -1.0f;
        static constexpr float z2 = -0.f;

        std::vector<uint32_t> indexes1 = { 0, 1, 2, 1, 2, 3 };
        std::vector<ShaderStruct::Vertex> vertices1 = {
            ShaderStruct::Vertex {
                .position = {
                    .x = -0.5f,
                    .y = -0.5f,
                    .z =  z1,
                },
                .color = {
                    .x = 1,
                    .y = 0,
                    .z = 0,
                    .w = 1,
                }
            },
            ShaderStruct::Vertex {
                .position = {
                    .x =  0.5f,
                    .y = -0.5f,
                    .z =  z1,
                },
                .color = {
                    .x = 0,
                    .y = 1,
                    .z = 0,
                    .w = 1,
                }
            },
            ShaderStruct::Vertex {
                .position = {
                    .x = -0.5f,
                    .y = 0.5f,
                    .z = z1,
                },
                .color = {
                    .x = 0,
                    .y = 0,
                    .z = 1,
                    .w = 1,
                }
            },
            ShaderStruct::Vertex {
                .position = {
                    .x =  0.5f,
                    .y =  0.5f,
                    .z =  z1,
                },
                .color = {
                    .x = 1,
                    .y = 0,
                    .z = 0,
                    .w = 1,
                }
            },
        };
        std::vector<Mesh::SubmeshRange> submesh_ranges1 = {
            Mesh::SubmeshRange { 0, (uint32_t) indexes1.size(), },
        };

        std::vector<uint32_t> indexes2 = { 0, 1, 2 };
        std::vector<ShaderStruct::Vertex> vertices2 = {
            ShaderStruct::Vertex {
                .position = {
                    .x = -1.0f,
                    .y = -1.0f,
                    .z =  z2,
                },
                .color = {
                    .x = 1,
                    .y = 0,
                    .z = 0,
                    .w = 0.2,
                }
            },
            ShaderStruct::Vertex {
                .position = {
                    .x =  1.0f,
                    .y = -1.0f,
                    .z =  z2,
                },
                .color = {
                    .x = 0,
                    .y = 1,
                    .z = 0,
                    .w = 0.2,
                }
            },
            ShaderStruct::Vertex {
                .position = {
                    .x = 0.0f,
                    .y = 1.0f,
                    .z = z2,
                },
                .color = {
                    .x = 0,
                    .y = 0,
                    .z = 1,
                    .w = 0.2,
                }
            }
        };
        std::vector<Mesh::SubmeshRange> submesh_ranges2 = {
            Mesh::SubmeshRange { 0, (uint32_t) indexes2.size(), },
        };
        MeshList::Eraser mesh1 = meshes.emplace_front(
            logical_device,
            graphics_queue,
            imm_command_buffer,
            imm_fence,
            allocator,
            indexes1,
            vertices1,
            submesh_ranges1
        );

        MeshList::Eraser mesh2 = meshes.emplace_front(
            logical_device,
            graphics_queue,
            imm_command_buffer,
            imm_fence,
            allocator,
            indexes2,
            vertices2,
            submesh_ranges2
        );

    public:
        inline Context() {
//            VkDescriptorImageInfo image_info {
//                .imageView = draw_image.view(),
//                .imageLayout = VK_IMAGE_LAYOUT_GENERAL,
//            };
//
//            VkWriteDescriptorSet draw_image_write = {
//                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
//                .pNext = nullptr,
//
//                .dstSet = draw_image_descriptor_set.handle(),
//                .dstBinding = 0,
//                .descriptorCount = 1,
//                .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
//                .pImageInfo = &image_info,
//            };
//
//            vkUpdateDescriptorSets(logical_device.handle(), 1, &draw_image_write, 0, nullptr);
        }

        inline ~Context() {
            for (FrameData & frame : frames) {
                frame.await_complete();
            }
        }

        inline Context(const Context &) = delete;
        inline Context(Context &&) = delete;

        inline void exit() noexcept {
            window.close();
        }

        [[nodiscard]] inline bool should_exit() const noexcept {
            return window.should_close();
        }

        inline void resize_window(int width, int height) {
            resize_info.target_width = width;
            resize_info.target_height = height;
            resize_info.ready = true;
        }
    };
}
