// Copyright (c) 2016-2019 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
#pragma once

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <optional>
#include <string>

std::optional<const std::string> read_file(const std::string path);

struct Image {
    int width;
    int height;
    int channels;
    unsigned char* bytes;
};

std::optional<Image> read_image(const std::string image);
