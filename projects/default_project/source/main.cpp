#include <vector>
#include <thread>

#include <pkengine/engine.hpp>

std::thread thread1;
std::thread thread2;

void init(PKEngine::Engine & engine, PKEngine::Context & context) {
    using namespace PKEngine;

//    thread1 = std::thread([&context]() {
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//
//        context.resize_window(1000, 1000);
//    });

    context.resize_window(1000, 1000);

    engine.add_context([](Context & ctx) {
        ctx.resize_window(1000, 1000);
    });
}