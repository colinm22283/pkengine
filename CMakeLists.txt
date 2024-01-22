cmake_minimum_required(VERSION 3.7)
project(pkengine)

cmake_policy(SET CMP0077 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(GLSLC /opt/shaderc/bin/glslc)

add_subdirectory(external)
add_subdirectory(lib)
add_subdirectory(projects)