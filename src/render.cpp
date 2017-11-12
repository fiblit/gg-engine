#include "render.h"
#include <glad.h>
#include <stb_image.h>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Camera.h"
#include "Shader.h"
#include "CubeMesh.h"
#include "PointLight.h"
#include "DirLight.h"
#include "SpotLight.h"
#include "io.h"

using namespace std;

shared_ptr<Shader> tricolor;
unique_ptr<DirLight> dir_light;
unique_ptr<PointLight> point_light;
unique_ptr<SpotLight> spot_light;
shared_ptr<Mesh> tri;
unique_ptr<Camera> cam;

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

void draw_init(glm::vec<2, int> dims) {
    string pwd(PROJECT_SRC_DIR);

    vector<Texture> textures = {
        {create_tex(pwd + "/res/container2.png"), Texmap::diffuse},
        {create_tex(pwd + "/res/container2_specular.png"), Texmap::specular}
    };

    tri = shared_ptr<Mesh>(new CubeMesh(textures));
    tricolor = shared_ptr<Shader>(new Shader());

    //build material
    tricolor->add(GL_VERTEX_SHADER, pwd + "/res/glsl/tex.vert");
    tricolor->add(GL_FRAGMENT_SHADER, pwd + "/res/glsl/lit_mtl.frag");
    tricolor->build();
    tri->set_material(tricolor, 1.0f);

    //test lighting
    dir_light = unique_ptr<DirLight>(new DirLight());
    dir_light->dir(glm::vec3(-1, -0, -1));
    dir_light->ambient(glm::vec3(.1, .1, .1));
    dir_light->diffuse(glm::vec3(1., 1., 1.));
    dir_light->specular(glm::vec3(1., 1., 1.));
    dir_light->pass_to(*tricolor, "dir_lights[0].");
    tricolor->set("n_dir_lights", 1);

    //set up camera
    cam = make_unique<Camera>();
    cam->set_aspect(static_cast<float>(dims.x / dims.y));
    cam->set_pos(glm::vec3(5, 5, 5));

    cam->set_rot(glm::vec3(-1, -1, -1), glm::vec3(0, 1, 0));

    //TODO: Uniform buffer object; see below
    //apply projection
    cam->apply_proj(*tricolor);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void draw() {
    glClearColor(.2f, .2f, .2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //TODO:
    //update camera view: note to self, the most efficient way to do this across
    // many shaders (i.e. materials) is to use Uniform Buffer Objects:
    //www.geeks3d.com/20140704/gpu-buffers-introduction-to-opengl-3-1-unfiorm-buffer-objects
    //is a good tutorial for them. Also, the Khronos standard seems pretty good.
    cam->apply_view(*tricolor);

    tricolor->set("model", glm::mat4(1.f));// temporary.
    //update models _and_ do glDraw; this combination seems to cause issues.
    tri->draw();
}
