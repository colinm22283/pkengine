#include <pkengine/engine.hpp>

void PKEngine::engine_instance::recreate_swap_chain() {
    vkDeviceWaitIdle(logical_device.handle());

    frame_buffers.free();
    image_views.free();
    swap_chain.free();

    swap_chain.init();
    image_views.init();
    frame_buffers.init();
}