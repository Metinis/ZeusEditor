cmake_minimum_required(VERSION 3.28)
project(ZeusEditor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Adding debug info to Release build")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g")
endif()


# ---- IMGUI ----
add_library(imgui STATIC)

target_include_directories(imgui PUBLIC
        include/imgui
        include/imgui/backends
)

target_compile_definitions(imgui PUBLIC VK_NO_PROTOTYPES)

target_link_libraries(imgui PUBLIC
        ZeusEngineCore
        Vulkan::Vulkan
)

target_sources(imgui PRIVATE
        include/imgui/imgui.cpp
        include/imgui/imgui_draw.cpp
        include/imgui/imgui_tables.cpp
        include/imgui/imgui_widgets.cpp
        include/imgui/imgui_demo.cpp
        include/imgui/backends/imgui_impl_glfw.cpp
        include/imgui/backends/imgui_impl_opengl3.cpp
        include/imgui/backends/imgui_impl_vulkan.cpp
)
# ---- MAIN EXECUTABLE ----
add_executable(ZeusEditor)

file(GLOB_RECURSE ZEUSEDITOR_SOURCES
        src/*.cpp
)

target_sources(ZeusEditor PRIVATE ${ZEUSEDITOR_SOURCES})

# ---- LINK LIBRARIES ----
find_package(Vulkan REQUIRED)

target_link_libraries(ZeusEditor PRIVATE
        imgui
        ZeusEngineCore
        Vulkan::Vulkan
)

# ---- macOS platform-specific ----
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

# ---- Post build ----
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

