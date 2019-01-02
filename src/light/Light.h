// Copyright (c) 2016-2019 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
#pragma once

#include "Shader.h"
#include <glm/vec3.hpp>
#include <string>

class Light {
public:
    // get/set colors
    glm::vec3 ambient();
    void ambient(glm::vec3);
    glm::vec3 diffuse();
    void diffuse(glm::vec3);
    glm::vec3 specular();
    void specular(glm::vec3);

protected:
    Light();

    // pass light's parameters into the shader's uniforms prefixed by id
    virtual void pass_to(Shader& shader, const std::string& id);

    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
};
