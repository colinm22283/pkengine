#include <vector>
#include <thread>
#include <cmath>

#include <pkengine/engine.hpp>

#include <pkengine/logger/logger.hpp>


static constexpr auto logger = PKEngine::Logger<"User Main">();

void init(PKEngine::Engine & engine, PKEngine::Context & context) {
    using namespace PKEngine;

//    context.resize_window(800, 600);

    engine.add_context([](Context &) {

    });
}
