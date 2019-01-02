#version 330 core
// Copyright (c) 2016-2019 dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec3 inorm;
layout (location = 2) in vec2 itex;

out vec2 ftex;

void main() {
    ftex = itex;
    gl_Position = vec4(ipos, 1.0);
}
