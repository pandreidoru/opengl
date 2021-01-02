cmake_minimum_required(VERSION 3.16)
project(first)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)

add_executable(first
    camera.cpp camera.h
    common_values.h
    directional_light.cpp directional_light.h
    gl_window.cpp gl_window.h
    light.cpp light.h
    main.cpp
    material.cpp material.h
    mesh.cpp mesh.h
    model.cpp model.h
    point_light.cpp point_light.h
    shader.cpp shader.h
    spot_light.cpp spot_light.h
    texture.cpp texture.h
    )

find_package(ASSIMP REQUIRED)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    "/usr/include/stb"
    ${ASSIMP_INCLUDE_DIR})

target_link_libraries(first
    ${ASSIMP_LIBRARIES}
    GLEW
    glfw
    OpenGL
    )