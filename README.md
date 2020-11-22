# OpenGL

- [OpenGL](#opengl)
  - [Documentation](#documentation)
    - [Tutorials](#tutorials)
    - [Websites](#websites)
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