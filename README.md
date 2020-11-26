# OpenGL

- [OpenGL](#opengl)
  - [Documentation](#documentation)
    - [Tutorials](#tutorials)
    - [Websites](#websites)
  - [Basic steps for creating shaders](#basic-steps-for-creating-shaders)
  - [Install](#install)
    - [GLEW](#glew)
    - [GLFW](#glfw)
    - [Test Installation](#test-installation)
    - [Others](#others)
  - [Glossary](#glossary)

## Documentation

### Tutorials

- [Udemy - Computer Graphics with Modern OpenGL and C++](https://www.udemy.com/course/graphics-with-modern-opengl/)
- [Udemy - Learn Modern OpenGL Programming](https://www.udemy.com/course/learn-modern-opengl-programming/)
- [Udemy - Learn GLSL Shaders from Scratch](https://www.udemy.com/course/learn-glsl-shaders-from-scratch/)

### Websites

- [khronos - OpenGL](https://www.khronos.org/opengl/wiki/Main_Page)
- [open.gl](https://open.gl/)
- [learnopengl](https://learnopengl.com/Introduction)
- [ogldev](http://ogldev.atspace.co.uk/)

## Basic steps for creating shaders

1. Create Vertex Shader

   ```C++
   const GLchar* vertex_shader_src =
     "#version 330 core\n"
     "layout (location = 0) in vec3 pos;"
     "void main() {"
     "  gl_Position = vec4(pos, 1.0);"
     "}";

   GLuint vs = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vs, 1, &vertex_shader_src, nullptr);

   const GLchar* fragment_shader_src =
     "#version 330 core\n"
     "out vec4 frag_color;"
     "void main() {"
     "  frag_color = vec4(1, 0, 0, 1);"
     "}";
   GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fs, 1, &fragment_shader_src, nullptr);
   ```

2. Create Fragment Shader
3. Compile the shaders

   ```C++
   glCompileShader(vs);
   GLint result;
   GLchar info_log[512];
   glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
   if (!result) {
     glGetShaderInfoLog(vs, sizeof(info_log), nullptr, info_log);
     std::cerr << "Error: Vertex shader failed to compile: " << info_log << std::endl;
   }

   glCompileShader(fs);
   glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
   if (!result) {
     glGetShaderInfoLog(vs, sizeof(info_log), nullptr, info_log);
     std::cerr << "Error: Fragment shader failed to compile: " << info_log << std::endl;
   }
   ```

4. Create a Shader Program

   ```C++
   GLuint shader_program = glCreateProgram();
   ```

5. Attach the shaders to the Program

   ```C++
   glAttachShader(shader_program, vs);
   glAttachShader(shader_program, fs);
   ```

6. Link the compiled shaders into a single Program

   ```C++
   glLinkProgram(shader_program);
   glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
   if (!result) {
     glGetProgramInfoLog(shader_program, sizeof(info_log), nullptr, info_log);
     std::cerr << "Error: Shader Program linker failure: " << info_log << std::endl;
   }

   glDeleteShader(vs);
   glDeleteShader(fs);
   ```

7. Use the Program

   ```C++
   glUseProgram(shader_program);
   glDeleteProgram(shader_program);
   ```

## Install

- Update graphic card's driver

### [GLEW](http://glew.sourceforge.net/)

- Install dependencies

  ```Bash
  sudo apt-get install build-essential libxmu-dev libxi-dev libgl-dev libosmesa-dev
  ```

- Download the latest release (2.1.0)
- Install GLEW

  ```Bash
  wget https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.tgz/download
  tar xzvf glew-2.1.0.tgz
  cd glew-2.1.0
  # Check README.md
  sudo make install
  sudo mv /usr/lib64/* /usr/lib/x86_64-linux-gnu
  make clean
  ```

### [GLFW](https://www.glfw.org/download.html)

- Install dependencies

  ```Bash
  sudo apt install xorg-dev libglfw3
  ```

- Clone and install

  ```Bash
  git clone https://github.com/glfw/glfw.git
  cd glfw
  mkdir build
  cd build
  cmake -G "Unix Makefiles" -DBUILD_SHARED_LIBS=ON ..
  make -j
  sudo make install
  ```

- Include in new project

```CMake
find_package(OpenGL REQUIRED)
add_executable(project main.cpp)
target_link_libraries(project glfw OpenGL::GL)
```

```C++
#include <GL/glew.h>
#include <GLFW/glfw3.h>
```

- Other installation docs
  - [Install and use GLFW in a project](https://stackoverflow.com/a/62905904/3227895)

### Test Installation

```Bash
apt-get update \
  && apt-get install -y -qq glmark2 \
  && glmark2
```

### Others

- [Modern OpenGL 3.0+ [SETUP] GLFW and GLEW on a Mac](https://www.youtube.com/watch?v=Tz0dq2krCW8&ab_channel=SonarSystems)

## Glossary

- **GLEW**
  - *OpenGL Extension Wrangler*
  - Interface for OpenGL version above 1.1
  - Load OpenGL extensions
  - Some extensions are platform specific, GLEW can check if they exist on that platform
  - Alternatives: GL3W, glLoadGen, glad, glsdk, glbinding, libepoxy, Glee
  - How to use
    - `#include <GL/glew.h>`
    - After initialization of OpenGL context: `glewExperimental = GL_TRUE`
    - `glewInit()`
    - Should return `GLEW_OK`. If it fails, it returns the error
    - Can read error with `glewGetErrorString(result)`
    - Check extensions exist: `if (!GLEW_EXT_framebuffer_object)`
    - `wglew.h` for Windows only functions
- **GLFW**
  - *OpenGL FrameWork* (probably)
  - Handles window creation and control
  - Pick up and process input from the keyboard, mouse, joystick and gamepad
  - Multiple monitors support
  - Uses OpenGL context for windows
  - Alternatives
    - **SDL** - *Simple DirectMedia Layer*
      - Can do almost everything GLFW can do and more (audio, threading, filesystems, etc.)
    - **SFML** - *Simple and Fast Multimedia Library*
      - Like SDL, but with even more features
      - But the OpenGL context is very weak. Based on 2D only graphics
- **[glvnd](https://github.com/NVIDIA/libglvnd#architecture)**
  - *GL Vendor-Neutral Dispatch Library*
  - An abstraction layer that sends OpenGL calls to one of potentially many drivers
  - It allows multiple drivers from different vendors to coexist on the same filesystem, and determines which vendor to dispatch each API call to at runtime
  - [Architecture](https://github.com/NVIDIA/libglvnd#architecture)
