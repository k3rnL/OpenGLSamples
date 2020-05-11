//
// Created by Erwan on 05-May-20.
//

#include <cmath>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <Program.h>
#include <iostream>
#include <FrameCounter.h>

#include <Window.h>
#include <CenteredCamera.h>

/** data to load on the GPU, it represent a square using homogeneous coordinate **/
GLfloat vertices[] = {
        -1.f, 0.0f, -1.f, 1,
        1.f, 0.0f, -1.f, 1,
        1.f, 0.0f, 1.f, 1,
        -1.f, 0.0f, 1.f, 1
};

static void error_callback(int error, const char *description) {
    fputs(description, stderr);
}

int main(int ac, char **av) {
    /** Window and OpenGL context creation **/
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    Window w(500, 500, "minimalOpenGL");
    w.makeContextCurrent();

    /** Load OpenGL function pointers using glew **/
    glewExperimental = GL_TRUE;
    glewInit();

    /** Load shaders and create drawing program **/
    Shader geometryShader("shaders/geometry.glsl", Shader::Geometry);
    Shader vertexShader("shaders/vertex.glsl", Shader::Vertex);
    Shader fragmentShader("shaders/fragment.glsl", Shader::Fragment);
    Program program;

    program.addShader(geometryShader);
    program.addShader(vertexShader);
    program.addShader(fragmentShader);
    program.link();

    /** Create buffer object and load our data **/
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /** Setting up the OpenGL program input parameters **/
    glVertexAttribPointer(glGetAttribLocation(program.getProgramID(), "point"), 4, GL_FLOAT, GL_FALSE,
                          4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    /** Get all the uniform locations **/
    GLuint u_View = glGetUniformLocation(program.getProgramID(), "View");
    GLuint u_Projection = glGetUniformLocation(program.getProgramID(), "Projection");
    GLuint u_Model = glGetUniformLocation(program.getProgramID(), "Model");
    GLuint u_sub_divisions = glGetUniformLocation(program.getProgramID(), "sub_divisions");
    GLuint u_point_on_plane = glGetUniformLocation(program.getProgramID(), "point_on_plane");

    /** Create the camera **/
    const glm::vec2 &winSize = w.getSize();
    CenteredCamera camera;
    camera.move({0.f, 3.f, 10.f});
    camera.projection = glm::perspective(glm::radians(45.0f), (float) winSize.x / (float) winSize.y, 0.1f, 100.0f);
    glm::mat4 Model = glm::scale(glm::mat4(1), glm::vec3(10));
    /** variables **/
    float mouseX = winSize.x / 2, mouseY = winSize.y / 2;
    int subdivisions = 4;
    glm::vec3 point_on_plane(0, 0, 0);
    bool display_triangles = true;

    /** setup callbacks for moue and keyboard **/
    w.setCursorPosCallback([&](double x, double y) {
        mouseX = (float) x;
        mouseY = (float) y;
    });

    w.setKeyboardCallback([&](int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
            switch (key) {
                case GLFW_KEY_UP:
                    camera.move({0, 0, -0.1});
                    break;
                case GLFW_KEY_DOWN:
                    camera.move({0, 0, 0.1});
                    break;
                case GLFW_KEY_LEFT:
                    camera.move({-0.1, 0, 0});
                    break;
                case GLFW_KEY_RIGHT:
                    camera.move({0.1, 0, 0});
                    break;
                case GLFW_KEY_LEFT_SHIFT:
                    camera.move({0, 0.1, 0});
                    break;
                case GLFW_KEY_LEFT_CONTROL:
                    camera.move({0, -0.1, 0});
                    break;
                case GLFW_KEY_A:
                    subdivisions -= 1;
                    break;
                case GLFW_KEY_Z:
                    subdivisions += 1;
                    break;
                case GLFW_KEY_S:
                    display_triangles = !display_triangles;
                    glPolygonMode(GL_FRONT_AND_BACK, display_triangles ? GL_FILL : GL_LINE);
                    break;
            }
    });


    /** Main loop **/
    FrameCounter frameCounter;
    while (w.isOpen()) {
        frameCounter.beginFrame();
        glfwPollEvents();

        glUseProgram(program.getProgramID());

        /** read depth buffer and get the point under the cursor **/
        float depth;
        glReadPixels(mouseX, winSize.y - mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        if (depth < 1.0)
            point_on_plane = glm::unProject(glm::vec3(mouseX, winSize.y - mouseY, depth), camera.view,
                                            camera.projection,
                                            glm::vec4(0, 0, winSize));

        /** update uniforms values **/
        glUniformMatrix4fv(u_View, 1, GL_FALSE, &camera.view[0][0]);
        glUniformMatrix4fv(u_Projection, 1, GL_FALSE, &camera.projection[0][0]);
        glUniformMatrix4fv(u_Model, 1, GL_FALSE, &Model[0][0]);
        glUniform1i(u_sub_divisions, subdivisions);
        glUniform3fv(u_point_on_plane, 1, &point_on_plane[0]);

        /** draw call **/
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        w.swapBuffers();

        /** update the title with fps value **/
        frameCounter.endFrame();
        const std::string title =
                std::to_string(frameCounter.fps()) + " / " + std::to_string(frameCounter.renderTime());
        w.setTitle(title);
    }

    return 0;
}