//
// Created by Erwan on 05-May-20.
//

#include <iostream>
#include "Program.h"

Program::Program() {
    if ((_programID = glCreateProgram()) == 0)
        std::cerr << "Cannot create program" << std::endl;
    GLenum err = glGetError();
    if (err != GL_FALSE)
        throw std::runtime_error("OpenGL error when creating program:" + std::to_string(err));
}

Program::~Program() {
    glUseProgram(0);
    glDeleteProgram(_programID);
}

bool Program::addShader(const Shader &shader) {
    glAttachShader(_programID, shader.getShaderID());
    GLenum err = glGetError();
    if (err != GL_FALSE)
        throw std::runtime_error("OpenGL error when attaching shader to program:" + std::to_string(err));
    return false;
}

bool Program::link() {


    GLint result = GL_FALSE;
    glLinkProgram(_programID);
    glGetProgramiv(_programID, GL_LINK_STATUS, &result);
     if (result == GL_FALSE) {
        char buffer[1024];
        glGetProgramInfoLog(_programID, 1024, NULL, buffer);
        if (buffer[0]) {
            std::cerr << "Cannot link program : " << buffer << std::endl;
            exit(1);
        }
    }
    GLenum err = glGetError();
    if (err != GL_FALSE)
        throw std::runtime_error("OpenGL error when linking program:" + std::to_string(err));
    return true;
}

GLuint Program::getProgramID() const {
    return _programID;
}

GLuint Program::uniform(const std::string &name) const {
    return glGetUniformLocation(_programID, name.c_str());
}
