#include <pkengine/logger/logger.hpp>

#include <pkengine/engine.hpp>

#include <pkengine/user_init.hpp>

int main() {
    static constexpr auto logger = PKEngine::Logger<"Entry">();

    if (
        !__builtin_cpu_supports("sse") ||
        !__builtin_cpu_supports("sse2")
    ) {
        logger.error() << "SSE is not supported on this CPU!";
        return 3;
    }

    using namespace PKEngine;
    using namespace GLFW;
    using namespace Vulkan::Util;

    try {
        static Engine engine;

        engine.add_context([](Context & context) { ::init(engine, context); });

        engine.start();
    }
    catch (const GLFWException & ex) {
        logger.error() << "GLFW ERROR";
        auto indent = logger.error().indent();
        indent << "File: " << ex.file();
        indent << "Exception Message: " << ex.what();
        indent << "GLFW Message: " << ex.glfw_error();

        return 1;
    }
    catch (const VulkanException & ex) {
        logger.error() << "VULKAN ERROR";
        auto indent = logger.error().indent();
        indent << "File: " << ex.file();
        indent << "Exception Message: " << ex.what();
        indent << "Vulkan Result: " << ex.vulkan_error();

        return 2;
    }
    catch (const std::exception & ex) {
        logger.error() << "ERROR";
        auto indent = logger.error().indent();
        indent << "Exception Message: " << ex.what();

        return 2;
    }

    return 0;
}
