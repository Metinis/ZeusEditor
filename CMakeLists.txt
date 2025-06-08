cmake_minimum_required(VERSION 3.28)
project(ZeusEditor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# IMGUI setup (keep this part)
set(IMGUI_SRC
    include/imgui/imgui.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/imgui_demo.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})

target_include_directories(imgui PUBLIC include/imgui include/imgui/backends)

set(IMGUI_BACKEND_SRC
    include/imgui/backends/imgui_impl_glfw.cpp
    include/imgui/backends/imgui_impl_opengl3.cpp
    # include/imgui/backends/imgui_impl_vulkan.cpp  # Optional
)

target_sources(imgui PRIVATE ${IMGUI_BACKEND_SRC})

set(SOURCES
    src/main.cpp
    src/core/Application.cpp
    src/imgui/ImGUILayer.cpp
    src/imgui/ImGUILayerOpenGL.cpp
    src/imgui/ImGUILayerVulkan.cpp
)

add_executable(ZeusEditor ${SOURCES})

# Platform-specific dependencies (macOS)
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

find_package(Vulkan REQUIRED)

target_link_libraries(imgui PUBLIC ZeusEngineCore)

target_link_libraries(ZeusEditor PRIVATE
    imgui
    ZeusEngineCore
    Vulkan::Vulkan
)
add_custom_command(TARGET ZeusEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    "$<TARGET_FILE_DIR:ZeusEditor>/resources"
    COMMENT "Copying resources to build directory"
)