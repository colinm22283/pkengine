add_library(
    pkengine
    INTERFACE
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB_RECURSE PKENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/pkengine/*.cpp")

target_sources(pkengine INTERFACE ${PKENGINE_SOURCES})
target_include_directories(pkengine INTERFACE header)
target_link_libraries(pkengine INTERFACE ext-vulkan glfw mipp libbmp libpng Threads::Threads)