#include "render.h"
#include <glad.h>
#include <stb_image.h>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <glm/gtc/quaternion.hpp>
#include "Camera.h"
#include "light/Shader.h"
#include "light/PointLight.h"
#include "light/DirLight.h"
#include "light/SpotLight.h"
#include "model/CubeMesh.h"
#include "io.h"
#include "ui.h"

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
    ui::add_handler(render_input_key);
    ui::add_handler(render_input_cursor);
    ui::add_handler(render_input_scroll);

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
    tri->set_material(tricolor, 4.0f);

    //test lighting
    tricolor->set("n_dir_lights", 1);
    dir_light = unique_ptr<DirLight>(new DirLight());
    dir_light->dir(glm::vec3(-.2, -1, -.1));
    dir_light->ambient(glm::vec3(.1, .1, .1));
    dir_light->diffuse(glm::vec3(.3, .3, .3));
    dir_light->specular(glm::vec3(1., 1., 1.));
    dir_light->pass_to(*tricolor, "dir_lights[0].");

    tricolor->set("n_point_lights", 1);
    point_light = unique_ptr<PointLight>(new PointLight());
    point_light->pos(glm::vec3(3, 0.6, 1));
    point_light->att_to_dist(50.f);
    point_light->ambient(glm::vec3(.01, .01, .01));
    point_light->diffuse(glm::vec3(1., .5, .5));
    point_light->specular(glm::vec3(1., 1., 1.));
    point_light->pass_to(*tricolor, "point_lights[0]."); 

    tricolor->set("n_spot_lights", 1);
    spot_light = unique_ptr<SpotLight>(new SpotLight());
    spot_light->dir(glm::vec3(-1, 0, 0));
    spot_light->cutoff(glm::cos(glm::radians(1.f)), glm::cos(glm::radians(1.f)));
    spot_light->pos(glm::vec3(-0.3, 0, 2));
    spot_light->att_to_dist(100.f);
    spot_light->ambient(glm::vec3(0, 0, 0));
    spot_light->diffuse(glm::vec3(.1, .1, 1.));
    spot_light->specular(glm::vec3(.1, .1, 1.));
    spot_light->pass_to(*tricolor, "spot_lights[0]."); 

    //set up camera
    cam = make_unique<Camera>();
    cam->aspect(static_cast<float>(dims.x / dims.y));
    cam->set_pos(glm::vec3(4, 5, 6));
    cam->set_rot(glm::vec3(-0.5, -1, -1), glm::vec3(0, 1, 0));

    //TODO: Uniform buffer object; see below
    //apply projection
    cam->apply_proj(*tricolor);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void draw() {
    glClearColor(.2f, .2f, .2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam->apply_proj(*tricolor);    
    //TODO:
    //update camera view: note to self, the most efficient way to do this across
    // many shaders (i.e. materials) is to use Uniform Buffer Objects:
    //www.geeks3d.com/20140704/gpu-buffers-introduction-to-opengl-3-1-unfiorm-buffer-objects
    //is a good tutorial for them. Also, the Khronos standard seems pretty good.
    cam->apply_view(*tricolor);

    glm::mat4 model = glm::mat4(3.f);
    model[3][3] = 1.f;
    tricolor->set("model", model);// temporary.
    //update models _and_ do glDraw; this combination seems to cause issues.
    tri->draw();
}

//custom handler for input
void render_input_key(GLFWwindow* w, double ddt) {
    float dt = static_cast<float>(ddt);
    glm::vec3 motion(0, 0, 0);
    if (ui::key_map[GLFW_KEY_W]) {
        motion += glm::vec3(0, 0, 1);
    }
    if (ui::key_map[GLFW_KEY_D]) {
        motion += glm::vec3(1, 0, 0);
    }
    if (ui::key_map[GLFW_KEY_R]) {
        motion += glm::vec3(0, 1, 0);
    }
    if (ui::key_map[GLFW_KEY_S]) {
        motion += glm::vec3(0, 0, -1);
    }
    if (ui::key_map[GLFW_KEY_A]) {
        motion += glm::vec3(-1, 0, 0);
    }
    if (ui::key_map[GLFW_KEY_F]) {
        motion += glm::vec3(0, -1, 0);
    }
    cam->move(motion * dt);

    float roll = 0;
    if (ui::key_map[GLFW_KEY_Q]) {
        roll -= 1.0f;
    }
    if (ui::key_map[GLFW_KEY_E]) {
        roll += 1.0f;
    }
    roll *= dt;
    glm::vec3 up_new = cam->up() + roll * cam->right();
    cam->set_rot(cam->look_dir(), up_new);

    if (ui::edge_up(GLFW_KEY_X)) {
        if (glfwGetInputMode(w, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    
}

void render_input_cursor(GLFWwindow*, double) {//ddt) {
    //float dt = static_cast<float>(ddt);
    glm::vec2 offset = ui::d_cursor_pos * 0.001f;
    glm::vec3 look_new = cam->look_dir() +
        + offset.x * cam->right() - offset.y * cam->up();
    cam->set_rot(look_new, cam->up());
}

void render_input_scroll(GLFWwindow*, double ddt) {
    float dt = static_cast<float>(ddt);
    cam->zoom(cam->zoom()*(1+ui::d_scroll * dt));
}

void framebuffer_resize(GLFWwindow* w, int width, int height) {
    int old_width;
    int old_height;
    glfwGetWindowSize(w, &old_width, &old_height);
    clog << "gg. Window resize (" << old_width << "," << old_height << ") -> "
         << "(" << width << "," << height << ")\n";
    glViewport(0, 0, width, height);

    cam->aspect(static_cast<float>(width)/static_cast<float>(height));
}
