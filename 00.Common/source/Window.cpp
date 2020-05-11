//
// Created by Erwan on 10-May-20.
//

#include "Window.h"

Window::Window(int width, int height, const std::string &title) {
    _window = glfwCreateWindow(width, height, "", NULL, NULL);
    glfwMakeContextCurrent(_window);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    g_windowInstances[_window] = this;

    glfwSetCursorPosCallback(_window, g_mouse_pos_callback);
    glfwSetKeyCallback(_window, g_keyboard_callback);

    glfwSetWindowSizeCallback(_window, g_resize_callback);

    _size = {width, height};
    _viewport = {0, 0, width, height};
}

Window::~Window() {
    g_windowInstances[_window] = nullptr;
    if (_window)
        glfwDestroyWindow(_window);
}

void Window::makeContextCurrent() {
    glfwMakeContextCurrent(_window);
}

void Window::setCursorPosCallback(const std::function<void(double x, double y)> &callback) {
    _cursorPosCallback = callback;
}

void Window::setKeyboardCallback(const std::function<void(int, int, int, int)> &callback) {
    _keyboardCallback = callback;
}

bool Window::isOpen() {
    return glfwWindowShouldClose(_window) == 0;
}

void Window::swapBuffers() {
    glfwSwapBuffers(_window);
}

void Window::setTitle(const std::string &title) {
    glfwSetWindowTitle(_window, title.c_str());
}

const glm::vec4 &Window::getViewport() const {
    return _viewport;
}

const glm::vec2 &Window::getSize() const {
    return _size;
}

void Window::resizeCallback(int x, int y) {
    _size = {x, y};
    _viewport.z = x;
    _viewport.w = y;

    glViewport(0, 0, x, y);
}

void g_resize_callback(GLFWwindow *w, int x, int y) {
    g_windowInstances[w]->resizeCallback(x, y);
}

void g_mouse_pos_callback(GLFWwindow *w, double x, double y) {
    g_windowInstances[w]->_cursorPosCallback(x, y);
}

void g_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    g_windowInstances[window]->_keyboardCallback(key, scancode, action, mods);
}

