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
#include <iostream>
#include <FrameCounter.h>
#include <mutex>

#include <Window.h>
#include <CenteredCamera.h>
#include <Program.h>
#include <Texture.h>
#include <CameraFPS.h>

/** data to load on the GPU, it represent a square using homogeneous coordinate **/
GLfloat vertices[] = {
        -1.f, 0.0f, -1.f, 1,
        1.f, 0.0f, -1.f, 1,
        1.f, 0.0f, 1.f, 1,
        -1.f, 0.0f, 1.f, 1
};

void err(const std::string &m) {
    GLenum e;
    if ((e = glGetError()) != GL_FALSE)
        std::cerr << m << " : " << e << std::endl;
}

static void error_callback(int error, const char *description) {
    fputs(description, stderr);
}

#include "utils.h"

int main(int ac, char **av) {

//    initRenderDoc();
//getchar();
    /** Window and OpenGL context creation **/
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwSetErrorCallback(error_callback);

    Window w(500, 500, "minimalOpenGL");
    FrameCounter frameCounter;
    w.makeContextCurrent();

    /** Load OpenGL function pointers using glew **/
    glewExperimental = GL_TRUE;
    glewInit();

    std::mutex renderMutex;
err("begin");
    std::shared_ptr<Program> program = std::make_shared<Program>();
    std::shared_ptr<Shader> geometryShader;
    std::shared_ptr<Shader> vertexShader;
    std::shared_ptr<Shader> fragmentShader;

    /** Load shaders and create drawing program **/
    try {
//        geometryShader = std::make_shared<Shader>("shaders/geometry.glsl", Shader::Geometry);
        vertexShader = std::make_shared<Shader>("shaders/vertex.glsl", Shader::Vertex);
        fragmentShader = std::make_shared<Shader>("shaders/fragment.glsl", Shader::Fragment);

//        program->addShader(*geometryShader);
        program->addShader(*vertexShader);
        program->addShader(*fragmentShader);
        err("bb link");
        program->link();
        err("aa link");

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
err("b buffer");

    /** Create buffer object and load our data **/
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
err("b attrib");
    /** Setting up the OpenGL program input parameters **/
    glVertexAttribPointer(glGetAttribLocation(program->getProgramID(), "point"), 4, GL_FLOAT, GL_FALSE,
                          4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    Texture texture;
    texture.load("assets/background.jpg");

    /** Create the camera **/
    const glm::vec2 &winSize = w.getSize();
    CameraFPS camera;
//    camera.move({0.f, 0.f, -5.f});
    camera.projection = glm::perspective(glm::radians(45.0f), (float) winSize.x / (float) winSize.y, 0.1f, 100.0f);
    glm::mat4 Model = glm::scale(glm::mat4(1), glm::vec3(10));
    /** variables **/
    float mouseX = winSize.x / 2, mouseY = winSize.y / 2;
    int subdivisions = 4;
    glm::vec3 point_on_plane(0, 0, 0);
    bool display_triangles = true;

    /** setup callbacks for moue and keyboard **/
    w.setCursorPosCallback([&](double x, double y) {
        if (glfwGetMouseButton(w.getWindow(), GLFW_MOUSE_BUTTON_LEFT)) {
            double deltaX = mouseX - x;
            double deltaY = mouseY - y;
            double yaw = 0.05 * deltaX * (1. / frameCounter.renderTime());
            double pitch = 0.05 * deltaY * (1. / frameCounter.renderTime());
            yaw = glm::clamp(yaw, -glm::pi<double>() * 2, glm::pi<double>() * 2);
            pitch = glm::clamp(pitch, -glm::pi<double>() * 2, glm::pi<double>() * 2);
            glm::mat4 rot = glm::rotate(glm::mat4(1), (float) yaw, glm::vec3(0, 1, 0));
            rot = glm::rotate(rot, (float) pitch, glm::vec3(1, 0, 0));
            camera.rotate(rot);
        }
        mouseX = (float) x;
        mouseY = (float) y;
    });

    w.setKeyboardCallback([&](int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
            switch (key) {
                case GLFW_KEY_UP:
                    camera.move(glm::vec3(0, 0, 0.1) * frameCounter.renderTime());
                    break;
                case GLFW_KEY_DOWN:
                    camera.move(glm::vec3(0, 0, -0.1) * frameCounter.renderTime());
                    break;
                case GLFW_KEY_LEFT:
                    camera.move(glm::vec3(-0.1, 0, 0) * frameCounter.renderTime());
                    break;
                case GLFW_KEY_RIGHT:
                    camera.move(glm::vec3(0.1, 0, 0) * frameCounter.renderTime());
                    break;
                case GLFW_KEY_LEFT_SHIFT:
                    camera.move(glm::vec3(0, 0.01, 0) * frameCounter.renderTime());
                    break;
                case GLFW_KEY_LEFT_CONTROL:
                    camera.move(glm::vec3(0, -0.01, 0) * frameCounter.renderTime());
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
                case GLFW_KEY_R:
                    try {
                        err("b load");

                        auto tmpVertexShader = std::make_shared<Shader>("shaders/vertex.glsl", Shader::Vertex);
//                        auto tmpGeometryShader = std::make_shared<Shader>("shaders/geometry.glsl", Shader::Geometry);
                        auto tmpFragmentShader = std::make_shared<Shader>("shaders/fragment.glsl", Shader::Fragment);
                        err("b compile");

                        auto tmpProgram = std::make_shared<Program>();
                        err("aaaaa compile");
                        tmpProgram->addShader(*tmpVertexShader);
                        err("aaaa compile");
//                        tmpProgram->addShader(*tmpGeometryShader);
                        tmpProgram->addShader(*tmpFragmentShader);
                        err("aaa compile");
                        tmpProgram->link();
                        err("aa compile");
                        program = tmpProgram;
                        vertexShader = tmpVertexShader;
//                        geometryShader = tmpGeometryShader;
                        fragmentShader = tmpFragmentShader;
                        err("a compile");


                    } catch (std::exception &e) {
                        std::cerr << "[ERROR] " << e.what() << std::endl;
                    }
                    break;
            }
    });


    /** Main loop **/
    std::chrono::time_point<std::chrono::system_clock>  begin = std::chrono::system_clock::now();
    while (w.isOpen()) {
        frameCounter.beginFrame();

        if(rdoc_api) rdoc_api->StartFrameCapture(NULL, NULL);
        glfwPollEvents();
        glUseProgram(program->getProgramID());

        auto elapsed_diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - begin);
        float elapsed = elapsed_diff.count() / 1000.f;

        /** update uniforms values **/
        glUniformMatrix4fv(program->uniform("Rotation"), 1, GL_FALSE, &camera.rotation[0][0]);
        glUniformMatrix4fv(program->uniform("View"), 1, GL_FALSE, &camera.view[0][0]);
        glUniformMatrix4fv(program->uniform("Projection"), 1, GL_FALSE, &camera.projection[0][0]);
        glUniformMatrix4fv(program->uniform("Model"), 1, GL_FALSE, &Model[0][0]);
        glUniform3fv(program->uniform("Resolution"), 1, &winSize[0]);
        glUniform1f(program->uniform("Time"), elapsed);

        /** draw call **/
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        w.swapBuffers();

//        if(rdoc_api) rdoc_api->EndFrameCapture(NULL, NULL);
//        renderMutex.unlock();

        /** update the title with fps value **/
        frameCounter.endFrame();
        const std::string title =
                std::to_string((int)frameCounter.fps()) + "fps | " + std::to_string(frameCounter.renderTime()) + "ms | time: " + std::to_string(elapsed);
        w.setTitle(title);
    }

    return 0;
}