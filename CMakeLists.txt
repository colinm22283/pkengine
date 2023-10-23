cmake_minimum_required(VERSION 3.7)
project(pkengine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

add_compile_options(-Wall -ffast-math)

add_subdirectory(external)
add_subdirectory(projects)