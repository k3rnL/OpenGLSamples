//
// Created by Erwan on 05-May-20.
//

#include <iostream>
#include "Program.h"

Program::Program() {
    _programID = glCreateProgram();
}

bool Program::addShader(const Shader &shader) {
    glAttachShader(_programID, shader.getShaderID());
    return false;
}

bool Program::link() {
    glLinkProgram(_programID);

    GLint result = GL_FALSE;
    glGetShaderiv(_programID, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        char buffer[1024];
        glGetProgramInfoLog(_programID, 1024, NULL, buffer);
        if (buffer[0]) {
            std::cerr << "Cannot link program : " << buffer << std::endl;
            exit(1);
        }
    }
    return true;
}

GLuint Program::getProgramID() const {
    return _programID;
}
