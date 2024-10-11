#include <vector>

#include <pkengine/engine.hpp>

void init(PKEngine::Engine & engine, PKEngine::Context & context) {
    using namespace PKEngine;

    context.device_queue.submit_queue();
}