//
// Created by Erwan on 11-May-20.
//

#include "utils.h"

std::ostream &operator<<(std::ostream & os, const glm::vec3 &v) {
    os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return os;
}