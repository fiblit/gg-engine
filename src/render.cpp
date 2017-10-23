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
    const float mesh_data[3*3] {
        -.5f, -.5f, .0f,
         .5f, -.5f, .0f,
         .0f,  .5f, .0f
    };

    //generate
    glGenBuffers(1, &tri.VBO);
    glGenVertexArrays(1, &tri.VAO);

    //bind
    glBindVertexArray(tri.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, tri.VBO);

    //buffer & interpret
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh_data), mesh_data, GL_STATIC_DRAW);
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
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
