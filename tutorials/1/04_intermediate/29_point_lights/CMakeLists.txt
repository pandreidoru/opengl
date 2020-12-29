cmake_minimum_required(VERSION 3.16)
project(first)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)

add_executable(first
    main.cpp
    gl_window.cpp gl_window.h
    mesh.cpp mesh.h
    shader.cpp shader.h
    camera.cpp camera.h texture.cpp texture.h light.cpp light.h material.cpp material.h directional_light.cpp directional_light.h point_light.cpp point_light.h common_values.h)

set(CMAKE_PREFIX_PATH "/usr/lib64")

target_include_directories(${PROJECT_NAME}
    PUBLIC
    "/usr/include/stb")

target_link_libraries(first
    GLEW
    glfw
    OpenGL
    )