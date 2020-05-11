# get all source files
file(GLOB source
        "source/*.cpp"
        "include/*h"
        )

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW3 3 REQUIRED)
find_package(GLM REQUIRED)

include_directories(include ${GLFW3_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})

add_library(common STATIC ${source})
target_include_directories(common PUBLIC "include" ${GLM_INCLUDE_DIRS})
target_link_libraries(common ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW3_LIBRARIES})

