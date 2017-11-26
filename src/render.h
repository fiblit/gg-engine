#ifndef RENDER_H
#define RENDER_H

#include <string>
#include <glad.h>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>

namespace render {
GLuint create_tex(std::string path);
//TODO: remove init in favor of SceneGraphs
void init(glm::vec<2, int>);
void draw();

void framebuffer_resize(GLFWwindow* w, int width, int height);
//input handler
void render_input_key(GLFWwindow* w, double dt);
void render_input_cursor(GLFWwindow* w, double dt);
void render_input_scroll(GLFWwindow* w, double dt);
}

#endif//RENDER_H
