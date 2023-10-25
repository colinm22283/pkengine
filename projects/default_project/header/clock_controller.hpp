#pragma once

#include <iostream>

#include "pkengine/node/base_node.hpp"

class ClockController : public PKEngine::BaseNode {
public:
    int time;

    explicit inline ClockController(int a): time(a) { }

    inline void start() override { BaseNode::start();
        logger << "Clock initialized\n";
    }
    inline void update() override { BaseNode::update();
        logger << "Time: " << time++ << "\n";
    }
};