#ifdef _WIN32
    //avoid including <windows.h> in glad
    #define APIENTRY __stdcall
#endif
#include <glad.h>
#ifdef _WINDOWS_
    #error windows.h included
#endif
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <iostream>
#include <cstdlib>

#include "util/Timer.h"
#include "render.h"
#include "io.h"
#include "ui.h"

using namespace std;

void glfw_error(int err, const char* msg);
void monitor_connect(GLFWmonitor*, int event);
void keymap_input(GLFWwindow*);

int main(int, char**) {
    //Setup pre-init glfw
    glfwSetErrorCallback(glfw_error);

    //initialize glfw, output version.
    if (!glfwInit()) {
        cerr << "gg! Failed to init glfw; exiting.\n";
        return EXIT_FAILURE;
    } else {
        clog << "gg. GLFW compiled as v"
             << GLFW_VERSION_MAJOR << "."
             << GLFW_VERSION_MINOR << "."
             << GLFW_VERSION_REVISION << "\n";
        int major, minor, revision;
        glfwGetVersion(&major, &minor, &revision);
        clog << "gg. GLFW running as v" 
             << major << "." << minor << "." << revision << "\n";
        clog << "gg. GLFW version string " << glfwGetVersionString() << "\n";
    }

    //obtain primary monitor
    int num_monitor;
    GLFWmonitor** monitors = glfwGetMonitors(&num_monitor);
    if (!monitors) {
        cerr << "gg! No monitor found.\n";
        glfwTerminate();
        return EXIT_FAILURE;
    } else {
        clog << "gg. Found " << num_monitor << " monitors. Using primary.\n";
    }
    //primary monitor is 0; so, mode0 is the primary video mode.
    const GLFWvidmode* mode0 = glfwGetVideoMode(monitors[0]);
    glfwSetMonitorCallback(monitor_connect);


    //initialize window and OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //for Mac OSX to work
    glm::vec<2, int> size(min(640, mode0->width), min(480, mode0->height));
    GLFWwindow* window = glfwCreateWindow(size.x, size.y, "gg", nullptr, nullptr);
    if (!window) {
        cerr << "gg! Failed to create window context.\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }
    //center window
    glfwSetWindowPos(window, mode0->width/2 - size.x/2, mode0->height/2 - size.y/2);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize);
    glfwMakeContextCurrent(window);

    //load glad in window context
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        cerr << "gg! Failed to load OpenGL context.\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glViewport(0, 0, size.x, size.y);

    ui::init_callbacks(window);
    draw_init(size);

    //main loop
    Timer init_time;
    Timer frame_time;
    int fps = 0;
    auto last_s = init_time.time();
    while (!glfwWindowShouldClose(window)) {
        frame_time.tick();
        //FPS recorder
        if (1 < std::chrono::duration_cast<std::chrono::seconds>(
                frame_time.time() - last_s).count()) { 
            std::clog << "FPS: " << fps << "\n";
            fps = 0;
            last_s = frame_time.time();
        } else {
            ++fps;
        }

        //render
        draw();

        //double buffer
        glfwSwapBuffers(window);
        //input handling
        glfwPollEvents();
        ui::handle_input(window, frame_time.delta_s());
    }

    //free all memory and libraries
    glfwTerminate();
    return EXIT_SUCCESS;
}

void glfw_error(int err, const char* msg) {
    cerr << "gg! GLFW error: #" << err << " " << msg << "\n";
}

void monitor_connect(GLFWmonitor* m, int event) {
    if (event == GLFW_CONNECTED) {
        clog << "gg. Monitor " << glfwGetMonitorName(m) << " connected.\n";
    } else if (event == GLFW_DISCONNECTED) {
        clog << "gg. Monitor " << glfwGetMonitorName(m) << " disconnected.\n";
    }
}
