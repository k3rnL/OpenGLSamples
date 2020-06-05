//
// Created by Erwan on 11-May-20.
//

#ifndef OPENGLSAMPLES_UTILS_H
#define OPENGLSAMPLES_UTILS_H

#include <iostream>
#include <glm/glm.hpp>

#include "renderdoc_app.h"
static RENDERDOC_API_1_1_2 *rdoc_api = NULL;

void initRenderDoc();

std::ostream &operator<<(std::ostream & os, const glm::vec3 &v);

#endif //OPENGLSAMPLES_UTILS_H
