# OpenGL

- [OpenGL](#opengl)
  - [Documentation](#documentation)
    - [Tutorials](#tutorials)
    - [Websites](#websites)
    - [Books](#books)
  - [Basic steps for creating shaders](#basic-steps-for-creating-shaders)
  - [Vertex Buffer Object Layout](#vertex-buffer-object-layout)
  - [Coordinate Systems](#coordinate-systems)
    - [Local Space](#local-space)
    - [World Space](#world-space)
    - [View Space](#view-space)
    - [Clip Space (Normalized Device Coords)](#clip-space-normalized-device-coords)
    - [Screen Space](#screen-space)
  - [Glossary](#glossary)

## Documentation

### Tutorials

- [Udemy - Computer Graphics with Modern OpenGL and C++](https://www.udemy.com/course/graphics-with-modern-opengl/)
- [Udemy - Learn Modern OpenGL Programming](https://www.udemy.com/course/learn-modern-opengl-programming/)
- [Udemy - Learn GLSL Shaders from Scratch](https://www.udemy.com/course/learn-glsl-shaders-from-scratch/)

### Websites

- [docs.GL - API Reference](http://docs.gl/)
- [open.gl](https://open.gl/)
- [khronos - OpenGL](https://www.khronos.org/opengl/wiki/Main_Page)
- [learnopengl](https://learnopengl.com/Introduction)
- [ogldev](http://ogldev.atspace.co.uk/)
- [Anton's OpenGL 4 Tutorials](https://antongerdelan.net/opengl/)

### Books

- [Reference Cards](https://www.khronos.org/developers/reference-cards/)
- [Anton's OpenGL 4 Tutorials](https://www.amazon.com/gp/product/B00LAMQYF2/ref=as_li_tl?ie=UTF8&camp=1789&creative=9325&creativeASIN=B00LAMQYF2&linkCode=as2&tag=drantger-20&linkId=31e011e15c43803bca938150e5038782)
- [Computer Graphics Programming in OpenGL with C++](https://www.amazon.com/Computer-Graphics-Programming-OpenGL-C/dp/1683922212)
- [OpenGL Programming Guide: The Official Guide to Learning OpenGL, Version 4.5 with SPIR-V 9th Edition](https://www.amazon.com/OpenGL-Programming-Guide-Official-Learning/dp/0134495497/ref=pd_sbs_14_1/138-8200178-1350956?_encoding=UTF8&pd_rd_i=0134495497&pd_rd_r=5afa904d-add1-4fd8-85b0-34b3789e28b6&pd_rd_w=yF5uX&pd_rd_wg=Ov6Oy&pf_rd_p=ed1e2146-ecfe-435e-b3b5-d79fa072fd58&pf_rd_r=J6ZXEB1ARRQ0ZG1BZ8JW&psc=1&refRID=J6ZXEB1ARRQ0ZG1BZ8JW)
- [OpenGL 4 Shading Language Cookbook: Build high-quality, real-time 3D graphics with OpenGL 4.6, GLSL 4.6 and C++17, 3rd Edition](https://www.amazon.com/OpenGL-Shading-Language-Cookbook-high-quality/dp/1789342252/ref=sr_1_1?dchild=1&keywords=OpenGL+4.0+Shading+Language+Cookbook&qid=1607437662&s=books&sr=1-1)
- [OpenGL Superbible: Comprehensive Tutorial and Reference 7th Edition](https://www.amazon.com/OpenGL-Superbible-Comprehensive-Tutorial-Reference/dp/0672337479/ref=sr_1_1?dchild=1&keywords=OpenGL+Superbible&qid=1607435300&s=books&sr=1-1)
- [Real-Time Rendering, Fourth Edition 4th Edition](https://www.amazon.com/Real-Time-Rendering-Fourth-Tomas-Akenine-M%C3%B6ller/dp/1138627003/ref=pd_sbs_14_3/138-8200178-1350956?_encoding=UTF8&pd_rd_i=1138627003&pd_rd_r=5afa904d-add1-4fd8-85b0-34b3789e28b6&pd_rd_w=yF5uX&pd_rd_wg=Ov6Oy&pf_rd_p=ed1e2146-ecfe-435e-b3b5-d79fa072fd58&pf_rd_r=J6ZXEB1ARRQ0ZG1BZ8JW&psc=1&refRID=J6ZXEB1ARRQ0ZG1BZ8JW)

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

## Vertex Buffer Object Layout

- All layouts

![Vertex Buffer Object Layout](vertex_buffer_object_layouts.png)

- Position layout

![Vertex Buffer Object Layout](vertex_buffer_object_layout_position.png)

- Interleaved layout

![Vertex Buffer Object Layout](vertex_buffer_object_layout_interleaved.png)

## Coordinate Systems

### Local Space

### World Space

### View Space

### Clip Space (Normalized Device Coords)

### Screen Space

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
