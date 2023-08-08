#include <pkengine/logger/logger.hpp>

#include <pkengine/pkengine.hpp>

using namespace PKEngine;

constexpr Logger<
    Util::ANSI::BlueFg,
    "MAIN"
> logger;

int main() {
    logger << "Start";

    engine_instance::run();

    logger << "Exit";
}
