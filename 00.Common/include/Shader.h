//
// Created by Erwan on 05-May-20.
//

#ifndef MINIMALOPENGL_SHADER_H
#define MINIMALOPENGL_SHADER_H


#include <string>
#include <GL/glew.h>
#include <stdexcept>

class ShaderException : public std::exception {
public:
    explicit ShaderException(const std::string &msg) : exception(msg.c_str()) {} ;
};

class Shader {
public:
    enum Type {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        Geometry = GL_GEOMETRY_SHADER
    };

    Shader(const std::string &file, Shader::Type type);
    ~Shader();

    GLuint getShaderID() const;

private:
    bool compile(const std::string &source);
    static std::string loadFile(const std::string &file);

    GLuint _shaderId;
};


#endif //MINIMALOPENGL_SHADER_H
