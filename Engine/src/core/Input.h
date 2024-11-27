//
// Created by allos on 11/21/2024.
//
#pragma once
#include "typedefs.h"
#include "Window.h"
#include "GLFW/glfw3.h"
#include "stl/string.h"

using namespace Tyche::STL;

namespace Tyche::Input {

    typedef string Action;

    enum Key: unsigned int {
        Q = 81,
        W = 87,
        E = 69,
        R = 82,
        T = 84,
        Y = 89,
        U = 85,
        I = 73,
        O = 79,
        A = 65,
        S = 83,
        D = 68,
        F = 70,
        G = 71,
        H = 72,
        J = 74,
        K = 75,
        L = 76,
        Z = 90,
        X = 88,
        C = 67,
        V = 86,
        B = 66,
        N = 78,
        M = 77,

        COMMA = 44,
        LSHIFT = 340,
        LCONTROL = 341,
        RSHIFT = 344,
        RCONTROL = 345,

        SPACE = 32,
        ESCAPE = 256,
        DELETE = 261,

        ONE = 49,
        TWO = 50,
        THREE = 51,
        FOUR = 52,
        FIVE = 53,
        SIX = 54,
        SEVEN = 55,
        EIGHT = 56,
        NINE = 57,
        ZERO = 48,

        BACKSPACE = 259,
        ENTER = 257,
        TAB = 258,

        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301
    };

    void setTargetWindow(Window& window);

    void addAction(const Action& action);
    void addKey(const Action& action, Key key);

    bool isActionPressed(const Action& action);

}