cmake_minimum_required(VERSION 3.20)

project(BinomialModel
    VERSION 0.1.0
    LANGUAGES CXX
)


# C++ configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Dependencies
include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)


# ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp

    # GLFW + OpenGL3 backends
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)


# Executable
add_executable(BinomialModel
    src/main.cpp
    src/CRR.cpp
    src/WindowCRR.cpp
    src/Greeks.cpp

    ${IMGUI_SOURCES}
)


# Include directories
target_include_directories(BinomialModel PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)


# Libraries
target_link_libraries(BinomialModel PRIVATE
    glfw
)

# OpenGL
if(WIN32)
    target_link_libraries(BinomialModel PRIVATE opengl32)
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(BinomialModel PRIVATE OpenGL::GL)
endif()