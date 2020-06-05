//
// Created by Erwan on 02-Jun-20.
//

#ifndef OPENGLSAMPLES_TEXTURE_H
#define OPENGLSAMPLES_TEXTURE_H

#include <GL/glew.h>
#include <string>

class Texture {
public:
    Texture();

    void load(const std::string &imagePath);
private:
    GLuint  _textureId;
};


#endif //OPENGLSAMPLES_TEXTURE_H
