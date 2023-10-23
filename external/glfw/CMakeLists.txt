include(FetchContent)

FetchContent_Declare(
        glfw_content
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw_content)