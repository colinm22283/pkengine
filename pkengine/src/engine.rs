use std::sync::Arc;

use vulkano::VulkanLibrary;
use vulkano::instance::{Instance, InstanceCreateInfo, InstanceCreateFlags};
use vulkano::device::physical::PhysicalDevice;
use vulkano::device::{Queue, QueueFlags, Device, DeviceCreateInfo, QueueCreateInfo};

pub struct Engine {
    vk_lib: Arc<VulkanLibrary>,
    vk_inst: Arc<Instance>,
    phys_dev: Arc<PhysicalDevice>,
    dev: Arc<Device>,
    queue: Arc<Queue>,
}

impl Engine {
    pub fn new() -> Self {
        let lib = VulkanLibrary::new().expect("no local Vulkan library");

        let inst = Instance::new(
                lib.clone(),
                InstanceCreateInfo {
                    flags: InstanceCreateFlags::ENUMERATE_PORTABILITY,
                    ..Default::default()
                }
            )
            .expect("failed to create instance");

        let phys_dev = inst
            .enumerate_physical_devices()
            .expect("could not enumerate physical devices")
            .next()
            .expect("no devices available");

        for family in phys_dev.queue_family_properties() {
            println!("Found a queue family with {:?} queue(s)", family.queue_count);
        }

        let queue_family_index = phys_dev
            .queue_family_properties()
            .iter()
            .position(|queue_family_properties| {
                queue_family_properties.queue_flags.contains(QueueFlags::GRAPHICS)
            })
            .expect("couldn't find a graphical queue family") as u32;

        let (device, mut queues) = Device::new(
                phys_dev.clone(),
                DeviceCreateInfo {
                    // here we pass the desired queue family to use by index
                    queue_create_infos: vec![QueueCreateInfo {
                        queue_family_index,
                        ..Default::default()
                    }],
                    ..Default::default()
                },
            )
            .expect("failed to create device");

        let queue = queues.next().unwrap();

        Self {
            vk_lib: lib.clone(),
            vk_inst: inst.clone(),
            phys_dev: phys_dev.clone(),
            dev: device.clone(),
            queue: queue.clone(),
        }
    }
}

