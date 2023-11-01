option(ENABLE_ASAN "Enable address sanitization" OFF)

function(new_project NAME SOURCES)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/${NAME})

    file(GLOB_RECURSE PKENGINE_SOURCES ${PROJECT_SOURCE_DIR}/lib/source/*.cpp)

    set(OpenGL_GL_PREFERENCE GLVND)

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    add_executable(${NAME} ${SOURCES} ${PKENGINE_SOURCES})
    target_include_directories(${NAME} PUBLIC config ${PROJECT_SOURCE_DIR}/lib/header header)
    target_link_libraries(${NAME} PUBLIC ext-vulkan glfw Threads::Threads)

    if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
        message(STATUS "Building for Debug")
        if (${ENABLE_ASAN})
            target_compile_options(${NAME} PUBLIC -fsanitize=address)
            target_link_libraries(${NAME} PUBLIC asan)
        endif ()
        target_compile_definitions(${NAME} PUBLIC PKENGINE_LOGGER_ENABLE PKENGINE_LOG_FILE_ENABLE)
    else ()
        message(STATUS "Building for Release")
        target_compile_options(${NAME} PUBLIC -O3 -flto)
        target_compile_definitions(${NAME} PUBLIC PKENGINE_LOG_FILE_ENABLE)
    endif ()

    macro(add_shader)
        set(SOURCES ${ARGV})

        foreach (SOURCE ${SOURCES})
            message(STATUS "Adding target for ${SOURCE}")

            add_custom_target(
                "shader.${SOURCE}"
                COMMAND glslc ${SOURCE} -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${SOURCE}.spv
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/projects/${NAME}/shaders"
            )
            add_dependencies(${NAME} "shader.${SOURCE}")
            file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)

            list(APPEND "${NAME}_SHADER_OUTPUT" "shader/${SOURCE}")
        endforeach ()
    endmacro()

    set("${NAME}_SHADER_OUTPUT" "")
    include(shaders/CMakeLists.txt)


endfunction()

file(GLOB PROJECTS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*")
foreach (PROJECT ${PROJECTS})
    message(STATUS ${PROJECT})
    if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT}")
        add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT}")
    endif ()
endforeach ()