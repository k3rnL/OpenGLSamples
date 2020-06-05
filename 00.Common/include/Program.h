//
// Created by Erwan on 05-May-20.
//

#ifndef MINIMALOPENGL_PROGRAM_H
#define MINIMALOPENGL_PROGRAM_H


#include "Shader.h"

class Program {
public:
    Program();
    ~Program();

    bool addShader(const Shader &shader);
    bool link();

    GLuint getProgramID() const;

    GLuint uniform(const std::string &name) const;

private:
    GLuint _programID;
};


#endif //MINIMALOPENGL_PROGRAM_H
