#include "Shader.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "io.h"

using namespace std;
using namespace std::experimental;

Shader::Shader()
    : program(0)
    , success(false) {
}

Shader::~Shader() {
    for (auto& sobj : shader_objects) {
        glDeleteShader(sobj);
        sobj = 0;
    }
    shader_objects.clear();
    glDeleteProgram(program);
    program = 0;
    success = false;
}

bool Shader::add(GLenum type, const std::string path) {
    if (file_types.count(path)) {
        cerr << "gg! Shader already being used by this shader program.\n";
        return false;
    }
    file_types[path] = type;
    return true;
}

bool Shader::del(const std::string path) {
    if (!file_types.erase(path)) {
        cerr << "gg! Shader " << path << " not in program to delete.\n";
        return false;
    }
    return true;
}

bool Shader::build() {
    program = glCreateProgram();
    shader_objects.clear();
    for (const auto& pair : file_types) {
        compile(pair.second, *read_file(pair.first));
    }
    if (success) {
        link();
    }
    return success;
}

void Shader::use() {
    assert(program != 0 && "gg! Program not built\n");
    glUseProgram(program);
}

bool Shader::compile(GLenum type, const string content) {
    GLuint shader_object = glCreateShader(type);
    const char* c = content.c_str();
    glShaderSource(shader_object, 1, &c, nullptr);
    glCompileShader(shader_object);

    //check for errors
    glGetShaderiv(shader_object, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_object, 512, nullptr, info_log);
        cerr << "gg! Shader failed to compile:\n" << info_log << "\n";
    }
    return success;
}

bool Shader::link() {
    for (auto& sobj : shader_objects) {
        glAttachShader(program, sobj);
    }
    glLinkProgram(program);

    //check for errors
    glGetShaderiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(program, 512, nullptr, info_log);
        cerr << "gg! Shader failed to link:\n" << info_log << "\n";
    }
    return success;
}

optional<GLint> Shader::check_uniform(string uniform) {
    if (uniform.substr(3) == "gl_") {
        cerr << "gg! Shader cannot use gl_ before uniforms " << uniform << "\n";
        return nullopt;
    } 
    GLint loc = glGetUniformLocation(program, uniform.c_str());
    if (loc == -1) {
        cerr << "gg! Shader does not contain uniform " << uniform << "\n";
        return nullopt;
    }
    return loc;
}

void Shader::set(string uniform, GLint i) {
    glUniform1i(*check_uniform(uniform), i);
}

void Shader::set(string uniform, GLfloat f) {
    glUniform1f(*check_uniform(uniform), f);
}

void Shader::set(string uniform, glm::vec3 v) {
    glUniform3f(*check_uniform(uniform), v.x, v.y, v.z);
}

void Shader::set(string uniform, glm::mat4 m) {
    //supplied in column major order
    glUniformMatrix4fv(*check_uniform(uniform), 1, GL_FALSE, glm::value_ptr(m));
}
