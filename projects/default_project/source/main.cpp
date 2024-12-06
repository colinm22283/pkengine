#include <vector>

#include <pkengine/engine.hpp>

void init(PKEngine::Engine & engine, PKEngine::Context & context) {
    using namespace PKEngine;

    engine.add_context([] (PKEngine::Context & ctx) {

    });
}