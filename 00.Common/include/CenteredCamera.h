//
// Created by Erwan on 11-May-20.
//

#ifndef OPENGLSAMPLES_CENTEREDCAMERA_H
#define OPENGLSAMPLES_CENTEREDCAMERA_H


#include "Camera.h"

class CenteredCamera : public Camera {
public:
    explicit CenteredCamera(const glm::vec3 &center = {0.f, 0.f, 0.f},
                            const glm::vec3 &eye = {0.f, 0.f, 0.f},
                            const glm::vec3 &up = {0.f, 1.f, 0.f});

    void move(const glm::vec3 &v) override;

private:
    glm::vec3 _up;
    glm::vec3 _eye;
    glm::vec3 _center;
};


#endif //OPENGLSAMPLES_CENTEREDCAMERA_H
