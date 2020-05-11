# OpenGLSamples

You can find here some of my experiments with OpenGL.

## Compilation

You can compile this project using cmake, the code is multi-platform.
```shell script
mkdir build ; cd build
cmake ..
make
```

On Windows you can specify which toolchain you want to use (Makefile, Visual Studio, MinGW), for example to create a visual sutdio project :
```shell script
cmake .. -G "Visual Studio 15 2017 x64"
```
You can get all available generators with **cmake --help**

### Dependencies

You have to install GLM, GLEW and GLFW3 on your system to be able to build the project.

#### Linux
On Ubuntu install this packages :
```shell script
sudo apt install libglm-dev libglfw3-dev libglew-dev mesa-common-dev
```

#### Windows
On Windows you can find already build libraries on their website:
- https://www.glfw.org/download.html
- http://glew.sourceforge.net/
- https://github.com/g-truc/glm/tags

CMake will not be able to find them by itself, you must execute CMake with those environment variables
```shell script
GLEW_DIR=C:/Users/..../glew
GLFW_ROOT=C:/Users/..../glfw3
GLM_ROOT_DIR=C:/Users/..../glm
```