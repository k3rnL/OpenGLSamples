//
// Created by Erwan on 02-Jun-20.
//

#ifndef OPENGLSAMPLES_CAMERAFPS_H
#define OPENGLSAMPLES_CAMERAFPS_H


#include "CenteredCamera.h"

class CameraFPS : public CenteredCamera {
public:
    explicit CameraFPS(const glm::vec3 &direction = {0.f, 0.f, -1.f},
                       const glm::vec3 &eye = {0.f, 0.f, 1.f},
                       const glm::vec3 &up = {0.f, 1.f, 0.f});

    void move(const glm::vec3 &v) override;
    void rotate(const glm::mat4 &v);

    glm::mat4 rotation;

private:
    glm::vec3   _direction;

};


#endif //OPENGLSAMPLES_CAMERAFPS_H
