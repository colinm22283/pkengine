#include <iostream>

#include <engine.hpp>
#include <util/engine_exception.hpp>
#include <logger/logger.hpp>

#include <util/const_string.hpp>
#include <util/ansi.hpp>

#include <logger/logfile.hpp>

#include <event/event_group.hpp>

using namespace PKEngine;

constexpr PKEngine::FileLogger<
    Util::ANSI::BlueFg,
    "MAIN",
    "pkengine.log"
> logger;

int update_count = 0;

void on_start(auto & control) {
    logger.log<"Engine started!">();
}

void update(auto & control) {
    logger.log<"Iteration #">() << ++update_count;

    if (update_count == 50) {
        logger.log<"I'm gonna throw an exception!!! >:)">();
        throw std::runtime_error("This is an exception");
    }

    if (update_count == 100) {
        logger.log<"Removing update handler">();
        control.remove();
    }
}

void on_close(auto & control) {
    logger.log<"Engine closing!">();
}

int main() {
    logger.log<"Starting PKEngine...">();

    try {
        PKEngine::Engine engine({
            .application = {
                .name    = "Test Program",
                .version = 0.1,
            },
        });

        PKEngine::EngineInstance & instance = engine.create_instance({
            .window = {
                .width = 800,
                .height = 600,
                .title = "Test PKEngine Program",
            },
        });

        instance.start_group.add(on_start);
        instance.update_group.add(update);
        instance.close_group.add(on_close);

        instance.start();
    }
    catch (PKEngine::Exception & e) {
        logger.error().log<"EXCEPTION: \"">().log(e.what()).log<"\"">();
        if (e.is_glfw_error()) logger.error().log<"GLFW: \"">().log(PKEngine::glfw_error_string).log<"\"\n">();

        exit(e.code());
    }

    logger.log<"Exiting PKEngine...">();
}
