// Copyright (c) 2016-2019 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
#include "DirLight.h"

DirLight::DirLight(): _dir(glm::vec3(0, -1, 0)) {}

void DirLight::pass_to(Shader& shader, const std::string& id) {
    Light::pass_to(shader, id);
    shader.set(id + "dir", _dir);
}

glm::vec3 DirLight::dir() { return _dir; }

void DirLight::dir(glm::vec3 dir) { _dir = dir; }
