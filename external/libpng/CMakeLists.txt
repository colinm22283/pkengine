include(FetchContent)

FetchContent_Declare(
    libpng_content
    GIT_REPOSITORY https://github.com/pnggroup/libpng
    GIT_TAG        v1.6.43
)
FetchContent_MakeAvailable(libpng_content)

add_library(libpng INTERFACE)
target_link_libraries(libpng INTERFACE png_static)