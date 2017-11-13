#ifndef IO_H
#define IO_H

#include <string>
#include <experimental/optional>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

std::experimental::optional<const std::string> read_file(const std::string path);

struct Image {
    int width;
    int height;
    int channels;
    unsigned char* bytes;
};

std::experimental::optional<Image> read_image(const std::string image);

void input_callbacks(GLFWwindow* w);
void input_handler(GLFWwindow* w, double delta_s);

extern int key_map[512];
void map_keys(GLFWwindow* w, int key, int scancode, int action, int mods);

extern glm::vec2 cursor_pos;
extern glm::vec2 d_cursor;
void map_cursor(GLFWwindow* w, double xpos, double ypos);

extern glm::vec2 d_scroll;
void map_scroll(GLFWwindow * w, double xoffset, double yoffset);

#endif//IO_H
