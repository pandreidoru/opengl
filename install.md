# Install

- [Install](#install)
  - [Docker](#docker)
    - [nvidia/docker](#nvidiadocker)
    - [Other](#other)
  - [GLEW](#glew)
  - [GLFW](#glfw)
  - [Test Installation](#test-installation)
  - [Others](#others)

- **Update graphic card's driver**

## Docker

- Check [Docker](docker/nvidia_opengl/Dockerfile)

### [nvidia/docker](https://hub.docker.com/r/nvidia/opengl)

### Other

- [OpenGL and CUDA Applications in Docker](https://medium.com/@benjamin.botto/opengl-and-cuda-applications-in-docker-af0eece000f1)

## [GLEW](http://glew.sourceforge.net/)

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

## [GLFW](https://www.glfw.org/download.html)

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

## Test Installation

```Bash
apt-get update \
  && apt-get install -y -qq glmark2 \
  && glmark2
```

## Others

- [Modern OpenGL 3.0+ [SETUP] GLFW and GLEW on a Mac](https://www.youtube.com/watch?v=Tz0dq2krCW8&ab_channel=SonarSystems)