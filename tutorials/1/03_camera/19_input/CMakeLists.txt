cmake_minimum_required(VERSION 3.16)
project(first)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)

add_executable(first
    main.cpp
    gl_window.cpp gl_window.h
    mesh.cpp mesh.h
    shader.cpp shader.h
    )

set(CMAKE_PREFIX_PATH "/usr/lib64")

target_link_libraries(first
    GLEW
    glfw
    OpenGL
    )