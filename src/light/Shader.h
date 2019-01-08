// Copyright (c) 2016-2019 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
#pragma once

#include <glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class Shader {
public:
    Shader();
    ~Shader();
    // It doesn't make sense to copy many of the private variables
    Shader(const Shader&) = delete;

    bool add(GLenum type, const std::string path);
    bool del(const std::string path);
    bool build();
    void use();

    void set(std::string uniform, GLint);
    void set(std::string uniform, GLuint);
    void set(std::string, GLfloat);
    void set(std::string, glm::vec3);
    void set(std::string, glm::mat4);
    // add more sets as needed. Maybe figure out how to do this generically
private:
    std::unordered_map<std::string, GLenum> file_types;
    std::vector<std::pair<GLuint, std::string>> shader_objects;
    GLuint program;
    GLchar info_log[512];
    GLint success;

    // parse for uniform types?
    std::optional<GLint> check_uniform(std::string);
    bool compile(GLenum type, const std::string& path);
    bool link();
};
