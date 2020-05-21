//
// Created by Erwan on 05-May-20.
//

#define _USE_MATH_DEFINES

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
#include "utils.h"

glm::vec3 trefoil_at(float t) {
    return {sin(3 * t),
            sin(t) + 2 * sin(2 * t),
            cos(t) - 2 * cos(2 * t)};
}

glm::vec3 tangent_at(float t) {
    return {3 * std::cos(3 * t),
            std::cos(t) + 4 * std::cos(2 * t),
            4 * std::sin(2 * t) - std::sin(t)};
}

glm::vec3 normal_at(float t) {
    auto T = tangent_at(t);
    return glm::normalize(glm::vec3{0, -T.z, T.y});
}

void generateTube(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, int precision, const glm::vec3 &pos, float t) {
    glm::vec3 tangent = tangent_at(t);
    glm::vec3 T = glm::normalize(tangent);
    glm::vec3 N = glm::normalize(normal_at(t));
    glm::vec3 B = glm::normalize(glm::cross(T, N));

    glm::vec3 first_v;
    glm::vec3 first_n;

    float s = 0;
    for (int i = 0; i < precision; i++) {
        float cos = std::cos(s) / 2;
        float sin = std::sin(s) / 2;
        normals.emplace_back(cos * N + sin * B);
        vertices.emplace_back(pos + normals.back());

        if (i == 0) {
            first_v = vertices.back();
            first_n = normals.back();
        }
        s += 2.f * M_PI / (float) precision;
    }
}

void generateKnot(std::vector<glm::vec3> &vertices, int precision, int tube_precision) {
    std::vector<glm::vec3> tube;
    std::vector<glm::vec3> normals;
    double t = 0;
    double step = 2 * M_PI / precision;
    for (int i = 0; i < precision ; i++) {
        glm::vec3 pos = trefoil_at(t);
        generateTube(tube, normals, tube_precision, pos, t);
        t += step;
    }

    glm::vec3 pos = trefoil_at(0);
    generateTube(tube, normals, tube_precision, pos, 0);


    for (int i = 0; i < precision ; i++) {
        for (int j = 0; j < tube_precision; j++) {
            /** vertices **/
            const glm::vec3 &p1 = tube[i * tube_precision + (j + 0) % tube_precision];
            const glm::vec3 &p2 = tube[(i + 1) * tube_precision + (j + 1) % tube_precision];
            const glm::vec3 &p3 = tube[i * tube_precision + (j + 1) % tube_precision];
            const glm::vec3 &p4 = tube[(i + 1) * tube_precision + (j + 2) % tube_precision];
            /** normals **/
            const glm::vec3 &n1 = normals[i * tube_precision + (j + 0) % tube_precision];
            const glm::vec3 &n2 = normals[(i + 1) * tube_precision + (j + 1) % tube_precision];
            const glm::vec3 &n3 = normals[i * tube_precision + (j + 1) % tube_precision];
            const glm::vec3 &n4 = normals[(i + 1) * tube_precision + (j + 2) % tube_precision];

            vertices.push_back(p1);
            vertices.push_back(n1);
            vertices.push_back(p2);
            vertices.push_back(n2);
            vertices.push_back(p3);
            vertices.push_back(n3);

            vertices.push_back(p3);
            vertices.push_back(n3);
            vertices.push_back(p4);
            vertices.push_back(n4);
            vertices.push_back(p2);
            vertices.push_back(n2);

        }
    }
}

static void error_callback(int error, const char *description) {
    fputs(description, stderr);
}

int main(int ac, char **av) {

    int precision = 360;
    int tube_precision = 16;
    if (ac > 1) {
        av++;
        while (*av) {
            if (strcmp(*av, "-p") == 0)
                precision = std::stoi(*++av);
            if (strcmp(*av, "-t") == 0)
                tube_precision = std::stoi(*++av);
            av++;
        }
    }

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
    Shader vertexShader("shaders/vertex.glsl", Shader::Vertex);
    Shader fragmentShader("shaders/fragment.glsl", Shader::Fragment);
    Program program;

    program.addShader(vertexShader);
    program.addShader(fragmentShader);
    program.link();

    /** Create buffer object and load our data **/
    std::vector<glm::vec3> vertices;
    generateKnot(vertices, precision, tube_precision);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    /** Setting up the OpenGL program input parameters **/
    glVertexAttribPointer(glGetAttribLocation(program.getProgramID(), "point"), 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void *) 0);
    glVertexAttribPointer(glGetAttribLocation(program.getProgramID(), "normal"), 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

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
    glm::mat4 Model = glm::scale(glm::mat4(1), glm::vec3(1));
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

        camera.projection = glm::perspective(glm::radians(45.0f), (float) winSize.x / (float) winSize.y, 0.1f, 100.0f);
        /** update uniforms values **/
        glUniformMatrix4fv(u_View, 1, GL_FALSE, &camera.view[0][0]);
        glUniformMatrix4fv(u_Projection, 1, GL_FALSE, &camera.projection[0][0]);
        glUniformMatrix4fv(u_Model, 1, GL_FALSE, &Model[0][0]);

        /** draw call **/
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);
        w.swapBuffers();

        /** update the title with fps value **/
        frameCounter.endFrame();
        const std::string title =
                std::to_string(frameCounter.fps()) + " / " + std::to_string(frameCounter.renderTime());
        w.setTitle(title);
    }

    return 0;
}