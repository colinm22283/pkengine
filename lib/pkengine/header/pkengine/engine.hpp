#pragma once

#include <forward_list>
#include <functional>

#include <pkengine/logger/logger.hpp>

#include <pkengine/glfw/glfw_instance.hpp>

#include <pkengine/context.hpp>

namespace PKEngine {
    class Engine {
    protected:
        static constexpr auto logger = PKEngine::Logger<"Engine">();

        GLFW::GLFWInstance glfw_instance;

        std::forward_list<Context> contexts;

    public:
        inline void add_context(const std::function<void(Context &)> && init_funct) {
            Context & ctx = contexts.emplace_front();
            init_funct(ctx);
        }

        void start();
    };
}