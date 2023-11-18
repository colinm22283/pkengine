add_library(pkpkg INTERFACE)

file(GLOB_RECURSE PKPKG_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/pkengine/*.cpp")
target_sources(pkpkg INTERFACE ${PKPKG_SOURCES})

target_link_libraries(pkpkg INTERFACE zip)

target_include_directories(pkpkg INTERFACE header)