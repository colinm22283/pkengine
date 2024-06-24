include(FetchContent)

FetchContent_Declare(
        libbmp_content
        GIT_REPOSITORY git@github.com:marc-q/libbmp.git
        GIT_TAG        66bec6d7daf254e6dc07d55c9383fd68276a6a39
)
FetchContent_Populate(libbmp_content)

add_library(libbmp ${libbmp_content_SOURCE_DIR}/libbmp.c)
target_include_directories(libbmp PUBLIC ${libbmp_content_SOURCE_DIR})