#include "io.h"
#include "render.h"
#include <fstream>
#include <iostream>
#include <stb_image.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

using namespace std;
using namespace std::experimental;

optional<const string> read_file(const string path) {
    ifstream file(path, ios::in | ios::binary);
    if (!file) {
        cerr << "gg! Failed to open file " << path << "\n";
        return nullopt;
    }

    file.seekg(0, ios::end);
    string content(static_cast<size_t>(file.tellg()), '\0');
    file.seekg(0, ios::beg);
    file.read(&content[0], static_cast<long int>(content.size()));
    if (!file) {
        cerr << "gg! Failed to read entire file " << path << "\n";
        return nullopt; 
    }

    return content;
}

optional<Image> read_image(const string path) {
    Image i;
    i.bytes = stbi_load(path.c_str(), &i.width, &i.height, &i.channels, 0);
    if (i.bytes) {
        return i;
    } else {
        return nullopt;
    }
}

void input_callbacks(GLFWwindow* w) {
    glfwSetKeyCallback(w, map_keys);
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(w, GLFW_STICKY_KEYS, 1);
    glfwSetCursorPosCallback(w, map_cursor);
    glfwSetScrollCallback(w, map_scroll);
}

int key_map[512] = {0};
void map_keys(GLFWwindow*, int key, int, int action, int) {
    key_map[key] = action;
}

void input_handler(GLFWwindow* w, double delta_s) {
    //global handlers
    if(key_map[GLFW_KEY_ESCAPE] == GLFW_PRESS)
        glfwSetWindowShouldClose(w, true);

    //custom handlers
    float fds = static_cast<float>(delta_s);
    render_input_key(w, key_map, fds);
    render_input_cursor(w, cursor_pos, d_cursor, fds);
    render_input_scroll(w, d_scroll, fds);

    d_cursor = glm::vec2(0, 0);
    d_scroll = glm::vec2(0, 0);
}

glm::vec2 cursor_pos = glm::vec2(-1000000, -1000000);
glm::vec2 d_cursor = glm::vec2(0, 0);
void map_cursor(GLFWwindow*, double xpos, double ypos) {
    //to handle initial focus
    if (glm::length2(cursor_pos) > 1000000000) {
        cursor_pos = glm::vec2(xpos, ypos);
    }

    //offseting
    d_cursor = glm::vec2(xpos - cursor_pos.x, ypos - cursor_pos.y);
    cursor_pos = glm::vec2(xpos, ypos);
}

glm::vec2 d_scroll = glm::vec2(0, 0);
void map_scroll(GLFWwindow*, double xoffset, double yoffset) {
    d_scroll = glm::vec2(xoffset, yoffset);
}
