# SimpleOpenGL 2D Robot Animation

A simple C/OpenGL project demonstrating 2D planar robot animation using GLFW and GLAD.

## Features

- Animates a 2-link planar robot arm.
- Uses **GLFW** for window/context creation and input handling.
- Uses **GLAD** for loading OpenGL functions.
- Demonstrates basic OpenGL rendering with lines and vertices.

## Getting Started

### Requirements

- Windows
- [MinGW-w64](https://www.mingw-w64.org/) or compatible C compiler
- [CMake](https://cmake.org/)
- GLFW (`glfw3.dll`) included in `/bin`

### Build

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
