cmake_minimum_required(VERSION 3.7)
project(pkengine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_compile_options(-O3)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

find_package(glfw3 3.3 REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/header/")
include_directories("${PROJECT_SOURCE_DIR}/custom/header/")

file(GLOB_RECURSE CUSTOM_SOURCES CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/custom/source/*.cpp")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/source/*.cpp")
add_executable(pkengine ${SOURCES} ${CUSTOM_SOURCES})

message(STATUS "Build mode: ${CMAKE_BUILD_TYPE}")

target_link_libraries(pkengine ${Vulkan_LIBRARIES} glfw Threads::Threads)