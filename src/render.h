#ifndef RENDER_H
#define RENDER_H

#include <string>
#include <glad.h>
#include <glm/vec2.hpp>

GLuint create_tex(std::string path);
//TODO: remove draw_init in favor of SceneGraphs
void draw_init(glm::vec<2, int>);
void draw();

#endif//RENDER_H
