cmake_minimum_required(VERSION 3.16)
project(triangle)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_PREFIX_PATH "/usr/lib64")

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    shader_program.cpp shader_program.h
    texture_2d.cpp texture_2d.h camera.cpp camera.h)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    "/usr/include/stb")

target_link_libraries(${PROJECT_NAME}
    GLEW
    glfw
    OpenGL
    )