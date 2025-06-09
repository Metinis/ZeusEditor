cmake_minimum_required(VERSION 3.28)
project(ZeusEditor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- IMGUI ----
add_library(imgui STATIC)

find_package(Vulkan REQUIRED)
add_subdirectory(../ZeusEngineCore ZeusEngineCore_build)

target_include_directories(imgui PUBLIC
        include/imgui
        include/imgui/backends
)

target_link_libraries(imgui PUBLIC
        ZeusEngineCore
        glfw
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
        include/imgui/backends/imgui_impl_vulkan.cpp  # Optional
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
