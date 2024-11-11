include(FetchContent)

cmake_policy(SET CMP0077 NEW)

FetchContent_Declare(
    vma_content
    GIT_REPOSITORY git@github.com:GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
    GIT_TAG v3.1.0
)
FetchContent_MakeAvailable(vma_content)

add_library(vma INTERFACE)
target_sources(vma INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/vma.cpp)
target_link_libraries(vma INTERFACE VulkanMemoryAllocator)
