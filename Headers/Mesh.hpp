#ifndef MESH_H_GUARD
#define MESH_H_GUARD
#pragma once

// glad: an OpenGL function loader: https://github.com/Dav1dde/glad
#include <glad/glad.h>
// glm: OpenGL mathematics: http://glm.g-truc.net/0.9.8/index.html
#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "Shader.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    //should really be an enum
    std::string type;
};

class Mesh {
public:
    /* Mesh data */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    /* Functions */
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
    void Draw(Shader shader);
private:
    /* Render Data */
    GLuint VAO, VBO, EBO;
    /* Functions */
    void setupMesh();
};

#endif//MESH_H_GUARD
