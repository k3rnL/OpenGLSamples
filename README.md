# MinimalOpenGL

A minimal code to run OpenGL using GLFW3 and GLEW.
This program just show a blue shaded square, but do all the primary stuff :
- Load and compile shader
- Create a buffer and store our data inside
- Draw call in a simple loop

You can use this code to start your new project from scratch without the pain of writing the very first running code.
This code is fully functional, and should work on very old version of OpenGL, minimum accepted is 1.40.

## Compilation

You can compile this project using cmake, the code is multi-platform.
```shell script
mkdir build ; cd build
cmake ..
make
```

On Windows you can specify which toolchain you want to use later (Makefile, Visual Studio, MinGW), for example to create a visual sutdio project :
```shell script
cmake .. -G "Visual Studio 15 2017 x64"
```
You can get all available generators with *cmake --help*

### Dependencies

You have to install GLEW and GLFW3 on your system to be able to build the project.

#### Linux
On Ubuntu install this packages :
```shell script
sudo apt install libglfw3-dev libglew-dev mesa-common-dev
```

#### Windows
On Windows you can find already build libraries on their website:
- https://www.glfw.org/download.html
- http://glew.sourceforge.net/

CMake will not be able to find them by itself, you must execute CMake with this two environment variable
```shell script
GLEW_DIR=C:/Users/..../glew
GLFW_ROOT=C:/Users/..../glfw3
```