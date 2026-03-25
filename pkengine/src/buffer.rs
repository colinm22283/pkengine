use std::sync::Arc;

use vulkano::buffer::{Buffer, BufferCreateInfo, BufferUsage};
use vulkano::memory::allocator::{AllocationCreateInfo, MemoryTypeFilter, GenericMemoryAllocator, FreeListAllocator};

use crate::engine::EngineAllocator;

pub struct GPUBuffer<T> {
    vk_buffer: vulkano::buffer::Subbuffer<[T]>,
}

impl<T: std::marker::Sync + std::marker::Send + bytemuck::Pod> GPUBuffer<T> {
    pub (crate) fn from_data(allocator: EngineAllocator, data: &T) -> Self {
        let data_iter = std::iter::once(data.clone());

        GPUBuffer::<T>::from_iter(allocator, data_iter)
    }

    pub (crate) fn from_iter(allocator: EngineAllocator, iter: (impl Iterator<Item = T> + ExactSizeIterator + Clone)) -> Self {
        let buffer = Buffer::from_iter(
            allocator.clone(),
            BufferCreateInfo {
                usage: BufferUsage::UNIFORM_BUFFER,
                ..Default::default()
            },
            AllocationCreateInfo {
                memory_type_filter:
                    MemoryTypeFilter::PREFER_DEVICE |
                    MemoryTypeFilter::HOST_SEQUENTIAL_WRITE,
                ..Default::default()
            },
            iter.clone(),
        )
        .expect("failed to create buffer");

        Self {
            vk_buffer: buffer,
        }
    }
}

