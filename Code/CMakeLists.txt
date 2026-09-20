cmake_minimum_required(VERSION 3.20)

project(BinomialModel VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------
# GLFW
# --------------------------------------------------

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

# --------------------------------------------------
# Executable
# --------------------------------------------------

add_executable(BinomialModel
    src/main.cpp
    src/CRR.cpp
    src/WindowCRR.cpp

    # Dear ImGui
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp

    # Dear ImGui backends
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

# --------------------------------------------------
# Include directories
# --------------------------------------------------

target_include_directories(BinomialModel PRIVATE
    src
    external/imgui
    external/imgui/backends
)

# --------------------------------------------------
# Libraries
# --------------------------------------------------

target_link_libraries(BinomialModel PRIVATE
    glfw
    opengl32
)