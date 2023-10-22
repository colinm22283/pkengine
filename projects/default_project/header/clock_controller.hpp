#pragma once

#include <iostream>

#include "pkengine/node/base_node.hpp"

class ClockController : public PKEngine::BaseNode {
public:
    int time;

    explicit inline ClockController(int a): time(a) { }


    inline void start() override {
        std::cout << "Clock initialized\n";
    }
    inline void update() override {
        std::cout << "Time: " << time++ << "\n";
    }
};