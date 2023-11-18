include(FetchContent)

set(ENABLE_BZIP2 OFF)
set(ENABLE_LZMA OFF)
set(ENABLE_ZSTD OFF)

set(BUILD_TOOLS OFF)
set(BUILD_REGRESS OFF)
set(BUILD_OSSFUZZ OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_DOC OFF)

FetchContent_Declare(
        libzip_content
        GIT_REPOSITORY https://github.com/nih-at/libzip.git
        GIT_TAG        v1.10.1
)
FetchContent_MakeAvailable(libzip_content)