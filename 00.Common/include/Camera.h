//
// Created by Erwan on 11-May-20.
//

#ifndef OPENGLSAMPLES_CAMERA_H
#define OPENGLSAMPLES_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();

    virtual void move(const glm::vec3 &v) = 0;

    glm::mat4   projection;
    glm::mat4   view;
};


#endif //OPENGLSAMPLES_CAMERA_H
