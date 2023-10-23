#pragma once

#include <iostream>

#include "pkengine/node/base_node.hpp"

class ClockController : public PKEngine::BaseNode {
public:
    int time;

    explicit inline ClockController(int a): time(a) { }


    inline void start() override {
        logger << "Clock initialized\n";
    }
    inline void update() override {
        logger << "Time: " << time++ << "\n";
    }
};