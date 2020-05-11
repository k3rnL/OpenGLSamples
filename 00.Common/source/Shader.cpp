//
// Created by Erwan on 05-May-20.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "Shader.h"

Shader::Shader(const std::string &file, Shader::Type type) {
    const std::string &source = loadFile(file);

    _shaderId = glCreateShader(type);

    compile(source);
}

std::string Shader::loadFile(const std::string &file) {
    std::ifstream ifs(file);
    std::stringstream buffer;

    buffer << ifs.rdbuf();
    return buffer.str();
}

bool Shader::compile(const std::string &source) {
    const char *src = source.c_str();
    glShaderSource(_shaderId, 1, &src, NULL);
    glCompileShader(_shaderId);

    GLint result = GL_FALSE;
    glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        char buffer[1024];
        glGetShaderInfoLog(_shaderId, 1024, NULL, buffer);
        if (buffer[0]) {
            std::cerr << "Cannot compile shader source: " << buffer << std::endl;
            exit(1);
        }
    };
    return true;
}

GLuint Shader::getShaderID() const {
    return _shaderId;
}
