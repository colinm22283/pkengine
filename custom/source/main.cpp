#include <iostream>

#include <engine.hpp>
#include <util/engine_exception.hpp>
#include <logger/logger.hpp>

int main() {
    Log::open_logfile("pkengine.log");

    Log::info() << "Starting PKEngine...";

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

        Log::info() << "\nEVERYTHING BE DONE\n";
    } catch (PKEngine::Exception & e) {
        Log::error() << "EXCEPTION: \"" << e.what() << "\"";
        if (e.is_glfw_error()) Log::error() << "GLFW: \"" << PKEngine::glfw_error_string << "\"\n";

        exit(e.code());
    }

    Log::info() << "Exiting PKEngine...";
}
