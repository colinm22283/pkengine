#include <pkengine/object_tree.hpp>

#include <clock_controller.hpp>

void init() {
    using namespace PKEngine;

    object_tree.add<ClockController>(10);
}