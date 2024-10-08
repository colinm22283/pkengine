#include <pkengine/glfw/glfw.hpp>

const char * PKEngine::GLFW::glfw_error_string = "";
void PKEngine::GLFW::error_callback(int, const char * str) { glfw_error_string = str; }