use std::sync::Arc;

use vulkano::VulkanLibrary;
use vulkano::instance::{Instance, InstanceCreateFlags, InstanceCreateInfo};
use vulkano::device::{Device, DeviceCreateInfo, QueueCreateInfo, QueueFlags};
use vulkano::memory::allocator::StandardMemoryAllocator;

fn main() {
    let library = VulkanLibrary::new().expect("local vulkan library");

    let instance = Instance::new(
        library,
        InstanceCreateInfo {
            flags: InstanceCreateFlags::ENUMERATE_PORTABILITY,
            ..Default::default()
        },
    )
    .expect("failed to create instance");

    let phys_dev = instance
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
        phys_dev,
        DeviceCreateInfo {
            queue_create_infos: vec![QueueCreateInfo {
                queue_family_index,
                ..Default::default()
            }],
            ..Default::default()
        },
    )
    .expect("failed to create device");

    let queue = queues.next().unwrap();

    let memory_alloc = Arc::new(StandardMemoryAllocator::new_default(device.clone()));


}
