#include <pkengine/engine.hpp>

#include <pkengine/logger/logger.hpp>

#include <pkengine/util/ansi_color.hpp>

int main() {
    static constexpr auto logger = PKEngine::Logger<"Entry">();

    if (
        !__builtin_cpu_supports("sse") ||
        !__builtin_cpu_supports("sse2")
    ) {
        logger.error() << "SSE is not supported on this architecture!";
        return 2;
    }

    if (!PKEngine::engine_instance::run()) return 1;
    else return 0;
}
