
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " - " "_" PROJECT_NAME ${PROJECT_NAME})
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME})

# get all source files
file(GLOB all_SRCS
        "include/*.h"
        "source/*.cpp"
        )

# add executable and addShader libraries
add_executable(${PROJECT_NAME} ${all_SRCS})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW3 3 REQUIRED)
find_package(GLM REQUIRED)


target_link_libraries(${PROJECT_NAME} common ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW3_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE
        include
        ${GLM_INCLUDE_DIRS}
        ${GLFW3_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS})