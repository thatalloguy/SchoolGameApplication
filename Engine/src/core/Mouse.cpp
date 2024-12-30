//
// Created by allos on 12/5/2024.
//

#include "Mouse.h"

#include "GLFW/glfw3.h"

//Global constants
namespace {
    GLFWwindow* _window_ptr = nullptr;
}

void Tyche::Mouse::setTargetWindow(void* window_ptr) {
    _window_ptr = (GLFWwindow*) window_ptr;
}

Tyche::Math::Vector2 Tyche::Mouse::getPosition() {
    double x, y;

    glfwGetCursorPos(_window_ptr, &x, &y);

    return Math::Vector2{(float) x, (float) y};
}

void Tyche::Mouse::setMouseMode(MouseMode mode) {
    glfwSetInputMode(_window_ptr, GLFW_CURSOR, mode);
}

bool Tyche::Mouse::isMouseButtonPressed(MouseButton button) {
    return (glfwGetMouseButton(_window_ptr, button) == GLFW_PRESS);
}
