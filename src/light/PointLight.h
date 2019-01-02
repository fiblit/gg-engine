// Copyright (c) 2016-2019 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
#pragma once

#include "Light.h"

class PointLight : public Light {
public:
    PointLight();

    virtual void pass_to(Shader& shader, const std::string& id);

    // attenuate so that the light has some noticeable effect until dist units
    void att_to_dist(float dist);

    // get/set
    glm::vec3 pos();
    void pos(glm::vec3);
    float att_constant();
    float att_linear();
    float att_quadratic();
    void att(float k, float x, float x2);

private:
    glm::vec3 _pos;

    // attenuation parameters
    // constant
    float _att_k;
    // linear
    float _att_x;
    // quadratic
    float _att_x2;
};
