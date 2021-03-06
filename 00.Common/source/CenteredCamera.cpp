//
// Created by Erwan on 11-May-20.
//

#include "CenteredCamera.h"

CenteredCamera::CenteredCamera(const glm::vec3 &center, const glm::vec3 &eye, const glm::vec3 &up) :
        _center(center),
        _eye(eye),
        _up(up) {
    view = glm::lookAt(_eye, _center, _up);
}

void CenteredCamera::move(const glm::vec3 &v) {
    _eye += v;
    view = glm::lookAt(_eye, _center, _up);
}
