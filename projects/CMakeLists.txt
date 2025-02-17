if (NOT WIN32)
    set(ENABLE_ASAN "Enable address sanitization" ON)
endif ()

function(new_project NAME SOURCES)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/${NAME})

    set(OpenGL_GL_PREFERENCE GLVND)

    add_executable(${NAME} ${SOURCES})
    target_include_directories(${NAME} PUBLIC config header)
    target_link_libraries(${NAME} PUBLIC pkengine)

    target_compile_options(${NAME} PUBLIC -Wall -ffast-math -rdynamic)
    target_link_options(${NAME} PUBLIC -rdynamic)

    if (WIN32)
        target_compile_options(${NAME} PUBLIC -static-libstdc++ -static-libgcc)
    endif ()

    if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
        message(STATUS "Building for Debug")
        target_compile_options(${NAME} PUBLIC -O0)
        if (ENABLE_ASAN)
            message(STATUS "Address sanitation enabled")
            target_compile_options(${NAME} PUBLIC -fsanitize=address)
            target_link_libraries(${NAME} PUBLIC asan)
        endif ()
        target_compile_definitions(${NAME} PUBLIC LOGGER_VISIBILITY_LEVEL=DEBUG ENABLE_VALIDATION_LAYERS)
    else ()
        message(STATUS "Building for Release")
        target_compile_options(${NAME} PUBLIC -O3)
        if (NOT WIN32)
            target_compile_options(${NAME} PUBLIC -flto)
        endif ()
        target_compile_definitions(${NAME} PUBLIC LOGGER_VISIBILITY_LEVEL=STATUS ENABLE_SIGSEGV_HANDLER)
    endif ()

    macro(add_shader)
        set(SOURCES ${ARGV})

        foreach (SOURCE ${SOURCES})
            message(STATUS "Adding target for ${SOURCE}")

            add_custom_command(
                OUTPUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${SOURCE}.spv"
                COMMAND ${GLSLC} ${SOURCE} -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${SOURCE}.spv
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/projects/${NAME}/shaders"
                VERBATIM
                DEPENDS "shaders/${SOURCE}"
            )
            target_sources(${NAME} PUBLIC "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${SOURCE}.spv")
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