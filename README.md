# OpenGL

- [OpenGL](#opengl)
  - [Documentation](#documentation)
    - [Tutorials](#tutorials)
    - [Websites](#websites)
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
