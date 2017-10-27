#include "render.h"
#include <glad.h>
#include <stb_image.h>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Shader.h"
#include "Mesh.h"
#include "io.h"

using namespace std;

shared_ptr<Shader> tricolor;
shared_ptr<Mesh> tri;

GLuint create_tex(std::string path) {
    auto img = read_image(path);
    if (!img) {
        cerr << "gg! Failed to create texture of " << path << "\n";
        return 0;
    }

	GLenum format = GL_RGB;
    if (img->channels == 1) {
        format = GL_RED;
    } else if (img->channels == 3) {
        format = GL_RGB;
    } else if (img->channels == 4) {
        format = GL_RGBA;
    }

    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format),
	    img->width, img->height, 0,
        format, GL_UNSIGNED_BYTE, img->bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(img->bytes);//would prefer if this was solved by RAII
    return tex;
}

void draw_init() {
    string pwd(PROJECT_SRC_DIR);

    vector<Vertex> vertices = {
        {glm::vec3(.5f, .5f, .0f), glm::vec3(0, 0, 1), glm::vec2(1, 1)},
        {glm::vec3(.5f,-.5f, .0f), glm::vec3(0, 0, 1), glm::vec2(1, 0)},
        {glm::vec3(-.5f,.5f, .0f), glm::vec3(0, 0, 1), glm::vec2(0, 1)},
        {glm::vec3(-.5f,-.5f,.0f), glm::vec3(0, 0, 1), glm::vec2(0, 0)}
    };
    vector<GLuint> indices = {
        0, 1, 2,
        1, 2, 3
    };
    vector<Texture> textures = {
        {create_tex(pwd + "/res/container2.png"), Texmap::diffuse}
    };

    tri = shared_ptr<Mesh>(new Mesh(vertices, indices, textures));
    tricolor = shared_ptr<Shader>(new Shader());

    //build material
    tricolor->add(GL_VERTEX_SHADER, pwd + "/res/glsl/1to1_tex.vert");
    tricolor->add(GL_FRAGMENT_SHADER, pwd + "/res/glsl/tex.frag");
    tricolor->build();
    tri->set_material(tricolor);
}

void draw() {
    glClearColor(.2f, .2f, .2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    tri->draw();
}
