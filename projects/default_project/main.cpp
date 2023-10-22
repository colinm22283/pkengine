#include <pkengine/logger/logger.hpp>

constexpr PKEngine::Logger<PKEngine::Util::ANSI::BlueFg, "MAIN"> logger;

void start() {
    logger << "Hello from main!";
}

void update() {

}