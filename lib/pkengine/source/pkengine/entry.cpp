#include <ctime>
#include <sstream>
#include <iomanip>

#include <pkengine/logger/logger.hpp>

#include <pkengine/engine.hpp>

#include <pkengine/user_init.hpp>

static constexpr auto logger = PKEngine::Logger<"Entry">();

inline void dump_log() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "logs/%m-%d-%Y--%H-%M-%S.log");

    PKEngine::dump_log_to(oss.str().c_str());

    logger.error() << "";
    logger.indent().error() << "Dumped debug log to '" << oss.view() << "'";
}

int main() {
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
        logger.debug() << "Starting PKEngine";

        static Engine engine;

        logger.debug() << "Initializing initial context";

        engine.add_context([](Context & context) { ::init(engine, context); });

        logger.debug() << "Starting engine run loop";

        engine.start();
    }
    catch (const GLFWException & ex) {
        logger.error() << "GLFW ERROR";
        auto indent = logger.error().indent();
        indent << "File: " << ex.file();
        indent << "Exception Message: " << ex.what();
        indent << "GLFW Message: " << ex.glfw_error();

        dump_log();

        return 1;
    }
    catch (const VulkanException & ex) {
        logger.error() << "VULKAN ERROR";
        auto indent = logger.error().indent();
        indent << "File: " << ex.file();
        indent << "Exception Message: " << ex.what();
        indent << "Vulkan Result: " << ex.vulkan_error();

        dump_log();

        return 2;
    }
    catch (const std::exception & ex) {
        logger.error() << "ERROR";
        auto indent = logger.error().indent();
        indent << "Exception Message: " << ex.what();

        dump_log();

        return 2;
    }

    return 0;
}
