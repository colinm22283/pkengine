use pkengine::engine::Engine;
use pkengine::buffer::GPUBuffer;

fn main() {
    let engine = Engine::new();

    let data: [u32; 3] = [ 1, 2, 3 ];

    let buffer = engine.buffer_from_iter(data.iter());
}
