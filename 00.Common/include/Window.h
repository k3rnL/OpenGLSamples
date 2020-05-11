//
// Created by Erwan on 10-May-20.
//

#ifndef OPENGLSAMPLES_WINDOW_H
#define OPENGLSAMPLES_WINDOW_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

static void g_resize_callback(GLFWwindow *, int, int);
static void g_mouse_pos_callback(GLFWwindow *, double, double);
static void g_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

class Window {
public:
    Window(int width, int height, const std::string &title);

    ~Window();

    bool isOpen();

    void makeContextCurrent();
    void swapBuffers();

    const glm::vec4 &getViewport() const;
    const glm::vec2 &getSize() const;

    void setTitle(const std::string &title);

    void setCursorPosCallback(const std::function<void(double x, double y)> &callback);
    void setKeyboardCallback(
            const std::function<void(int key, int scancode, int action, int mods)> &callback);

private:
    GLFWwindow *_window;

    glm::vec4 _viewport;
    glm::vec2 _size;

    std::function<void(double x, double y)> _cursorPosCallback;
    std::function<void(int key, int scancode, int action, int mods)> _keyboardCallback;

    void resizeCallback(int x, int y);

    friend void g_resize_callback(GLFWwindow *, int, int);
    friend void g_mouse_pos_callback(GLFWwindow *, double, double);
    friend void g_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

static std::unordered_map<GLFWwindow *, Window *> g_windowInstances;


#endif //OPENGLSAMPLES_WINDOW_H
