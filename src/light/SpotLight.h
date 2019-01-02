// Copyright (c) 2016-2018 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
#pragma once

#include "PointLight.h"

class SpotLight : public PointLight {
public:
    SpotLight();

    virtual void pass_to(Shader& shader, const std::string& id);

    // get/set
    glm::vec3 dir();
    void dir(glm::vec3);
    float cutoff_100p();
    float cutoff_0p();
    void cutoff(float p0, float p100);

private:
    glm::vec3 _dir;

    // 100% brightness cutoff (before this angle this is a point light)
    float _cutoff_100p;
    // 0% brightness cutoff (past this angle there is no light)
    float _cutoff_0p;
};
