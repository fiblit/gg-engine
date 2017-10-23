#include "render.h"
#include <glad.h>
#include <string>
#include "Shader.h"
#include <iostream>
#include "debug.h"

using namespace std;

//TODO: move this off into a class that handles all mesh things for me
struct Mesh {
    uint VBO;
    uint VAO;
    uint IBO;
};

Shader tricolor;
Mesh tri;

void draw_init() {
    //test that all functions can work before moving it off into a mesh.
    //gotta be incremental about this!
    const float mesh_vertices[4*3] {
         .5f,  .5f, .0f,
         .5f, -.5f, .0f,
        -.5f,  .5f, .0f,
        -.5f, -.5f, .0f
    };
    const uint mesh_indices[2*3] {
        0, 1, 2,
        1, 2, 3
    };

    //generate
    glGenBuffers(1, &tri.VBO);
    glGenBuffers(1, &tri.IBO);
    glGenVertexArrays(1, &tri.VAO);

    //bind
    glBindVertexArray(tri.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, tri.VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri.IBO);

    //buffer & interpret
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh_vertices), mesh_vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh_indices), mesh_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    //clean up
    glBindVertexArray(0);

    //build material
    string pwd(PROJECT_SRC_DIR);
    tricolor.add(GL_VERTEX_SHADER, pwd + "/res/glsl/ivec3_1to1.vert");
    tricolor.add(GL_FRAGMENT_SHADER, pwd + "/res/glsl/flat_red.frag");
    tricolor.build();
}

void draw() {
    glClearColor(.2f, .2f, .2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    tricolor.use();
    glBindVertexArray(tri.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
