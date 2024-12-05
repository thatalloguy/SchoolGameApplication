//
// Created by allos on 12/5/2024.
//
#pragma once
#include "GLFW/glfw3.h"
#include "math/vecs.h"

namespace Tyche {

    namespace Mouse {

        enum MouseMode {
            HIDDEN = 0x00034002,
            NORMAL = 0x00034001,
            DISABLED = 0x00034003,
        };

        enum MouseButton {
            LEFT = 0,
            RIGHT = 1,
            MIDDLE = 2,
        };

        void setTargetWindow(void * window_ptr);

        Math::Vector2 getPosition();

        bool isMouseButtonPressed(MouseButton button);

        void setMouseMode(MouseMode mode);

    };

}

