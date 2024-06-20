#include <pkengine/engine.hpp>

#include <pkengine/logger/logger.hpp>

#include <pkengine/util/ansi.hpp>

int main() {
    static constexpr PKEngine::Logger<PKEngine::Util::ANSI::BlueFg, "Entry"> logger;

    if (
        !__builtin_cpu_supports("sse") ||
        !__builtin_cpu_supports("sse2")
    ) {
        logger.error() << "SSE is not supported on this architecture!";
        return 1;
    }

    PKEngine::engine_instance::run();

    return 0;
}
