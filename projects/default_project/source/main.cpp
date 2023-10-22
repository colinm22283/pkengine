#include <pkengine/object_tree.hpp>

#include <clock_controller.hpp>

void init() {
    using namespace PKEngine;

    auto & clock = object_tree.add<ClockController>(10);

    clock.add<ClockController>(20);
}