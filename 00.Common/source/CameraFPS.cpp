//
// Created by Erwan on 02-Jun-20.
//

#include <iostream>
#include "CameraFPS.h"
#include "utils.h"


CameraFPS::CameraFPS(const glm::vec3 &direction,
                     const glm::vec3 &eye,
                     const glm::vec3 &up)
                     : CenteredCamera(direction, eye, up), _direction(direction), rotation(1) {
    view = glm::lookAt(_eye, _eye + _direction, _up);
}

void CameraFPS::move(const glm::vec3 &v) {
    glm::vec3 right = glm::cross(_up, _direction) * v.x;
    glm::vec3 up = _up * v.y;
    glm::vec3 front = _direction * v.z;
    std::cout << " -- move --" << std::endl;
    std::cout << "v: " << v << std::endl;
    std::cout << "right: " << right << std::endl;
    std::cout << "up: " <<  up << std::endl;
    std::cout << "front: " <<  front << std::endl;

    std::cout << " --" << std::endl;
    std::cout << "eye: " << _eye << std::endl;
    std::cout << "direction: " << _direction << std::endl;
    std::cout << "up: " <<  _up << std::endl;
    std::cout << " --" << std::endl;

    _eye = _eye + right + up + front;

    std::cout << _eye << std::endl;
    std::cout << _direction << std::endl;
    std::cout << " --" << std::endl;

    view = glm::lookAt(_eye, _eye + _direction, _up);
}

void CameraFPS::rotate(const glm::mat4 &v) {
    std::cout << " -- rotate --" << std::endl;
    std::cout << glm::length(_direction) << " " << (_direction) << std::endl;
    _direction = glm::normalize(v * glm::vec4(_direction, 1));
    std::cout << glm::length(_direction) << " " << (_direction) << std::endl;
    _up = v * glm::vec4(_up, 1);
    _up.x = 0;
    _up.z = 0;
    rotation = v * rotation;
    view = glm::lookAt(_eye, _eye + _direction, _up);
}
