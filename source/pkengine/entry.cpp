#include <pkengine/logger/logger.hpp>

#include <pkengine/pkengine.hpp>

using namespace PKEngine;

constexpr Logger<Util::ANSI::BlueFg, "Entry"> logger;

int main() {
    engine_instance::run();
}
