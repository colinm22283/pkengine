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

void int_handler_1(auto & control, int test) {
    logger.log<"Int handler 1: ">() << test;

    if (test == 20) throw Exceptions::UnableToCreatePipeline();
}
void int_handler_2(auto & control, int test) {
    logger.log<"Int handler 2: ">() << test;
}
void float_handler_1(auto & control, float test) {
    logger.log<"Float Handler 1: ">() << test;
}
void float_handler_2(auto & control, float test) {
    logger.log<"Float handler 2: ">() << test;
}

int main() {
    logger.log<"Starting PKEngine...">();

    EventGroup<"Int Handler", void(int)> int_group;
    EventGroup<"Float Handler", void(float)> float_group;

    int_group.add(int_handler_1);
    int_group.add(int_handler_2);
    float_group.add(float_handler_1);
    float_group.add(float_handler_2);

    for (int i = 0; i < 100; i++) {
        int_group(10);
        float_group(1.5);
        int_group(20);
        float_group(2.25);
        int_group(30);
        float_group(3.0);
    }

    return 0;

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

        logger.success().log<"EVERYTHING BE DONE!">();

//        while (!glfwWindowShouldClose(instance.window.handle()));
    } catch (PKEngine::Exception & e) {
        logger.error().log<"EXCEPTION: \"">().log(e.what()).log<"\"">();
        if (e.is_glfw_error()) logger.error().log<"GLFW: \"">().log(PKEngine::glfw_error_string).log<"\"\n">();

        exit(e.code());
    }

    logger.log<"Exiting PKEngine...">();
}
