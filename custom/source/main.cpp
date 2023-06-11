#include <iostream>

#include <engine.hpp>
#include <util/engine_exception.hpp>
#include <logger/logger.hpp>

#include <util/const_string.hpp>
#include <util/ansi.hpp>

#include <logger/logfile.hpp>

using namespace PKEngine;

PKEngine::FileLogger<
    Util::ANSI::BlueFg,
    "MAIN",
    "pkengine.log"
> logger;

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

        logger.success().log<"EVERYTHING BE DONE!">();

//        while (!glfwWindowShouldClose(instance.window.handle()));
    } catch (PKEngine::Exception & e) {
        logger.error().log<"EXCEPTION: \"">().log(e.what()).log<"\"">();
        if (e.is_glfw_error()) logger.error().log<"GLFW: \"">().log(PKEngine::glfw_error_string).log<"\"\n">();

        exit(e.code());
    }

    logger.log<"Exiting PKEngine...">();
}
