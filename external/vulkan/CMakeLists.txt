find_package(Vulkan REQUIRED)
add_library(ext-vulkan INTERFACE)
target_include_directories(ext-vulkan INTERFACE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(ext-vulkan INTERFACE ${Vulkan_LIBRARIES})