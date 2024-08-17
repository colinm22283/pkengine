#include <pkengine/engine.hpp>

void PKEngine::engine_instance::enter_main_loop() {
    glfwSetKeyCallback(glfw_window.handle(), [](GLFWwindow * window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            logger << "KEY DOWN: " << key;
        }
    });

    while (!glfwWindowShouldClose(glfw_window.handle())) {
        glfwPollEvents();

        Time::update();

        object_tree.update();

        draw_frame();
    }
}