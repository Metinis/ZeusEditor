cmake_minimum_required(VERSION 3.28)
project(ZeusEditor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(imgui STATIC)

target_include_directories(imgui PUBLIC
        include/imgui
        include/imgui/backends
)

target_link_libraries(imgui PUBLIC
        ZeusEngineCore
)

target_sources(imgui PRIVATE
        include/imgui/imgui.cpp
        include/imgui/imgui_draw.cpp
        include/imgui/imgui_tables.cpp
        include/imgui/imgui_widgets.cpp
        include/imgui/imgui_demo.cpp
        include/imgui/backends/imgui_impl_glfw.cpp
        include/imgui/backends/imgui_impl_opengl3.cpp
)
add_executable(ZeusEditor
        src/imgui/panels/MenuBarPanel.h)

file(GLOB_RECURSE ZEUSEDITOR_SOURCES
        src/*.cpp
)
target_sources(ZeusEditor PRIVATE ${ZEUSEDITOR_SOURCES})

target_link_libraries(ZeusEditor PRIVATE
        imgui
        ZeusEngineCore
)

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(ZeusEditor PRIVATE
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
    )
endif()

add_custom_command(TARGET ZeusEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:ZeusEditor>/resources"
        COMMENT "Copying resources to build directory"
)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/src/config.h"
    @ONLY
)

target_include_directories(ZeusEditor PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

target_compile_definitions(ZeusEditor PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

