#ifndef RENDER_H
#define RENDER_H

#include <string>
#include <glad.h>

GLuint create_tex(std::string path);
//TODO: remove draw_init in favor of SceneGraphs
void draw_init();
void draw();

#endif//RENDER_H
