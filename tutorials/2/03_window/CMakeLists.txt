cmake_minimum_required(VERSION 3.16)
project(window)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)

add_executable(first main.cpp)

set(CMAKE_PREFIX_PATH "/usr/lib64")

target_link_libraries(first
    GLEW
    glfw
    OpenGL
    )