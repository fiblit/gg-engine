#version 330 core
// Copyright (c) 2016-2018 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
in vec2 ftex;

out vec4 ocolor;

struct Material {
    sampler2D diffuse1;
};

uniform Material material;

void main() {
    ocolor = texture(material.diffuse1, ftex);
}
