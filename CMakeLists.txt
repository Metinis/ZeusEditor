cmake_minimum_required(VERSION 3.16)
project(ZeusEditor LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ZeusEditor)
if(MSVC)
    set_property(TARGET ZeusEditor PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    )
endif()
file(GLOB_RECURSE ZEUSEDITOR_SOURCES
        src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/tinyfiledialogs/tinyfiledialogs.c
)
target_sources(ZeusEditor PRIVATE ${ZEUSEDITOR_SOURCES})


target_link_libraries(ZeusEditor PRIVATE
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

if (WIN32)
    add_custom_command(TARGET ZeusEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:ZeusEditor>
            $<TARGET_FILE_DIR:ZeusEditor>
        COMMAND_EXPAND_LISTS
    )
endif()


