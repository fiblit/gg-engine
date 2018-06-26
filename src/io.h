// Copyright (c) 2016-2018 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
#ifndef IO_H
#define IO_H

#include <string>
#ifdef WIN32
    #include <optional>
#else
    #include <experimental/optional>
#endif
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#ifdef WIN32
std::optional<const std::string> read_file(const std::string path);
#else
std::experimental::optional<const std::string> read_file(const std::string path);
#endif



struct Image {
    int width;
    int height;
    int channels;
    unsigned char* bytes;
};

#ifdef WIN32
std::optional<Image> read_image(const std::string image);
#else
std::experimental::optional<Image> read_image(const std::string image);
#endif

#endif//IO_H
