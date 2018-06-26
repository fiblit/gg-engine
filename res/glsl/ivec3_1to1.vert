#version 330 core
// Copyright (c) 2016-2018 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
layout (location = 0) in vec3 ipos;

void main() {
    gl_Position = vec4(ipos, 1.0);
}
