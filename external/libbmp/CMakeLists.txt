include(FetchContent)

FetchContent_Declare(
        cbmp_content
        GIT_REPOSITORY git@github.com:mattflow/cbmp.git
        GIT_TAG        v1.0.1
)
FetchContent_GetProperties(cbmp_content)
if(NOT cbmp_content_POPULATED)
    FetchContent_Populate(cbmp_content)
endif()

add_library(cbmp INTERFACE)
target_include_directories(cbmp INTERFACE ${cbmp_content_SOURCE_DIR})
target_sources(cbmp INTERFACE ${cbmp_content_SOURCE_DIR}/cbmp.c)