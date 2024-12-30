//
// Created by allos on 12/5/2024.
//
#pragma once
#include "GLFW/glfw3.h"
#include "math/vecs.h"

namespace Tyche {

    //Very barebones mouse input layer
    namespace Mouse {

        // values are the same as GLFW 3
        enum MouseMode {
            // Hidden from the user but still allows moving and input
            HIDDEN = 0x00034002,
            // Normal behaviour
            NORMAL = 0x00034001,
            // Both hidden and doesnt allow moving or input.
            DISABLED = 0x00034003,
        };

        enum MouseButton {
            LEFT = 0,
            RIGHT = 1,
            MIDDLE = 2,
        };


        //Tell the mouse what window it should capture input for.
        void setTargetWindow(void * window_ptr);

        //returns the mouse position
        Math::Vector2 getPosition();

        bool isMouseButtonPressed(MouseButton button);
        void setMouseMode(MouseMode mode);

    };

}

